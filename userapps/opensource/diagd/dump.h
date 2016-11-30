/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2005, ��Ϊ�������޹�˾.
  �ļ���: dump.h
  ����: y45539
  �汾: V500R003
  ��������: 2005-8-16
  �������: 
  ��������: 
      ͷ�ļ����������շ������ݵ����ڵĵĺ궨��ͺ�������
  ��Ҫ�����б�: 
      
  �޸���ʷ��¼�б�: 
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    
  ��ע: 
************************************************************************/
#ifndef _DUMP_H
#define _DUMP_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdarg.h>
#include <paths.h>

#define _DEBUG_DIAG

#ifdef _DEBUG_DIAG
#define DEBUGPRINT(format,arg...) do{ \
    printf("%s:%d " format,__FUNCTION__,__LINE__,##arg); \
}while(0)

//need replace printf with info
#define syslog(format,arg...)    do{ \
	printf("[%s: %4dL]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
}while(0)
#else
#define syslog(format,arg...)    do{ \
    info("[%s: %4dL]:" format "\n", __FUNCTION__, __LINE__, ##arg); \
}while(0)

#define DEBUGPRINT(format,arg...)   do{ \
   
}while(0)
#endif

#define BCM_UPGRADE       1                
#define MSM_UPGRADE       2                    
#define MSM_FORCE_UPGRADE 3                
#define NOT_UPGRADE       0
#define EQUIP_RUN         4

#define MSM_UPGRADE_STR         "Huawei FWT Begin Download Pkt"
#define MSM_FORCE_UPGRADE_STR   "Huawei FWT Force Download Pkt"
#define BCM_UPGRADE_STR         "Huawei RUT Begin Download Pkt"

#define ENODIAG        161                          //�Ҳ���DIAG ��
#define SERVER_PORT    1280                         //�����˿ں�
#define DIAG_PORT        "/dev/ttyUSB3"               //DIAG ���豸�ڵ�
#define MODEM_PORT     "/dev/ttyUSB0"               //MODEM_PORT�˿��豸�ڵ�
#define VOICE_PORT      "/dev/ttyUSB1"
#define PCUI_PORT        "/dev/ttyUSB2"               //PCUI���豸�ڵ�
#define USB_ERR           "no usb device\r\n"          //�Ҳ���usb�豸
#define INTERFACE_ERR  "interface error\r\n"        //�򲻿��˿�
#define OTHER_ERR        "other error\r\n"            //��������
#define OPEN_SUCCESS   "sucess open port\r\n"       //�ɹ��򿪶˿�
#define _DIAG_LOG         "/var/log/diagd.log"         //logfile
#define BUFFER_LEN     4097                         //ÿ�ν��շ����ֽ���
#define SOCKET_ERROR   -1                           //�������Ӵ���
#define OPER_SUCCESS   0                            //�����ɹ�
#define OPER_FAILURE   -1                           //����ʧ��
#define _r             0x0d                         //���з�
#define _n             0x0a                         //�س���
#define _start        0x23                         //��ʼ��ʾ��'#'
#define _interval       1                           //��ʱ����ʱʱ��, ��������Ϊ0
#define isdigit(x) (((x) >= '0') && ((x) <= '9'))

#ifndef FEATURE_E960
#define NET_TIMEOUT    120
#else
#define NET_TIMEOUT    240
#endif

#define UDP_SRV_TIMEOUT             0
#define NOT_DISCONNET                 0
#define TCP_TIMEOUT                      1  
#define UDP_NOTIFY_DISCONNECT  2

#define REPORT_CMD_WORD              255
#define CMD_WORD_LEN                    1
#define UPGRADE_RESULT_SUCCESS 4
#define UPGRADE_RESULT_FAILURE 3

#define UPGRADE_RESULT_PAYLOAD_LEN      40
#define MAX_NEGRO_LEN   1000
char    g_udp_buf[MAX_NEGRO_LEN]; 


/* Async HDLC achieves data transparency at the byte level by using
   two special values. The first is a flag value which begins and ends
   every packet: */
#define  ASYNC_HDLC_FLAG           0x7e
#define  ASYNC_HDLC_ESC             0x7d
#define  ASYNC_HDLC_ESC_MASK  0x20

/* Minimum size of a received packet. */
#define  MIN_PACKET_LEN              3        /* 2 bytes CRC + 1 byte command code */

#define UPGRADE_CMD_TYPE          0xff

/*-------------------------------------------------------------------------*/

/* Mask for CRC-16 polynomial:
**
**      x^16 + x^12 + x^5 + 1
**
** This is more commonly referred to as CCITT-16.
** Note:  the x^16 tap is left off, it's implicit.
*/
//#define CRC_16_L_POLYNOMIAL     0x8408

/* Seed value for CRC register.  The all ones seed is part of CCITT-16, as
** well as allows detection of an entire data stream of zeroes.
*/
#define CRC_16_L_SEED       0xFFFF

/* Residual CRC value to compare against return value of a CRC_16_L_STEP().
** Use CRC_16_L_STEP() to calculate a 16 bit CRC, complement the result,
** and append it to the buffer.  When CRC_16_L_STEP() is applied to the
** unchanged entire buffer, and complemented, it returns CRC_16_L_OK.
** That is, it returns CRC_16_L_OK_NEG.
*/
#define CRC_16_L_OK         0x0F47
#define CRC_16_L_OK_NEG     0xF0B8

