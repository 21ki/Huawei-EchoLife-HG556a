/***********************************************************************
��Ȩ��Ϣ: ��Ȩ����(C) 1988-2005, ��Ϊ�������޹�˾.
�ļ���: Diagd.c
����: l65457
�汾: V100R001
��������: 2007-3-20
�������: 2007-3-27
��������: ʵ��PC ���ں�DIAG��͸������;MSM��ǿ��������·����ģ��ĺ�̨������
          ��˿�����Э�̣����⴮������͸����
  
��Ҫ�����б�:
    int diag_daemon( int nochdir, int noclose )
    ʵ�ֺ�̨����
    
    static int diag_init_tcp_listen_socket( usockaddr* uSockaddr_tcp )
    ʵ�ִ���tcp�����׽���
    
    static int diag_udpinit_socket( )
    ��ʼ��UDP�׽���
    
    void close_pcui_shake( )
    ����ʱ�ر�pppd�Ƚ���
    
    void net_to_diag()
    �����������ݣ��������뵽Diag��
    
    void diag_to_net ( )
    ����Diag�����ݣ��������뵽���ڣ�����һ���ӽ���
    
    int open_usb_port()
    ��usb�˿�
    
    int diag_accept_connect (int listen_fd )
    ���ܶԶ�tcp��������
    
    void tcp_timeout ( int sig )
    tcp���Ӷ�ʱ������ʱ����g_disconnect=1
    
    static void main_coordinate ( int sig )
    �������źŴ������

    static void sub_coordinate ( int sig )
    �ӽ����źŴ������
    
    long analyse_cmd(unsigned char* buf,long *img_len)
    ����hdlc����payload�������������·��������������ģ�顢ǿ�����������������

    static EN_UPGRADE_ERRCODE route_fwt_upgrade(char *part_img, long len, long *total)
    ���������𽫽��ܵ�tcp����д���²㺯��

    void udp_server_main(pid_t parent_pid)
    ����udp��1280�˿ڣ�����virtual console�Ͷ�˿�Э�̱���

    nego_state parse_nego_cmd (unsigned char *buf, int len, unsigned char *report_buf)
    ��˿�����Э���У���������Э���������ݡ�

    int ip_change(char * Reportbuf)
    ����IP
    
    int report_to_pc(int sock_fd, char* report_buf, struct sockaddr_in* pClientSock)
    ʹ��hdlc���ģ���ִ�н�������PC

    void report_upgrade_result(int sock_fd, int result)
    ��pc����·����ģ��FWT�������
    
    void  ip_restore( int flag ) 
    ��˿�������ɺ�ָ�����IP��ַ

    static inline unsigned char str2hexnum(unsigned char c)
    ���ַ�ת��������

    static int str2ipaddr(const char *str, unsigned char *ip)
    ��"192.168.1.1"��ʽ��ip��ַת��������֣�����4�ֽڵ�����

    int scratch_ip_mac(char* report_buf)
    ��nvram�л�ȡip��mac��ַ

    void multi_upgrade_nego(int udp_fd,unsigned char *buf,int len, 
        struct sockaddr_in* client_udp_addr )
    װ������Э�̵������򡣷���udp�е�hdlc��ʽ���ִ����Ӧ����������ִ�����

    <D00030.DOWN: lichenglong 2007-4-10 ADD BEGIN
    void gen_hdlc_packet(char* payload,long size,char* hdlc)
    D00030.DOWN: lichenglong 2007-4-10 ADD END> 

    ����hdlc���ģ�����crc��ת��payload�е�0x7e��0x7d�ֽ�
    char parse_tcp_payload (unsigned char *buf,long len, 
        unsigned char* out_buf,long* out_len)
    ����tcp�з�װ��һ��hdlc����

    long parse_udp_payload (unsigned char *buf,long len, 
        unsigned char* out_buf,long* out_len)
    ����udp�з�װ��һ��hdlc����
    
    int main(int argc, char **argv)
    ��������·����ģ�顢����ģ��������ں������̣�
    ʵ�ֶ˿�����������diag�����������д·����ģ��flash���ߣ�ת��Diag���ݵ�����

�޸���ʷ��¼�б�: 
<��  ��>    lichenglong
<�޸�ʱ��>
<�汾>      1.0
<�޸�����>

��ע: 
************************************************************************/
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/errno.h>
#include <linux/errno.h>
#include <sys/signal.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/tcp.h>
#include <sys/time.h>
#include "netjet.h"
#include "dump.h"

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#ifdef ORG_DIAG
#include "atptag.h"
#include "atpflash.h"
#include <bcmnvram.h>
#include <shutils.h>

#define IMAGE_MAIN_S             "MAIN_S"
#endif

#define MAX_NVLINESTR     256 

#define SIGNAL(s, handler)	do { \
	sa.sa_handler = handler; \
	if (sigaction(s, &sa, NULL) < 0) \
	    printf("Couldn't establish signal handler (%d): %m", s); \
    } while (0)

#ifndef ORG_DIAG
typedef enum
{
    MAIN_F,
    SLAVE_F,
    MAIN_S,
    SLAVE_S,
}ATP_UPG_EN;
#endif


int        g_listen_fd = -1;
int        g_netfd = -1;  //tcp socket������
int        g_usbfd = -1;  //usb�豸������
int        g_udpfd = -1;  //udp socket������
int        g_logsys = -1; //��־�ļ�������

pid_t     g_udp_pid   = -1;
pid_t     g_sub_pid   = -1;
pid_t     g_main_pid = -1;

int        g_Serverport = SERVER_PORT;  //tcp��udp�����˿ں�
int        g_current_udp_port = -1;     //��¼�ĵ�ǰudp�Զ˶˿ں�

char       g_i_upgrade = NOT_UPGRADE;   //״̬���Ƿ���ĳ������״̬��MSM͸��״̬
int        g_net_alive = NET_TIMEOUT;   //tcp ���ӳ�ʱtimerֵ
USB_PORT_E g_ePort = _DIAG;             //usb�豸��

static int g_disconnect = NOT_DISCONNET;//�Ƿ�Ͽ�tcp����

static diag_status  g_state;            //����״̬����״̬



#ifndef ORG_DIAG
extern unsigned int BSP_MSM_On( );

extern unsigned int BSP_MSM_Off( );

extern unsigned int BSP_MSM_Force_On( );

extern unsigned int BSP_MSM_Force_Off( );
#endif


#define      NVRAM_PARTITION    "mtd5"  
static   int   g_current_tcp_port = -1;
#define      EXPECT_UDP_PORT  "expect_udp_port"
#define      VALID_CONNECT      0
#define      INVALID_CONNECT  -1
static   int   g_next_connect     = VALID_CONNECT;

static int g_waitMsmTimeout = 0;            //�Ƿ�Ͽ�tcp����
static int g_reboot_all = 0;            //�Ƿ��������ߺ�·����ģ��        

#ifndef ORG_DIAG

/*hdlc crc compute table*/
unsigned short crc_16_l_table[ 256 ] = {
  0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
  0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
  0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
  0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
  0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
  0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
  0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
  0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
  0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
  0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
  0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
  0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
  0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
  0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
  0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
  0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
  0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
  0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
  0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
  0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
  0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
  0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
  0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
  0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
  0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
  0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
  0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
  0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
  0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
  0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
  0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
  0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

static unsigned long Crc32_table[256] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};
#endif

#define assert(cond) do { \
    if (!(cond)) \
    { \
        syslog("##cond is not"); \
        exit(0); \
    } \
    else \
    { \
        ; \
    } \
}while(0)


#ifndef ORG_DIAG

#define NV_FILE               "/var/nvfile"
#define NV_TMP_FILE       "/var/nvoldfile"
#define NV_ITEM_LEN     			256
#define NV_ITEM_NOT_FOUND 		0
#define NV_ITEM_FOUND 			1


/*****************************************************************************
 �� �� ��  : file_set
 ��������  : ���nvram_set
 �������  : ��
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��9��6��
    ��    ��   : liuyang 65130
    �޸�����   : �����ɺ���

*****************************************************************************/
void file_set( char* pcKey,   char* pcValue)
{
	FILE* fs = NULL;
	FILE* fsTmp = NULL;
	char acBuf[NV_ITEM_LEN];
	char acKey[NV_ITEM_LEN/2];
	int iFlag = NV_ITEM_NOT_FOUND;

	memset(acBuf, 0, sizeof(acBuf));
	
	fs = fopen(NV_FILE, "r");
	if (!fs)
	{
		fs = fopen(NV_FILE, "wt+");
		if (!fs)
		{
			printf("Can't create file for nvfile\r\n");
			return;
		}

		sprintf(acBuf, "%s=%s\n", pcKey, pcValue);

		fputs(acBuf, fs);
		fclose(fs);
		return;
	}

	fsTmp = fopen(NV_TMP_FILE, "wt+");
	if (!fsTmp)
	{
		fclose(fs);
		return;
	}

	while (!feof(fs)) 
	{
		memset(acBuf, 0, sizeof(acBuf));

		if (!fgets(acBuf, NV_ITEM_LEN, fs))
		{
			continue;
		}

		sprintf(acKey, "%s=", pcKey);
		if (0 == strncmp(acKey, acBuf, strlen(acKey)))
             {
			sprintf(acBuf, "%s=%s\n", pcKey, pcValue);
			fputs(acBuf, fsTmp);
			iFlag = NV_ITEM_FOUND;
             }
		else
		{
			fputs(acBuf, fsTmp);
		}
	}
	
	if (NV_ITEM_NOT_FOUND == iFlag)
	{
		sprintf(acBuf, "%s=%s\n", pcKey, pcValue);
		fputs(acBuf, fsTmp);
	}

	fclose(fs);
	fclose(fsTmp);

	unlink(NV_FILE);
	rename(NV_TMP_FILE, NV_FILE);

}

/*****************************************************************************
 �� �� ��  : file_get
 ��������  : ���nvram_get
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��9��6��
    ��    ��   : liuyang 65130
    �޸�����   : �����ɺ���

*****************************************************************************/
char*  file_get(char* pcKey)
{
	FILE* fs = NULL;
	char acBuf[NV_ITEM_LEN];
	char acKey[NV_ITEM_LEN/2];
	int iFlag = NV_ITEM_NOT_FOUND;

	fs = fopen(NV_FILE, "r");
	if (!fs)
	{
		return NULL;
	}

	while(!feof(fs))
	{
		memset(acBuf, 0, sizeof(acBuf));

		if (!fgets(acBuf, NV_ITEM_LEN, fs))
		{
			continue;
		}

		sprintf(acKey, "%s=", pcKey);
		if (0 == strncmp(acKey, acBuf, strlen(acKey)))
             {
			iFlag = NV_ITEM_FOUND;
			break;
             }
	}

	fclose(fs);

	acBuf[strlen(acBuf) - 1] = '\0';

	if (NV_ITEM_NOT_FOUND == iFlag)
	{
		return NULL;
	}

	return acBuf+strlen(acKey);
}

#endif