extern unsigned short crc_16_l_table[  ];
/*===========================================================================

MACRO CRC_16_L_STEP

DESCRIPTION
  This macro calculates one byte step of an LSB-first 16-bit CRC.
  It can be used to produce a CRC and to check a CRC.

PARAMETERS
  xx_crc  Current value of the CRC calculation, 16-bits
  xx_c    New byte to figure into the CRC, 8-bits

DEPENDENCIES
  None

RETURN VALUE
  The new CRC value, 16-bits.  If this macro is being used to check a
  CRC, and is run over a range of bytes, the return value will be equal
  to CRC_16_L_OK_NEG if the CRC checks correctly according to the DMSS
  Async Download Protocol Spec.

SIDE EFFECTS
  xx_crc is evaluated twice within this macro.
*/

#define CRC_16_L_STEP(xx_crc,xx_c) \
  (((xx_crc) >> 8) ^ crc_16_l_table[((xx_crc) ^ (xx_c)) & 0x00ff])

typedef enum
{
    HDLC_HUNT_FOR_FLAG,       /* Waiting for a flag to start a packet       */
    HDLC_GOT_FLAG,            /* Have a flag, expecting the packet to start */
    HDLC_GATHER,              /* In the middle of a packet                  */
    HDLC_PACKET_RCVD,         /* Now have received a complete packet        */
} hdlc_state;

typedef enum
{
    EQUIP_IP_MAC_REQUERY = 1,     /* MAC requery command  */
    EQUIP_IP_CHANGE,              /* IP change command  */
    EQUIP_IP_RESTORE,             /* IP restore command  */
    EQUIP_RESTART_DIAGD,      /* �������ؽ��� */
    EQUIP_REBOOT_GW,           /* �������� */
    EQUIP_RESTORE_GW,          /* erase nvram */
    EQUIP_ENFORCE_DOWN,     /* MSM enforce down */
    EQUIP_EXIT_ENFORCE_DOWN,  /* MSM unlock enforce down */
    EQUIP_RESTART_MSM,          /*restart msm */
    EQUIP_DIAG_STATUS,          /* require diag port status, success = 0, failure = 1 */
    EQUIP_DOWNLOAD,             /* goto download mode */
    EQUIP_INVALID                  /* multi_grade udp negotiation invalid */
} nego_state;

/*********************************************************** 
  1�� 0x7e          //1�ֽ����ʼ��������д
  2�� 0x03          //1�ֽ�������
  3�� Ipaddr        //4�ֽڣ�PCָ������Ҫ�ָ���IP�����PC��������ֶΣ�
                     �򵥰�Ĭ�ϰ�IP��ַ�ָ���192.168.1.1��
  4�� Mac           //6�ֽڣ����PCָ�������Mac����ֻ�и�MAC�ĵ���Ż�ָ�IP��
                    ���MacΪ""�������еĵ��嶼��ָ�IP��
  5�� Version      //20�ֽڣ�������������ֶ������塣
  6�� Reserved     //1�ֽڱ�����������չ��
  7�� CRC          //2�ֽڵ�CRC�룬����������������CRC������Ҫ���ɲ����ģ�
                    �����λ����Ҫ����
  8�� 0x7e        //1�ֽڵİ�������������д
************************************************************/
typedef struct REPORT_DATA
{
     //unsigned char hdlc_start;
     unsigned char cmd;
     unsigned char ip[4];
     unsigned char mac[6];
     unsigned char version[20];
     unsigned char reserved[1];
     //unsigned char crc[2];
     //unsigned char hdlc_end;
} s_report_data;

typedef enum PORT_E
{
    _MODEM = 0,
    _VOICE,
    _PCUI,
    _DIAG
}USB_PORT_E;

//����״̬����״̬
typedef enum {
TCP_INIT_S = 0, //tcp���ӳ�ʼ��
WAIT_CONNECT_S, //�ȴ�tcp��������
RECV_S,         //����tcp��������
PARSE_S,        //����tcp��������
N2D_LOOP_S,     //͸��tcp��������
FORCE_MSM_S,    //ǿ��MSMģ������
TCP_CON_CLOSE_S,//tcp���ӹر�
WRITE_BCM_S,    //��bcmӳ������д���²㺯��
}diag_status;

typedef union 
{
    struct sockaddr    sa;
    struct sockaddr_in sa_in;
} usockaddr;

#define  MAX_CMD_LEN         sizeof(s_report_data)

//ת���udpЭ������ĵ���󳤶�
#define  MAX_CMD_TRANS_LEN   (MAX_CMD_LEN*2)

extern int        g_logsys;
extern USB_PORT_E g_ePort;
extern int        g_netfd;
extern int        g_usbfd;
extern int        g_udpfd;
extern int        g_Serverport;

extern void sb_enforce_down(void);
extern void msm_poweroff(void);
extern void msm_poweron(void);

void info (char *fmt, ...);
void show_usuage ( );
int diag_parse_lines(int argc, char **argv);
int echo_to_net(char* buf, int ilen, int arg);
int send_to_net(char* buf, int ilen, int arg);
int parse_packet ( unsigned char *buf,  int (* func) ( unsigned char* ), int ) ;
long parse_udp_payload (unsigned char *buf, long buf_len, unsigned char* out_buf,long* out_len);
long gen_hdlc_packet(char* payload,long size,char* hdlc);
int multi_upgrade_nego(int udp_fd,unsigned char *buf,int len, struct sockaddr_in* client_udp_addr );
#endif