#ifdef ORG_DIAG
/*****************************************************************************
 �� �� ��  : UPG_GetCrc
 ��������  : 
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2007��9��8��
    ��    ��   : liuyang 65130
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned int upgrade_bcm_getCrc( unsigned char* pcData, unsigned long ulSize, unsigned long ulCrc )
{
    while (ulSize-- > 0)
    {
      ulCrc = (ulCrc >> 8) ^ Crc32_table[(ulCrc ^ *pcData++) & 0xff];
    }
    
    return ulCrc;
}

/*****************************************************************************
 �� �� ��  : UPG_FlashImage
 ��������  : ����Image
 �������  : ��
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  : 
 ��������  : 
 �޸���ʷ      :
  1.��    ��   : 2008��3��29��
    ��    ��   : yangmuhan 00115589
    �޸�����   : �����ɺ���
*****************************************************************************/
 int upgrade_bcm_image(const char *pcImage, int ulImageLen )
{
    PFILE_TAG  pstTag;
    unsigned int ulNowGrade    = 0;
    ATP_UPG_EN enUpgState    = MAIN_F;
    char *pcKernelFs    = pcImage;
    int   bBootloader   = 0;
    unsigned int ulMainOffset  = 0;
    unsigned int ulSlaveOffset = 0;
    char*  pcTmp         = NULL;
    unsigned int ulLen = 0;
   unsigned int ulCrc = CRC32_INIT_VALUE;
    unsigned char  cCrc[CRC_LEN];
    unsigned int ImageLen = 0;	
    unsigned int ImageCrcLen = 0;

    syslog ("upgrade_bcm_image  ulImageLen=%d",ulImageLen);
    /*Start of comment :�����ļ�����С��У��crc�ĳ���*/
    if (NULL  == pcImage || 0x200000 > ulImageLen)
    {
        printf("pcImage is Illegal.\n");
        printf("\r\nUpgrade image failed. image file's empty.\r\n");
        return -1;
    }
    
   ImageLen=pcImage + ulImageLen -TOKEN_LEN ;
   ImageCrcLen=ulImageLen -TOKEN_LEN ; 	 
   memcpy((void *)cCrc , (void *)(ImageLen), 4);  

   ulCrc = upgrade_bcm_getCrc(pcImage, ImageCrcLen  , ulCrc); 

    if(((unsigned char)(ulCrc>>24) == cCrc[0]) && ((unsigned char)(ulCrc>>16) == cCrc[1]) 
    	&& ((unsigned char)(ulCrc>>8) == cCrc[2]) && ((unsigned char)(ulCrc) == cCrc[3]))
	{	 
		printf("\n crc check ok\n");
		BSP_FLASH_Write(pcImage, 0x20000, (ulImageLen-TOKEN_LEN));
		printf("\r\nwriting Main success flag:%s\r\n", IMAGE_MAIN_S);
		BSP_FLASH_WriteArea(BSP_E_FLASH_AREA_TEMP_PARAM,
		                        IMAGE_MAIN_S,
		                        0,
		                        UGPINFO_SIZE);
		 return 0;
	}
    else
	{
		printf("all crc error:ulCrc =  %x \r\n", ulCrc);
		return -1;
	}
	
}
    

/*---------------------------------------------------------------------------
  ����ԭ��:int nvram_restore_defaults(void)
  ��     ��:�˺������ڶ�ȡ��Ӫ��Ҫ���Ƶ���Դ�ļ��е�NV
                   �����nv ����.
  ��     ��:��.
  ��     ��:����Դ�ļ��е�nv���õ�mtd5��
  ����ֵ:-1 ����
                    0 ��ȷ
  ----------------------------------------------------------------------------*/
int nvram_restore_defaults(void)
{
    FILE *fp;
    int i = 0, value_len = 0;
    char *get_buf = NULL,  *nv_name = NULL, *nv_value = NULL;
     
    if ((fp = fopen("/var/custom/res_custom/vendor_nvram.res","r")) == NULL)
    {
        printf("cannot open file\n");
        return -1;
    }
    
    get_buf = (char*)malloc(MAX_NVLINESTR);
    if (get_buf == NULL)  
    {
        free(get_buf);
        get_buf = NULL;
        fclose(fp);
        return -1;
    }
     
    memset(get_buf, 0, sizeof(MAX_NVLINESTR));
    while (fgets(get_buf, MAX_NVLINESTR, fp) != NULL) 
    {
        nv_name = get_buf;
        
        while ((get_buf[i] != '=') && (get_buf[i] !='\0'))
        {
            i++;
        }
        
        nv_value = &get_buf[i + 2];
        get_buf[i] = '\0';
        value_len = strlen(nv_value) -3;
        nv_value[value_len] = '\0'; 
        
        nvram_set(nv_name,nv_value);
        
        memset(get_buf, 0, sizeof(MAX_NVLINESTR));
        i = 0;               
    }       
     
    free(get_buf); 
    get_buf =  NULL;
    fclose(fp); 
    return 0;
}
#endif

/*------------------------------------------------------------
  ����ԭ�� : int __msleep(int millisecond )
  ����     : ʵ���Ժ���Ϊ��λ�Ĳ������ѵĵȴ�
  ����     : millisecond(˯��ʱ��)
  ���     : ��
  ����ֵ   : OPER_SUCCESS: �ɹ�
-------------------------------------------------------------*/
static int __msleep(int millisecond) 
{
    int ret;
    struct timeval tvSelect;
    
    tvSelect.tv_sec  = millisecond / 1000;
    tvSelect.tv_usec = (millisecond % 1000) * 1000;

    while (1)
    {
        ret = select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0,
                    (struct timeval*)&tvSelect);
        if (ret < 0)
        {
            if (EINTR == errno)
            {
                DEBUGPRINT("interrupt by isr in sleep \n");
                continue;
            }
        }
        break;
    }

    return OPER_SUCCESS;
}

/*------------------------------------------------------------
����ԭ��: int daemon( int nochdir, int noclose )
����: ʵ��linux ��̨����
����:  int nochdir �� 1�� ���л���ǰĿ¼���� 0 �л�
       int noclose �� 1�� ���رձ�־���롢����������ļ�������� 0 �ر�
���:  ��
����ֵ: �ɹ�����0�����󷵻أ�1
-------------------------------------------------------------*/
int diag_daemon(int nochdir, int noclose )
{
    int fd = -1;

    switch ( fork() ) 
    {
        case -1:
    	        return OPER_FAILURE;
        case 0:
            break;
        default:
            exit(0);
    }

    if (setsid() == OPER_FAILURE)
    {
        return OPER_FAILURE;
    }

    if (!nochdir)
    {
        chdir("/");
    }
    
    if (!noclose && (fd = open(_PATH_DEVNULL, O_RDWR, 0)) != OPER_FAILURE) 
    {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > 2)
        {
            close(fd);
        }
    }
    
    return 0;
}

/*------------------------------------------------------------
  ����ԭ��: static int   init_listen_socket( usockaddr* usaP )
  ����: ���������׽���
  ����: �����׽��ֽṹ
  ���: �����׽��ֽṹ
  ����ֵ: �����׽���������
-------------------------------------------------------------*/
static int diag_init_tcp_listen_socket( usockaddr* uSockaddr_tcp )
{
    int listen_fd = -1;

    if (NULL == uSockaddr_tcp)
    {
        syslog("para error");
        return OPER_FAILURE;
    }
    
    memset( uSockaddr_tcp, 0, sizeof(usockaddr) );
    uSockaddr_tcp->sa.sa_family = AF_INET;
    uSockaddr_tcp->sa_in.sin_addr.s_addr = htonl( INADDR_ANY );
    uSockaddr_tcp->sa_in.sin_port = htons( SERVER_PORT );

    listen_fd = socket( uSockaddr_tcp->sa.sa_family, SOCK_STREAM, 0 );
    if ( listen_fd < 0 )
    {
        syslog ("create socket failure");
        return OPER_FAILURE;
    }
    if ( bind( listen_fd, &uSockaddr_tcp->sa, sizeof(struct sockaddr_in) ) < 0 )
    {
        syslog ("bind socket failure");
        close ( listen_fd );
        return OPER_FAILURE;
    }
    if ( listen( listen_fd, 1 ) < 0 )
    {
        syslog ("create listen socket failure");
        close ( listen_fd );
        return OPER_FAILURE;
    }
    return listen_fd; 
  }

/*------------------------------------------------------------
  ����ԭ��: static int diag_udpinit_socket( )
  ����: ��ʼ��udp�׽��֣���������PC�����⴮�ڲ����ʺ�״̬;
        ͬʱ��װ������Э��Ҳʹ������˿ڡ�
  ����: ��
  ���: ��
  ����ֵ: ��ʼ����ķ�������(������)UDP�׽���
-------------------------------------------------------------*/
static int diag_udpinit_socket( void )
{
    struct sockaddr_in servaddr;                                   //IPv4�׽ӿڵ�ַ����
    bzero (&servaddr, sizeof (servaddr) );                        //��ַ�ṹ����
    servaddr.sin_family        = AF_INET;                           //IPv4Э��
    servaddr.sin_addr.s_addr= htonl (INADDR_ANY);        //�ں�ָ����ַ
    servaddr.sin_port            = htons (g_Serverport);    //�˿�
    
    g_udpfd  = socket (AF_INET, SOCK_DGRAM, 0);       //����UDP�׽���
    if ( g_udpfd < 0 )
    {
        syslog("create socket failure");
        return OPER_FAILURE;
    }
    if (bind(g_udpfd , (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) /*����Э���ַ,�󶨶˿�*/
    {
        syslog("bind udp socket failure\n");
        close ( g_udpfd );
        g_udpfd = -1;
        return OPER_FAILURE;
    }
    return OPER_SUCCESS;
}


/*------------------------------------------------------------
  ����ԭ��: void close_pcui_shake()
  ����: �ر�ppp����
  ����: ��
  ���: ��
  ����ֵ: ��
-------------------------------------------------------------*/
void close_pcui_shake( )
{
    system ("killall pppc");
    system ("killall usbserver");
}

/*------------------------------------------------------------
  ����ԭ��: int open_usb_port()
  ����:  ��usb diag�˿�
  ����:  ��
  ���:  ��
  ����ֵ:�ɹ�����OPER_SUCCESS��ʧ�ܷ���OPER_FAILURE
-------------------------------------------------------------*/
int open_usb_port( void )
{
    char* pcReal_port = NULL;
    switch( g_ePort )
    {
        case _DIAG:
  	        pcReal_port = DIAG_PORT;
  	        break;
        case _PCUI:
  	        pcReal_port = PCUI_PORT;
  	        break;
        case _MODEM:
  	        pcReal_port = MODEM_PORT;
  	        break;
	 case _VOICE:
	 	 pcReal_port = VOICE_PORT;
        default:
  	        break;
    }

    if ((g_usbfd = open (pcReal_port, O_RDWR)) < 0)
    {
        if (errno == ENODEV )
        {
            syslog ( USB_ERR);
        } 
        else if (errno == ENODIAG)
        {
            syslog ( INTERFACE_ERR);
        }
        else
        {
            syslog ("%s \t ERROR CODE = [%d]", OTHER_ERR, errno);
        }
        return OPER_FAILURE;
    } 
    else 
    {
        syslog ( OPEN_SUCCESS );
        return OPER_SUCCESS;
    }
}

/*------------------------------------------------------------
  ������
  ����ԭ��: int net_to_diag()  
  ����: ת�����ݵ�Diag �ڣ�usb�豸д����ͷ��ء�����д����������
  ����:   ת����MSM����������
  ���:   ��
  ����ֵ: usb�豸д�������Ƿ�ɹ� 
          �ɹ�:OPER_SUCCESS
          ʧ��:OPER_FAILURE
-------------------------------------------------------------*/
int net_to_diag (unsigned char* buf,long len)
{
    int   send_len = 0;
    char  *pCur      = buf;
    int   retval     = OPER_SUCCESS;

    if (NULL == buf)
    {
        syslog("para error");
        return OPER_FAILURE;
    }
		
    /*���USB �ܹ���ȷ��д�ͻ�ѭ�����ͣ�ֱ�������ڽ��յ�����ȫ��
       ���ͳ�ȥ�����USB �˿ڲ��ܷ������˳�DIAG�ڷ�����ѭ��*/
    while ( len > 0 )
    {
        send_len = 0;
        send_len = write (g_usbfd, pCur, len);

        if (send_len > 0)
        {
            pCur += send_len;
            len -= send_len;
        }
        else if ((send_len < 0 ))
        {
            DEBUGPRINT("len=%ld errno=%d\n",len,errno);
            if (errno == EINTR)
            {
              continue;
            }
            else
            {
                close( g_usbfd );
                g_usbfd = -1;
                int  g_wait_usb = 0;
                struct stat s_stat;
                
                do
                {
                    if (( stat ( DIAG_PORT, &s_stat) == -1) 
                        || (open_usb_port() != OPER_SUCCESS))
                    { 
                        syslog( "Diag is Not exist !" ); 

                        __msleep(2000);
                        if ( ++g_wait_usb > 20)
                        {
                            syslog( "Can not open diag after 40s !" );
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }while(1);
                
                if ( -1 != g_usbfd )
                {
                    continue;
                }

               retval = OPER_FAILURE;
               break;
            }
        }
        else /*==0*/
        {
            ;/*empty sentence*/
        }
    }

    return retval;
}
/*------------------------------------------------------------
  ����ԭ��: char diag_to_net()
  ����: �ӽ��̴�diag�ڽ������ݣ�ת��������
  ����: ��
  ���: ��
  ����ֵ: ��
-------------------------------------------------------------*/
void diag_to_net ( void )
{
    char    rcv_buf [BUFFER_LEN]; //DIAG ����buffer
    int     rcv_len = 0;
    int     retval = OPER_SUCCESS;

    while (1)
    {
        rcv_len = read ( g_usbfd, rcv_buf, BUFFER_LEN-1);
        if ( (rcv_len < 0))
        {
            if ( errno == EINTR)
            {
            	  syslog("receive EINTR!");
                retval = OPER_SUCCESS;
                continue;
            }
        }
        else if(rcv_len == 0)
        {
            int  g_wait_usb = 0;
            struct stat s_stat;
            do
            {
                if (( stat ( DIAG_PORT, &s_stat) == -1) 
                    || (open_usb_port() != OPER_SUCCESS))
                { 
                    syslog( "Diag is Not exist !" ); 

                    __msleep(2000);
                    if ( ++g_wait_usb > 20)
                    {
                        syslog( "can not open diag after 40s !" );
                        break;
                    }
                }
                else
                {
                    break;
                }
            }while(1);
            
            if ( -1 != g_usbfd )
            {
                continue;
            }
        
            syslog ("Usb errno = [%d]", errno);
            retval = OPER_FAILURE;
            exit(0);
        }
        else
        {
            switch ( g_ePort )
            {
                case _MODEM:
                case _PCUI:
                    retval = echo_to_net (rcv_buf, rcv_len, 0);
                    break;
                case _DIAG:
                    retval = send_to_net (rcv_buf, rcv_len, 0);
                    break;
                default:
                    syslog("Usb port are not exist!");
                    break;
            }
            if( OPER_SUCCESS == retval )
            {
                g_net_alive = NET_TIMEOUT;
            }
        }
    }
}

/*------------------------------------------------------------
  ����ԭ��: int diag_accept_connect (int listen_fd)
  ����: ���նԷ�TCP������ȫ������g_netfd��ΪTCP�����׽���������
  ����: int listen_fd �����׽���
  ���: ��
  ����ֵ: �ɹ� ����OPER_SUCCESS
          ʧ�� ����OPER_FAILURE
-------------------------------------------------------------*/
int diag_accept_connect (int listen_fd )
{
    struct sockaddr_in client_addr;
    socklen_t socket_Len = sizeof(client_addr);

    if((g_netfd = accept (listen_fd, (struct sockaddr* )&client_addr, &socket_Len)) < 0)
    { 
        if (errno != EINTR)
        {
            syslog ("Tcp connect failure! errno = [%d]", errno);
        }
        else
        {
            __msleep (100);
        }
        return OPER_FAILURE;
    }
    g_current_tcp_port = ntohs(client_addr.sin_port);
    if ( INVALID_CONNECT == g_next_connect )
    {
        g_next_connect = VALID_CONNECT;
        return OPER_FAILURE;
    }
    return OPER_SUCCESS;
}

/*------------------------------------------------------------
  ����ԭ��: void tcp_timeout( )
  ����: 1��Ķ�ʱ�����ϻ�g_net_aliveֵ;Ϊ�ȴ�MSM��ʱ
  ����: ��
  ���: ��
  ����ֵ: ��
-------------------------------------------------------------*/
void tcp_timeout ( int sig )
{
    /*TCP_INIT_S,WAIT_CONNECT_S,WRITE_BCM_S cann't timeout tcp connection*/
    if (SIGALRM != sig || (TCP_INIT_S == g_state) 
         || (WAIT_CONNECT_S == g_state) || (WRITE_BCM_S == g_state))
    {
        return;
    }

    //ǿ������״̬������Ϊ�ȴ�MSM��ʱ�����ϻ�tcp����
    if ( FORCE_MSM_S == g_state )
    {
        g_waitMsmTimeout += 1;
        return;
    }
    
    if ( g_net_alive-- < 0 )
    {
        g_disconnect = TCP_TIMEOUT;/*disconnect tcp connection*/
    }
}

/*------------------------------------------------------------
  ����ԭ��: static void main_coordinate(int sig)
  ����: �������źŴ������
  ����: sig  �����ź�ֵ
  ���: ��
  ����ֵ: ��
-------------------------------------------------------------*/
static void main_coordinate ( int sig )
{
    syslog("Procedure %d recv Signal = %d", getpid (), sig );

    if (SIGPIPE == sig)
    {
        if ( NOT_UPGRADE != g_i_upgrade )
        {
            g_reboot_all = 1;/*reboot msm and bcm*/
        }
    }
    else if (SIGTERM == sig)
    {
        if ( -1 != g_usbfd )
        {
            close(g_usbfd);
            g_usbfd = -1;
        }

        if ( -1 != g_netfd )
        {
            close(g_netfd);
            g_netfd = -1;
        }

        if ( -1 != g_listen_fd )
        {
            close(g_listen_fd);
            g_listen_fd = -1;
        }
        exit(0);
    }
    else if (SIGUSR1 == sig)
    {
#ifdef ORG_DIAG 
        int  expect_udp_port = atoi(nvram_get(EXPECT_UDP_PORT));
#else
	  int  expect_udp_port = atoi(file_get(EXPECT_UDP_PORT));
#endif
	  
	  if (-1 != g_current_tcp_port)
        {
            if ((g_current_tcp_port - 1) <  expect_udp_port)
            {
                g_disconnect = UDP_NOTIFY_DISCONNECT;/*notify tcp need re-connect*/
                g_current_tcp_port = -1;
                g_next_connect = INVALID_CONNECT;
            }
            else
            {
               ;
            }
        }
    }
    else if (SIGCHLD == sig)
    {
        int  retval = 0;
        if( g_udp_pid == waitpid(-1, &retval, WNOHANG) )
        {
            syslog ("Exit main process due to udp server [%d] has down", g_udp_pid);
            exit(0);
        }
    }
    else
    {
        ;
    }

}

/*------------------------------------------------------------
  ����ԭ��: static void sub_coordinate(int sig)
  ����: �ӽ����źŴ������
  ����: sig  �����ź�ֵ
  ���: ��
  ����ֵ: ��
-------------------------------------------------------------*/
static void sub_coordinate ( int sig )
{
    syslog("Procedure %d recv Signal = %d", getpid (), sig );

    if ( (SIGPIPE == sig) || (SIGTERM == sig) )
    {
        if ( -1 != g_usbfd )
        {
            close(g_usbfd);
            g_usbfd = -1;
        }

        if ( -1 != g_netfd )
        {
            close(g_netfd);
            g_netfd = -1;
        }
        
        if ( -1 != g_listen_fd )
        {
            close(g_listen_fd);
            g_listen_fd = -1;
        }
        exit(0);
    }
}

/*------------------------------------------------------------
  ����ԭ��: static void udp_coordinate ( int sig )
  ����:     udp�ӽ����źŴ������
  ����:     sig  �����ź�ֵ
  ���:     ��
  ����ֵ:   ��
-------------------------------------------------------------*/
static void udp_coordinate ( int sig )
{
    syslog("Procedure %d recv Signal = %d", getpid (), sig );

    if ( (SIGPIPE == sig) || (SIGTERM == sig) )
    {
        if ( -1 != g_udpfd )
        {
            close(g_udpfd);
            g_udpfd = -1;
        }
        exit(0);
    }
}

/*------------------------------------------------------------
Function:     analyse_cmd
Description:����hdlc����payload���ҳ�����·��������������ģ�飬ǿ�����������������
Calls:           ��
Data Accessed: ��
Data Updated:   ��
Input:   
     unsigned char* buf ��ʾ�������Ҫ������hdlc payload.		
Output: 
     long *img_len: ���Ϊ����·����ģ�飬��ʾ·����ģ���ӳ�󳤶ȣ�
                    ���������������ͣ������塣
Return:
 	��������ֵΪBCM_UPGRADE����ʾ�յ�����·����ģ���������Խ��պ���·����image,����·�������������ˡ�
 	��������ֵΪMSM_UPGRADE����ʾ�յ���������ģ����������͸����������ӳ��usb���ˡ�
 	��������ֵΪMSM_FORCE_UPGRADE����ʾ�յ�ǿ����������ģ���������Է�������������ģ�飬͸�����յ���������ģ������image�ˡ�
 	��������ֵΪNOT_UPGRADE����ʾ����ģ��������������������������ֱ��͸����usb�ڡ�
Others:          ��
-------------------------------------------------------------*/
long analyse_cmd(unsigned char* buf,long *img_len)
{
    long retval=0;
    
    if (NULL == buf || NULL == img_len)
    {
        syslog("impossible,para error");
        return OPER_FAILURE;
    }
    
    if (UPGRADE_CMD_TYPE != *buf)
    {
        retval = NOT_UPGRADE;
    }
    else if (0 == strncmp(MSM_UPGRADE_STR,buf+1,29))
    {
        DEBUGPRINT(" FIND MSM_UPGRADE cmd\n");
        retval = MSM_UPGRADE;
    }
    else if (0 == strncmp(MSM_FORCE_UPGRADE_STR,buf+1,29))
    {
        DEBUGPRINT(" FIND MSM_FORCE_UPGRADE_STR cmd\n");
        retval = MSM_FORCE_UPGRADE;
    }
    else if (0 == strncmp(BCM_UPGRADE_STR,buf+1,29))
    {
        DEBUGPRINT(" FIND BCM_UPGRADE_STR cmd\n");
        
        /*get bcm image len after 1 byte cmd_type and len of BCM_UPGRADE_STR*/
        buf = buf+1+strlen(BCM_UPGRADE_STR);
        
        *img_len = *(buf+3) * (1<<24) 
                  + *(buf+2) * (1<<16) 
                  + *(buf+1) * (1<<8) 
                  + *(buf);
        DEBUGPRINT("bcm_img_len=%ld\n",*img_len);
        retval = BCM_UPGRADE;
    }
    else
    {
        ;//empty sentence
    }
    return retval;
}

#ifdef ORG_DIAG
extern unsigned int UPG_FlashImage(const unsigned char *pcImage, unsigned int ulImageLen);
/*------------------------------------------------------------
  ����ԭ��: static EN_UPGRADE_ERRCODE route_fwt_upgrade(char *part_img, long len, long *total)
  ����:     ���������𽫽��ܵ�tcp����д���²㺯��
  ����:     ���ܵ�����
  ���:     ·����ӳ��ʣ�೤��
  ����ֵ:   ������������
-------------------------------------------------------------*/
static int route_fwt_upgrade(char *part_img, long img_len)
{
   return upgrade_bcm_image(part_img,img_len);
}
#endif
/*------------------------------------------------------------
  ����ԭ��: void udp_server_main(pid_t parent_pid)
  ����:     ��һ���ӽ��̣�����udp��1280�˿ڣ�����virtual console�Ͷ�˿�Э�̱���
  ����:     �����̵Ľ��̺ţ����ڷ��źŷ���tcp������Ϣ
  ���:     ��
  ����ֵ:   ��
-------------------------------------------------------------*/
void udp_server_main(pid_t parent_pid)
{
    fd_set rfds;
    struct timeval tv;
    int retval = 0; 
    tv.tv_sec  = _interval;
    tv.tv_usec = UDP_SRV_TIMEOUT;

    struct sockaddr_in client_udp_addr;
    socklen_t socket_Len = sizeof(client_udp_addr);
    char    udp_port[10] = {0};

#ifndef FEATURE_E960
    sigset_t sigmask, orig_sigmask;
#endif

    signal (SIGTERM,  udp_coordinate);
    
    if ( diag_udpinit_socket() !=  OPER_SUCCESS)
    {
        syslog ("Udp initialize failed!");
        exit(0);
    }
    else
    {
        syslog ("Udp initialize successful");
    }

    DEBUGPRINT(" g_udpfd=%d parent_pid=%d\n",g_udpfd,parent_pid);

    while (1)
    {
        bzero ( g_udp_buf, MAX_NEGRO_LEN );
        FD_ZERO ( &rfds );
        if ( g_udpfd > 0 )
        {
            FD_SET ( g_udpfd,  &rfds );
        }
        else
        {
            syslog("g_udpfd become error value %d!",g_udpfd);
            exit(0);
        }
        
        tv.tv_sec  = _interval;
        tv.tv_usec = UDP_SRV_TIMEOUT;
        retval = select ( g_udpfd + 1, &rfds, NULL, NULL, &tv);

        if (0 >= retval) 
        {
            continue;
        }
        
        if (FD_ISSET (g_udpfd, &rfds))
        {
            retval = recvfrom (g_udpfd, g_udp_buf, MAX_NEGRO_LEN, 0,\
                      (struct sockaddr *)&client_udp_addr, &socket_Len);

            //�������
            if (retval < 0)
            {
                syslog("udp recvfrom fail, g_udpfd=%d errno=%d", g_udpfd, errno);
                continue;
            }


           retval = multi_upgrade_nego(g_udpfd, g_udp_buf, retval, &client_udp_addr);


           if ( (0 == retval) || (EQUIP_RUN == g_i_upgrade))
           {
                continue;
           }

            /*���������udp����Դ�˿ڱ仯�ˣ�֪ͨ��������tcp*/          
            if ( -1 != g_current_udp_port && ntohs (client_udp_addr.sin_port) != g_current_udp_port)
            {
                sprintf(udp_port, "%d", g_current_udp_port);
#ifdef ORG_DIAG		
                nvram_set(EXPECT_UDP_PORT, udp_port);
#else
		   file_set(EXPECT_UDP_PORT, udp_port);
#endif
		   kill (parent_pid,SIGUSR1);
            }

            g_current_udp_port = ntohs (client_udp_addr.sin_port);
            DEBUGPRINT(" recvfrom udp_port:%d\n",g_current_udp_port);

            PNETJET_FRAME  pFrame = (PNETJET_FRAME) g_udp_buf;
            if ( pFrame )
            {
                switch ( pFrame->Type )
                {
                    case NETJET_COMMAND_OPEN:
                    case NETJET_COMMAND_SET_LINE_CONTROL:
                    case NETJET_COMMAND_SET_BAUD_RATE:
                    case NETJET_COMMAND_SET_HARDLINE:
                    case HARDLINE_CLRDTR:
                    case HARDLINE_CLRRTS:
                    case HARDLINE_SETDTR:
                    case HARDLINE_SETRTS:
                    case HARDLINE_SETBREAK:
                    case HARDLINE_CLRBREAK:
                    case HARDLINE_RESETDEV:
                        break;
                    case NETJET_COMMAND_CLOSE:
                        syslog ("The peer close socket own to virtual serial disconnect!");
                        sprintf(udp_port, "%d", g_current_udp_port + 1);
#ifdef ORG_DIAG
                        nvram_set(EXPECT_UDP_PORT, udp_port);
#else
			     file_set(EXPECT_UDP_PORT, udp_port);
#endif
                        kill (parent_pid, SIGUSR1);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return ;
}

/*------------------------------------------------------------
Function:      parse_nego_cmd
Description:   ��˿������У���������Э���������ݡ�
Calls:         ��
Data Accessed: ��
Data Updated:  ��
Input:         unsigned char *buf��udp payload�Ĵ��buf
 	           int len��udp payload�ĳ���
Output:        char*   report_buf������ʹ�õ���ֵ����hdlc��ͷβ��
               ip_changeʱ��ip��ַ�ȣ���ŵ����棬�Թ�report ��pcʱʹ�á�
Return:        Э����������
Others:        ��
-------------------------------------------------------------*/
nego_state parse_nego_cmd (unsigned char *buf, int len, unsigned char *report_buf)
{
    long out_len = 0;
    int retval = 0; 

    if (NULL == buf || NULL == report_buf)
    {
        syslog("parameter error");
        return EQUIP_INVALID;
    }
    
    retval = parse_udp_payload( buf, len, report_buf, &out_len);
    if((OPER_SUCCESS != retval)
        || (out_len - 2 != sizeof (s_report_data)))    /*not get complete hdlc payload*/
    {
        return EQUIP_INVALID;
    }

    return   report_buf[0];
}

/*------------------------------------------------------------
  ����ԭ��: int  diagd_ip_set() 
  ����:    ���������� ��������IP��ַ
  ����:     ��
  ���:     ��
  ����ֵ:   ��
-------------------------------------------------------------*/
void  diagd_ip_set() 
{
    char *value = NULL;
	char  buf[50];
#ifdef ORG_DIAG 
   value = nvram_get("lan_ipaddr");
#else
   value = file_get("lan_ipaddr");
#endif


	printf("\ndiagd_ip_set value = %s\n",value);
	if((NULL != value) && (value[0] != '\0'))
	{
		memset(buf,0,50);
		sprintf(buf,"ifconfig br0 %s",value);
		system(buf);
	}
}

/*------------------------------------------------------------
  ����ԭ��: int ip_change(char * Reportbuf)
  ����: ����IP
  ����: char * Reportbuf, ����Ҫ���ĵ�IP��ַ
  ���: ��
  ����ֵ: ���ĳɹ����� : OPER_SUCCESS 
                  ʧ�� : OPER_FAILURE
-------------------------------------------------------------*/
int ip_change(char * Reportbuf)
{
    if (NULL == Reportbuf)
    {
        syslog("para error");
        return OPER_FAILURE;
    }
    
    s_report_data*  pReport = (s_report_data *)Reportbuf;
    char ip_addr[17];
	memset(ip_addr,0,17);

    sprintf(ip_addr, "%d.%d.%d.%d", pReport->ip[0], pReport->ip[1],
        pReport->ip[2],pReport->ip[3]);
  
#ifdef ORG_DIAG
    nvram_set("lan_ipaddr", ip_addr);
    nvram_commit();
#else
    file_set("lan_ipaddr", ip_addr);
#endif
    DEBUGPRINT("The gateway ip address have success to change to %s\n", ip_addr);

	diagd_ip_set();
    return OPER_SUCCESS;
}

/*------------------------------------------------------------
  ����ԭ��: int report_to_pc(int sock_fd, char* report_buf, struct sockaddr_in* pClientSock)
  ����:     ʹ��hdlc���ģ���udpЭ�̵�ִ�н�������PC
  ����:     int sock_fd                      UDP �׽��� 
            char* report_buf                 UDPЭ������buf��û��ת��ģ�������ȷ����
            struct sockaddr_in* pClientSock  PC��IP��ַ
  ���:     ��
  ����ֵ:   �ϱ��ɹ�����OPER_SUCCESS��ʧ�ܷ���OPER_FAILURE
-------------------------------------------------------------*/
int report_to_pc(int sock_fd, char* report_buf, struct sockaddr_in* pClientSock)
{
    int   iNet_sLen = 0;
    long   hdlc_len=0;
    char  hdlc[MAX_CMD_TRANS_LEN] = {0},*phdlc=hdlc;
    socklen_t socket_Len = sizeof(struct sockaddr_in);

    if (NULL == report_buf || NULL == pClientSock)
    {
        syslog("para error");
        return OPER_FAILURE;
    }

    hdlc_len = gen_hdlc_packet(report_buf, sizeof(s_report_data), phdlc);
    if ( -1 == hdlc_len )
    {
        syslog("gen_hdlc_packet fail");
        return OPER_FAILURE;
    }
    
    while (hdlc_len != 0)
    {
        iNet_sLen = sendto(sock_fd, phdlc, hdlc_len, 0, (struct sockaddr *)pClientSock, socket_Len);                

        if (iNet_sLen == SOCKET_ERROR && errno != EINTR)
        {
            syslog("failure when report status to pc %d", errno);
            return OPER_FAILURE;
        }

        hdlc_len -= iNet_sLen;
        phdlc  += iNet_sLen;
    }
    return OPER_SUCCESS;
}

/*------------------------------------------------------------
  ����ԭ��: int report_upgrade_result(int sock_fd, int result)
  ����:     �ϱ�����·����ģ��Ľ����PC    
  ����:     tcp socket��������
            result: OPER_SUCCESS��ʾ�����ɹ�,OPER_FAILURE��ʾ����ʧ��
  ���:     ��
  ����ֵ:   ��
-------------------------------------------------------------*/
void report_upgrade_result(int sock_fd, int result)
{
    char payload[UPGRADE_RESULT_PAYLOAD_LEN] = {0};
    char hdlc[UPGRADE_RESULT_PAYLOAD_LEN*2] = {0},*phdlc=hdlc;
    long hdlc_len = 0,iNet_sLen = 0;
    
    payload[0] = REPORT_CMD_WORD;
    memcpy(payload+CMD_WORD_LEN,BCM_UPGRADE_STR,strlen(BCM_UPGRADE_STR));

    if ( OPER_SUCCESS == result )
    {
        payload[CMD_WORD_LEN + strlen(BCM_UPGRADE_STR)] = UPGRADE_RESULT_SUCCESS;
    }
    else
    {
        payload[CMD_WORD_LEN + strlen(BCM_UPGRADE_STR)] = UPGRADE_RESULT_FAILURE;
    }
    
    hdlc_len = gen_hdlc_packet(payload, UPGRADE_RESULT_PAYLOAD_LEN, phdlc);
    if ( -1 == hdlc_len )
    {
        syslog("gen_hdlc_packet fail");
        return;
    }

    while (hdlc_len != 0)
    {
        iNet_sLen = send(sock_fd, phdlc, hdlc_len, 0);                

        if (iNet_sLen == SOCKET_ERROR ||iNet_sLen == 0)
        {
            syslog("Can't report upgrade result!");
            return;
        }

        hdlc_len -= iNet_sLen;
        phdlc    += iNet_sLen;
    }
    return;
}

/*------------------------------------------------------------
  ����ԭ��: int  ip_restore() 
  ����:     ��˿�������ɺ�ָ�����IP��ַ
  ����:     ��
  ���:     ��
  ����ֵ:   ��
-------------------------------------------------------------*/
void  ip_restore() 
{
	system("ifconfig br0 192.168.1.1");
#ifdef ORG_DIAG
	nvram_set("lan_ipaddr", "192.168.1.1");
	nvram_commit();
#else
	file_set("lan_ipaddr", "192.168.1.1");
#endif

    return ;
}



/*------------------------------------------------------------
  ����ԭ��: static inline unsigned char str2hexnum(unsigned char c)
  ����:     ���ַ�ת��������
  ����:     �ַ�
  ���:     ��
  ����ֵ:   ����
-------------------------------------------------------------*/
static inline unsigned char str2hexnum(unsigned char c)
{
    if(c >= '0' && c <= '9')
       return c - '0';
    if(c >= 'a' && c <= 'f')
       return c - 'a' + 10;
    
    if(c >= 'A' && c <= 'F')
       return c - 'A' + 10;

    return 0; /* foo */
}

/*------------------------------------------------------------
  ����ԭ��: static inline void str2eaddr(unsigned char *ea, unsigned char *str)
  ����:     ���ַ���ת��Ϊmac��ַ
  ����:     �ַ�����ʽ��mac��ַ  
  ���:     ������ʽ��mac��ַ
  ����ֵ:   ��
-------------------------------------------------------------*/
static inline void str2eaddr(unsigned char *ea, unsigned char *str)
{
    int i;
	
    if (NULL == ea || NULL == str)
    {
        syslog("para error");
        return ;
    }

    for (i = 0; i < 6; i++) {
        unsigned char num;

        if(*str == ':')
        {
            str++;
        }
        num = str2hexnum(*str++) << 4;
        num |= (str2hexnum(*str++));
        ea[i] = num;
    }
}

/*------------------------------------------------------------
  ����ԭ��: static void str2ipaddr(const char *str, unsigned char *ip)
  ����:     ��"192.168.1.1"��ʽ��ip��ַת��������֣�����4�ֽڵ�����
  ����:     ��ת�����ַ���
  ���:     4�ֽ�����
  ����ֵ:   �ɹ� : OPER_SUCCESS
            ʧ�� : OPER_FAILURE
-------------------------------------------------------------*/
static int str2ipaddr(const char *str, unsigned char *ip)
{
    if (NULL == str || NULL == ip)
    {
        syslog("para error!");
        return OPER_FAILURE;
    }
    
    int split[4];
    
    sscanf(str, "%d.%d.%d.%d", &split[0], &split[1], &split[2], &split[3]);

    if ( split[0]>255 || split[1]>255 || split[2]>255 || split[3]>255 )
    {
        syslog("Invalid IP address");
        return OPER_FAILURE;
    }

    ip[0] = split[0] & 0xff;
    ip[1] = split[1] & 0xff;
    ip[2] = split[2] & 0xff;
    ip[3] = split[3] & 0xff;

    return OPER_SUCCESS;
}

/*------------------------------------------------------------------------------
  ����ԭ��: int scratch_ip_mac(char* report_buf)
  ����:     ��nvram�л�ȡip��mac��ַ
  ����:     ��
  ���:     ��õ�ip��mac��ַ
  ����ֵ:   �����ɹ�����OPER_SUCCESS��ʧ�ܷ���OPER_FAILURE
/*ͨ��ifconfig����ȡMAC��IP��ַ���ʽ����
* br0     Link encap:Ethernet  HWaddr 00:11:22:33:AB:DE
*           inet addr:192.168.1.3  Bcast:192.168.1.255  Mask:255.255.255.0
*           UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
            RX packets:0 errors:0 dropped:0 overruns:0 frame:0
            TX packets:71 errors:0 dropped:0 overruns:0 carrier:0
            collisions:0 txqueuelen:0
            RX bytes:0 (0.0 B)  TX bytes:14220 (13.8 KiB)
																																							*/
/*----------------------------------------------------------------------------*/

int scratch_ip_mac(char* report_buf)
{
	FILE* diagd_tmp_file = NULL;
	char rcv_buf[600];
	char tempBuf[50];
	char *cpread = NULL,*head = NULL,*temp = NULL;
	int retval;
	int i =0;

	memset(rcv_buf,0,600);
	s_report_data*  pReport = (s_report_data *)report_buf;	
	
	system("ifconfig br0 > var/tmp/diagd_macip.txt");
	__msleep(10);
	diagd_tmp_file  = fopen("var/tmp/diagd_macip.txt","a+");
	if(NULL == diagd_tmp_file)
	{
		return OPER_FAILURE;
	}
	retval = fread(rcv_buf, 600, 1, diagd_tmp_file);	
	head = rcv_buf;
	cpread = strstr(head,"HWadd");
	memset(tempBuf,0,50);
	cpread = cpread+strlen("HWadd")+2;
	memcpy(tempBuf,cpread,17);
	str2eaddr(pReport->mac,tempBuf);
	head = cpread +17;
	cpread = strstr(head,"inet addr:");
	temp = strstr(head,"Bcast");
	cpread = cpread+strlen("inet addr:");
	memset(tempBuf,0,50);
	memcpy(tempBuf,cpread,temp-cpread);
	retval = str2ipaddr(tempBuf,pReport->ip);
	fclose(diagd_tmp_file);
	system("rm -f var/tmp/diagd_macip.txt");
	if(-1 == retval)
	{
		printf("\r\n IP :%s,retval = %d\r\n",tempBuf,retval);
		return OPER_FAILURE;
	}

	return OPER_SUCCESS;
	
}

/*------------------------------------------------------------
  ����ԭ��: int mac_match(char*localmac, char * reqmac)
  ����:     ����ַ�Ƿ�ƥ��
  ����:     ��
  ���:     ��
  ����ֵ:  ƥ��ɹ�����OPER_SUCCESS��ƥ��ʧ�ܷ���OPER_FAILURE
-------------------------------------------------------------*/
int mac_match(char*localmac, char * reqmac)
{
     char  zero[6] = {0};
    if ( ((localmac[0] != reqmac[0])
        || (localmac[1] != reqmac[1])
        || (localmac[2] != reqmac[2])
        || (localmac[3] != reqmac[3])
        || (localmac[4] != reqmac[4])
        || (localmac[5] != reqmac[5]))
        && (0 != memcmp(reqmac, zero, 6)))
    {
        return OPER_FAILURE;
    }
    else
    {
        return OPER_SUCCESS;
    }
}

/*------------------------------------------------------------
  ����ԭ��: int multi_upgrade_nego(int udp_fd,unsigned char *buf,
            int len, struct sockaddr_in* client_udp_addr )
  ����:     װ������Э�̵������򡣷���udp�е�hdlc��ʽ���ִ����Ӧ����������ִ�����
  ����:     udp����payload����udp�˿��������Լ���ַ��
  ���:     ��
  ����ֵ:   0: ��ʾ��ǰUDP ���Ƕ�˿��������ݰ���-1 : ��ʾ���⴮�ڵ����ݰ�
-------------------------------------------------------------*/
int multi_upgrade_nego(int udp_fd,unsigned char *buf,int len, 
    struct sockaddr_in* client_udp_addr )
{
    static nego_state  nego_state = EQUIP_INVALID;

    /*�����nv��ȡ�õ�ip/mac��*/
    s_report_data     report_buf;
    s_report_data     struct_report;
    int   is_equip_pkg = -1;
    struct stat s_stat;
    int   reported = 0;
    int   index  = -1;

    if (NULL == buf || NULL == client_udp_addr)
    {
        syslog("para error!");
        return -1;
    }

    /*parse_net_package return machine state value*/
    nego_state = parse_nego_cmd(buf, len,(unsigned char*)&report_buf);
    if( EQUIP_INVALID != nego_state )
    {        
        is_equip_pkg = 0;
    }
    else
    {
        return -1;
    }
    
    /*���report_buf�е�mac��ַ���ж��Ƿ�Ϊ�Լ���mac��գ�����ǲŸı�ip�����򷵻�*/
    scratch_ip_mac((char*)&struct_report);
    if(OPER_FAILURE == mac_match(struct_report.mac, report_buf.mac))
    {
        syslog("Is not my Mac address, do not change anything");
        return -1;
    }

    g_i_upgrade = EQUIP_RUN;
    switch (nego_state)
    {
        case EQUIP_IP_MAC_REQUERY:
            DEBUGPRINT("Enter EQUIP_IP_MAC_REQUERY\n");
            /*��ȡ����汾*/
            index = scratch_ip_mac( (char*)&report_buf );
            if (OPER_FAILURE == index)
            {
                syslog("Multi upgrad: mac scratch failed!");
                break;
            }
            break;
			
#ifdef ORG_DIAG
        case EQUIP_IP_CHANGE:                    
            DEBUGPRINT("Enter EQUIP_IP_CHANGE \n");
            //�޸�ip��ַ֮�󣬲���ͨ��udp_fd��pc���档�����ȱ��棬���޸�ip    
            if( OPER_SUCCESS != report_to_pc(udp_fd, (char*)&report_buf, client_udp_addr))
            {
                syslog("Run Udp command %d failed!", nego_state);
            }
            else
            {
                syslog("Run Udp command %d success!", nego_state);
            }
            reported = 1;
            if (OPER_SUCCESS == ip_change((char*)&report_buf))
            {
                syslog("Multi upgrade: ip change success");
            }
            else
            {
                syslog("impossible, ip change failed");
            }
            break;
			

        case EQUIP_IP_RESTORE:
            DEBUGPRINT(" enter EQUIP_IP_RESTORE\n");
            /*�յ�IP������Ժ��Ȼָ����ϱ�*/
            ip_restore();

            /*��ȡip��mac��ַ*/
            scratch_ip_mac((char*)&report_buf);

            /*��ȡ����汾*/
            memset (report_buf.version, 0, sizeof (report_buf.version));
            break;
#endif

        case EQUIP_RESTART_DIAGD:
             kill( getpid(), SIGTERM );            
            break;
			
#ifdef ORG_DIAG
        case EQUIP_REBOOT_GW:
             if ( OPER_SUCCESS != report_to_pc(udp_fd, (char*)&report_buf, client_udp_addr) )
             {
                syslog("Multi upgrade: reboot report failed");
             }
             else
             {
                syslog("Multi upgrade: reboot report success");
             }
             reported = 1;
			 
/* <BB5D00010.VOICE: yangjianping 2008-8-4 MODIFY BEGIN */
            BSP_SYS_SoftReboot();
/* BB5D00010.VOICE: yangjianping 2008-8-4 MODIFY END> */
		break;
#endif
        case EQUIP_RESTORE_GW:
					/*�ָ�NV*/
					/*system( "restoredefault" );     */
            break;
					
#ifdef ORG_DIAG
        case EQUIP_ENFORCE_DOWN:

				/*���ú���,����GPIO��,ʹ������ģ�鴦��ǿ������״̬*/
				BSP_MSM_Force_On();

				/*����GPIO��,������������ģ��;*/
				BSP_MSM_Off();
				__msleep(1000);
				BSP_MSM_On();
    
            break;

        case EQUIP_EXIT_ENFORCE_DOWN:
            /*���ú���,����GPIO��,ʹ������ģ�鴦���˳�ǿ������״̬*/
            //sb_enforce_down();
            BSP_MSM_Force_Off();
            
            break;

        case EQUIP_RESTART_MSM:
			/*����GPIO��,������������ģ��;*/
			BSP_MSM_Off();
			__msleep(1000);
			BSP_MSM_On();
            break;
#endif
        case EQUIP_DIAG_STATUS:
            if (stat ( DIAG_PORT, &s_stat) == -1) 
            { 
                syslog( "usb diag port is down!" ); 
                report_buf.reserved[0] = 1;
            }
            else
            {
                syslog( "usb diag port is exist!" ); 
                report_buf.reserved[0] = 0;
            }
            break;
			
#ifdef ORG_DIAG            
        case EQUIP_DOWNLOAD:
            close_pcui_shake();
            break;
#endif

        default:
            DEBUGPRINT(" enter default\n");
            break;
    }

    if( 1 != reported )
    {        
        if( OPER_SUCCESS != report_to_pc(udp_fd, (char*)&report_buf, client_udp_addr))
        {
            syslog("Run Udp command %d failed!", nego_state);
        }
        else
        {
            syslog("Run Udp command %d success!", nego_state);
        }
    }
    
    return is_equip_pkg;
}

/*------------------------------------------------------------
  ����ԭ��: void gen_hdlc_packet(char* payload,long size,char cmdWord,char* hdlc)
  ����:     ����hdlc���ģ�����crc��ת��payload�е�0x7e��0x7d�ֽڡ�
  ����:     hdlc����payload
  ���:     char* hdlc�еı���
  ����ֵ:   ������ȷ������hdlc���ĳ��ȣ�ת��֮���п��ܱȱ�ñȽϳ�
            �������󣬷���-1
-------------------------------------------------------------*/
long gen_hdlc_packet(char* payload,long size,char* hdlc)
{
    long off=0;
    /* CRC in its initial state */
    unsigned short  crc = CRC_16_L_SEED;

    char one_char=0;

    if (NULL == payload || NULL == hdlc)
    {
        syslog("para error!");
        return -1;
    }

    hdlc[0] = ASYNC_HDLC_FLAG;
    hdlc++;

    long tmp = 0;
    for (off = 0, tmp = 0; off < size; off++)
    {
        one_char = *( payload + off );

        if (ASYNC_HDLC_FLAG == one_char
          || ASYNC_HDLC_ESC == one_char)/*need translate*/
        {
            hdlc[tmp++] = ASYNC_HDLC_ESC;
            hdlc[tmp++] = one_char ^ ASYNC_HDLC_ESC_MASK;
            crc = CRC_16_L_STEP (crc, (unsigned short)one_char);/* Update the CRC     */
        }
        else
        {
            hdlc[tmp++] = one_char;
            crc = CRC_16_L_STEP (crc, (unsigned short)one_char);/* Update the CRC     */
        }
    } 
    off = tmp;

    crc = ~crc;
    unsigned char halfcrc = crc & 0xFF;
    
    if (ASYNC_HDLC_FLAG == halfcrc
      || ASYNC_HDLC_ESC == halfcrc)/*need translate*/
    {
        hdlc[off++] = ASYNC_HDLC_ESC;
        hdlc[off++] = halfcrc ^ ASYNC_HDLC_ESC_MASK;
    }
    else
    {
        hdlc[off++] = halfcrc;
    }
    
    halfcrc = crc >> 8;
    
    if (ASYNC_HDLC_FLAG == halfcrc
      || ASYNC_HDLC_ESC == halfcrc)/*need translate*/
    {
        hdlc[off++] = ASYNC_HDLC_ESC;
        hdlc[off++] = halfcrc ^ ASYNC_HDLC_ESC_MASK;
    }
    else
    {
        hdlc[off++] = halfcrc;
    }
        
    hdlc[off] = ASYNC_HDLC_FLAG;

    return off+2;//hdlc��ָ���Լ���1���������СΪoff+2
}

/*------------------------------------------------------------
  ����ԭ��: char parse_tcp_payload (unsigned char *buf,long len, 
  ����:     ����tcp�з�װ��һ��hdlc����
  ����:     ��Ҫparse��tcp payload
  ���:     �������ĺϷ�hdlc payload
  ����ֵ:   �����һ��������hdlc payload ����OPER_SUCCESS��
            �������󡢵���bufĩβȴû�л��һ��������hdlc payload������OPER_FAILURE
-------------------------------------------------------------*/
int parse_tcp_payload (unsigned char *buf,long len, 
    unsigned char* out_buf,long* out_len)
{
    unsigned char  chr;
    unsigned short crc = 0;
    hdlc_state state = HDLC_HUNT_FOR_FLAG;
    long off=0;
    if ( len == 0 )
    {
        DEBUGPRINT("no data!\n");
        return OPER_FAILURE;        
    }
    
    if ((NULL == buf) || (NULL == out_buf) || (NULL == out_len))
    {
        assert("para error\n");
        return OPER_FAILURE;
    }
    
    //reset out len,avoid caller maintain this
    *out_len = 0;
    
    DEBUGPRINT("enter\n");

    /* Look at characters and try to find a valid async-HDLC packet of
    ** length at least MIN_PACKET_LEN with a valid CRC.
    ** Keep looking until we find one, or to the end of buf. */
    for (state = HDLC_HUNT_FOR_FLAG; state != HDLC_PACKET_RCVD; /* nil */)
    {
        chr=buf[off++];/*get a char,off value increment*/

        switch( state )                  /* Process according to which state */
        {
            /*lint -esym(788,HDLC_PACKET_RCVD)  No need to deal with HDLC_PACKET_RCVD
            since we're in a loop that guarantees we're not in that state. */
            case HDLC_HUNT_FOR_FLAG:            /* We're looking for a flag ... */
                if (chr == ASYNC_HDLC_FLAG)     /*   and we got one ...         */
                {
                    state = HDLC_GOT_FLAG;      /*   so go on to the next step. */
                }
                break;

            case HDLC_GOT_FLAG:                   /* Had a flag, now expect a packet */
                if (chr == ASYNC_HDLC_FLAG)    /* Oops, another flag.  No change. */
                {
                    break;
                }
                else
                {                                                    /* Ah, we can really begin a packet */
                    crc = CRC_16_L_SEED;               /* and the CRC in its initial state */
                    state = HDLC_GATHER;               /* and we begin to gather a packet  */
                }
            /*pass through*/
            case HDLC_GATHER:                         /* We're gathering a packet      */
                if (chr == ASYNC_HDLC_FLAG)       /* We've reached the end         */
                {
                    if (*out_len < MIN_PACKET_LEN)        /* Reject any too-short packets  */
                    {
                        state = HDLC_HUNT_FOR_FLAG;           /* Start over     */
                    }
                    else  /* Yay, it's a good packet! */
                    {
                        DEBUGPRINT("parse ok\n");
                        state = HDLC_PACKET_RCVD;                    /* Done for now   */
                        return OPER_SUCCESS;     /*return 0: get complete hdlc payload */
                    }
                    break;           /* However it turned out, this packet is over.  */
                }

                /* It wasn't a flag, so we're still inside the packet. */
                if (chr == ASYNC_HDLC_ESC)               /* If it was an ESC       */
                {
                    chr=buf[off++];/*get next char,off value increment*/

                    chr ^= ASYNC_HDLC_ESC_MASK;       /* de-mask it  */
                    
                    out_buf[ (*out_len)++ ] = chr;                    /* Add byte to out_buf */

                    crc = CRC_16_L_STEP (crc, (unsigned short) chr);      /* Update the CRC     */
                    /* No break; process the de-masked byte normally */
                }
                else
                {
                    out_buf[ (*out_len)++ ] = chr; 
                    crc = CRC_16_L_STEP (crc, (unsigned short) chr);      /* Update the CRC     */
                }
                break;

            default:       /* Shouldn't happen with an enum, but for safety ...  */
                //DEBUGPRINT("enter default\n");
                state = HDLC_HUNT_FOR_FLAG;             /* Start over         */
                break;
        }/* switch on state */
        if  ( off >= len )
        {
            return OPER_FAILURE;/*reach to the end of buf*/
        }
    }
  
    return OPER_FAILURE;/*return uncomplete*/
} /* parse_tcp_payload() */

/*------------------------------------------------------------
  ����ԭ��: char parse_udp_payload (unsigned char *buf,long len, 
  ����:     ����udp�з�װ��һ��hdlc����
  ����:     ��Ҫparse��udp payload
  ���:     �������ĺϷ�hdlc payload
  ����ֵ:   �����һ��������hdlc payload ����OPER_SUCCESS��
            �������󡢵���bufĩβȴû�л��һ��������hdlc payload������OPER_FAILURE
-------------------------------------------------------------*/
long parse_udp_payload (unsigned char *buf,long len, 
    unsigned char* out_buf,long* out_len)
{
    unsigned char  chr;
    unsigned short crc = 0;
    hdlc_state state = HDLC_HUNT_FOR_FLAG;
    long off=0;
    
    if ((NULL == buf) || (NULL == out_buf) || (NULL == out_len))
    {
        syslog("para error\n");
        return OPER_FAILURE;
    }

    /* Look at characters and try to find a valid async-HDLC packet of
    ** length at least MIN_PACKET_LEN with a valid CRC.
    ** Keep looking until we find one, or to the end of buf. */
    for (state = HDLC_HUNT_FOR_FLAG; state != HDLC_PACKET_RCVD; /* nil */)
    {
        chr=buf[off++];/*get a char,off value increment*/

        switch( state )                  /* Process according to which state */
        {
            /*lint -esym(788,HDLC_PACKET_RCVD)  No need to deal with HDLC_PACKET_RCVD
            since we're in a loop that guarantees we're not in that state. */
            case HDLC_HUNT_FOR_FLAG:            /* We're looking for a flag ... */
                if (chr == ASYNC_HDLC_FLAG)     /*   and we got one ...         */
                {
                    state = HDLC_GOT_FLAG;      /*   so go on to the next step. */
                }
                break;

            case HDLC_GOT_FLAG:                   /* Had a flag, now expect a packet */
                if (chr == ASYNC_HDLC_FLAG)    /* Oops, another flag.  No change. */
                {
                    break;
                }
                else
                {                                                    /* Ah, we can really begin a packet */
                    crc = CRC_16_L_SEED;               /* and the CRC in its initial state */
                    state = HDLC_GATHER;               /* and we begin to gather a packet  */
                }
            /*pass through*/
            case HDLC_GATHER:                         /* We're gathering a packet      */
                if (chr == ASYNC_HDLC_FLAG)       /* We've reached the end         */
                {
                    if (*out_len < MIN_PACKET_LEN)        /* Reject any too-short packets  */
                    {
                        state = HDLC_HUNT_FOR_FLAG;           /* Start over     */
                    }
                    /*because reuse udp port, must crc check*/
                    else if (crc != CRC_16_L_OK_NEG)          /* Reject any with bad CRC */
                    {
                        state = HDLC_HUNT_FOR_FLAG;           /* Start over     */
                    }
                    else  /* Yay, it's a good packet! */
                    {
                        state = HDLC_PACKET_RCVD;                    /* Done for now   */
                        return OPER_SUCCESS;     /*return 0: get complete hdlc payload */
                    }
                    break;           /* However it turned out, this packet is over.  */
                }

                /* It wasn't a flag, so we're still inside the packet. */
                if (chr == ASYNC_HDLC_ESC)               /* If it was an ESC       */
                {
                    chr=buf[off++];/*get next char,off value increment*/

                    chr ^= ASYNC_HDLC_ESC_MASK;       /* de-mask it  */
                    
                    out_buf[ (*out_len)++ ] = chr;                    /* Add byte to out_buf */

                    crc = CRC_16_L_STEP (crc, (unsigned short) chr);      /* Update the CRC     */
                    /* No break; process the de-masked byte normally */
                }
                else
                {
                    out_buf[ (*out_len)++ ] = chr; 
                    crc = CRC_16_L_STEP (crc, (unsigned short) chr);      /* Update the CRC     */
                }
                break;

            default:       /* Shouldn't happen with an enum, but for safety ...  */
                //DEBUGPRINT("enter default\n");
                state = HDLC_HUNT_FOR_FLAG;             /* Start over         */
                break;
        }/* switch on state */
        if  ( off >= len )
        {
            return OPER_FAILURE;/*reach to the end of buf*/
        }
    }
  
    return OPER_FAILURE;/*return uncomplete*/
}

/*------------------------------------------------------------
  ����ԭ��: int usb_init(void)
  ����:     ��ʼ��usb�˿�
  ����:     ��
  ���:     ��
  ����ֵ:   �ɹ�����OPER_SUCCESS,ʧ�ܷ���OPER_FAILURE
-------------------------------------------------------------*/
static int usb_init(void)
{
    struct stat s_stat;

    if (-1 != g_usbfd)
    {
        close(g_usbfd);
    }

    int g_wait_usb = 0;
    do
    {
        if (( stat ( DIAG_PORT, &s_stat) == -1) 
            || (open_usb_port() != OPER_SUCCESS))
        { 
            syslog( "Diag is Not exist !" ); 

            __msleep(2000);
            if ( ++g_wait_usb > 2)
            {
                syslog( "can not open diag after 4s !" );
                return OPER_FAILURE;
            }
        }
        else
        {
            break;
        }
    }while(1);
    
    return OPER_SUCCESS;
}

/*------------------------------------------------------------
  ����ԭ�� : void diagd_hup_signal_exception(int sig)
  ����     : client�����źŴ�����
  ����     : sig(�źű��); 
  ���     : ��
  ����ֵ   : ��
-------------------------------------------------------------*/
void diagd_hup_signal_exception(int sig)
{
     syslog("Receive signal = [%d]", sig);
	  close( g_usbfd );
	  g_usbfd = -1;
}

/*------------------------------------------------------------
  ����ԭ��: int bifurcate(void)
  ����:     ����d2n�ӽ���
  ����:     pold_timer����ԭʼtimer
  ���:     �ɹ������fork����ֵ
  ����ֵ:   �ɹ� : OPER_SUCCESS
            ʧ�� : OPER_FAILURE
-------------------------------------------------------------*/
static int bifurcate(pid_t* ppid, struct itimerval* pold_timer)
{
    *ppid = fork();
    
    if (*ppid == -1) //����d2n�ӽ��̲��ɹ�
    {
        syslog ("Create sub procedure failure");
					 __msleep(4000);
        exit(0);
    }
    else if( *ppid == 0 )//�ӽ��̣�����D2N_LOOP_S״̬
    {
        signal (SIGCHLD,  sub_coordinate);
        signal (SIGPIPE,   sub_coordinate);
        signal (SIGTERM,  sub_coordinate);
        signal (SIGUSR1,  sub_coordinate);
		 signal(SIGHUP,    diagd_hup_signal_exception);

        if (setitimer (ITIMER_REAL, pold_timer, NULL))
        {
            syslog ("Can't cancel the timer!");
        }

        syslog ("Start sub branch transfer data from Usb to Net!");
    }
    else //�����̣������߾��������Ǹ�״̬
    { 
        syslog ("Main branch fork sub %d OK!",*ppid);
    }

    DEBUGPRINT(" BIFURCATE_S OK\n");

    return OPER_SUCCESS;
}

/*------------------------------------------------------------
  ����ԭ��: void exit_function(void)
  ����:     �����˳�ʱ���رմ���Դ
  ����:     ��
  ���:     ��
  ����ֵ:   ��
-------------------------------------------------------------*/
void exit_function(void)
{
    DEBUGPRINT("PID=%d exit\n",getpid());
    DEBUGPRINT("listen_fd=%d netfd=%d usbfd=%d\n",g_listen_fd,g_netfd,g_usbfd);
    if ( -1 != g_usbfd )
    {
        close(g_usbfd);
        g_usbfd = -1;
    }

    if ( -1 != g_netfd )
    {
        close(g_netfd);
        g_netfd = -1;
    }

    if ( -1 != g_listen_fd )
    {
        close(g_listen_fd);
        g_listen_fd = -1;
    }
    if ( -1 != g_logsys )
    {
        close(g_logsys);
        g_logsys = -1;
    }
    if( g_main_pid == getpid())
    {
        if( -1 != g_udp_pid)
        {
            kill( g_udp_pid, SIGTERM );
            g_udp_pid = -1;
        }
        if( -1 != g_sub_pid)
        {
            kill( g_sub_pid, SIGTERM );
            g_sub_pid = -1;
        }
    }
}





/*------------------------------------------------------------
  ����ԭ�� : void diagd_signal_exception(int sig)
  ����     : client�����źŴ�����
  ����     : sig(�źű��); 
  ���     : ��
  ����ֵ   : ��
-------------------------------------------------------------*/
void diagd_signal_exception(int sig)
{
     syslog("Receive signal = [%d]", sig);
}


/*------------------------------------------------------------
  ����ԭ�� : void diagd_server_bad_signal(int signum)
  ����     : ����������źŴ�����
  ����     : signum �źű��
  ���     : g_child_exited ����ΪAT_CHLD_EXITED
  ����ֵ   : ��
-------------------------------------------------------------*/
void diagd_server_bad_signal(int sig)
{
	syslog("Receive signal = [%d]", sig);
		 __msleep(4000);
	exit(1);
}

/*------------------------------------------------------------
  ����ԭ��: void at_setup_sig()
  ����    : ��װ�źż��źŴ�����
  ����    : �����в���
  ���    : ��
  ����ֵ  : ��
-------------------------------------------------------------*/
void diagd_setup_sig(void)
{
    struct sigaction sa;
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGCHLD);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);

    sa.sa_mask = mask;
    sa.sa_flags = 0;
    
	SIGNAL(SIGHUP,    diagd_hup_signal_exception);
	SIGNAL(SIGINT,    diagd_signal_exception);
	SIGNAL(SIGABRT,   diagd_server_bad_signal);
	SIGNAL(SIGFPE,    diagd_server_bad_signal);
	SIGNAL(SIGILL,    diagd_server_bad_signal);
	SIGNAL(SIGQUIT,   diagd_server_bad_signal);
	SIGNAL(SIGSEGV,   diagd_signal_exception);
	SIGNAL(SIGBUS,    diagd_server_bad_signal);    
//	SIGNAL(SIGEMT,    diagd_server_bad_signal);
	SIGNAL(SIGPOLL,   diagd_server_bad_signal);
	SIGNAL(SIGPROF,   diagd_server_bad_signal);
	SIGNAL(SIGSYS,    diagd_server_bad_signal);
	SIGNAL(SIGTRAP,   diagd_server_bad_signal);
	SIGNAL(SIGVTALRM, diagd_server_bad_signal);
	SIGNAL(SIGXCPU,   diagd_server_bad_signal);
	SIGNAL(SIGXFSZ,   diagd_server_bad_signal);
	SIGNAL(SIGALRM,  NULL);
	SIGNAL(SIGPIPE,  main_coordinate);
	SIGNAL(SIGTERM,  main_coordinate);
	SIGNAL(SIGCHLD,  main_coordinate);
	SIGNAL(SIGUSR1,  main_coordinate);
	SIGNAL(SIGCONT, NULL);
	
    int i = SIGRTMIN;
    for(; i < SIGRTMAX; i++)
    {
        SIGNAL(i, diagd_signal_exception);
    }
}



/*------------------------------------------------------------
  ����ԭ��: int main(int argc, char **argv)
  ����:  ʵ��Diag����͸��,·����ģ��������
  ����:  ��
  ���:  ��
  ����ֵ: �ɹ�����0��ʧ�ܷ��ط�0
-------------------------------------------------------------*/
int main (int argc, char **argv)
{
    int    retval = 0;
    pid_t  d2n_pid = 0, udp_pid=0;
    long   rcv_len = 0, payload_len = 0;
    long   bcm_img_len = 0,bcm_img_left=0;//bcmӳ���ܳ���ʣ�೤��
    long   upgrade_history = 0;//��¼������ʷ��·����ģ�������ɹ��������Ҫ�ɱ���º�����ϵͳ
    
    char   rcv_buf[BUFFER_LEN];
    char   payload_buf[BUFFER_LEN];
    usockaddr uSockaddr_tcp;
  //  EN_UPGRADE_ERRCODE
    int err = 0;
    int  usb_state = OPER_FAILURE;/*usb not open*/
    int  bifurcate_state = OPER_FAILURE;/*�Ƿ񴴽�d2n�ӽ���*/    
    sigset_t mask;
    struct sigaction sigact, old_alarm_handler;
    struct itimerval timerv,old_timer;

    char* imageTemp = NULL ;

    char* imageNode = NULL ;
    int   nBcmMolloc = -1;
    pid_t parent_pid = 0;
    
    g_state = TCP_INIT_S;

#ifndef ORG_DIAG
    FILE* fsNv = NULL;
#endif

    if (( g_logsys = open(_DIAG_LOG, O_RDWR|O_CREAT, 644)) < 0)
    {
        exit(0);
    }

    if (diag_parse_lines(argc, argv) != OPER_SUCCESS)
    {
        exit(0);
    }
    
    if (g_ePort == _DIAG)
    {
        if (diag_daemon(1, 1) != OPER_SUCCESS)
        {
            syslog ( "can't establish daemon program");
            exit(errno);
        }
    }

    if ( 0 == atexit(exit_function))
    {
        syslog ("exit func regist ok");
    }


	diagd_setup_sig();


#ifdef ORG_DIAG
    nvram_unset( EXPECT_UDP_PORT );
#else
    fsNv = fopen(NV_FILE, "w+");
    if (fsNv)
    {
    	fclose(fsNv);
	fsNv = NULL;
    }
#endif

	
    /*get current pid*/
    parent_pid = getpid ();
    udp_pid = fork();
    if (udp_pid == -1)
    {
        syslog ("Create sub procedure failure");
        exit(0);
    }
    else if( udp_pid == 0 )
    {
        udp_server_main(parent_pid);
        syslog ("impossible,udp_server_main go here!");     
        exit(0);
    }
    else
    { 
        g_udp_pid   = udp_pid;
        g_main_pid = parent_pid;
        syslog ("Main branch continue execute!");                    
    }

    sigact.sa_handler = tcp_timeout;
    sigact.sa_flags = 0;
    memset((void *)&sigact.sa_mask, 0, sizeof(sigset_t));
    sigemptyset(&mask);
    sigaddset (&mask, SIGALRM);
    sigact.sa_mask = mask;
    sigaction(SIGALRM, &sigact, &old_alarm_handler);
    
    timerv.it_interval.tv_sec = _interval;
    timerv.it_interval.tv_usec = 0;
    timerv.it_value = timerv.it_interval; 
    if (setitimer (ITIMER_REAL, &timerv, &old_timer) != 0)
    {
        exit (0);
    }

    do
    {
        switch( g_state )
        {
            case TCP_INIT_S:

                if ((g_listen_fd = diag_init_tcp_listen_socket(&uSockaddr_tcp)) < 0)
                {
                    sleep(2);
                    syslog ( "Tcp initialize failure");
                    DEBUGPRINT(" err=%d\n",errno);

                    if ( 0 < udp_pid )
                    {
                        //ɱ��udp server����
                        kill (udp_pid, SIGTERM);
                        waitpid(udp_pid,&retval,0);
                        udp_pid = 0;
                    }
                    exit(0);
                }
                syslog ( "Tcp initialize successful");
                g_state = WAIT_CONNECT_S;
                break;
                
            case WAIT_CONNECT_S: 
                g_i_upgrade = NOT_UPGRADE;
                g_net_alive = NET_TIMEOUT;
                g_disconnect = NOT_DISCONNET;
                
                //ɱ��D2N͸���ӽ���
                if ( 0 < d2n_pid)
                {
                    syslog ("kill d2n_pid=%d ",d2n_pid);
                    kill(d2n_pid, SIGTERM);
                    waitpid(d2n_pid,&retval,0);
                    d2n_pid = 0;
                    bifurcate_state = OPER_FAILURE;
                }

                /*�ر����⴮��*/
                if ( -1 != g_netfd )
                {
                    close(g_netfd);
                    g_netfd = -1;
                }
                
                //�ر�usb��
                if ( -1 != g_usbfd )
                {
                    close(g_usbfd);
                    g_usbfd = -1;
                    usb_state = OPER_FAILURE;
                }
                
                //��ʼ��ȫ���׽���������g_netfd
                if (diag_accept_connect ( g_listen_fd ) == OPER_SUCCESS)
                {
                    g_state = RECV_S;/*jump to recv packet*/
                    syslog ("Client access successful");
                }
                break;

            case RECV_S:      
                /*receive TCP packet*/
                rcv_len = recv(g_netfd, rcv_buf, BUFFER_LEN-1, 0);
                /*check if Virtual console request to disconnect tcp*/
                if (TCP_TIMEOUT == g_disconnect)
                {
                    g_disconnect = NOT_DISCONNET;
                    
                    if( NOT_UPGRADE == g_i_upgrade && 0 != upgrade_history )
                    {
                        DEBUGPRINT("In upgrade, Jump to TCP_CON_CLOSE_S\n");
                        g_state = TCP_CON_CLOSE_S;
                        break;
                    }
                    
                    DEBUGPRINT("Not in upgrade, Jump to WAIT_CONNECT_S\n");
                    g_state = WAIT_CONNECT_S;
                    break;
                }
                else if (UDP_NOTIFY_DISCONNECT == g_disconnect)//udp notify
                {
                    g_disconnect = NOT_DISCONNET;
                    
                    if ( NOT_UPGRADE != g_i_upgrade || 0 != upgrade_history )
                    {
                        syslog("In upgrading, ignore udp notification");
                        break;
                    }
                    
                    DEBUGPRINT("Not in upgrade,Jump to WAIT_CONNECT_S\n");
                    g_state = WAIT_CONNECT_S;
                    break;
                }

                /*check if sig_pipe(Tcp_connect_close signal) request to reboot all*/
                if (1 == g_reboot_all)
                {
                    g_state = TCP_CON_CLOSE_S;
                    break;
                }        

                //������źŴ�ϣ���������    
                if ( rcv_len == SOCKET_ERROR && errno == EINTR )
                {
                    break;/*continue to recv*/
                }

                if ( rcv_len > BUFFER_LEN - 1 )
                {
                    syslog ("The recv data too long!");
                    break;
                }
                else if ( rcv_len <= 0 )//�����źŴ�ϵĴ��� 
                {
                    if ( BCM_UPGRADE == g_i_upgrade )
                    {
                        syslog("In upgrading, ignore tcp disconnect notification");
                        g_state = WAIT_CONNECT_S;
                        break;
                     }

                    syslog ( "The peer close socket!");
                    g_state = TCP_CON_CLOSE_S;
                    break;
                }
                else /*recv good data,keep alive*/
                {
                    g_net_alive = NET_TIMEOUT;
                }

                //�ж��Ƿ�����������ת��״̬
                if (NOT_UPGRADE == g_i_upgrade)
                {
                    g_state = PARSE_S;
                }
                else if (BCM_UPGRADE == g_i_upgrade)
                {
                    g_state = WRITE_BCM_S;
                }                      
                else if (MSM_UPGRADE == g_i_upgrade)
                {
                    g_state = N2D_LOOP_S;
                }
                else
                {
                    ;/*empty sentence*/
                }
                break;
              
            case PARSE_S:
                DEBUGPRINT(" PARSE_S\n");
                retval = parse_tcp_payload(rcv_buf,rcv_len, 
                  payload_buf, &payload_len);

                if(OPER_SUCCESS != retval)/*don't get a complete hdlc payload*/
                {
                    g_state = N2D_LOOP_S;
                    break;
                }


                /*����������*/    
                g_i_upgrade = analyse_cmd(payload_buf,&bcm_img_len);
                assert (-1 != g_i_upgrade);/*impossible situation*/



                if ( NOT_UPGRADE == g_i_upgrade )
                {
                    g_state = N2D_LOOP_S;
                }
                else if (BCM_UPGRADE == g_i_upgrade)
                {
                    /*initialize bcm_img_left before write bcm flash*/
                    bcm_img_left = bcm_img_len;
                    close_pcui_shake();

                    //��ȥWRITE_BCM_S״̬��������mtd_write�Ĺܵ���
                    g_state = WRITE_BCM_S;
                }                      
                else /*MSC_UPGRADE or MSM_FORCE_UPGRADE*/
                {
                    syslog ("start upgrade msm!");
                    close_pcui_shake();
                    
                    //ȥ�ȴ�MSM������Ȼ���ʼ��usb��
                    g_state = FORCE_MSM_S;
                }
                
                break;
            
            case FORCE_MSM_S:
							
                if ( -1 != g_usbfd)
                {
                    close(g_usbfd);
                    g_usbfd = -1;
                }
                
#ifdef ORG_DIAG                 
                if (MSM_FORCE_UPGRADE == g_i_upgrade)
                {
                     syslog("Force MSM reboot");
                   
						/*���ú���,����GPIO��,ʹ������ģ�鴦��ǿ������״̬*/
						syslog("BSP_MSM_Force_On");
						BSP_MSM_Force_On();


						/*����GPIO��,������������ģ��;*/
						syslog("BSP_MSM_On");
						BSP_MSM_Off();
						
						__msleep(1000);
						syslog("BSP_MSM_Off");
			
						BSP_MSM_On();

                    upgrade_history |= 8;//��¼ǿ��������
                    //ɱ��D2N͸���ӽ���
                    /*if ( 0 < d2n_pid)
                    {
                        syslog( "kill d2n" );
                        kill(d2n_pid,SIGTERM);
                        waitpid(d2n_pid,&retval,0);
                        bifurcate_state = OPER_FAILURE;
                        d2n_pid = 0;
                    }
                    */
                    syslog("Wait MSM be ready");    
                    g_waitMsmTimeout = 0;//��ʼ��ʱ
                    while (1)//�ȴ�����ģ����������
                    {
                        pause();
                        if ( errno == EINTR && 2 == g_waitMsmTimeout )//�ȴ�msm 2*1=2��
                        {
                            break;
                        }
                    }
                }

                /*next is same,so set to MSM_UPGRADE*/
                g_i_upgrade = MSM_UPGRADE;
#endif
                g_state = RECV_S;

                //��¼������MSM
                upgrade_history |= 4;
                break;
            case N2D_LOOP_S:
                if (OPER_SUCCESS != usb_state)/*usb state not ok*/
                {
                    syslog ("usb is init!");
                    usb_state=usb_init();
                    
                    //��ʼ��usb���ɹ�
                    if (OPER_SUCCESS != usb_state)
                    {
                        g_state = RECV_S;
                        break;
                    }
                }
                
                 if ( OPER_FAILURE == bifurcate_state )
                {
                    bifurcate(&d2n_pid,&old_timer);
                    bifurcate_state = OPER_SUCCESS;      
                    syslog ("bifurcate!");
                    if (0 == d2n_pid)//D2N�ӽ���
                    {
                        DEBUGPRINT(" D2N_LOOP_S\n");
                        /*�ӽ��̶�ȡDIAG������ת��������*/
                        diag_to_net(); 
                        syslog ("impossible! D2N sub proc return\n");
                        exit(0);/*sub progress exit*/
                    }
                    g_sub_pid = d2n_pid;
                }
                
                /*�����̶�ȡ��������ת����DIAG��*/
                retval = net_to_diag(rcv_buf,rcv_len);
                if (retval == OPER_FAILURE)
                {
                    syslog ("usb dev is down!");
                    DEBUGPRINT(" N2D failure\n");
                    //ֱ�ӷ���WAIT_CONNECT_S״̬�����ǹرմ򿪵���Դ
                    g_state = WAIT_CONNECT_S;
                    break;
                }
                DEBUGPRINT(" N2D OK\n");

                g_state = RECV_S;
                break;
				
#ifdef ORG_DIAG
            case WRITE_BCM_S:
					if( -1 == nBcmMolloc)
					{
						syslog("BCM begin!");
						imageNode =  malloc(bcm_img_len);
						imageTemp = imageNode;
						bcm_img_left = bcm_img_len;
						nBcmMolloc = 0;
						g_state = RECV_S;
                     			break;
					}
	          	  
					if(bcm_img_left>0)
					{
						memcpy(imageTemp,rcv_buf, rcv_len);
						imageTemp = imageTemp +rcv_len;
						bcm_img_left = bcm_img_left- rcv_len;
						
						if(0 == bcm_img_left)
						{
							err = upgrade_bcm_image(imageNode,bcm_img_len);
						}
						else
						{
							g_state = RECV_S;
							break;
						}
					}


                if ((0 == bcm_img_left) && (0 == err))
                {
                    syslog ("upgrade router success");
                    report_upgrade_result(g_netfd,OPER_SUCCESS);
                    nvram_set("is_upgrade_restore", "1");
                    nvram_commit();

                    //�����ɹ����g_i_upgrade��������������BCM�ɹ�
                    g_i_upgrade = NOT_UPGRADE;
                    upgrade_history |= 1;
                }
                else if (0 != err)
                {
                    report_upgrade_result(g_netfd,OPER_FAILURE);
                    
                    //�����д����g_i_upgrade,������������BCMʧ��
                    g_i_upgrade = NOT_UPGRADE;
                    upgrade_history |= 2;
                }
                
                g_state = RECV_S;
                break;
#endif

            case TCP_CON_CLOSE_S:
                DEBUGPRINT(" TCP_CON_CLOSE_S\n");
                
                //g_reboot_allΪ1����;Ŀǰû��������������Ҳû����������������ϵͳ
                if ((g_reboot_all==0) && (NOT_UPGRADE==g_i_upgrade) 
                    && (0==upgrade_history))
                {
                    g_state = WAIT_CONNECT_S;
                    break;
                }

                g_reboot_all = 0;

                /*now, in upgrade state*/
                if ( -1 != g_usbfd )
                {
                    close (g_usbfd);
                    g_usbfd = -1;
                    usb_state == OPER_FAILURE;
                }
                if ( -1 != g_netfd )
                {
                    close (g_netfd);
                    g_netfd = -1;
                }
#ifdef ORG_DIAG
                DEBUGPRINT("Reboot all system\n");
                //ǿ�����������ָ�ǿ������GPIO����������ģ��������������boot��
                if ( 0x8 & upgrade_history ) 
                {
                    BSP_MSM_Force_Off();
                }

                /*����GPIO��,������������ģ��;*/
                BSP_MSM_Off();
               __msleep(1000);
                BSP_MSM_On();
                
                /*reboot BCM*/
/* <BB5D00010.VOICE: yangjianping 2008-8-4 MODIFY BEGIN */
                BSP_SYS_SoftReboot();
/* BB5D00010.VOICE: yangjianping 2008-8-4 MODIFY END> */
#endif
                break;
                
           default:
                DEBUGPRINT(" abnormally enter here\n");
                exit(0);
                break;
        }

    }while(1);

    return 0;
}

