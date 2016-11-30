/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : ttyUSB.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2008��1��11��
  ����޸�   :
  ��������   : ���HSPA����ģ
  �����б�   :
 
  �޸���ʷ   :
  1.��    ��   : 2008��1��11��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/
#include <stdio.h>
#include <memory.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <sys/msg.h>
#include<stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include"ttyUSB.h"
#include<signal.h>
#include "board_api.h"
#include <semaphore.h>
#include<sys/ioctl.h>
#include <stdarg.h>
#include "syscall.h"

/*add by w00135351*/
#include "tr69cdefs.h"
#include "app/cwmp_init.h"
/*end by w00135351*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/
static void*  WritePcui(void* arg);/*��HSPAдָ���*/
static void* SendSysinfo(void* arg);
static   int ReadPcui();
static  int MsgIdCompute(unsigned char ucCmdFrom);

/*HSPA������Ϣ��������*/

static void FacInfoAnlysis(char *pszInfoBack); //FACTORY_INFO_QUERY 17--AT+GMI\r
static void FacInfoAnlysis_C(char *pszInfoBack); //FACTORY_INFO_QUERY_C 18--AT+CGMI\r
static void SoftwareVersionAnlysis(char *pszInfoBack) ; //SOFTWARE_VERSION_QUERY 19--AT+GMR\r
static void SoftwareVersionAnlysis_C(char *pszInfoBack) ; //SOFTWARE_VERSION_QUERY_C	20--AT+CGMR\r
static void HardwareVersionAnlysis(char *pszInfoBack) ;//HARDWARE_VERSION_QUERY  21--AT^HVER\r
static void HardwareVersionAnlysis_W(char *pszInfoBack) ;//HARDWARE_VERSION_QUERY  22--AT^HWVER\r
static void ImsiInfoAnlysis(char *pszInfoBack) ;//IMSI_QUERY 27--AT+CIMI\r
static void ImeiInfoAnlysis(char *pszInfoBack) ;//IMEI_QUERY 28--AT+CGSN\r
static void RssiInfoAnlysis(char *pszInfoBack) ;//RSSI_QUERY 31----AT+CSQ\r
static void SystemInfoQuery(char *pszInfoBack);
static void CfgInfoSetBack(char *pszInfoBack);
static void CfgInfoQuery(char *pszInfoBack);
static void ConfigInfoSet(char *pszInfoBack);
static void ConfigInfoQuery(char *pszInfoBack);
static void CgdcontSet(char *pszInfoBack);
static void CgdcontQuery(char *pszInfoBack);
static void OperationInfoQuery(char *pszInfoBack);
static void CardModeQuery(char *pszInfoBack);

static void PinNumModify(char *pszInfoBack);
static void PinEnableSet(char *pszInfoBack);
static void PinEnableQuery(char *pszInfoBack);
static void PinNumVerfication(char *pszInfoBack);
static void PinNumLeftQuery(char *pszInfoBack);

static void TelNumReport(char *pszInfoBack);
static void CallDial(char *pszInfoBack);
static void AcceptCall(char *pszInfoBack);
static void EndCallReport(char *pszInfoBack);
static void ChupBack(char *pszInfoBack);
static void AutoConf(char *pszInfoBack);

/* HUAWEI HGW l65130 2008-08-07 begin*/
static void CgactQuery(char *pszInfoBack);
/* HUAWEI HGW l65130 2008-08-07 begin*/

static void CfunSet(char *pszInfoBack);
static void CfunQuery(char *pszInfoBack);
static void AutoModeReport(char *pszInfoBack);
static void AutoServerStatus(char *pszInfoBack);
static void AutoRing(char *pszInfoBack);
static void AutoConn(char *pszInfoBack);
static void AutoDataFlow(char *pszInfoBack);
/* HUAWEI HGW s48571 2008��2��15�� ��ȡSN���� add begin:*/
static void SnQuery(char *pszInfoBack);
/* HUAWEI HGW s48571 2008��2��15�� ��ȡSN���� add end.*/
static int  ImeiInfoGet();
static void DExSet(char *pszInfoBack);
/* HUAWEI HGW s48571 2008��8��16�� �����ر����� add begin:*/
static void VoiceEnableSet(char *pszInfoBack);
/* HUAWEI HGW s48571 2008��8��16�� �����ر����� add end.*/

static int  PinPukProcess(char *pszInitHspa);
void HspaDeviceSignalProcess(int x);
void HspaDeviceDetect(void);
void HspaSwtichDelay(void);
//add by z67625
int TTY_Output(const char* fmt,...);
int debugswitch(void);
void TTY_WriteDebug(char *pcTmp);

 unsigned long int HexToNum( char cChar);
unsigned long int  aHexToUl(char * pszChar);

static int RegisterNtwk(void);

/* HUAWEI HGW s48571 2008��8��16�� �����ر����� add begin:*/
static int VoiceJudge(void);
/* HUAWEI HGW s48571 2008��8��16�� �����ر����� add end.*/

/*Begin:Added by luokunling 00192527 for imsi inform requirement,2011/9/19*/
int ImsiNumIsOldImsiNum(char *newImsiNum); 
void DelayTimeInformIMSI(void* arg);
/*End:Added by luokunling 00192527,2011/9/19*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/
struct stAt_Fun_Com
{
	char acAtCmdPart[COMMON_SHORT_STRING_LENGTH];
	void (*pfAtBackInfoAnalysis)(char *pszChar);
};

static struct stAt_Fun_Com 	g_stAtFunCom[] =
{
	{"AT+GMI",	FacInfoAnlysis},
	{"AT+CGMI", FacInfoAnlysis_C},
	{"AT+GMR",	SoftwareVersionAnlysis},
	{"AT+CGMR", SoftwareVersionAnlysis_C},
	{"AT^HVER",	HardwareVersionAnlysis},
	{"AT^HWVER",	HardwareVersionAnlysis_W},
	{"AT+CIMI",	ImsiInfoAnlysis},
	{"AT+CGSN",	ImeiInfoAnlysis},
	{"AT+CSQ",	RssiInfoAnlysis},
	{"AT^SYSINFO",	SystemInfoQuery},
	{"AT^SYSCFG=",	CfgInfoSetBack},
	{"AT^SYSCFG?",	CfgInfoQuery},
	{"AT^SYSCONFIG=",	ConfigInfoSet},
	{"AT^SYSCONFIG?",	ConfigInfoQuery},
	{"AT+CGDCONT=",	CgdcontSet},
	{"AT+CGDCONT?",	CgdcontQuery},
	{"AT+CPWD=\"SC\",",	PinNumModify},
	{"AT+CLCK=\"SC\",2",	PinEnableQuery},
	{"AT+CLCK=\"SC\",0,",	PinEnableSet},
	{"AT+CLCK=\"SC\",1,",	PinEnableSet},
	{"AT+CPIN=",	PinNumVerfication},
	{"AT^CPIN=",	PinNumVerfication},
	{"AT^CPIN?",	PinNumLeftQuery},
	{"AT+CPIN?",	PinNumLeftQuery},
	{"ATD",	CallDial},
	{"ATA",	AcceptCall},
	{"\r\nRING\r\n",	AutoRing},
	{"\r\n+CLIP:",	TelNumReport},
	{"\r\n^CEND:",	EndCallReport},
	{"AT+CFUN=",	CfunSet},
	{"AT+CFUN?",	CfunQuery},
	{"\r\n^SRVST:",	AutoServerStatus},
	{"\r\n^MODE:",	AutoModeReport},
    	{"\r\n^CONN",  AutoConn}  ,
	{"AT+COPS?", OperationInfoQuery},
	{"AT^CARDMODE", CardModeQuery},
	{"AT+CHUP", ChupBack},
	{"\r\n^DSFLOWRPT:", AutoDataFlow},
	{"\r\n^CONF", AutoConf},
/* HUAWEI HGW s48571 2008��2��15�� ��ȡSN���� add begin:*/
    {"AT^SN",   SnQuery},
/* HUAWEI HGW s48571 2008��2��15�� ��ȡSN���� add end.*/
    {"AT^DDSETEX=2",DExSet},
    {"AT+CGACT?", CgactQuery},
/* HUAWEI HGW s48571 2008��8��16�� �����ر����� add begin:*/
    {"AT^CVOICE=",VoiceEnableSet}
/* HUAWEI HGW s48571 2008��8��16�� �����ر����� add end:*/
};

/*Begin:Added by luokunling 00192527 for imsi inform requirement,2011/9/19*/
static int g_iDelaytime_flg    = 0;                /*���ز������ݿ�������ʱ�ӳ������ϱ�TR069 ���*/
int g_iHspaWeatherAutoPin = 0;                /*hspa �Ƿ��Զ���  pin   ������pin�룬1 - �Զ���Ƹ*/
/*End:Added by luokunling 00192527 ,2011/9/19*/

static struct stAtSndMsg g_stAtFromOther;/*ҵ����Ϣ���з���HSPA����ģ�����Ϣ*/
static struct stAtRcvMsg g_stAtToOther;/*HSPA����ģ�鷢��ҵ��ģ�����Ϣ*/
/*�Ƿ���Ҫ�������д�ȷ��*/
static int g_lAtMsgId = -1;	/*HSPA����ģ����Ϣ����ID*/
static int g_lModemMsgId = -1;/*����ҵ����Ϣ����ID*/
static int g_lVoiceMsgId = -1;/*����ҵ����Ϣ����ID*/
static int g_lTr069MsgId = -1;/*tr069��Ϣ����ID*/
static int g_lHttpMsgId = -1;/*WEB���ò�ѯ��Ϣ����ID*/
static int g_lLedMsgId = -1;/*LEDָʾ����Ϣ����ID*/
#ifdef CX_DEBUG_VERSION	
static int g_lCmdMsgId = -1;/*׮������Ϣ����ID*/
#endif

#ifdef POTS_PINMNGR
static int g_lPinMngrMsgId = -1; /*PIN����ҵ����Ϣ����ID*/
#endif
static int g_iPcuiFd = -1;	/*PCUI�ڶ�Ӧ�Ĵ����豸�ļ�fd*/
static int g_ShareiFd = -1;/*���ݹ����ļ�fd*/
static int g_HspaExistFlag = HSPA_UNKNOWN;
static int g_iWriteFlag = -1;
static int g_SignalRcv = 0;
static pthread_mutex_t work_mutex;
static sem_t sem;
static struct stHspaInfo g_stHspa;/*�洢���ݿ��ĳ���״̬��
							 ����/var/HspaStatus�ļ��У���
							 	����ҵ��ģ���ѯ*/
static int g_sndflg = 0;

/*Begin:Added by luokunling 00192527 for imsi inform requirement,2011/9/19*/
#define TR69_PID_FILE  "/var/tr69/tr69pid"
/*End:Added by luokunling 00192527,2011/9/19*/
#define HSPA_OFF 0
#define HSPA_ON 1
#define MID_STATE_MASK 0x00f0
#define HSPA_STATE_MASK 0x000f
#define HSPA_LINE_MASK 0x00ff

static int g_HspaLine = HSPA_ON;

#define VOICE_CALLOUT_IDLE 0
#define VOICE_CALLOUT_DAIL 1
#define VOICE_CALLOUT_AUTOCONF 2
static int g_iCallInFlag = VOICE_CALLOUT_IDLE;

#define VOICE_CALLIN_IDLE 0
#define VOICE_CALLIN_ATA 1
#define VOICE_CALLIN_CONN 2
#define VOICE_CALLIN_SETD 3
static int g_iCallOutFlag = VOICE_CALLIN_IDLE;
/* BEGIN: Added by c106292, 2009/3/16   PN:Led light method changes*/

#define UNKNOW_ON_HSPA  0x10
#define NO_CALL_DATA  0x0
#define CALL_ON_HSPA  0x2
#define DATA_ON_HSPA  0x4
static unsigned int  g_ulTrafficFlag = NO_CALL_DATA;
/* END:   Added by c106292, 2009/3/16 */

/* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add begin:*/
static struct stHspaInfo  g_stHspaBackup;

/*add by z67625*/
int debug = 0;//���Կ���
int OutputTerm = 2;//2//2��������ڣ�1��������ں�telnet

#define TTY_DEBUG TTY_Output("\r\nfunction: [%s] ,Line [%u] ,",__FUNCTION__, __LINE__);    TTY_Output
#define TTY_LOG  TTY_Output
#define TTYDEBUGPATH "/var/ttydebuginfo"
#define TTYDEBUG  "/var/ttydebug"
#define TTYPID    "/var/ttypid"
//add by z67625

#define MAX_PIN_TEST 5

#define VOICE_UNAVAILABLE 1
#define VOICE_AVAILABLE 0

#define MAX_ATCMD_CNT  3

static void BackupHspaInfo(struct stHspaInfo *pstHspa )
{
	memcpy(&g_stHspaBackup, pstHspa, sizeof(g_stHspaBackup));
}

static int CheckHspaInfo( struct stHspaInfo *pstHspa)
{
	static int voiceStatusBackup = VOICE_UNAVAILABLE ;
	int voiceStatus =  VOICE_UNAVAILABLE;
	voiceStatus = ( 0 == pstHspa->sDeviceCount ) ||( -1 == pstHspa->sDeviceCount )
				|| ( 0 == pstHspa->sSimExist ) || ( 255 == pstHspa->sSimExist )
				|| ( 1 == pstHspa->sPinPukNeed ) ||( 2 == pstHspa->sPinPukNeed )
				|| ( 0 == pstHspa->sSysMode ) || ( -1 == pstHspa->sSysMode );
	
	if ( ( g_stHspaBackup.sDeviceCount != pstHspa->sDeviceCount )
		|| ( g_stHspaBackup.sSimExist != pstHspa->sSimExist ) 
		||( g_stHspaBackup.sPinPukNeed != pstHspa->sPinPukNeed ) 
		|| ( g_stHspaBackup.sSysMode != pstHspa->sSysMode ))
	{
		if ( voiceStatus != voiceStatusBackup )
		{
			voiceStatusBackup = voiceStatus;
			return HSPA_OK;
		}
	}
	
	return -1;
}
/* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add end.*/

/*begin:Added by luokunling 00192527 for imsi info requirement,2011/9/19*/
//*********************************************************************************
// Function Name: DelayTimeInformIMSI
// Description  : ���ز������ݿ����������Զ���pin���ӳ��ϱ�IMSIֵ
// Returns      : void
// �޸���ʷ:
//                     �����ɺ���
//                     �޸���:luokunling
//                     �޸�����:2011/9/19
//*********************************************************************************
void DelayTimeInformIMSI(void* arg)
{
   int DelayTime = 60 ;
   int Number = 0;
   FILE *tr69cpidflie = NULL;
   int tr69cpid = 0;
   struct stAtSndMsg stAtFromOther;
   
   while(Number < DelayTime)
   {
        tr69cpidflie = fopen(TR69_PID_FILE,"r");
	 if(NULL != tr69cpidflie)   /*tr69c�����������������ݿ�����IMSI����*/
	 {
	    if(1 == g_iHspaWeatherAutoPin)
	    {
        	    AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, IMSI_QUERY, "AT+CIMI");
        	    usleep(500);
	    }
	    else
	    {
	           TR069_SendMessage(TTYUSB_INFORM_TR069,NULL,0);
	    }
		   
	    g_iDelaytime_flg = 0;   
		fclose(tr69cpidflie);	
		break;  
	 }
	 
	 Number = Number + 3;
	 sleep(3);
   }
   
   g_iDelaytime_flg = 0;
   
}
/*End:Added by luokunling 00192527 for imsi info requirement,2011/9/19*/



int HspaMatchRead(char *pszInfoBack)
{
    int lLen = 0;
	if ( NULL == strstr(pszInfoBack, "\r\n") )
	{
		memset(pszInfoBack, 0, AT_RCV_MSG_LENGTH);
		lLen= read(g_iPcuiFd, pszInfoBack, AT_RCV_MSG_LENGTH);
		//APN ZHOURONGFEI KF34753
		if(NULL==strstr(pszInfoBack,"CGDCONT"))
		{
        		TTY_LOG("[ %s ]\n",pszInfoBack);
		}
		//APN ZHOURONGFEI KF34753
	}
    return lLen;

}

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define HSPA_DEVICE_COUNT "Usb-Serial Port Num:3\r\nOpen:1\r\n"
#define HSPA_ERR_LIMIT      10

#ifdef CX_DEBUG_VERSION	
#define cx_printf(format, args...) printf(format, ## args)
#else
#define cx_printf(format, args...)
#endif

int glbSpecStick = 0;   /*�Ƿ�����������ݿ�����e180�������ܷ�cfun��0��ֻ��
                          ��cfun��7*/
char glbStick[]="K3715";
                          
int main()
{
	pid_t pid;
	pid =  fork();
	if( 0==pid)
	{
		cx_printf("%s,%d,%s():child process going!\n",__FILE__, __LINE__, __FUNCTION__);
        	TTY_LOG("child process going!\n");
		ttyusb_main();
	}
	else
	if(pid!=0)
	{
		cx_printf("%s,%d,%s():parent process exit!\n",__FILE__, __LINE__, __FUNCTION__);
        	TTY_LOG("parent process exit!\n");
		exit(0);
	}
	return 0;
}
/*****************************************************************************
 �� �� ��  : main
 ��������  : HSPA����ģ�������������к��������̣߳�
 				һ���̶߳�ȡָ�����Ϣ��һ���߳��·�
 				ATָ��
 �������  : void  
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

 ִ������:
 1�����Ϣ��������
 2��������Ϣ���У�
 3����"/proc/UsbModem"�ļ������������ļ�����ȡ����PID,
 	�ж��豸�ڷ��ڣ�>4����>5��
 4����PCUI�ڣ��豸���ڱ�־��1����ʼ���������ԣ���ʼ��HSPA�豸
 	���ͻ�ȡ�����ļ��в�����ATָ�
 5���豸���ڱ�־��0��
 6��д�빲���ļ���PCUI��дʹ����1��
 8��������ȡPCUI���̡߳�����дPCUI�ں�����

*****************************************************************************/

int ttyusb_main( void )
{
	pthread_t thread_Write;/*дPCUI�ڵ��̱߳�־*/
	pthread_t thread_SendSysinfo;/*��AT^SYSINFO�̱߳�־*/
	int fdProc = -1;
    int fd = -1;
	char acPid[16];
    char acCmd[64];
	/*�趨�źŴ�����*/
	signal(SIGUSR1, HspaDeviceSignalProcess);
    //add by z67625
    signal(SIGUSR2,debugswitch);
    fd = open(TTYPID, O_RDWR|O_CREAT);
    if (fd < 0)
    {
        TTY_DEBUG("open /var/ttypid failed\n");
        return 0;
    }
    else
    {
        sprintf(acPid, "%d", getpid());
        write(fd, acPid, sizeof(acPid));
        fsync(fd);
        close(fd);
    }
    //add by z67625
	
	if (0 != pthread_mutex_init(&work_mutex,NULL))
	{
	    TTY_DEBUG("init work_mutex failed\n");
		return 0;
	}
	if( 0 != sem_init(&sem, 0, 1) )
	{
	    TTY_DEBUG("init sem failed\n");
		return 0;
	}

	memset(acPid, 0, sizeof(acPid));
	memset(&g_stHspa, 0, sizeof(struct stHspaInfo));

	/*���������ļ�*/
	while(g_ShareiFd<0)
		g_ShareiFd = open(HSPA_SHARE_FILE_PATH, O_RDWR|O_CREAT);
	cx_printf("%s, %d, %s, pid = %d\n", __FILE__, __LINE__, __FUNCTION__, getpid());
    TTY_LOG("pid = %d\n",getpid());
	g_stHspa.lPid =getpid();
	/*���豸�����ļ��е���������Ϊδ֪	*/
	g_stHspa.sDeviceCount = HSPA_UNKNOW;
	g_stHspa.sDeviceStatus = HSPA_UNKNOW;
	g_stHspa.sSimExist = HSPA_UNKNOW;
	g_stHspa.sSimStatus = HSPA_UNKNOW;
	g_stHspa.sSysMode = HSPA_UNKNOW;
	g_stHspa.sSubSysMode = HSPA_UNKNOW;
	g_stHspa.sSrvStatus = HSPA_UNKNOW;
	g_stHspa.sPinPukNeed= HSPA_UNKNOW;

	/* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add begin:*/
	BackupHspaInfo(&g_stHspa);
	/* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add end.*/

    fdProc = open("/proc/UsbModem", O_RDWR);
    if (fdProc < 0)
    {
        TTY_DEBUG("open /proc/UsbModem failed\n");
        return 0;
    }
    sprintf(acPid, "%d", g_stHspa.lPid);
    write(fdProc, acPid, sizeof(acPid));
    fsync(fdProc);
    close(fdProc);

	/*����*/
	lseek(g_ShareiFd, 0, SEEK_SET);
	write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
	fsync(g_ShareiFd);


	/*������Ϣ����*/
	while ( g_lAtMsgId < 0 )
		g_lAtMsgId = msgget((key_t)MSG_AT_QUEUE, IPC_CREAT |0666);/*HSPA����ģ����Ϣ����ID*/

	while ( g_lModemMsgId < 0 )
		g_lModemMsgId = msgget((key_t)MSG_MODEM_QUEUE,  IPC_CREAT |0666);/*����ҵ����Ϣ����ID*/

	while ( g_lVoiceMsgId < 0 )
		g_lVoiceMsgId = msgget((key_t)MSG_VOICE_QUEUE,  IPC_CREAT |0666);/*����ҵ����Ϣ����ID*/

	while ( g_lTr069MsgId < 0 )
		g_lTr069MsgId = msgget((key_t)MSG_TR069_QUEUE,  IPC_CREAT |0666);/*tr069��Ϣ����ID*/

	while ( g_lHttpMsgId < 0 )
		g_lHttpMsgId = msgget((key_t)MSG_HTTP_QUEUE,  IPC_CREAT |0666);/*WEB���ò�ѯ��Ϣ����ID*/

	while ( g_lLedMsgId < 0 )
		g_lLedMsgId = msgget((key_t)MSG_LED_QUEUE,  IPC_CREAT |0666);/*LEDָʾ����Ϣ����ID*/

#ifdef CX_DEBUG_VERSION	
	while ( g_lCmdMsgId < 0 )
		g_lCmdMsgId = msgget((key_t)MSG_CMD_QUEUE,  IPC_CREAT |0666);/*׮������Ϣ����ID*/
#endif
#ifdef POTS_PINMNGR
    while ( g_lPinMngrMsgId < 0 )
        g_lPinMngrMsgId = msgget((key_t)MSG_PINMNGR_QUEUE,  IPC_CREAT |0666);/*����PIN������Ϣ����ID*/
#endif

#ifdef CX_DEBUG_VERSION	
	cx_printf( "g_lAtMsgId = %d\n", g_lAtMsgId);
	cx_printf( "g_lModemMsgId = %d\n", g_lModemMsgId);
	cx_printf( "g_lVoiceMsgId = %d\n", g_lVoiceMsgId);
	cx_printf( "g_lTr069MsgId = %d\n", g_lTr069MsgId);
	cx_printf( "g_lHttpMsgId = %d\n", g_lHttpMsgId);
	cx_printf( "lCmdMsgId = %d\n", g_lCmdMsgId);
#ifdef POTS_PINMNGR    
    cx_printf( "g_lPinMngrMsgId = %d\n", g_lPinMngrMsgId);
#endif
#endif
    TTY_LOG( "g_lAtMsgId = %d\n", g_lAtMsgId);
	TTY_LOG( "g_lModemMsgId = %d\n", g_lModemMsgId);
	TTY_LOG( "g_lVoiceMsgId = %d\n", g_lVoiceMsgId);
	TTY_LOG( "g_lTr069MsgId = %d\n", g_lTr069MsgId);
	TTY_LOG( "g_lHttpMsgId = %d\n", g_lHttpMsgId);
		//HspaDeviceSignalProcess(1);
        HspaDeviceDetect();

	/*��USB-SERIAL��IFD�����ӽ����շ���Ϣѭ��
	����:Ŀǰ��û���ǵ����豸��ѭ�����豸�Ͽ���δ���?	*/	
	cx_printf("%s, %d, %s(),pid = %d\n", __FILE__, __LINE__, __FUNCTION__, getpid());
	while ( 0 != pthread_create(&thread_Write,  NULL, WritePcui, NULL) )
		;
	/* start of VOICE ���������� by pengqiuming(48166), 2008/9/9 
	   for ���3G������,����E180���ݿ�һֱ���������.��ΪE180����ʱ��ϳ�,�������ݿ���ϵͳ����ʱ����AT^SYSINFO,���ص�SysMode��δ�仯
	   ������ֻ���յ����ݿ�����������^MODEָ�����ǲŻᷢ��AT^SYSINFO. ����ttyUSB����ʱ����һ�߳�ÿ��10��һ��AT^SYSINFO,�������붼���޸�,��֤��Ӱ��ԭ�е�����.*/
	usleep(100);
	while ( 0 != pthread_create(&thread_SendSysinfo,  NULL, SendSysinfo, NULL) )
		;
	/* end of VOICE ���������� by pengqiuming(48166), 2008/9/9 */
	cx_printf("%s, %d, %s(),pid = %d\n", __FILE__, __LINE__, __FUNCTION__, getpid());
	ReadPcui();
	cx_printf("%s, %d, %s(),pid = %d\n", __FILE__, __LINE__, __FUNCTION__, getpid());
	pthread_mutex_destroy(&work_mutex);
	sem_destroy(&sem);
	return 0;
	
}

/*****************************************************************************
 �� �� ��  : ImsiInfoGet
 ��������  : 
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��16��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static int  ImeiInfoGet()
{
	char acInitHspa[COMMON_LONG_STRING_LENGTH];
	char *pChar = NULL;
	int len = 0;
	int i = 0;
	len = write(g_iPcuiFd, "AT+CGSN\r", strlen("AT+CGSN\r"));
	if (len <0 )
	{
	    TTY_DEBUG("write AT+CGSN failed\n");
		return HSPA_ERROR;
	}
	
	usleep(20);
	len = read(g_iPcuiFd, acInitHspa, sizeof(acInitHspa));
	if (len <0 )
	{
	    TTY_DEBUG("read pcuifd failed\n");
		return HSPA_ERROR;
	}	
	cx_printf("%s, %d, %s(), acInitHspa = %s\n",__FILE__, __LINE__, __FUNCTION__, acInitHspa);
	
	if ( NULL == strstr(acInitHspa, "AT+CGSN") )/*�жϷ�����Ϣ*/
	{
		memset(acInitHspa, 0, sizeof(acInitHspa));
		len = read(g_iPcuiFd, acInitHspa, sizeof(acInitHspa));
		if (len <0 )
		{
		    TTY_DEBUG("read pcuifd failed\n");
			return HSPA_ERROR;
		}
	}
	cx_printf("%s, %d, %s(), acInitHspa = %s\n",__FILE__, __LINE__, __FUNCTION__, acInitHspa);
	
	if ( NULL != strstr(acInitHspa, "AT+CGSN") )/*�жϷ�����Ϣ*/
	{
		if(  NULL == strstr(acInitHspa, "\r\n") )
		{
			memset(acInitHspa, 0, sizeof(acInitHspa));
			len = read(g_iPcuiFd, acInitHspa, sizeof(acInitHspa));
			if (len <0 )
			{
			    TTY_DEBUG("read pcuifd failed\n");
				return HSPA_ERROR;
			}
		}
		cx_printf("%s, %d, %s(), acInitHspa = %s\n",__FILE__, __LINE__, __FUNCTION__, acInitHspa);
		
		if ( NULL != strstr(acInitHspa, "OK") )/*�жϷ�����Ϣ�Ƿ�OK*/
		{
			pChar = strstr(acInitHspa, "\r\n");
			if ( NULL == pChar)
			{
			    TTY_DEBUG("acInitHspa string error\n");
				return;
			}
			pChar += 2;

            len = strlen(pChar);
			while( i <= len )
			{
				if ( isdigit(*pChar) )
				{
					g_stHspa.acIMEI[i] = *pChar;
				}
				else
				{
					g_stHspa.acIMEI[i] = 0;
					break;
				}
				++pChar;
				++i;				
			}
		}
	}
	cx_printf("%s, %d, %s(), acIMEI = %s\n",__FILE__, __LINE__, __FUNCTION__, g_stHspa.acIMEI);
    TTY_LOG("acIMEI = %s\n",g_stHspa.acIMEI);
	lseek(g_ShareiFd, 0,SEEK_SET);
	write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
	fsync(g_ShareiFd);
	return HSPA_OK;
	
}

/*****************************************************************************
 �� �� ��  : PinPukProcess
 ��������  : 
 �������  : char *pszInitHspa  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��15��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static int PinPukProcess(char *pszInitHspa)
{
	
#define IFC_IMEI_LEN         16          //IMEI��������С
#define IMEI_OFFSET          96          //IMEI��Ա����洢���׵�ַ��ƫ����
#define IFC_PIN_PASSWORD_LEN   16          //PIN��Ļ�������С
#define PINCODE_OFFSET         80          //��Ա����洢���׵�ַ��ƫ����  	  
 	char acPincode[IFC_PIN_PASSWORD_LEN] = {0};
	char acInitHspa[COMMON_LONG_STRING_LENGTH];
	int len = 0;
    int i = 0;
    /* HUAWEI HGW s48571 2008��6��28�� �����ݿ�E160�Խ����⣨��ʼ��ʱ��ϳ������ܷ����Ƿ���ҪPIN�룩 add begin. */
    int iRet = HSPA_OK;
    /* HUAWEI HGW s48571 2008��6��28�� �����ݿ�E160�Խ����⣨��ʼ��ʱ��ϳ������ܷ����Ƿ���ҪPIN�룩 add end. */
    
    memset(acInitHspa, 0, COMMON_LONG_STRING_LENGTH);
    TTY_DEBUG("pszInitHspa = %s\n", pszInitHspa);
	memcpy(acInitHspa, pszInitHspa, COMMON_LONG_STRING_LENGTH);

    for (i = 0; i < MAX_ATCMD_CNT; i++)
    {
        if ( NULL == strstr(acInitHspa,"AT+CLCK=\"SC\",2") )
    	{
    		memset(acInitHspa, 0, COMMON_LONG_STRING_LENGTH);
    		len = read(g_iPcuiFd, acInitHspa, COMMON_LONG_STRING_LENGTH);
    		if ( len < 0 )
            {   
                TTY_DEBUG("read pcuifd failed\n");
    			return HSPA_ERROR;
            }
            TTY_DEBUG("acInitHspa = %s\n", acInitHspa);
    	}
        else
        {
            break;
        }
    }

    for (i = 0; i < MAX_ATCMD_CNT; i++)
    {
        if ( (NULL == strstr(acInitHspa,"+CME ERROR"))&&(NULL == strstr(acInitHspa,"OK"))&& (NULL == strstr(acInitHspa,"ERROR")) )
    	{
    		memset(acInitHspa, 0, COMMON_LONG_STRING_LENGTH);
    		len = read(g_iPcuiFd, acInitHspa, COMMON_LONG_STRING_LENGTH);
    		if ( len < 0 )
            {   
                TTY_DEBUG("read pcuifd failed\n");
    			return HSPA_ERROR;
            }
            TTY_DEBUG("acInitHspa = %s\n", acInitHspa);
    	}
        else
        {
            break;
        }
    }

    
	/*�ж��Ƿ����޿�����*/
	if ( NULL != strstr(acInitHspa, "+CME ERROR: 13") ) /*SIM������*/
	{
        TTY_DEBUG("No SIM, blink HSPA LED\n");

#if 0
        if( (g_HspaLine & HSPA_LINE_MASK) == HSPA_ON )
        {
    		/* BEGIN: Modified by c106292, 2009/3/17   PN:Led light method changes*/
    		//	sysLedCtrl(kLedHspa, kLedStateFastBlinkContinues);
    		sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);
    		/* END:   Modified by c106292, 2009/3/17 */
        }
#endif
        sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);
		g_stHspa.sSimExist = HSPA_NO_SIM;
	}
	else
	{
		g_stHspa.sSimExist = HSPA_EXIST;
		cx_printf("%s, %d, %s(), pszInitHspa = %s\n",__FILE__, __LINE__, __FUNCTION__, acInitHspa);
		/*�жϿ��Ƿ���ҪPIN�룬��PUK��*/
		if (( NULL == strstr(acInitHspa, "ERROR")) && ( NULL != strstr(acInitHspa, "OK") )  ) /*����ҪPINУ��*/
		{
			cx_printf("%s, %d, %s(), Don't need Pin!\n",__FILE__, __LINE__, __FUNCTION__);
			g_stHspa.sPinPukNeed = HSPA_PIN_PUK_OK;
		}		
		else
		if ( NULL != strstr(acInitHspa, "+CME ERROR: 11") ) /*ҪPINУ��*/
		{
			/*��ȡPIN������ݿ�IMEI����*/
			memset(acInitHspa, 0, COMMON_LONG_STRING_LENGTH);
			memset(acPincode, 0, IFC_PIN_PASSWORD_LEN);
			/* HUAWEI HGW s48571 2008��3��28�� FLASH ���� modify begin:
			sysVariableGet(acPincode, IFC_PIN_PASSWORD_LEN, PINCODE_OFFSET);
			*/

			//sysAvailGet(acPincode, IFC_PIN_PASSWORD_LEN, PINCODE_OFFSET);

			
			
			strcpy(acPincode, g_stHspa.acPinCode);
			
			/* HUAWEI HGW s48571 2008��3��28�� FLASH ���� modify end. */
			/* HUAWEI HGW s48571 2008��3��28�� FLASH ���� modify begin:
			sysVariableGet(acInitHspa, IFC_IMEI_LEN, IMEI_OFFSET);
			*/

			//sysAvailGet(acInitHspa, IFC_IMEI_LEN, IMEI_OFFSET);

			//strcpy(acInitHspa, g_stHspa.acOldIMEI);
			
			/* HUAWEI HGW s48571 2008��3��28�� FLASH ���� modify end. */
            TTY_LOG("PIN CODE = %s\n IMEI in Flash = %s",acPincode, acInitHspa);

			if ( HSPA_ERROR == ImeiInfoGet() )
			{
			    TTY_DEBUG("ImeiInfoGet failed");
				return HSPA_ERROR;
			}

			/*��ȡ���ݿ�����*/
			if ( (NULL == strstr(g_stHspa.acOldIMEI, g_stHspa.acIMEI))  && (g_stHspa.acOldIMEI[0] != '\0'))
			{
				g_stHspa.sPinPukNeed = HSPA_PIN_NEED;
                TTY_DEBUG("Need PIN Code,blink HSPA LED\n");
#if 0
                if( (g_HspaLine & HSPA_LINE_MASK) == HSPA_ON )
                {
					/* BEGIN: Modified by c106292, 2009/3/17   PN:Led light method changes*/
					//	sysLedCtrl(kLedHspa, kLedStateFastBlinkContinues);
					sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);
                }
#endif
                sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);

			}
			else
			{
				/*���¹����ļ������ݿ�IMEI����*/
			//	memcpy(g_stHspa.acIMEI, acInitHspa, HSPA_IMEI_LENGTH);
				/*�·�ATָ����֤PIN��*/
				memset(acInitHspa, 0, sizeof(acInitHspa));
				sprintf(acInitHspa, "AT^CPIN=\"%s\"\r", acPincode);
				write(g_iPcuiFd, acInitHspa, strlen(acInitHspa));

				/*�ж��Ƿ�ɹ�*/
				memset(acInitHspa, 0, sizeof(acInitHspa));
				len = read(g_iPcuiFd, acInitHspa, COMMON_LONG_STRING_LENGTH);
				if ( len < 0 )
				{
				    TTY_DEBUG("read pcuifd failed\n");
					return HSPA_ERROR;
				}
				
                TTY_DEBUG("acInitHspa = %s\n", acInitHspa);
				if ( NULL != strstr(acInitHspa, "ERROR") )
				{
					g_stHspa.sPinPukNeed = HSPA_PIN_NEED;
                    TTY_DEBUG("Need PIN, blink HSPA LED!");
                    sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);

#if 0
                    if( (g_HspaLine & HSPA_LINE_MASK) == HSPA_ON )
                    {

					/* BEGIN: Modified by c106292, 2009/3/17   PN:Led light method changes*/
						//	sysLedCtrl(kLedHspa, kLedStateFastBlinkContinues);
					  sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);
					/* END:   Modified by c106292, 2009/3/17 */
                    }
#endif
				}
				else
				{
				
                    for(i = 0; i < MAX_ATCMD_CNT; i++ )
                    {
    					if  (( NULL == strstr(acInitHspa, "OK") ) && ( NULL == strstr(acInitHspa, "ERROR") ))
    					{
    						memset(acInitHspa, 0, sizeof(acInitHspa));
    						len = read(g_iPcuiFd, acInitHspa, COMMON_LONG_STRING_LENGTH);
    						if ( len < 0 )
    						{
    						    TTY_DEBUG("read pcuifd failed\n");
    							return HSPA_ERROR;
    						}
                            TTY_DEBUG("acInitHspa = %s\n", acInitHspa);
    					}
                        else
                        {
                            break;
                        }
                    }
					if( NULL != strstr(acInitHspa, "OK") )
					{/*��֤ͨ��*/
						g_stHspa.sSimStatus = HSPA_VALID_SIM;
						g_stHspa.sPinPukNeed = HSPA_PIN_PUK_OK;
                        TTY_DEBUG("PIN Verificate OK!");
						/*�����ϵ�IMEI��Ϣ*/
						strcpy(g_stHspa.acOldIMEI, g_stHspa.acIMEI);

                        /* stop blinking led when pin verified correct. <2009.1.5 tanyin> */
                        TTY_DEBUG("PIN verified, turn on HSPA LED!");
                        sysLedCtrl(kLedHspa, kLedStateOn);

#if 0
                        if( (g_HspaLine & HSPA_LINE_MASK) == HSPA_ON )
                        {
                            TTY_DEBUG("PIN verified, turn on RED LED!");
			                sysLedCtrl(kLedHspa, kLedStateOn);
                        }
#endif
					}
					else
					{/*PIN�뱻�ģ���Ҫ�û�����PIN��*/
						g_stHspa.sPinPukNeed = HSPA_PIN_NEED;
                        TTY_DEBUG("Need PIN, blink HSPA LED!");
                        sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);

#if 0
                        if( (g_HspaLine & HSPA_LINE_MASK) == HSPA_ON )
                        {
                            TTY_DEBUG("PIN is needed, fast blink RED LED!");
						/* BEGIN: Modified by c106292, 2009/3/17   PN:Led light method changes*/
							//	sysLedCtrl(kLedHspa, kLedStateFastBlinkContinues);
						  sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);
						/* END:   Modified by c106292, 2009/3/17 */
                        }
#endif
					}				
				}
			}
		}
		else
		if ( NULL != strstr(acInitHspa, "+CME ERROR: 12")) /*PUKУ��*/
		{/*��Ҫ�û�����PUK��*/
			g_stHspa.sPinPukNeed = HSPA_PUK_NEED;
			/*���*/

            TTY_DEBUG("PUK is needed, blink HSPA LED!");
            sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);

#if 0
	            if( (g_HspaLine & HSPA_LINE_MASK) == HSPA_ON )
	            {
	                TTY_DEBUG("PUK is needed, turn on RED LED!");
        			/* BEGIN: Modified by c106292, 2009/3/17   PN:Led light method changes*/
      				//	sysLedCtrl(kLedHspa, kLedStateFastBlinkContinues);
       			    sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);
	            }
#endif
		}
		else/**/
		{
		    TTY_DEBUG("System can't know whether PIN/PUK need or not! Error!!!!!\n");
            /* HUAWEI HGW s48571 2008��6��28�� �����ݿ�E160�Խ����⣨��ʼ��ʱ��ϳ������ܷ����Ƿ���ҪPIN�룩 add begin. */
            iRet = HSPA_ERROR_OPERATION_REFUSED;
            /* HUAWEI HGW s48571 2008��6��28�� �����ݿ�E160�Խ����⣨��ʼ��ʱ��ϳ������ܷ����Ƿ���ҪPIN�룩 add end. */
		}	
	}
	lseek(g_ShareiFd, 0,SEEK_SET);
	write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
	fsync(g_ShareiFd);

	/* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add begin:*/
	if ( HSPA_OK == CheckHspaInfo(&g_stHspa))
	{
		sysWakeupMonitorTask();
        TTY_DEBUG("sysWakeupMonitorTask\n");
	}
    
	BackupHspaInfo(&g_stHspa);
	/* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add end.*/

	/* HUAWEI HGW s48571 2008��6��28�� �����ݿ�E160�Խ����⣨��ʼ��ʱ��ϳ������ܷ����Ƿ���ҪPIN�룩 modify begin:
	return HSPA_OK;
	*/
	return iRet;
	/* HUAWEI HGW s48571 2008��6��28�� �����ݿ�E160�Խ����⣨��ʼ��ʱ��ϳ������ܷ����Ƿ���ҪPIN�룩 modify end. */
	
}
/*****************************************************************************
 �� �� ��  : AtRcv
 ��������  : ҵ�����
 �������  : int lMsgKey                     
             struct stAtRcvMsg *pstAtRcvMsg  
             int lMsgType                    
 �������  : ��
 �� �� ֵ  : static inline int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
int AtRcv(int lMsgKey, struct stAtRcvMsg *pstAtRcvMsg, int lMsgType)
{
	memset(pstAtRcvMsg, 0, sizeof(*pstAtRcvMsg));
	return  msgrcv(lMsgKey, (void*)pstAtRcvMsg, AT_RCV_MSG_LENGTH, lMsgType, 0);
}
/*****************************************************************************
 �� �� ��  : AtSend
 ��������  : 
 �������  : int lMsgKey                     
             struct stAtSndMsg *pstAtSndMsg  
             char cFrom                      
             int lMsgType                    
             const char *pszParam            
 �������  : ��
 �� �� ֵ  : static inline int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
int AtSend(int lMsgKey, struct stAtSndMsg *pstAtSndMsg, char cFrom,  int lMsgType,  const char *pszParam)
{
	int flag = -1;
	memset(pstAtSndMsg, 0, sizeof(*pstAtSndMsg));	
	pstAtSndMsg->lMsgType = lMsgType;
	pstAtSndMsg->acParam[HSPA_FROM_POSITION] = cFrom;
	if(NULL!= pszParam)
		strcpy(pstAtSndMsg->acParam+1, pszParam);
	while ( flag < 0 )
	{
		flag = msgsnd(lMsgKey, (void*)pstAtSndMsg, AT_SND_MSG_LENGTH, IPC_NOWAIT);
		if ( flag < 0 )
		{
		    TTY_DEBUG("Snd msg failed\n");
			usleep(100);
		}
	}
	return flag;
}

void HspaSwtichDelay(void)
{
    int i = 0, success_flag = 0;
    int iRetryTimes = 0;
	struct stAtSndMsg stAtFromOther;

    g_HspaLine = g_HspaLine & HSPA_STATE_MASK;
    AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, CFUN_QUERY, "AT+CFUN?");
    AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, SYSTEM_INFO_QUERY, "AT^SYSINFO");
}
/*****************************************************************************
 �� �� ��  : HspaDeviceSignalProcess
 ��������  : 
		 �źŴ����������������:
		һ��ͻȻ�Ͽ����ݿ�:
			1����ȡUSBMODEM�ļ��ж��豸�ڷ�
			2���ر�PCUI��
			3��������Ϣ������û�д������Ϣ������ҵ��ģ�飬�����豸���ˡ�
			����:���̻᲻�������ڶ�д�����ĵط���
		����HG��������ʱ��������ݿ�:
			1����ȡUSBMODEM�ļ��ж��豸�ڷ�
			2�����´�PCUI�ڣ�
			3�����³�ʼ��PCUI��
			4����������ҵ��ģ�鷢��������Ϣ			

 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��10��
    ��    ��   : ����
    �޸�����   : �����ɺ���
ִ������:
 3����"/proc/UsbModem"�ļ������������ļ�����ȡ����PID,
 	�ж��豸�ڷ��ڣ�>4����>5��
 4����PCUI�ڣ��豸���ڱ�־��1����ʼ���������ԣ���ʼ��HSPA�豸
 	���ͻ�ȡ�����ļ��в�����ATָ�
 5���豸���ڱ�־��0��
 6��д�빲���ļ���PCUI��дʹ����1��
*****************************************************************************/
void HspaDeviceSignalProcess(int x)
{
	  g_SignalRcv = 1;

	 
}

/*add by w00135351 20080916*/
int TR069_SendMessage(long msg_type, void *msg_content, int content_len)
{
    int msgid;
    int iKey;
    int iPid;
    S_TR069_MSG sMsg;
  
    if (content_len > TR069_MAX_CONTENT)
    {
        return -1;
    }
    /* start of maintain ������tr69c����ʱ��δ���ҳ������tr069���������Ϣ�ᵼ����ҳ������������ by xujunxia 43813 2006��9��13��*/
    FILE *pidfile;
    int  tr69c_pid = 0;
    pidfile = fopen("/var/tr69/tr69pid", "r");
    if (pidfile == NULL) 
    {
        return -1;
    }  
    else
    {
        if (fscanf(pidfile, "%d", &tr69c_pid) != 1) 
        {
           fclose(pidfile);
    	    return -1;
        }
        fclose(pidfile);
    }
    /* end of maintain ������tr69c����ʱ��δ���ҳ������tr069���������Ϣ�ᵼ����ҳ������������ by xujunxia 43813 2006��9��13�� */
    
    iKey = ftok(TR069_KEY_PATH, TR069_KEY_SEED);
    msgid = msgget(iKey, IPC_CREAT | 0660);
    if (-1 == msgid) 
    {
        printf("Get message queue failed.\n");
        return -1;
    }

    iPid = getpid();
    
    memset(&sMsg, 0, sizeof(S_TR069_MSG));
    sMsg.lMsgType = msg_type;
    if (msg_content != NULL)
    {
        memcpy(sMsg.achContent, msg_content, content_len);
    }
    if (-1 == msgsnd(msgid, &sMsg, sizeof(sMsg) - sizeof(long), 0))
    {
        printf("TR069_SendMessage send error  %d\n");
        return -1;
    }

    return 0;
}
/*end by w00135351 20080916*/

getCgmmInfo()
{
    char bufTem[COMMON_LONG_STRING_LENGTH] = {0};
    fd_set read_set;
    struct timeval tvSelect;
    tvSelect.tv_sec  = 5; // ��ȴ�10��
    tvSelect.tv_usec = 0;
    int ret = 0;
    glbSpecStick = 0;
    int i = 0;
    
    write(g_iPcuiFd, "AT+CGMM\r", strlen("AT+CGMM\r"));
    
    while (1)
    {
        memset(bufTem, 0, sizeof(bufTem));
        FD_ZERO(&read_set);
        FD_SET(g_iPcuiFd, &read_set);
        ret = select(g_iPcuiFd + 1, &read_set, NULL, NULL, &tvSelect);
        if (ret > 0)
        {
            ret = read(g_iPcuiFd, bufTem, COMMON_LONG_STRING_LENGTH);
            if (0 < strlen(bufTem))
            {
                TTY_DEBUG("==%s:%s:%d:%s:VHG==\n",__FILE__,__FUNCTION__,__LINE__,bufTem);
                if ( strstr(bufTem,"OK")||strstr(bufTem,"CME ERROR") )
                {
                    if ( strstr(bufTem,glbStick))
                    {
                        TTY_DEBUG("Stick is %s",glbStick);
                        glbSpecStick = 1;
                    }
                    else
                    {
                        TTY_DEBUG("Stick is not %s",glbStick);
                        glbSpecStick = 0;
                    }
                    break;
                }
            }
            else if ( HSPA_NO == g_stHspa.sDeviceCount)
            {
                TTY_DEBUG("No HSPA Card\n");
                 break;
            }

        }
        else if ( HSPA_NO == g_stHspa.sDeviceCount)
        {
            TTY_DEBUG("No HSPA Card\n");
             break;
        }
        if ( i++ > 10 )
        {
            TTY_DEBUG("==%s:%s:%d:get the response of cgmm erro!==\n",__FILE__,__FUNCTION__,__LINE__);
            return;
        }
    }


}

void HspaDeviceDetect(void)
{
      /*Begin:Added by luokunling 00192527 for new requirement,IMSI report,2011/9/29*/
       FILE *ftr69cpid_file = NULL;
	pthread_t delaytimethread;
	int itr69c_pid = 0;
      /*End:Added by luokunling 00192527,2011/9/29*/
	struct termios stTerm;
	struct stAtSndMsg stAtFromOther;
	int lDeviceFd = -1;
	char acInitHspa[COMMON_LONG_STRING_LENGTH];
    char* pChar = NULL;
    char* pTmp = NULL;
	int fd;
	int lLen = 0;
    char CendCmd[64] = "";
    int lFlag = 0;
    FILE* fs = NULL;

    /*add by w00135351*/
    int iLen = 0;
    /*end by w00135351*/
    /*Begin:Added by luokunling 00192527 for new requirement,IMSI report,2011/9/29*/
     ftr69cpid_file = fopen(TR69_PID_FILE,"r");
     if(NULL == ftr69cpid_file)
     {
          g_iDelaytime_flg = 1;
     }
     else
     {
         fclose(ftr69cpid_file);
     }     
    /*End:Added by luokunling 00192527,2011/9/29*/
    fs = fopen(HSPA_SYSCFG_FILE, "r");
    if (!fs)
    {
        return;
    }

    fclose(fs);
    fs = NULL;
    
	cx_printf( "Signal process function begin!\n" );		 	
    TTY_LOG("Signal process function begin!\n");
	/*��"/proc/UsbModem"�ļ�*/
	while ( lDeviceFd<0 )
	{
	    sleep(3);
		lDeviceFd = open("/proc/UsbModem",O_RDONLY);
	}
	memset(acInitHspa, 0, sizeof(acInitHspa));
	/* start of VOICE ���������� by pengqiuming(48166), 2008/10/14 
	   for �ú��滻*/
	read(lDeviceFd,acInitHspa, HSPA_MAX_LINE_LEN);/*"Usb-Serial Port Num:4\r\n"*/
	/* end of VOICE ���������� by pengqiuming(48166), 2008/10/14 */
	close(lDeviceFd);

	/*��ʱ����������ݿ���������*/
	pChar = strstr(acInitHspa, ":");
	cx_printf("%s \n", acInitHspa);

	if (NULL == pChar)
	{
        TTY_LOG("HSPA device count parse Error!\n");
	    return;
	}

	/*�ж��豸�ڷ�*/
	if ( atoi(pChar+1) > 1 )
	{/*���豸���ڱ�־��Ϊ1����PCUI�ڴ�*/
        TTY_LOG("HSPA device exist!\n");
		//sysLedCtrl(kLedHspa, kLedStateOff);
		    
            if(g_HspaExistFlag == HSPA_EXIST)/*֮ǰ�Ѿ�������HSPA���ݿ�����*/
            {
                   
            	   pTmp = strstr(pChar+1, ":");
                
		   if (!pTmp)
		   {
		   	  return;
		   }

		   if (atoi(pTmp+1) == 0)
	   	   {
	   	   	 g_SignalRcv = 1;
	   	   	 goto OpenTtyChange;
		   }
                return;
            }

            g_iPcuiFd = -1;

		while( g_iPcuiFd < 0 )
		{
			g_stHspa.sDeviceCount = 3;
			g_iPcuiFd = open("/dev/ttyUSB2",O_RDWR);
			if (g_iPcuiFd < 0)
			{			    
                TTY_DEBUG("TRYING to Open ttyUSB1 \n");
			    g_stHspa.sDeviceCount = 2;
			    g_iPcuiFd = open("/dev/ttyUSB1",O_RDWR);
			}
			if ( g_iPcuiFd < 0 )
			{
			    TTY_DEBUG("Open ttyUsb failed");
				goto OpenTtyChange;
			}
		}

        /*���¹�������*/
		g_stHspa.sDeviceStatus = HSPA_UNKNOW;
		g_stHspa.sSimExist = HSPA_UNKNOW;
		g_stHspa.sSimStatus = HSPA_UNKNOW;
		g_stHspa.sSysMode = HSPA_UNKNOW;
		g_stHspa.sSubSysMode = HSPA_UNKNOW;
		g_stHspa.sSrvStatus = HSPA_UNKNOW;
		g_stHspa.sPinPukNeed = HSPA_UNKNOW;
		memset(g_stHspa.acIMEI, 0, HSPA_IMEI_LENGTH);
		memset(&g_stHspa.stDataFlowInfo, 0, sizeof(g_stHspa.stDataFlowInfo));

		/*w00135358 ��ʼ��ȫ�ֱ����ﱣ���pin�룬���Ժ�֧�������˴������޸�*/
   	    memset(g_stHspa.acPinCode, 0, HSPA_IMEI_LENGTH);
	
		/*���¹����ļ�*/
		lseek(g_ShareiFd, 0,SEEK_SET);
		write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
		fsync(g_ShareiFd);

		/* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add begin:*/
		BackupHspaInfo(&g_stHspa);
		/* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add end.*/

		/*��ʼ��PCUI��*/
		if ( tcgetattr(g_iPcuiFd, &stTerm) < 0 )
		{
		    TTY_DEBUG("Get Usb-Serial port's parameter failure!\n");
			goto OpenTtyChange;
		}
		stTerm.c_lflag &= ~( ECHO | ICANON | IEXTEN | ISIG );
		stTerm.c_iflag &= ~( INLCR | ICRNL | IGNCR );
		stTerm.c_iflag &= ~( IXON | IXOFF | IXANY );
		stTerm.c_oflag  = 0;
/*S48571 ttyUSB �������� */
        stTerm.c_cc[VTIME] = 50; /* ���ó�ʱ5 seconds*/ 
        stTerm.c_cc[VMIN] = 0; /* Update the options and do it NOW */


		if ( tcsetattr(g_iPcuiFd, TCSAFLUSH, &stTerm) < 0 )
		{
		    TTY_DEBUG("Set Usb-Serial port's parameter failure!\n");
			goto OpenTtyChange;
		}

        if( HSPA_OFF == (g_HspaLine & HSPA_STATE_MASK))
        {
            lFlag = 1;
        }

		lLen = write(g_iPcuiFd, "AT+CFUN=1\r", strlen("AT+CFUN=1\r"));
		usleep(300);
		lLen = read(g_iPcuiFd, acInitHspa, COMMON_LONG_STRING_LENGTH);
        cx_printf("%s,%d----%s\n",__FILE__, __LINE__,acInitHspa);
        //s48571 modified for the wrong led state when plug in a dongle.
        //g_HspaLine = HSPA_ON;

		/*��ʼ��HSPA�豸*/
		lLen = write(g_iPcuiFd, "AT^CURC=0\r", strlen("AT^CURC=0\r"));
		usleep(100);
		lLen = read(g_iPcuiFd, acInitHspa, COMMON_LONG_STRING_LENGTH);
        /* HUAWEI HGW s48571 2008��8��16�� �����ر�����  add begin:*/
        VoiceJudge();
        /* HUAWEI HGW s48571 2008��8��16�� �����ر�����  add end.*/
		lLen = write(g_iPcuiFd, "AT+CMEE=1\r", strlen("AT+CMEE=1\r"));
		usleep(300);
		lLen = read(g_iPcuiFd, acInitHspa, COMMON_LONG_STRING_LENGTH);

        /* turn on HSPA led */
        sysLedCtrl(kLedHspa, kLedStateOn);

#if 0 //����VDFҪ�󣬴����ݿ���ָʾ��
		lLen = write(g_iPcuiFd, "AT^LIGHT=0\r", strlen("AT^LIGHT=0\r"));
		usleep(300);
		lLen = read(g_iPcuiFd, acInitHspa, COMMON_LONG_STRING_LENGTH);
#endif
/* HUAWEI HGW s48571 2008��6��28�� �����ݿ�E160�Խ����⣨��ʼ��ʱ��ϳ������ܷ����Ƿ���ҪPIN�룩 modify begin:*/

        int lPinCounter = 0;

        for( lPinCounter = 0; lPinCounter < MAX_PIN_TEST; lPinCounter++)
        {

		/*************�����Ƿ���ҪPINУ��,������*************/
		if (strlen(g_stHspa.acPinCode) < 4)
		{
			sysAvailGet(g_stHspa.acPinCode, IFC_PIN_PASSWORD_LEN, PINCODE_OFFSET);
		}
		
   		sysAvailGet(g_stHspa.acOldIMEI, IFC_IMEI_LEN, IMEI_OFFSET);
   		lLen = strlen(g_stHspa.acOldIMEI);

		cx_printf("%s, %d, acOldIMEI in Flash = %s\n",__FILE__, __LINE__, g_stHspa.acOldIMEI);
		
   		while(lLen > 0)
   		{
   			if (isdigit(g_stHspa.acOldIMEI[lLen-1]))
   			{
   				lLen--;
   			}
   			else
   			{
   				memset(g_stHspa.acOldIMEI, 0, sizeof(g_stHspa.acOldIMEI));
   				break;
   			}
   		}

    		
    	
    		lLen = write(g_iPcuiFd, "AT+CLCK=\"SC\",2\r", strlen("AT+CLCK=\"SC\",2\r"));
    		memset(acInitHspa, 0, COMMON_LONG_STRING_LENGTH);
    		usleep(500);
    		lLen = read(g_iPcuiFd, acInitHspa, COMMON_LONG_STRING_LENGTH);
            TTY_DEBUG("acInitHspa = %s\n", acInitHspa);
            
    		if ( lLen < 0 )
    		{
    		    TTY_DEBUG("len =%d\n",lLen);
    			cx_printf("%s,%d----lLen = %d\n",__FILE__, __LINE__, lLen);
    			goto OpenTtyChange;
    		}
            
    		if  (HSPA_OK != PinPukProcess(acInitHspa) )
    		{
    		    TTY_DEBUG("HSPA_OK != PinPukProcess(acInitHspa)\n");
                if ( lPinCounter == (MAX_PIN_TEST - 1))
                {
                    goto OpenTtyChange;
                }
                sleep(5);
    		}
            else
            {
                break;
            }
        }
        /* HUAWEI HGW s48571 2008��6��28�� �����ݿ�E160�Խ����⣨��ʼ��ʱ��ϳ������ܷ����Ƿ���ҪPIN�룩 modify end. */

        if ( HSPA_PIN_PUK_OK == g_stHspa.sPinPukNeed )
        {
            /*Begin:Added by luokunling 00192527 for imsi inform requirement,2011/9/19*/
            g_iHspaWeatherAutoPin = 1;
	     /*Begin:Added by luokunling 00192527 for imsi inform requirement*/	
            lLen = write(g_iPcuiFd, "AT+CLIP=1\r", strlen("AT+CLIP=1\r"));
            memset(acInitHspa, 0, COMMON_LONG_STRING_LENGTH);
            usleep(500);
            lLen = read(g_iPcuiFd, acInitHspa, COMMON_LONG_STRING_LENGTH);
            
            if ( NULL == strstr(acInitHspa, "OK") )
            {
                cx_printf("%s,%d----CLIP set failed.\n",__FILE__, __LINE__);
            }
            else
            {
                cx_printf("%s,%d----CLIP set ok.\n",__FILE__, __LINE__);
            }
        }
  
		/*****************************************************************************/
		g_HspaExistFlag = HSPA_EXIST;

        /*add by w00135351 20080918*/
            //iLen = sizeof(TR069_CONFIG_TTYUSBFO);
        printf("%s %d the g_iHspaWeatherAutoPin is :%d#########\n",__FILE__, __LINE__,g_iHspaWeatherAutoPin);
        if(0 == g_iHspaWeatherAutoPin)
        {
            TR069_SendMessage(TTYUSB_INFORM_TR069,NULL,0);
        }
        /*end by w00135351 20080918*/
        getCgmmInfo();

        /*��ȡhspa���ã�����ע���������ͣ��ָ����ݿ�����ģʽ*/    
        RegisterNtwk();
        
        if (lFlag)
        {
            /*modified by w00135351 20081231 ����E180�ڵ͹����µ���*/
            if ( glbSpecStick )
            {
                lLen = write(g_iPcuiFd, "AT+CFUN=7\r", strlen("AT+CFUN=7\r"));            
            }
            else
            {
                lLen = write(g_iPcuiFd, "AT+CFUN=0\r", strlen("AT+CFUN=0\r"));            
            }
            usleep(500);
            lLen = read(g_iPcuiFd, acInitHspa, COMMON_LONG_STRING_LENGTH);
            cx_printf("%s,%d----%s\n", __FILE__, __LINE__, acInitHspa);
            sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);
            //lDeviceFd = AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, CFUN_SET, "AT+CFUN=0");
            //s48571 modified for the wrong led state when plug in a dongle.            
            //g_HspaLine = HSPA_OFF;
        }
        
		lDeviceFd = AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, CFUN_QUERY, "AT+CFUN?");
		cx_printf("%s,%d----SEND1---%d\n",__FILE__, __LINE__, lDeviceFd);
		lDeviceFd = AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, PIN_ENABLE_QUERY, "AT+CLCK=\"SC\",2");
		cx_printf("%s,%d----SEND2---%d\n",__FILE__, __LINE__, lDeviceFd);
        sleep(5);// s48571 add to prevent short red led perion after pluging into a dongle, 2008.06.21
		lDeviceFd = AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, SYSTEM_INFO_QUERY, "AT^SYSINFO");
		cx_printf("%s,%d----SEND3---%d\n",__FILE__, __LINE__, lDeviceFd);  
		
		/*Begin:Added by luokunling 00192527 for new requirement,IMSI report,2011/9/16*/	
		if(0 == g_iDelaytime_flg)
		{
		    usleep(500);
		    lDeviceFd = AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, IMSI_QUERY, "AT+CIMI");
	           usleep(500);
		}
		else
		{
		   while(0 != pthread_create(&delaytimethread,NULL,DelayTimeInformIMSI,NULL))
		   {
		        ;
		   }
		}
	      /*End:Added by luokunling 00192527 for new requirement.*/
		
	}
	else/*�豸������*/
	{/*���豸���ڱ�־��Ϊ0��
		��PCUI���ļ���־�ر�*/
OpenTtyChange:
        //֪ͨhspa����
        strcpy(CendCmd,"\r\n^CEND:1,0,255,255\r\n");
        EndCallReport(CendCmd);
        
        TTY_DEBUG("There isn't any HSPA device!\n");
        if(g_HspaExistFlag == HSPA_NO)
        {
            cx_printf("%s, %d, %s() HSPA device already pulled out! return! \n", __FILE__, __LINE__, __FUNCTION__);
            return;
        }

        /*�ص�*/
		sysLedCtrl(kLedHspa, kLedStateOff);

		close(g_iPcuiFd);
		g_iPcuiFd = -1;
		/*���¹�������*/
		g_stHspa.sDeviceCount = 0;
		g_stHspa.sDeviceStatus = HSPA_UNKNOW;
		g_stHspa.sSimExist = HSPA_UNKNOW;
		g_stHspa.sSimStatus = HSPA_UNKNOW;
		g_stHspa.sSysMode = HSPA_UNKNOW;
		g_stHspa.sSubSysMode = HSPA_UNKNOW;
		g_stHspa.sSrvStatus = HSPA_UNKNOW;
		g_stHspa.sPinPukNeed = HSPA_UNKNOW;
		
		memset(g_stHspa.acIMEI, 0, HSPA_IMSI_LENGTH);
		memset(&g_stHspa.stDataFlowInfo, 0, sizeof(g_stHspa.stDataFlowInfo));
		/*���¹����ļ�*/
		lseek(g_ShareiFd, 0,SEEK_SET);
		write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
		fsync(g_ShareiFd);	

		g_HspaExistFlag = HSPA_NO;
        
        /*add by w00135351 20080918 ������Ϣ��TR069����*/	
	     g_iHspaWeatherAutoPin = 0;
        //TR069_SendMessage(TTYUSB_INFORM_TR069,NULL,0);
       TR069_SendMessage( TTYUSB_PLUGOUT_INFORM_TR069,NULL,0);
        /*end by w00135351 20080918*/
        
		//֪ͨwan��س���
		if( (fd = open( "/dev/brcmboard", O_RDWR )) != -1 )
		{
			ioctl( fd, BOARD_IOCTL_WAKEUP_MONITOR_TASK, NULL);
			close(fd);
		}

            FILE *fp_pppou = NULL;
            char acCmd[10];
            int pppoupid = 0;
            memset(acCmd, 0x00, 10);
            if(NULL != (fp_pppou = fopen("/var/pppoupid", "r")))
            {
                fgets(acCmd, 10, fp_pppou);
                fclose(fp_pppou);
                pppoupid = atoi(acCmd);
            }
                
            if( pppoupid > 0)
            {
                kill(pppoupid, SIGHUP);
            }
	}
    sleep(2);
    sysWakeupMonitorTask();
    TTY_LOG("Signal process function is over!\n");
	return;
	
}

/*****************************************************************************
 �� �� ��  : SendSysinfo
 ��������  : �����ݿ����ڲ�����LPMʱ,ÿ��10s����ָ��AT^SYSINFO,��ѯϵͳ��Ϣ
 �������  : void* arg  
 �������  : ��
 �� �� ֵ  : static void*
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��9��9��
    ��    ��   : pengqiuming(48166)
    �޸�����   : �����ɺ���

*****************************************************************************/

static void* SendSysinfo(void* arg)
{
	struct stAtSndMsg stAtFromOther;
	memset(&stAtFromOther, 0, sizeof(stAtFromOther));
	
	while(1)
	{
		sleep(15);
		if ((HSPA_EXIST == g_HspaExistFlag) && ((g_HspaLine & HSPA_LINE_MASK) == HSPA_ON))
		{
			AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, SYSTEM_INFO_QUERY, "AT^SYSINFO");
		}
	}
}

/*****************************************************************************
 �� �� ��  : WritePcui
 ��������  : 
 �������  : void* arg  
 �������  : ��
 �� �� ֵ  : static void*
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void*  WritePcui(void* arg)
{
	int wflag = 0;
	int lLen =0;
	static int iErrNum = 0;

	
    signal(SIGALRM,HspaSwtichDelay);
	while(1)
	{
		sem_wait(&sem);
        sem.__sem_value = 0;
		memset(&g_stAtFromOther, 0, sizeof(g_stAtFromOther));
#if 0
cx_printf("sDeviceCount = %d\n", g_stHspa.sDeviceCount);
cx_printf("sDeviceStatus = %d\n", g_stHspa.sDeviceStatus);
cx_printf("sSimExist = %d\n", g_stHspa.sSimExist);
cx_printf("sSimStatus = %d\n", g_stHspa.sSimStatus);
cx_printf("sSysMode = %d\n", g_stHspa.sSysMode);
cx_printf("sSubSysMode = %d\n", g_stHspa.sSubSysMode);
cx_printf("sSrvStatus = %d\n", g_stHspa.sSrvStatus);
cx_printf("sPinPukNeed = %d\n", g_stHspa.sPinPukNeed);
cx_printf("acIMEI = %s\n", g_stHspa.acIMEI);	
#endif
		/*��lAtMsgId����ȡҵ����̷�����������Ϣ��stAtSnd
		wflag��ȡ������Ϣ�Ƿ�ɹ���־*/
        while (-1 == ( wflag = msgrcv(g_lAtMsgId, (void*)&g_stAtFromOther, AT_SND_MSG_LENGTH, 0, 0)))
        {      
            printf("%s,%d,%s(),msgrcv is interrupted, try again\n", __FILE__,  __LINE__, __FUNCTION__);
        }
                 if ( PDP_SET != g_stAtFromOther.lMsgType)
                 {
                            cx_printf("%s,%d,%s(),AT =%d, %s, sem value is %d\n", __FILE__,  __LINE__, __FUNCTION__, g_stAtFromOther.lMsgType, g_stAtFromOther.acParam+1, sem.__sem_value);
		            TTY_LOG("%s,%d,%s(),AT =%d, %s, sem value is %d\n", __FILE__,  __LINE__, __FUNCTION__, g_stAtFromOther.lMsgType, g_stAtFromOther.acParam+1, sem.__sem_value);
                 }
		/*����wflag�ж��ͷ��յ���������Ϣ��
		����ɹ���ָ��д��HSPA������������
		ֱ�Ӷ�ȡHSPA��������Ϣ*/
		if ( -1 != wflag )
		{
			/*�Ի���������,��֤�ڴ��ڼ䲻��read���*/
			pthread_mutex_lock(&work_mutex);
	
			strcat(g_stAtFromOther.acParam+HSPA_OFFSET, "\r");
			lLen = strlen(g_stAtFromOther.acParam+HSPA_OFFSET);
			cx_printf("%s, %d, %s(),len = %d\n", __FILE__, __LINE__, __FUNCTION__,  lLen);
            if ( CFUN_SET == g_stAtFromOther.lMsgType )
            {
                /*modified by w00135351 20081231 ����180�ڵ͹����µ���*/
                //if( NULL != strstr(g_stAtFromOther.acParam, "AT+CFUN=0"))
                if( NULL != strstr(g_stAtFromOther.acParam, "AT+CFUN=0"))
                {
                    if ( glbSpecStick)
                    {
                        g_stAtFromOther.acParam[9] = '7';
                    }
                    TTY_DEBUG("hspa off!");
                    TTY_DEBUG("\nCFUN FROM %d\n",g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
                    g_HspaLine = HSPA_OFF|MID_STATE_MASK;
                    alarm(10);
                }
                else if( NULL != strstr(g_stAtFromOther.acParam, "AT+CFUN=1"))
                {
                    TTY_DEBUG("hspa on!");
                    TTY_DEBUG("\nCFUN FROM %d\n",g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
                    g_HspaLine = HSPA_ON|MID_STATE_MASK;
                    alarm(10);
                }
            }
			/*��������ϢstAtSndд��HSPA*/
			if( HSPA_EXIST == g_HspaExistFlag )/*�豸�ڵ�ʱ����ܽ���д����*/
			{		
				wflag = write(g_iPcuiFd, g_stAtFromOther.acParam+HSPA_OFFSET,	lLen);		
				if( lLen == wflag )
				{
					g_iWriteFlag = HSPA_WRITE_OK;
					iErrNum = 0;
				}
				else
				{
					g_iWriteFlag = HSPA_WRITE_NO;
					if (-1 == wflag)
					{

						iErrNum++;
						if (iErrNum > HSPA_ERR_LIMIT)
						{
							iErrNum = 0;
							g_SignalRcv = 1;
						}
					}
					
				}
				cx_printf("%s, %d, g_stAtFromOther.lMsgType = %d, g_iWriteFlag = %d\n", __FILE__,  __LINE__,g_stAtFromOther.lMsgType,g_iWriteFlag );
				cx_printf("%s, %d, wflag = %d\n", __FILE__, __LINE__,wflag);
			}
			else
			{
				g_iWriteFlag = HSPA_WRITE_NO;	
				cx_printf("%s, %d, g_stAtFromOther.lMsgType = %d\n", __FILE__, __LINE__, g_stAtFromOther.lMsgType);
			}

            if ( PDP_SET != g_stAtFromOther.lMsgType)
            {
                    TTY_DEBUG("g_stAtFromOther.lMsgType = %d, g_iWriteFlag = %d, wflag = %d\n", g_stAtFromOther.lMsgType, g_iWriteFlag,wflag);
            }
			/*�Ի���������*/
			pthread_mutex_unlock(&work_mutex);

		}	

	}
	msgctl(g_lAtMsgId, IPC_RMID,0);
    #ifdef CX_DEBUG_VERSION	
	msgctl(g_lCmdMsgId, IPC_RMID,0);
    #endif
	msgctl(g_lModemMsgId, IPC_RMID,0);
	msgctl(g_lVoiceMsgId, IPC_RMID,0);
	msgctl(g_lTr069MsgId, IPC_RMID,0);
	msgctl(g_lHttpMsgId, IPC_RMID,0);
	msgctl(g_lLedMsgId, IPC_RMID,0);
#ifdef POTS_PINMNGR    
	msgctl(g_lPinMngrMsgId, IPC_RMID,0);
#endif    
	cx_printf("%s, %d, %s()\n", __FILE__, __LINE__, __FUNCTION__);
	close(g_iPcuiFd);	
	close(g_ShareiFd);
	exit(0);
}
/*****************************************************************************
 �� �� ��  : ReadPcui
 ��������  : 
 �������  : ��
 �� �� ֵ  : static int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static int   ReadPcui()
{
	char acInfoBack[AT_RCV_MSG_LENGTH];
	char acInfoBackAnalysis[AT_RCV_MSG_LENGTH];
	char *pChar = NULL;
	int lPairNum = sizeof(g_stAtFunCom)/(sizeof(struct stAt_Fun_Com));/*�ṹ��������Ԫ�ظ���*/
	int i = 0;
	int lMsgId = 0;
	int len = 0;

	static int iErrReadNum = 0;

	
	/*��ȡUSB-SERIAL������Ϣ*/
	while(1)
	{
//        cx_printf("%s, %d\n", __FILE__, __LINE__);

         memset(&g_stAtToOther, 0, sizeof(g_stAtToOther));
         g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;

         /*�Ի���������*/
         pthread_mutex_lock(&work_mutex);

	  if (iErrReadNum > HSPA_ERR_LIMIT)
	  {
			g_SignalRcv = 1;
			iErrReadNum = 0;
	  }

		 
         if( 1 == g_SignalRcv)
         {
             g_SignalRcv = 0;
             TTY_DEBUG("connect/disconnect hspa CARD \n");
             HspaDeviceDetect();
        }
         
		/*���û��д�����ݿ���ֱ�ӽ�����Ϣ����������
		ҵ��ʱû���豸*/
		if  ( (HSPA_WRITE_NO  == g_iWriteFlag) && ( 0 != g_stAtFromOther.acParam[HSPA_FROM_POSITION]) ) 
		{
			g_iWriteFlag = HSPA_WRITE_UNKNOW;
            /*�Ի���������*/
            pthread_mutex_unlock(&work_mutex);
			lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
			g_stAtToOther.lMsgType = g_stAtFromOther.lMsgType;
			g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;
			g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_NO_E172;
			if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
			{
                TTY_DEBUG("lMsgId is %d\n", lMsgId);
                msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
			}
            g_sndflg = 0;
			sem_post(&sem);
            TTY_DEBUG("lMsgId is %d && msg is sent\n", lMsgId);
			continue;
		}
		/*	1.�豸���ڲ��ܶ�
			2.û���յ���Ϣ���ܶ�*/
	//	if (( HSPA_NO == g_HspaExistFlag ) || (HSPA_WRITE_UNKNOW == g_iWriteFlag) ) 
		if ( HSPA_NO == g_HspaExistFlag ) 
		{
			usleep(500);
            if  ( (HSPA_WRITE_OK == g_iWriteFlag) && ( 0 != g_stAtFromOther.acParam[HSPA_FROM_POSITION])) 
            {
                g_iWriteFlag = HSPA_WRITE_UNKNOW;
                /*�Ի���������*/
                pthread_mutex_unlock(&work_mutex);
                lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
                g_stAtToOther.lMsgType = g_stAtFromOther.lMsgType;
                g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;
                g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_NO_E172;
                if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
                {
                    TTY_DEBUG("lMsgId is %d\n", lMsgId);
                    msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
                }
                g_sndflg = 0;
                sem_post(&sem);
                TTY_DEBUG("lMsgId is %d && msg is sent\n", lMsgId);
                continue;
            }
            
            /*�Ի���������*/
            pthread_mutex_unlock(&work_mutex);
			continue;
		}
#if 0		
cx_printf("sDeviceCount = %d\n", g_stHspa.sDeviceCount);
cx_printf("sDeviceStatus = %d\n", g_stHspa.sDeviceStatus);
cx_printf("sSimExist = %d\n", g_stHspa.sSimExist);
cx_printf("sSimStatus = %d\n", g_stHspa.sSimStatus);
cx_printf("sSysMode = %d\n", g_stHspa.sSysMode);
cx_printf("sSubSysMode = %d\n", g_stHspa.sSubSysMode);
cx_printf("sSrvStatus = %d\n", g_stHspa.sSrvStatus);
cx_printf("sPinPukNeed = %d\n", g_stHspa.sPinPukNeed);
cx_printf("acIMEI = %s\n", g_stHspa.acIMEI);
#endif
        /*�Ի���������*/
        pthread_mutex_unlock(&work_mutex);

		memset(acInfoBack, 0, AT_RCV_MSG_LENGTH);
		len = read(g_iPcuiFd, acInfoBack, AT_RCV_MSG_LENGTH); //��HSPA��û�Ӧ��Ϣ
		if ( PDP_SET != g_stAtFromOther.lMsgType)
	        {
		TTY_LOG("%s, %d, %s, len = %d\n", __FILE__, __LINE__, acInfoBack,len);
		}
		
		/*�����쳣:����д�����豸�������ڶ�ȡǰ���豸�Ƴ�
			��ȡʧ��*/
        //if( ( len < 0 ) ||( len == 0 )) 
        if(( len < 0 )||(( len == 0 )&&( 1 == g_sndflg )))
        {
		iErrReadNum++;
				
            if ((0 != g_stAtFromOther.acParam[HSPA_FROM_POSITION]))
            {
                pthread_mutex_lock(&work_mutex);
        		if  ( HSPA_WRITE_OK  == g_iWriteFlag  )
        		{
        			g_iWriteFlag = HSPA_WRITE_UNKNOW;
                    /*�Ի���������*/
                    pthread_mutex_unlock(&work_mutex);
        			lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
        			g_stAtToOther.lMsgType = g_stAtFromOther.lMsgType;
        			g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;
        			g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_NO_E172;
        			if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
        			{
                        TTY_DEBUG("lMsgId is %d\n", lMsgId);
                        msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
        			}
                    g_sndflg = 0;
        			sem_post(&sem);
                    TTY_DEBUG("lMsgId is %d && msg is sent\n", lMsgId);
        			continue;
        		}
                TTY_DEBUG("len=%d, g_sndflg=%d\n", len, g_sndflg);
                /*�Ի���������*/
                pthread_mutex_unlock(&work_mutex);
            }
            continue;            
        }
        else if(( len == 0 )&&( 0 == g_sndflg ))
        {

		iErrReadNum++;
            if ((0 != g_stAtFromOther.acParam[HSPA_FROM_POSITION]))
            {
                g_sndflg = 1;  /*modified by w00135351 09.4.1*/
                TTY_DEBUG("set g_sndflg...\n");
            }
        }
	  else 
  	 {
  	 	iErrReadNum = 0;
  	 }

	   if ( PDP_SET != g_stAtFromOther.lMsgType)
	   {
        
        TTY_LOG("TTYUSB: {%s}\n",acInfoBack);
		/*^BOOT�Զ��ϱ���Ϣ������*/		
		//if ( NULL != strstr(acInfoBack, "BOOT") )		
		//	continue;
		
		cx_printf("%s, %d, %s(),len = %d ,acInfoBack= %s\n", __FILE__, __LINE__, __FUNCTION__, len, acInfoBack);
	   }
		for ( i=0;  i<lPairNum;  ++i)
		{
			pChar = strstr(acInfoBack, g_stAtFunCom[i].acAtCmdPart);
			if ( NULL == pChar )
			{
				continue;
			}
			else
			{	
				memset (acInfoBackAnalysis, 0, AT_RCV_MSG_LENGTH);
				strcpy(acInfoBackAnalysis, pChar);
				g_stAtFunCom[i].pfAtBackInfoAnalysis(acInfoBackAnalysis);
			}
		}
	}
}

/*****************************************************************************
 �� �� ��  : MsgIdCompute
 ��������  : ������Ϣ���Ե�ģ�飬������Ϣ������ MsgId
 �������  : unsigned char ucCmdFrom  
 �������  : ��
 �� �� ֵ  : static inline int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static  int MsgIdCompute(unsigned char ucCmdFrom)
{
	if ( LED_MODULE == ucCmdFrom )
		return g_lLedMsgId;
	else
	if ( HTTP_MODULE == ucCmdFrom )
		return g_lHttpMsgId;
	else
	if ( TR069_MODULE == ucCmdFrom )
		return g_lTr069MsgId;
	else
	if ( MODEM_MODULE == ucCmdFrom )
		return g_lModemMsgId;
	else
	if ( VOICE_MODULE == ucCmdFrom )
		return g_lVoiceMsgId;
#ifdef CX_DEBUG_VERSION
	else
	if ( CMD_MODULE == ucCmdFrom )
		return g_lCmdMsgId;
#endif
#ifdef POTS_PINMNGR
	else
	if ( PINMNGR_MODULE	== ucCmdFrom )
		return g_lPinMngrMsgId;
#endif    
	else
		return -1;
}


/*****************************************************************************
 �� �� ��  : StrStrToUnChar
 ��������  : ��acCharParam�еĵ�һ�������ַ�������Ϊ��ֵ
 �������  : char **pChar       
             char *acCharParam  
 �������  : ��
 �� �� ֵ  : static unsigned char
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��14��
    ��    ��   : ����
    �޸�����   : �����ɺ���
  2.��    ��   : 2008��2��2��
    ��    ��   : s48571
    �޸�����   : ��������ֹ�ַ�����û�����֣������ַ�Խ��

*****************************************************************************/
static unsigned char StrStrToUnChar(char **pChar, char *acCharParam)
{
	int i = 0;
	int iLen = strlen(*pChar);
	
	while(1)
	{
		++*pChar;
		if ( isdigit(**pChar) ||(' ' == **pChar) )
		{
			acCharParam[i] = **pChar;
		}
		else
		{
			acCharParam[i] = 0;
			break;
		}
		++i;
		if ( i == iLen)
			break;
	}
	
	if (( 0 == i )||( iLen == i ))
	{
		return HSPA_UNKNOW_VALUE;
	}
	return ((unsigned char) atoi(acCharParam));
}
//add by z67625 20080330 for cend, start
/*****************************************************************************
 �� �� ��  : StrStrToInt
 ��������  : ��acCharParam�еĵ�һ�������ַ�������Ϊ��ֵ
 �������  : char **pChar       
             char *acCharParam  
 �������  : ��
 �� �� ֵ  : static unsigned char
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��14��
    ��    ��   : ����
    �޸�����   : �����ɺ���
  2.��    ��   : 2008��2��2��
    ��    ��   : s48571
    �޸�����   : ��������ֹ�ַ�����û�����֣������ַ�Խ��

*****************************************************************************/
static int StrStrToInt(char **pChar, char *acCharParam)
{
	int i = 0;
	int iLen = strlen(*pChar);
	
	while(1)
	{
		++*pChar;
		if ( isdigit(**pChar) ||(' ' == **pChar) )
		{
			acCharParam[i] = **pChar;
		}
		else
		{
			acCharParam[i] = 0;
			break;
		}
		++i;
		if ( i == iLen)
			break;
	}
	
	if (( 0 == i )||( iLen == i ))
	{
		return HSPA_UNKNOW_VALUE;
	}
	return atoi(acCharParam);
}
//add by z67625 20080330 for cend, end
/*****************************************************************************
 �� �� ��  : FacInfoAnlysis
 ��������  : ���ָ��AT+GMI\r������Ϣ�Ľ�����ַ�
 �������  : char *pszInfoBack ��������Ϣ�ַ��� 
 �������  : ��
 �� �� ֵ  : static void
ָ��ͷ�����Ϣ��ʽ:
*FACTORY_INFO_QUERY	17----AT+GMI\r
*NORMAL:7----AT+GMI<CR>, 16----<CR><LF>huawei<CR><LF><CR><LF>OK<CR><LF>
*UNNORMAL:7----AT+GMI<CR>, 5----<CR><LF>ERROR<CR><LF>
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void FacInfoAnlysis(char *pszInfoBack) 
{
	struct stMsg_Factory_Info_Query stFacInfo;/*������Ϣ��Ӧ�Ľṹ��*/
	int lMsgId = 0;
	int lLen = 0;
	memset(&stFacInfo, 0, sizeof(stFacInfo));
	g_stAtToOther.lMsgType = FACTORY_INFO_QUERY;/*������Ϣ��Ӧ������*/
	stFacInfo.ucCmdFrom = AT_MODULE;	/*������Ϣ��Դ*/
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	/*�ж�������Ժͷ�����Ϣ�Ƿ�ͬʱ����
	��������һ����Ҫ��ȡ������Ϣ*/

    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	pszInfoBack = strstr(pszInfoBack, "\r\n");
	if ( (NULL!=pszInfoBack) && (NULL!=strstr(pszInfoBack, "\r\nOK\r\n")) )
	{
		lLen = strlen(pszInfoBack);
		stFacInfo.ucResult = HSPA_OK;
		lLen = lLen-strlen("\r\n\r\n\r\nOK\r\n");
		if ( lLen >= sizeof(stFacInfo.acFactoryInfo) )
		{
			lLen = sizeof(stFacInfo.acFactoryInfo) -1;
		}
		memcpy(stFacInfo.acFactoryInfo, pszInfoBack+strlen("\r\n"), lLen);
	}	
	else
	{
		stFacInfo.ucResult = HSPA_ERROR_UNKNOW;
	}
	
	memcpy(g_stAtToOther.acText, (void*)&stFacInfo, sizeof(stFacInfo));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);

	cx_printf("%s,%d,%s(),acFactoryInfo = %s\n", __FILE__,  __LINE__, __FUNCTION__,  stFacInfo.acFactoryInfo);
	return;
}
/*****************************************************************************
 �� �� ��  : FacInfoAnlysis_C
 ��������  : ���ָ��AT+CGMI\r������Ϣ�Ľ�����ַ�
 �������  : char *pszInfoBack ��������Ϣ�ַ��� 
 �������  : ��
 �� �� ֵ  : static void
ָ��ͷ�����Ϣ��ʽ:
*FACTORY_INFO_QUERY_C	18----AT+CGMI\r
*NORMAL:8----AT+CGMI<CR>, 16----<CR><LF>huawei<CR><LF><CR><LF>OK<CR><LF>
*UNNORMAL:8----AT+CGMI<CR>, 5----<CR><LF>ERROR<CR><LF>
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void FacInfoAnlysis_C(char *pszInfoBack) 
{
	struct stMsg_Factory_Info_Query stFacInfo;/*������Ϣ��Ӧ�Ľṹ��*/
	int lMsgId;
	int lLen;
	memset(&stFacInfo, 0, sizeof(stFacInfo));
	g_stAtToOther.lMsgType = FACTORY_INFO_QUERY_C;/*������Ϣ��Ӧ������*/
	stFacInfo.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	/*�ж�������Ժͷ�����Ϣ�Ƿ�ͬʱ����
	��������һ����Ҫ��ȡ������Ϣ*/
        
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	pszInfoBack = strstr(pszInfoBack, "\r\n");
	if ( (NULL != pszInfoBack) && (NULL != strstr(pszInfoBack, "\r\nOK\r\n")) )/*�жϷ�����Ϣ�Ƿ�ɹ�*/
	{
		lLen = strlen(pszInfoBack);
		stFacInfo.ucResult = HSPA_OK;
		lLen = lLen-strlen("\r\n\r\n\r\nOK\r\n");
		if ( lLen >= sizeof(stFacInfo.acFactoryInfo) )
		{
			lLen = sizeof(stFacInfo.acFactoryInfo) -1;
		}
		memcpy(stFacInfo.acFactoryInfo, pszInfoBack+strlen("\r\n"), lLen);
	}	
	else
	{
		stFacInfo.ucResult = HSPA_ERROR_UNKNOW;
	}	
	memcpy(g_stAtToOther.acText, (void*)&stFacInfo, sizeof(stFacInfo));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	cx_printf("%s, %d, %s(), lMsgId = %dn",__FILE__, __LINE__, __FUNCTION__, lMsgId);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	cx_printf("%s, %d, %s(), acFactoryInfo = %s\n",__FILE__, __LINE__, __FUNCTION__, stFacInfo.acFactoryInfo);
	return;
}

/*****************************************************************************
 �� �� ��  : SoftwareVersionAnlysis
 ��������  : ���AT+GMR\r�ķ�����Ϣ�����ͷַ�
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
ָ��ͷ�����Ϣ��ʽ:
*SOFTWARE_VERSION_QUERY	19----AT+GMR\r
*NORMAL:7----AT+GMR<CR>,25----<CR><LF>11.310.09.00.00<CR><LF><CR><LF>OK<CF><LF>
*UNNORMAL:7----AT+GMR<CR>,9----<CR><LF>ERROR<CR><LF>
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void SoftwareVersionAnlysis(char *pszInfoBack) 
{
	struct stMsg_Software_Version_Query stSoftwareVer;
	int lMsgId;
	int lLen;
	memset(&stSoftwareVer, 0, sizeof(stSoftwareVer));
	g_stAtToOther.lMsgType = SOFTWARE_VERSION_QUERY;/*������Ϣ��Ӧ������*/
	stSoftwareVer.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	/*�ж�������Ժͷ�����Ϣ�Ƿ�ͬʱ����
	��������һ����Ҫ��ȡ������Ϣ*/
            
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	pszInfoBack = strstr(pszInfoBack, "\r\n");
	if ( (NULL!=pszInfoBack) && (NULL!=strstr(pszInfoBack, "\r\nOK\r\n")) )/*�жϷ�����Ϣ�Ƿ�ɹ�*/
	{
		lLen = strlen(pszInfoBack);
		stSoftwareVer.ucResult = HSPA_OK;
		lLen = lLen-strlen("\r\n\r\n\r\nOK\r\n");
		if ( lLen >= sizeof(stSoftwareVer.acSoftWareVersion) )
		{
			lLen = sizeof(stSoftwareVer.acSoftWareVersion) -1;
		}
		memcpy(stSoftwareVer.acSoftWareVersion, pszInfoBack+strlen("\r\n"), lLen);
	}	
	else
	{
		stSoftwareVer.ucResult = HSPA_ERROR_UNKNOW;
	}	
	memcpy(g_stAtToOther.acText, (void*)&stSoftwareVer, sizeof(stSoftwareVer));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	cx_printf("%s, %d, %s(), lMsgId = %d\n",__FILE__, __LINE__, __FUNCTION__, lMsgId);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    g_sndflg = 0;
	sem_post(&sem);
	cx_printf("%s,%d, %s(), acSoftWareVersion = %s\n",__FILE__, __LINE__, __FUNCTION__ , stSoftwareVer.acSoftWareVersion );
	return;
}


/*****************************************************************************
 �� �� ��  : SoftwareVersionAnlysis_C
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
ָ��ͷ�����Ϣ��ʽ:
*SOFTWARE_VERSION_QUERY_C	20----AT+CGMR\r
*NORMAL:8----AT+CGMR<CR>,25----<CR><LF>11.310.09.00.00<CR><LF><CR><LF>OK<CF><LF>
*UNNORMAL:8----AT+CGMR<CR>,9----<CR><LF>ERROR<CR><LF>
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void SoftwareVersionAnlysis_C(char *pszInfoBack) 
 {
	struct stMsg_Software_Version_Query stSoftwareVer;
	int lMsgId;
	int lLen;
	memset(&stSoftwareVer, 0, sizeof(stSoftwareVer));
	g_stAtToOther.lMsgType = SOFTWARE_VERSION_QUERY_C;/*������Ϣ��Ӧ������*/
	stSoftwareVer.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	/*�ж�������Ժͷ�����Ϣ�Ƿ�ͬʱ����
	��������һ����Ҫ��ȡ������Ϣ*/
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	pszInfoBack = strstr(pszInfoBack, "\r\n");
	if ( (NULL!=pszInfoBack) && (NULL!=strstr(pszInfoBack, "\r\nOK\r\n")) )/*�жϷ�����Ϣ�Ƿ�ɹ�*/
	{
		lLen = strlen(pszInfoBack);
		stSoftwareVer.ucResult = HSPA_OK;
		lLen = lLen-strlen("\r\n\r\n\r\nOK\r\n");
		if ( lLen >= sizeof(stSoftwareVer.acSoftWareVersion) )
		{
			lLen = sizeof(stSoftwareVer.acSoftWareVersion) -1;
		}
		memcpy(stSoftwareVer.acSoftWareVersion, pszInfoBack+strlen("\r\n"), lLen);
	}	
	else
	{		
		stSoftwareVer.ucResult = HSPA_ERROR_UNKNOW;
	}	
	memcpy(g_stAtToOther.acText, (void*)&stSoftwareVer, sizeof(stSoftwareVer));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	cx_printf("%s, %d, %s(), acSoftWareVersion = %s\n",__FILE__, __LINE__, __FUNCTION__, stSoftwareVer.acSoftWareVersion );
	return;

 }

/*****************************************************************************
 �� �� ��  : HardwareVersionAnlysis
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*HARDWARE_VERSION_QUERY	21----AT^HVER\r
*NORMAL:8----AT^HVER<CR>,26----<CR><LF>^HVER:"CD33TCPW"<CR><LF><CR><LF>OK<CF><LF>
*UNNORMAL:8----AT^HVER<CR>,<CR><LF>+CME ERROR: <err><CR><LF>
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void HardwareVersionAnlysis(char *pszInfoBack) 
 {
 	struct stMsg_Hardware_Version_Query stHardwareVer;
	int lMsgId;
	int lLen;
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	memset(&stHardwareVer, 0, sizeof(stHardwareVer));
	g_stAtToOther.lMsgType = HARDWARE_VERSION_QUERY;/*������Ϣ��Ӧ������*/
	stHardwareVer.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	/*�ж�������Ժͷ�����Ϣ�Ƿ�ͬʱ����
	��������һ����Ҫ��ȡ������Ϣ*/
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	pszInfoBack = strstr(pszInfoBack, "\r\n");
	if ( (NULL!=pszInfoBack) && (NULL!=strstr(pszInfoBack, "\r\nOK\r\n")))/*�жϷ�����Ϣ�Ƿ�ɹ�*/
	{
		lLen = strlen(pszInfoBack);
		stHardwareVer.ucResult = HSPA_OK;
		lLen = lLen-strlen("\r\n^HVER:\"\"\r\n\r\nOK\r\n");
		if(lLen>=sizeof(stHardwareVer.acHardWareVersion))
			lLen = sizeof(stHardwareVer.acHardWareVersion) -1;
		memcpy(stHardwareVer.acHardWareVersion, pszInfoBack+strlen("\r\n^HVER:\""), lLen);
	}	
	else
	{
		stHardwareVer.ucResult = HSPA_ERROR_UNKNOW;
	}	
	memcpy(g_stAtToOther.acText, (void*)&stHardwareVer, sizeof(stHardwareVer));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	cx_printf("%s,%d, %s(), acHardWareVersion = %s\n",__FILE__, __LINE__,  __FUNCTION__, stHardwareVer.acHardWareVersion);
	return;	
 }
 
/*****************************************************************************
 �� �� ��  : HardwareVersionAnlysis_W
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*HARDWARE_VERSION_QUERY_W	22----AT^HWVER\r
*NORMAL:9----AT^HWVER<CR>,27----<CR><LF>^HWVER:"CD33TCPW"<CR><LF><CR><LF>OK<CF><LF>
*UNNORMAL:<CR><LF>+CME ERROR: <err><CR><LF>
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void HardwareVersionAnlysis_W(char *pszInfoBack) 
{
 	struct stMsg_Hardware_Version_Query stHardwareVer;
	int lMsgId;
	int lLen;
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	memset(&stHardwareVer, 0, sizeof(stHardwareVer));
	g_stAtToOther.lMsgType = HARDWARE_VERSION_QUERY_W;/*������Ϣ��Ӧ������*/
	stHardwareVer.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	/*�ж�������Ժͷ�����Ϣ�Ƿ�ͬʱ����
	��������һ����Ҫ��ȡ������Ϣ*/
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	pszInfoBack = strstr(pszInfoBack, "\r\n");
	if ( (NULL!=pszInfoBack) && (NULL!=strstr(pszInfoBack, "\r\nOK\r\n")) )/*�жϷ�����Ϣ�Ƿ�ɹ�*/
	{
		lLen = strlen(pszInfoBack);
		stHardwareVer.ucResult = HSPA_OK;
		lLen = lLen-strlen("\r\n^HWVER:\"\"\r\n\r\nOK\r\n");
		if ( lLen >= sizeof(stHardwareVer.acHardWareVersion) )
			lLen = sizeof(stHardwareVer.acHardWareVersion) -1;
		memcpy(stHardwareVer.acHardWareVersion, pszInfoBack+strlen("\r\n^HWVER:\""), lLen);
	}	
	else
	{
		stHardwareVer.ucResult = HSPA_ERROR_UNKNOW;
	}	
	memcpy(g_stAtToOther.acText, (void*)&stHardwareVer, sizeof(stHardwareVer));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	cx_printf("%s,%d, %s(), acHardWareVersion = %s\n",__FILE__, __LINE__, __FUNCTION__, stHardwareVer.acHardWareVersion);
	return;	

}

/*****************************************************************************
 �� �� ��  : ImsiInfoAnlysis
 ��������  : 
 �������  : char *pszInfoBack  
             int lLen           
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*IMSI_QUERY	27----AT+CIMI\r
*NORMAL:8----AT+CIMI<CR>, 25----<CR><LF>460028174600118<CR><LF><CR><LF>OK<CR><LF>
*UNNORMAL:<CR><LF>+CME ERROR: <err><CR><LF>
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void ImsiInfoAnlysis(char *pszInfoBack) 
{
 	struct stMsg_IMSI_Query stImsi;
	int lMsgId;
	int lLen;
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	memset(&stImsi, 0, sizeof(stImsi));
	g_stAtToOther.lMsgType = IMSI_QUERY;/*������Ϣ��Ӧ������*/
	stImsi.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	/*�ж�������Ժͷ�����Ϣ�Ƿ�ͬʱ����
	��������һ����Ҫ��ȡ������Ϣ*/
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	
	pszInfoBack = strstr(pszInfoBack, "\r\n");
	if ( (NULL != pszInfoBack) && (NULL != strstr(pszInfoBack, "\r\nOK\r\n")) )/*�жϷ�����Ϣ�Ƿ�ɹ�*/
	{
		lLen = strlen(pszInfoBack);
		stImsi.ucResult = HSPA_OK;
		lLen = lLen-strlen("\r\n\r\n\r\nOK\r\n");
		if ( lLen >= sizeof(stImsi.acIMSI) )
		{
			lLen = sizeof(stImsi.acIMSI) -1;
		}
		memcpy(stImsi.acIMSI, pszInfoBack+strlen("\r\n"), lLen);
		
		sysAvailSet(stImsi.acIMSI, IFC_IMSI_LEN, IMSI_OFFSET);
		/*begin:Added by luokunling 00192527 for imsi info requirement,2011/9/19*/
		if(0 == g_iHspaWeatherAutoPin)
		{
	          //ImsiNumIsOldImsiNum(stImsi.acIMSI);
	          TR069_SendMessage(CWMP_MSG_IMSI, (void*)(stImsi.acIMSI), strlen(stImsi.acIMSI)+1);
		}
		else
		{
		    TR069_SendMessage(CWMP_MSG_CONNECTED_IMSI_INFORM, (void*)(stImsi.acIMSI), strlen(stImsi.acIMSI)+1);
		    g_iHspaWeatherAutoPin = 0;		
		}
		/*End:Added by luokunling 00192527,2011/9/19*/
	}	
	else
	{
		stImsi.ucResult = HSPA_ERROR_UNKNOW;
	}	
	memcpy(g_stAtToOther.acText, (void*)&stImsi, sizeof(stImsi));

	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	cx_printf("%s,%d,%s(), acIMSI = %s", __FILE__, __LINE__, __FUNCTION__, stImsi.acIMSI);
	return;

}
/*****************************************************************************
 �� �� ��  : ImeiInfoAnlysis
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*IMEI_QUERY	28----AT+CGSN?\r
*NORMAL:8----AT+CGSN<CR>,  25----<CR><LF>358192015569919<CR><LF><CR><LF>OK<CR><LF>
*UNNORMAL:<CR><LF>+CME ERROR: <err><CR><LF>
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void ImeiInfoAnlysis(char *pszInfoBack) 
{
 	struct stMsg_IMEI_Query stImei;
	int lMsgId;
	int lLen;
	memset(&stImei, 0, sizeof(stImei));
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	g_stAtToOther.lMsgType = IMEI_QUERY;/*������Ϣ��Ӧ������*/
	stImei.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	/*�ж�������Ժͷ�����Ϣ�Ƿ�ͬʱ����
	��������һ����Ҫ��ȡ������Ϣ*/
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	pszInfoBack = strstr(pszInfoBack, "\r\n");
	if ( (NULL != pszInfoBack) && (NULL != strstr(pszInfoBack, "\r\nOK\r\n")) )/*�жϷ�����Ϣ�Ƿ�ɹ�*/
	{
		lLen = strlen(pszInfoBack);
		stImei.ucResult = HSPA_OK;
		lLen = lLen-strlen("\r\n\r\n\r\nOK\r\n");
		if ( lLen >= sizeof(stImei.acIMEI) )
			lLen = sizeof(stImei.acIMEI) -1;
		memcpy(stImei.acIMEI, pszInfoBack+strlen("\r\n"), lLen);
	}	
	else
	{
		stImei.ucResult = HSPA_ERROR_UNKNOW;
	}	
	if ( HSPA_OK == g_stAtToOther.acText[HSPA_RESULT_POSITION] )
	{
		strcpy(g_stHspa.acIMEI, stImei.acIMEI);
		lseek(g_ShareiFd, 0,SEEK_SET);
		write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
		fsync(g_ShareiFd);	
	}	
	memcpy(g_stAtToOther.acText, (void*)&stImei, sizeof(stImei));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	cx_printf("%s,%d,%s(), acIMEI = %s", __FILE__, __LINE__, __FUNCTION__,  stImei.acIMEI);
	return;

}
/*****************************************************************************
 �� �� ��  : RssiInfoAnlysis
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*RSSI_QUERY	31----AT+CSQ\r
*NORMAL:7----AT+CSQ<CR>, 21----<CR><LF>+CSQ: 25,99<CR><LF><CR><LF>OK<CR><LF>
*UNNORMAL:
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void RssiInfoAnlysis(char *pszInfoBack) 
{
 	struct stMsg_RSSI_Query stRssi;
	int lMsgId;
	char *pChar;
	char acRssi[COMMON_VERY_SHORT_STRING_LENGTH];
	char acBer[COMMON_VERY_SHORT_STRING_LENGTH];
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	memset(&stRssi, 0, sizeof(stRssi));
	g_stAtToOther.lMsgType = RSSI_QUERY;/*������Ϣ��Ӧ������*/
	stRssi.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	TTY_LOG("TTYUSB: {%s}\n", pszInfoBack);

	pszInfoBack = strstr(pszInfoBack, "\r\n");
	if ( (NULL!=pszInfoBack)  &&  (NULL!=strstr(pszInfoBack, "\r\nOK\r\n") ) 
		&&  (NULL != (pChar = strstr(pszInfoBack, "+CSQ:")))  )/*�жϷ�����Ϣ�Ƿ�ɹ�*/
	{
		stRssi.ucResult = HSPA_OK;
		pChar += strlen("+CSQ:");	

		stRssi.ucRSSI =StrStrToUnChar(&pChar, acRssi);
		stRssi.ucBer= StrStrToUnChar(&pChar, acBer);
	}	
	else
	{
		if ( (NULL != pszInfoBack) || (0 == strcmp(pszInfoBack, "\r\nERROR\r\n")) )
		{
			stRssi.ucResult = HSPA_ERROR_UNKNOW;
		}
	}	
	memcpy(g_stAtToOther.acText, (void*)&stRssi, sizeof(stRssi));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
	cx_printf("%s,%d,%s(), ucRSSI = %d, ucBer = %d\n", __FILE__, __LINE__, __FUNCTION__, stRssi.ucRSSI, stRssi.ucBer);
    g_sndflg = 0;
	sem_post(&sem);
	return;	
}
static int getWanStatus()
{
    FILE *fp = NULL;
    char acCmd[10] = "";
    int wanStatus = -1;

    if(NULL == (fp = fopen("/var/HspaDialFlag", "r")))
    {
        return -1;
    }
    fgets(acCmd, 10, fp);
    fclose(fp);
    wanStatus = atoi(acCmd);
    return wanStatus;
}
/*****************************************************************************
 �� �� ��  : SystemInfoQuery
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*SYSTEM_INFO_QUERY	34----AT^SYSINFO\r
*NORMAL:11---AT^SYSINFO<CR>, 31----<CR><LF>^SYSINFO:2,3,1,3,1,,3<CR><LF><CR><LF>OK<CR><LF>
*UNNORMAL:
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void SystemInfoQuery(char *pszInfoBack) 
{
	struct stMsg_System_Info_Query stSysInfo;
	int lMsgId;
	char *pChar;
	char acCharParam[COMMON_VERY_SHORT_STRING_LENGTH];
	int lLightFlag = 0;
	struct stAtSndMsg stAtFromOther;
	char acInitHspa[COMMON_LONG_STRING_LENGTH];
	int lLen = 0;
	int lPinCounter = 0;
	
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	g_stAtToOther.lMsgType = SYSTEM_INFO_QUERY;/*������Ϣ��Ӧ������*/
	stSysInfo.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	if ( NULL != strstr(pszInfoBack, "AT^SYSINFO") )
	{
		pszInfoBack = strstr(pszInfoBack, "\r\n");
	}	
	if ( (NULL != pszInfoBack) && (NULL != strstr(pszInfoBack, "\r\nOK\r\n")) )/*�жϷ�����Ϣ�Ƿ�ɹ�*/
	{
		stSysInfo.ucResult = HSPA_OK;
		pChar = index(pszInfoBack, ':');
		if ( NULL == pChar )
		{
			goto Exit1;
		}
		/*�����һ������ֵ*/
		stSysInfo.ucSrvStatus = StrStrToUnChar(&pChar, acCharParam);
		if ( ',' != *pChar )
			goto Exit2;
		/*����ڶ�������ֵ*/
		stSysInfo.ucSrvDomain = StrStrToUnChar(&pChar, acCharParam);
		if ( ',' != *pChar )
			goto Exit3;
		/*�������������ֵ*/
		stSysInfo.ucRoamStatus = StrStrToUnChar(&pChar, acCharParam);

		if ( ',' != *pChar )
			goto Exit4;
		/*������ĸ�����ֵ*/	
		stSysInfo.ucSysMode = StrStrToUnChar(&pChar, acCharParam);

		if ( ',' != *pChar )
			goto Exit5;
		/*������������ֵ*/
		stSysInfo.ucSimState = StrStrToUnChar(&pChar, acCharParam);

		if ( ',' != *pChar )
			goto Exit6;
		/*�������������ֵ*/
		stSysInfo.ucLockState = StrStrToUnChar(&pChar, acCharParam);

		if ( ',' != *pChar )
			goto Exit7;
		/*������߸�����ֵ*/
		stSysInfo.ucSysSubMode = StrStrToUnChar(&pChar, acCharParam);

		memcpy(g_stAtToOther.acText, (void*)&stSysInfo, sizeof(stSysInfo));
		goto Exit8;
		
	}
	else
	{
		stSysInfo.ucResult = HSPA_ERROR_UNKNOW;
		goto Exit1;
	}
Exit1:
	stSysInfo.ucSrvStatus = 250;
Exit2:
	stSysInfo.ucSrvDomain = 250;	/*255��ʾ��֧��CDMA�����Դ�����Ҫ�޸�250*/
Exit3:
	stSysInfo.ucRoamStatus = 250;
Exit4:
	stSysInfo.ucSysMode = 250;
Exit5:
	 stSysInfo.ucSimState = 250;/*255��ʾ��֧��SIM�������ڣ����Դ�����Ҫ�޸�250*/
Exit6:
	stSysInfo.ucLockState = 250;
Exit7:
	stSysInfo.ucSysSubMode = 250;
Exit8:
	memcpy(g_stAtToOther.acText, (void*)&stSysInfo, sizeof(stSysInfo));

	/*���¹�������*/
	g_stHspa.sSimExist = stSysInfo.ucSimState;
	g_stHspa.sSysMode = stSysInfo.ucSysMode;
	g_stHspa.sSubSysMode = stSysInfo.ucSysSubMode;
    g_stHspa.sSrvStatus = stSysInfo.ucSrvStatus;/*add by w00135351 20081107 AU8D01187*/
	
	if ( HSPA_UNKNOW_VALUE != stSysInfo.ucLockState)
	{
		g_stHspa.sSimStatus = stSysInfo.ucLockState;	
	}
    /*start of hspa with no sim minpengwei 20101208*/
	lLen = write(g_iPcuiFd, "AT+CLCK=\"SC\",2\r", strlen("AT+CLCK=\"SC\",2\r"));
	memset(acInitHspa, 0, COMMON_LONG_STRING_LENGTH);
	usleep(500);
	lLen = read(g_iPcuiFd, acInitHspa, COMMON_LONG_STRING_LENGTH);
	TTY_DEBUG("acInitHspa = %s\n", acInitHspa);
	if ( lLen < 0 )
	{
		TTY_DEBUG("len =%d\n",lLen);
		cx_printf("%s,%d----lLen = %d\n",__FILE__, __LINE__, lLen);
		g_stHspa.sSimExist = UNKNOW_ON_HSPA;
	}
	else if ( NULL != strstr(acInitHspa, "+CME ERROR: 13") ) /*SIM������*/
	{
		TTY_DEBUG("No SIM, blink HSPA LED\n");
		sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);
		g_stHspa.sSimExist = HSPA_NO_SIM;
	}
       /*end of hspa with no sim minpengwei 20101208*/
	TTY_DEBUG("g_stHspa.sSimExist = %d\n", g_stHspa.sSimExist);
	TTY_DEBUG("g_stHspa.sSimStatus = %d\n", g_stHspa.sSimStatus);
    TTY_DEBUG("g_stHspa.sSysMode = %d\n", g_stHspa.sSysMode);
    TTY_DEBUG("g_stHspa.sPinPukNeed = %d\n", g_stHspa.sPinPukNeed);
    TTY_DEBUG("g_stHspa.sDeviceStatus = %d\n", g_stHspa.sDeviceStatus);
    

	/* start of VOICE ���������� by pengqiuming(48166), 2008/9/5 
	   for ���E180V���ݿ� HSPA�л���ADSL���л�HSPA, ��һֱ��ʾ��ɫ����. SYSINFO���ݿ��ϱ��ĵ�5������Ϊ0,��ʾUSIM��״̬��Ч,
	   ��������Ҫpin��,������ѯ�����Ƿ���Ҫ,��ΪE180V��LPMģʽ��ʧ���ѱ����pin��,������������flash����һֱ�����*/
    /*start of hspa with no sim minpengwei 20101208*/
    //if (0 == stSysInfo.ucSimState)
    if(0 == g_stHspa.sSimExist)
    /*end of hspa with no sim minpengwei 20101208*/
    {
		for( lPinCounter = 0; lPinCounter < MAX_PIN_TEST; lPinCounter++)
        {

			/*************�����Ƿ���ҪPINУ��,������*************/
			if (strlen(g_stHspa.acPinCode) < 4)
			{
				sysAvailGet(g_stHspa.acPinCode, IFC_PIN_PASSWORD_LEN, PINCODE_OFFSET);
			}
		
   			sysAvailGet(g_stHspa.acOldIMEI, IFC_IMEI_LEN, IMEI_OFFSET);
   			lLen = strlen(g_stHspa.acOldIMEI);

			TTY_DEBUG("In Flash acPinCode = %s, acOldIMEI = %s\n", g_stHspa.acPinCode, g_stHspa.acOldIMEI);
		
   			while(lLen > 0)
   			{
   				if (isdigit(g_stHspa.acOldIMEI[lLen-1]))
   				{
   					lLen--;
   				}
   				else
   				{
   					memset(g_stHspa.acOldIMEI, 0, sizeof(g_stHspa.acOldIMEI));
   					break;
   				}
   			}

    		lLen = write(g_iPcuiFd, "AT+CLCK=\"SC\",2\r", strlen("AT+CLCK=\"SC\",2\r"));
    		memset(acInitHspa, 0, COMMON_LONG_STRING_LENGTH);
    		usleep(500);
    		lLen = read(g_iPcuiFd, acInitHspa, COMMON_LONG_STRING_LENGTH);
                TTY_DEBUG("len = %d, acInitHspa = %s\n", lLen, acInitHspa);
            
    		if ( lLen < 0 )
    		{
    			break;
    		}
			if ( (HSPA_PIN_NEED == g_stHspa.sPinPukNeed) 
			  || (HSPA_PUK_NEED == g_stHspa.sPinPukNeed))			  
			{
				break;
			}
			else
			{
        		if  (HSPA_OK != PinPukProcess(acInitHspa) )
        		{
        		    TTY_DEBUG("HSPA_OK != PinPukProcess(acInitHspa)\n");
                    sleep(5);
        		}
                else
                {
                    break;
                }
            }
        }
    }
	/* end of VOICE ���������� by pengqiuming(48166), 2008/9/5 */

    /*modified by w00135351 20081231*/
    /*star:������ݿ������ļ��е��豸״̬�����ݿ��Ƿ���g_HspaLine����һ��,��һ��ʱg_HspaLineΪ׼�·�AT^CFUN��������ʹһ��*/
    if ((HSPA_OFF == g_HspaLine & HSPA_LINE_MASK) && (g_stHspa.sDeviceStatus == 1))
    {
        TTY_DEBUG("( HSPA_OFF ==  g_HspaLine  ) && (g_stHspa.sDeviceStatus == 1)\n");
        AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, CFUN_SET, "AT+CFUN=0");
    }

    if ((HSPA_ON == g_HspaLine & HSPA_LINE_MASK) && (g_stHspa.sDeviceStatus == 0))
    {
        TTY_DEBUG("( HSPA_ON ==  g_HspaLine  ) && (g_stHspa.sDeviceStatus == 0)\n");
        AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, CFUN_SET, "AT+CFUN=1");
    }

    //printf("%d, %s() : g_HspaLine = %x\n", __LINE__, __FUNCTION__, g_HspaLine);
    /*end ״̬һ���Լ��*/
    /*end by w00135351 20081231*/

    int wanStatus = getWanStatus();
	/*HSPA��������*/
        if ( 0 == g_stHspa.sDeviceStatus || 7 == g_stHspa.sDeviceStatus)
        {
            TTY_DEBUG("HSPA in LPM mode\n");
            sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);
        }
    /*start of hspa with no sim minpengwei 20101208*/
       //else if ( ( 0 < g_stHspa.sSysMode)  && ( HSPA_NO_SIM != stSysInfo.ucSimState ) 	&& ( HSPA_INVALID_SIM != stSysInfo.ucSimState ))
	else if ( ( 0 < g_stHspa.sSysMode)  && ( HSPA_NO_SIM != g_stHspa.sSimExist) 	&& ( HSPA_INVALID_SIM != g_stHspa.sSimExist ))
    /*end of hspa with no sim minpengwei 20101208*/
	{

		/*start of when hspa call in the hspa led should be blink minpengwei 20101013*/
		
		FILE *pInCall = fopen("/var/incomingcall", "r");		
        char InCall = 0;
		if ( pInCall != NULL)
		{
		   //ringing
           fread(&InCall, 1, 1, pInCall);
           fclose(pInCall);
		}
	    if(InCall == 49)
	    {
	    	TTY_DEBUG("HSPA on Call In.\n");
	        sysLedCtrl(kLedHspa, kLedStateFastBlinkContinues);
	    }else		
	   /*end of when hspa call in the hspa led should be blink minpengwei 20101013*/		
		/* BEGIN: Added by c106292, 2009/3/16   PN:Led light method changes*/
		if (  CALL_ON_HSPA & g_ulTrafficFlag )
		{
		    //�绰��
            TTY_DEBUG("HSPA on voice\n");
    	    sysLedCtrl(kLedHspa, kLedStateFastBlinkContinues);
		}
		else if (2 == wanStatus)
		{

            TTY_DEBUG("HSPA on data\n");
            sysLedCtrl(kLedHspa, kLedStateVerySlowBlinkContiunes);
		}
		else/*�����Ѿ�ͨ�ˣ�����û�г������ݺ�����ҵ��*/
		{
            TTY_DEBUG("HSPA has no data or voice\n");
			//sysLedCtrl(kLedHspa, kLedStateOff);
			sysLedCtrl(kLedHspa, kLedStateOn);

		}
		/* END:   Added by c106292, 2009/3/16 */
	}
    else if ( ( HSPA_NO_SIM == g_stHspa.sSimExist ) ||( HSPA_INVALID_SIM == g_stHspa.sSimExist ) 
		|| ( 0 == stSysInfo.ucSysMode ) || (HSPA_PIN_PUK_OK != g_stHspa.sPinPukNeed))/*������Ϣ255��ʾSIM��������*/
	{
		memset(g_stHspa.acIMEI, 0, HSPA_IMSI_LENGTH);

/* BEGIN: Modified by y67514, 2009/12/8   ���ⵥ��:��δ��pin��״̬���������к�hspa�Ƴ���7��8��*/
            if (  CALL_ON_HSPA & g_ulTrafficFlag )
            {
                //�绰��
                TTY_DEBUG("HSPA on voice\n");
                sysLedCtrl(kLedHspa, kLedStateFastBlinkContinues);
            }
            else if( (g_HspaLine & HSPA_LINE_MASK) == HSPA_ON)
            {
                TTY_DEBUG("HSPA abnormal, Blink HSPA Led!\n");
            	/* BEGIN: Modified by c106292, 2009/3/17   PN:Led light method changes*/
           	    sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);
            	/* END:   Modified by c106292, 2009/3/17 */	   
            }
/* END:   Modified by y67514, 2009/12/8 */
	}
	
	/*û���豸���ص�
	��ֹ�����濪��ʱ�򣬱��źŴ�����
	���*/
	if ( HSPA_NO == g_stHspa.sDeviceCount)
	{
        TTY_DEBUG("No HSPA Card: Turn off Led Led!\n");
		sysLedCtrl(kLedHspa, kLedStateOff);
	}

	/*���¹����ļ�*/
	lseek(g_ShareiFd, 0,SEEK_SET);
	write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
	fsync(g_ShareiFd);
    
    /* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add begin:*/
	if ( HSPA_OK == CheckHspaInfo(&g_stHspa))
	{
		sysWakeupMonitorTask();
        TTY_DEBUG("sysWakeupMonitorTask\n");
	}
	BackupHspaInfo(&g_stHspa);
    /* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add end.*/

	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    g_sndflg = 0;
	sem_post(&sem);

	return;
}

/*****************************************************************************
 �� �� ��  : CfgInfoSetBack
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*SYSCFG_SET	39---AT^SYSCFG=.....
*NORMAL:4----<CR><LF>OK<CR><LF>
*UNNORMAL:31---<CR><LF>INPUT VALUE IS OUT OF RANGE<CR><LF>
*9----<CR><LF>ERROR<CR><LF>
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void CfgInfoSetBack(char *pszInfoBack)
{
	struct stMsg_Syscfg_Set	stSyscfgSetBack;
	int lMsgId;
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	memset(&stSyscfgSetBack, 0, sizeof(stSyscfgSetBack));
	g_stAtToOther.lMsgType = SYSCFG_SET;/*������Ϣ��Ӧ������*/
	stSyscfgSetBack.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/

    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	if ( NULL != strstr(pszInfoBack, "\r\nOK\r\n") )
	{
	  	stSyscfgSetBack.ucResult = HSPA_OK;
	}
	else
	{
		stSyscfgSetBack.ucResult = HSPA_ERROR_UNKNOW;
	}
	memcpy(g_stAtToOther.acText, (void*)&stSyscfgSetBack, sizeof(stSyscfgSetBack));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    g_sndflg = 0;
	sem_post(&sem);
	return;
}

/*****************************************************************************
 �� �� ��  : CfgInfoQuery
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*SYSCFG_QUERY	40----AT^SYSCFG?\r
*NORMAL:34:  <CR><LF>^SYSCFG:2,1,3FFFFFFF,1,2<CR><LF><CR><LF>OK<CR><LF>
*UNNORMAL:
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void CfgInfoQuery(char *pszInfoBack)
{
	struct stMsg_Syscfg_Query	stSyscfgQuery;
	int lMsgId;
	char *pChar;
	char acCharParam[COMMON_VERY_SHORT_STRING_LENGTH];
	int i = 0;
	memset(&stSyscfgQuery, 0, sizeof(stSyscfgQuery));
	g_stAtToOther.lMsgType = SYSCFG_QUERY;/*������Ϣ��Ӧ������*/
	stSyscfgQuery.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/

	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	
	if ( (NULL != strstr(pszInfoBack, "\r\nOK\r\n"))  && ( NULL != (pChar = strstr(pszInfoBack, ":")) ) )
	{		
	  	stSyscfgQuery.ucResult = HSPA_OK;
		//pChar = strstr(pszInfoBack, ":");
		/*�����һ������*/
		stSyscfgQuery.ucMode =StrStrToUnChar(&pChar, acCharParam);
		cx_printf("%s, %d,pChar = %s, ucMode = %d\n",__FILE__,  __LINE__, pChar, stSyscfgQuery.ucMode);
		/*����ڶ�������*/
		stSyscfgQuery.ucAcqorder = StrStrToUnChar(&pChar, acCharParam);
		cx_printf("%s %d,acCharParam = %s, ucAcqorder = %d\n",  __FILE__, __LINE__, acCharParam, stSyscfgQuery.ucAcqorder);

		/*�������������*/
		while(1)
		{
			++pChar;
			if ( isxdigit(*pChar) )
			{
				stSyscfgQuery.acBand[i] = *pChar;
			}
			else
			{
				stSyscfgQuery.acBand[i] = 0;
				i=0;
				break;
			}
			++i;	
		}
		/*������ĸ�����*/	

		stSyscfgQuery.ucRoam = StrStrToUnChar(&pChar, acCharParam);
		cx_printf("%s %d,acCharParam = %s, ucRoam = %d\n",  __FILE__, __LINE__, acCharParam, stSyscfgQuery.ucRoam);

		/*������������*/
		stSyscfgQuery.usSrvDomain = StrStrToUnChar(&pChar, acCharParam);
		cx_printf("55--%d,acCharParam = %s, usSrvDomain = %d\n",  __LINE__, acCharParam, stSyscfgQuery.usSrvDomain);
		
		
	}
	else
	{
		stSyscfgQuery.ucResult = HSPA_ERROR_UNKNOW;
	}
	memcpy(g_stAtToOther.acText, (void*)&stSyscfgQuery, sizeof(stSyscfgQuery));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    g_sndflg = 0;
	sem_post(&sem);
	
	return;
}
/*****************************************************************************
 �� �� ��  : ConfigInfoSet
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*SYSCONFIG_SET	41---AT^SYSCONFIG =<mode>,<acqorder>,<roam>,<srvdomain> 
*NORMAL:
*UNNORMAL:
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void ConfigInfoSet(char *pszInfoBack)
{
	struct stMsg_Sysconfig_Set stSysconfigSetBack;
	int lMsgId;
	memset(&stSysconfigSetBack, 0, sizeof(stSysconfigSetBack));
	g_stAtToOther.lMsgType = SYSCONFIG_SET;/*������Ϣ��Ӧ������*/
	stSysconfigSetBack.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	if ( NULL != strstr(pszInfoBack, "\r\nOK\r\n") )
	{
	  	stSysconfigSetBack.ucResult = HSPA_OK;
	}
	else
	{
		stSysconfigSetBack.ucResult = HSPA_ERROR_UNKNOW;
	}
	memcpy(g_stAtToOther.acText, (void*)&stSysconfigSetBack, sizeof(stSysconfigSetBack));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	return;
}
/*****************************************************************************
 �� �� ��  : ConfigInfoQuery
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*SYSCONFIG_QUERY	42----AT^SYSCONFIG?\r
*NORMAL:28--<CR><LF>^SYSCONFIG:2,1,1,2<CR><LF><CR><LF>OK<CR><LF>
*UNNORMAL:
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void ConfigInfoQuery(char *pszInfoBack)
{
	struct stMsg_Sysconfig_Query stSysConfigQuery;
	int lMsgId;
	char *pChar;
	char acCharParam[COMMON_VERY_SHORT_STRING_LENGTH];
	memset(&stSysConfigQuery, 0, sizeof(stSysConfigQuery));
	g_stAtToOther.lMsgType = SYSCONFIG_QUERY;/*������Ϣ��Ӧ������*/
	stSysConfigQuery.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/

	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	pChar = strstr(pszInfoBack, ":");
	if ( (NULL != strstr(pszInfoBack, "\r\nOK\r\n")) && (NULL != pChar) )
	{
		stSysConfigQuery.ucResult = HSPA_OK;
		/*�����һ������*/
		stSysConfigQuery.ucMode = StrStrToUnChar(&pChar, acCharParam);
		cx_printf("%d,ucMode = %d\n",  __LINE__, stSysConfigQuery.ucMode);
		/*����ڶ�������*/
		stSysConfigQuery.ucAcqorder= StrStrToUnChar(&pChar, acCharParam);
		cx_printf("%d,ucAcqorder = %d\n",  __LINE__, stSysConfigQuery.ucAcqorder);
		/*�������������*/
		stSysConfigQuery.ucRoam = StrStrToUnChar(&pChar, acCharParam);
		cx_printf("%d,ucRoam = %d\n",  __LINE__, stSysConfigQuery.ucRoam);
		/*������ĸ�����*/
		stSysConfigQuery.usSrvDomain = StrStrToUnChar(&pChar, acCharParam);
		cx_printf("%d,usSrvDomain = %d\n",  __LINE__, stSysConfigQuery.usSrvDomain);
	}
	else
	{
		stSysConfigQuery.ucResult = HSPA_ERROR_UNKNOW;
	}
	memcpy(g_stAtToOther.acText, (void*)&stSysConfigQuery, sizeof(stSysConfigQuery));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	return;
}
/*****************************************************************************
 �� �� ��  : CgdcontSet
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*PDP_SET	51--+CGDCONT=<cid>[,<PDP_type>[,<APN>[,<PDP_addr>[,<d_comp>[,<h_comp>]]]]]
*NORMAL:
*UNNORMAL:
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void CgdcontSet(char *pszInfoBack)
{
	struct  stMsg_PDP_Set stPdpSet;
	int lMsgId;
	memset(&stPdpSet, 0, sizeof(stPdpSet));
	g_stAtToOther.lMsgType = PDP_SET;/*������Ϣ��Ӧ������*/
	stPdpSet.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	if ( NULL != strstr(pszInfoBack, "\r\nOK\r\n") )
	{
	  	stPdpSet.ucResult = HSPA_OK;
	}
	else
	{
		stPdpSet.ucResult = HSPA_ERROR_UNKNOW;
	}
	memcpy(g_stAtToOther.acText, (void*)&stPdpSet, sizeof(stPdpSet));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	return;
}

void ParsePdpParam(char *pDst, char *pSrc, int len)
{
    int i = 0;
    for (i = 0; i < len; i++ )
    {
        *pDst = *pSrc;
        ++pSrc;
        ++i;
        ++pDst;
        if ( ',' == *pSrc )
        {
            *pDst = 0;
            break;
        }
    }
}
/*****************************************************************************
 �� �� ��  : CgdcontQuery
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*PDP_QUERY	52----AT+CGDCONT?\r
*NORMAL:48----<CR><LF>+CGDCONT: 1,"IP","CMNET","0.0.0.0",0,0 <CR><LF><CR><LF>OK<CR><LF>
*UNNORMAL:
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void CgdcontQuery(char *pszInfoBack)
{
	struct stMsg_PDP_Query stPdpQuery;
	int lMsgId;
	char *pChar;
	char acCharParam[COMMON_VERY_SHORT_STRING_LENGTH];
	int i = 0;
	memset(&stPdpQuery, 0, sizeof(stPdpQuery));
	g_stAtToOther.lMsgType = PDP_QUERY;/*������Ϣ��Ӧ������*/
	stPdpQuery.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	if ( NULL != strstr(pszInfoBack, "\r\nOK\r\n") )	
	{
		stPdpQuery.ucResult = HSPA_OK;
		pChar = index(pszInfoBack, ':' );/*������ǰû�б����κ�����*/
		if ( NULL == pChar)
			goto Exit1;
		/*������һ������1,"IP","CMNET","0.0.0.0",0,0*/
		stPdpQuery.ucCidIndex = StrStrToUnChar(&pChar, acCharParam);
		cx_printf("%d,acCharParam = %s, ucCidIndex = %u\n",  __LINE__, acCharParam, stPdpQuery.ucCidIndex);
	
		/*�����ڶ�������1,"IP","CMNET","0.0.0.0",0,0*/
		pChar = index(pChar, '"' );
		ParsePdpParam(stPdpQuery.acProtocolKind, pChar, HSPA_PROTOCOL_KIND_LENGTH);
		cx_printf("%d,acProtocolKind = %s\n",  __LINE__,stPdpQuery.acProtocolKind);
		/*��������������1,"IP","CMNET","0.0.0.0",0,0*/
		pChar = index(pChar, '"' );
		ParsePdpParam(stPdpQuery.acAPN, pChar, HSPA_APN_NAME_LENGTH);
		cx_printf("%d, acAPN = %s\n",  __LINE__,stPdpQuery.acAPN);
		/*�������ĸ�����1,"IP","CMNET","0.0.0.0",0,0*/
		pChar = index(pChar, '"' );
		ParsePdpParam(stPdpQuery.acDomainName, pChar, HSPA_DOMAIN_ADDR_LENGTH);
		cx_printf("%d, acDomainName = %s\n",  __LINE__,  stPdpQuery.acDomainName);
		/*���������������*/
		++pChar;
		stPdpQuery.ucDComp = (unsigned char)(*pChar - '0');/*���ַ�ת��Ϊ��Ӧ����ֵ*/
		pChar +=2;
		stPdpQuery.ucHComp = (unsigned char)(*pChar - '0');/*���ַ�ת��Ϊ��Ӧ����ֵ*/
		cx_printf("%d, ucDComp = %d, ucHComp = %d\n",  __LINE__,  stPdpQuery.ucDComp, stPdpQuery.ucHComp);
	}
	else
	{
		stPdpQuery.ucResult = HSPA_ERROR_UNKNOW;
	}
Exit1:
	memcpy(g_stAtToOther.acText, (void*)&stPdpQuery, sizeof(stPdpQuery));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	return;
}
/*****************************************************************************
 �� �� ��  : PinNumModify
 ��������  : 
 �������  : char *pszInfoBack  
             int lLen           
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*PIN��ʹ�ܼ��������²ſ�ʹ��
*PIN_PASSWORD_MODIFY	45--AT+CPWD=<fac>,<oldpwd>,<newpwd>
*NORMAL:
*UNNORMAL:	+CME ERROR: incorrect password
*				+CME ERROR: operation not allowed
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void PinNumModify(char *pszInfoBack)
{
	int lMsgId;
	g_stAtToOther.lMsgType = PIN_PASSWORD_MODIFY;/*������Ϣ��Ӧ������*/
	g_stAtToOther.acText[HSPA_FROM_POSITION]= AT_MODULE;/*������Ϣ��Դ*/
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	if ( NULL != strstr(pszInfoBack, "OK") )
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_OK;
	}
	else
	if ( NULL != strstr(pszInfoBack, "incorrect password") )
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_INCORRECT_PASSWORD;
	}
	else
	if ( NULL != strstr(pszInfoBack, "operation not allowed") )
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_OPERATION_REFUSED;
	}
	else
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_UNKNOW;
	}
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	return;
}
/*****************************************************************************
 �� �� ��  : PinEnableSet
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 

ָ��ͷ�����Ϣ��ʽ:
*PIN_ENABLE	48---AT+CLCK=
*NORMAL:		
*UNNORMAL:		+CME ERROR: incorrect password

 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void PinEnableSet(char *pszInfoBack)
{
	int lMsgId;
	g_stAtToOther.lMsgType = PIN_ENABLE;/*������Ϣ��Ӧ������*/
	g_stAtToOther.acText[HSPA_FROM_POSITION]= AT_MODULE;/*������Ϣ��Դ*/
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

    HspaMatchRead(pszInfoBack);
    /*add by w00135351 20080908*/
	if ( NULL != strstr(pszInfoBack, "OK") )
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_OK;
	}
	else
	if ( NULL != strstr(pszInfoBack, "+CME ERROR: 16") )
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_INCORRECT_PASSWORD;
	}
	else
	if ( NULL != strstr(pszInfoBack, "+CME ERROR: 3") )
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_OPERATION_REFUSED;
	}
	else
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_UNKNOW;
	}
    /*end by w00135351 20080908*/
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	return;
}
/*****************************************************************************
 �� �� ��  : PinNumVerfication
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 

*����PIN����֤
*PIN_MANAGE	49---AT+CPIN=<pin>[,<newpin>] 
*NORMAL:
*UNNORMAL:
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void PinNumVerfication(char *pszInfoBack)
{
	int lMsgId;
	struct stAtSndMsg stAtFromOther;
	g_stAtToOther.lMsgType = PIN_MANAGE;/*������Ϣ��Ӧ������*/
	g_stAtToOther.acText[HSPA_FROM_POSITION]= AT_MODULE;/*������Ϣ��Դ*/
	char* pPinCode = NULL;
	char* pPinEnd = NULL;
    char buf[COMMON_LONG_STRING_LENGTH];
	

	TTY_LOG("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

    usleep(500);
    HspaMatchRead(pszInfoBack);
    
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	if ( NULL!=strstr(pszInfoBack, "OK") )
	{
        /* turn on led. <2009.1.5 tanyin> */
        TTY_DEBUG("pin verified ok,turn on led\n");
        sysLedCtrl(kLedHspa, kLedStateOn);

		g_stAtToOther.acText[HSPA_RESULT_POSITION]= HSPA_OK;
		
		g_stHspa.sPinPukNeed = HSPA_PIN_PUK_OK;
		/*���¹����ļ�*/
		lseek(g_ShareiFd, 0,SEEK_SET);
		write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
		fsync(g_ShareiFd);

		
		if ( pPinCode = strstr(g_stAtFromOther.acParam, "AT+CPIN=\""))
		{
            if(NULL != strstr(g_stAtFromOther.acParam, ",\""))
            {
                pPinCode = strstr(g_stAtFromOther.acParam, ",\"");
                strcpy(g_stHspa.acPinCode, pPinCode+2);
            }
            else
            {
                strcpy(g_stHspa.acPinCode, pPinCode+strlen("AT+CPIN=\""));
            }
                
			TTY_DEBUG("pincode command: %s\n", pPinCode);
			//strcpy(g_stHspa.acPinCode, pPinCode+strlen("AT+CPIN=\""));
			pPinEnd = strstr(g_stHspa.acPinCode, "\"");
			if (pPinEnd)
			{
				*pPinEnd = '\0';
			}
			TTY_DEBUG("pincode: %s \n",g_stHspa.acPinCode);
		}


		strcpy(g_stHspa.acOldIMEI, g_stHspa.acIMEI);
		
        /* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add begin.*/
		if ( HSPA_OK == CheckHspaInfo(&g_stHspa))
		{
			sysWakeupMonitorTask();
            TTY_DEBUG("sysWakeupMonitorTask\n");
			cx_printf("%s, %d, %s(),       sysWakeupMonitorTask\n", __FILE__, __LINE__, __FUNCTION__);
		}
		BackupHspaInfo(&g_stHspa);
        /* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add end.*/
	}
	else
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION]= HSPA_ERROR_UNKNOW;
	}
	/*�ж��Ƿ�غ�ƣ��ж��Ƿ�����*/
	if ( ( HSPA_PIN_PUK_OK==  g_stHspa.sPinPukNeed) &&  ( g_stHspa.sSysMode > 0)  
		&& (HSPA_NO_SIM != g_stHspa.sSimStatus) && (0 != g_stHspa.sSimStatus)
		&&(0 != g_stHspa.sDeviceStatus)&&(7 != g_stHspa.sDeviceStatus))	
	{
		
		sysLedCtrl(kLedHspa, kLedStateOn); 
	}


	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId !=  lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}

    g_sndflg = 0;
	sem_post(&sem);

	if( HSPA_OK == g_stAtToOther.acText[HSPA_RESULT_POSITION]  )
	{
		write(g_iPcuiFd, "AT+CLIP=1\r",	strlen("AT+CLIP=1\r"));	
        usleep(500);
        read(g_iPcuiFd, buf, COMMON_LONG_STRING_LENGTH);
		#if 0
		write(g_iPcuiFd, "AT^LIGHT=0\r", strlen("AT^LIGHT=0\r"));
		#endif
		AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, PIN_ENABLE_QUERY, "AT+CLCK=\"SC\",2");
		sleep(1);
             /*Begin:Added by luokunling 00192527 for IMSI inform requirement,2011/9/20*/
		AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, IMSI_QUERY,"AT+CIMI");
	      /*end:Added by luokunling 00192527,2011/9/20*/
	}
	return;
}
/*****************************************************************************
 �� �� ��  : PinNumLeftQuery
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 

ָ��ͷ�����Ϣ��ʽ:
*PIN_PUK_TIMES_QUERY	69---AT^CPIN?\r
*NORMAL:		^CPIN: READY,,10,3,10,3
*UNNORMAL:

 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void PinNumLeftQuery(char *pszInfoBack)
{
	struct stMsg_Pin_Time_Query stPinTimeLeft;
	int lMsgId;
	char *pChar;
	char acCharParam[COMMON_VERY_SHORT_STRING_LENGTH];
	memset(&stPinTimeLeft, 0, sizeof(stPinTimeLeft));
	g_stAtToOther.lMsgType = PIN_PUK_TIMES_QUERY;/*������Ϣ��Ӧ������*/
	stPinTimeLeft.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

    HspaMatchRead(pszInfoBack);

    pChar=strstr(pszInfoBack, ",");
	if ( NULL != strstr(pszInfoBack, "^CPIN: READY") )
	{
		stPinTimeLeft.ucResult = HSPA_OK;
		/*stPinTimeLeft.ucTimes = 3;
		stPinTimeLeft.ucPukTimes= 10;
		stPinTimeLeft.ucPinTimes= 3;
		stPinTimeLeft.ucPuk2Times= 10;
		stPinTimeLeft.ucPin2Times= 3;*/ /*modified by w00135351 20081110*/
        //stPinTimeLeft.ucTimes = StrStrToUnChar(&pChar, acCharParam);
        stPinTimeLeft.ucTimes = 3;
        TTY_DEBUG("CPIN is ready\n");
        pChar++;
        stPinTimeLeft.ucPukTimes= StrStrToUnChar(&pChar, acCharParam);
        stPinTimeLeft.ucPinTimes= StrStrToUnChar(&pChar, acCharParam);
        stPinTimeLeft.ucPuk2Times= StrStrToUnChar(&pChar, acCharParam);
        stPinTimeLeft.ucPin2Times= StrStrToUnChar(&pChar, acCharParam);

        g_stHspa.sPinPukNeed = 0;  /*add by w00135351 20081226 ���ż��״̬�ļ��и�ֵδ���µ�����*/
		/*���¹����ļ�*/
		lseek(g_ShareiFd, 0,SEEK_SET);
		write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
		fsync(g_ShareiFd);
        
		goto Exit1;
	}
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	pChar=strstr(pszInfoBack, ",");
	if( ( NULL != strstr(pszInfoBack, "^CPIN:") ) && ( NULL != pChar ) )
	{
		stPinTimeLeft.ucResult = HSPA_OK;
		/*1*/
		stPinTimeLeft.ucTimes = StrStrToUnChar(&pChar, acCharParam);
		cx_printf("%d,ucTimes = %d\n",__LINE__, stPinTimeLeft.ucTimes);
		/*2*/
		stPinTimeLeft.ucPukTimes= StrStrToUnChar(&pChar, acCharParam);
		cx_printf("%d,ucPukTimes = %d\n",__LINE__, stPinTimeLeft.ucPukTimes);
		/*3*/
		stPinTimeLeft.ucPinTimes= StrStrToUnChar(&pChar, acCharParam);
		cx_printf("%d,ucPinTimes = %d\n",__LINE__, stPinTimeLeft.ucPinTimes);
		/*4*/
		stPinTimeLeft.ucPuk2Times= StrStrToUnChar(&pChar, acCharParam);
		cx_printf("%d,ucPuk2Times = %d\n",__LINE__, stPinTimeLeft.ucPuk2Times);
		/*5*/
		stPinTimeLeft.ucPin2Times= StrStrToUnChar(&pChar, acCharParam);
		cx_printf("%d,ucPin2Times = %d\n",__LINE__, stPinTimeLeft.ucPin2Times);
		if( 0 == stPinTimeLeft.ucPinTimes)
		{
			g_stHspa.sPinPukNeed = 2;
			/*���¹����ļ�*/
			lseek(g_ShareiFd, 0,SEEK_SET);
			write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
			fsync(g_ShareiFd);
            
            /* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add begin.*/
			if ( HSPA_OK == CheckHspaInfo(&g_stHspa))
			{
				sysWakeupMonitorTask();
                TTY_DEBUG("sysWakeupMonitorTask\n");
				cx_printf("%s, %d, %s(),       sysWakeupMonitorTask\n", __FILE__, __LINE__, __FUNCTION__);
			}
			BackupHspaInfo(&g_stHspa);
            /* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add end.*/

		}
	}
	else
	{
		stPinTimeLeft.ucResult = HSPA_ERROR_UNKNOW;
	}
Exit1:
	memcpy(g_stAtToOther.acText, &stPinTimeLeft, sizeof(stPinTimeLeft));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId != lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    g_sndflg = 0;
	sem_post(&sem);
	return;
}
/*****************************************************************************
 �� �� ��  : PinEnableQuery
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 

ָ��ͷ�����Ϣ��ʽ:
*PIN_ENABLE_QUERY	70---AT+CLCK="SC",2
*NORMAL:		
*UNNORMAL:		+CME ERROR: incorrect password
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void PinEnableQuery(char *pszInfoBack)
{
	int lMsgId;
	struct stMsg_Pin_Password_Verification stPinEnableSet;
	char *pChar = NULL;
	memset(&stPinEnableSet, 0, sizeof(stPinEnableSet));
	g_stAtToOther.lMsgType = PIN_ENABLE_QUERY;/*������Ϣ��Ӧ������*/
	stPinEnableSet.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	if ( (NULL != strstr(pszInfoBack, "OK")) ||  (NULL != strstr(pszInfoBack, "+CLCK:")) )
	{
		stPinEnableSet.ucResult = HSPA_OK;
		pChar = index(pszInfoBack, ':');
		if ( NULL != pChar )
		{
			++pChar;
			if ( isdigit(*pChar) )
			{
				cx_printf("%s,%d,pChar = %c\n", __FILE__, __LINE__, *pChar);
				g_stAtToOther.acText[2] =  (unsigned char)(*pChar - '0');
			}
			else
			{
				++pChar;
				cx_printf("%s,%d,pChar = %c\n", __FILE__, __LINE__, *pChar);
				g_stAtToOther.acText[2] = (unsigned char)(*pChar - '0');
			}
            stPinEnableSet.ucStatus = g_stAtToOther.acText[2]; /*add by 
            w00135351 20081030*/
		}
	
	}
    else if((NULL != strstr(pszInfoBack,"+CME ERROR: 11"))/*modified by 
    w00135351 20081030   need pin or need puk*/
    ||(NULL != strstr(pszInfoBack,"+CME ERROR: 12")))
    {
        stPinEnableSet.ucResult = HSPA_PIN_PUK_NEEDED;
    }
	else
	{
		stPinEnableSet.ucResult = HSPA_ERROR_UNKNOW;
	}

	if(HSPA_OK == g_stAtToOther.acText[HSPA_RESULT_POSITION])
	{
		g_stHspa.sSimStatus = stPinEnableSet.ucStatus;
		lseek(g_ShareiFd, 0,SEEK_SET);
		write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
		fsync(g_ShareiFd);
        
        /* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add begin.*/
		if ( HSPA_OK == CheckHspaInfo(&g_stHspa))
		{
			sysWakeupMonitorTask();
            TTY_DEBUG("sysWakeupMonitorTask\n");
			cx_printf("%s, %d, %s(),       sysWakeupMonitorTask\n", __FILE__, __LINE__, __FUNCTION__);
		}
		BackupHspaInfo(&g_stHspa);
        /* HUAWEI HGW s48571 2008��2��2��" Httpd requirement for voice state monitor add end.*/
	}	
	if ( ( 0 < g_stHspa.sPinPukNeed) || ( HSPA_NO_SIM ==  g_stHspa.sSimExist)
		|| ( 0 ==  g_stHspa.sSimExist) )
	{
		/*���*/
        if( (g_HspaLine & HSPA_LINE_MASK) == HSPA_ON )
        {
            /* BEGIN: Modified by c106292, 2009/3/17   PN:Led light method changes*/
        	//	sysLedCtrl(kLedHspa, kLedStateFastBlinkContinues);
         	 sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);
            /* END:   Modified by c106292, 2009/3/17 */
    	}
	}
	else
	{
		//sysLedCtrl(kLedHspa, kLedStateOn); 
	}
	
	cx_printf("%s,%d,%s\n",__FILE__, __LINE__, __FUNCTION__);
	memcpy(g_stAtToOther.acText, (void*)&stPinEnableSet, sizeof(stPinEnableSet));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
	cx_printf("%s,%d,%s\n",__FILE__, __LINE__, __FUNCTION__);
	write(g_iPcuiFd, "AT^CURC=1\r", strlen("AT^CURC=1\r"));
    g_sndflg = 0;
	sem_post(&sem);
	return;
}
/*****************************************************************************
 �� �� ��  : TelNumReport
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 

*CALL_NUM_REPORT	65	+CLIP: "075526747084",129,,,,0
*NORMAL:
*UNNORMAL:
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void TelNumReport(char *pszInfoBack)
{
	struct stVoiceMsg_Call_Num stTelNum;
	char *pChar;
	char acCharParam[COMMON_VERY_SHORT_STRING_LENGTH];
	int i = 0;
	memset(&stTelNum, 0, sizeof(stTelNum));
	g_stAtToOther.lMsgType = CALL_NUM_REPORT;/*������Ϣ��Ӧ������*/
	stTelNum.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	stTelNum.ucResult = HSPA_ERROR_UNKNOW;
	pChar = index(pszInfoBack, '"');
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	while(1)
	{
		++pChar;
		if('"'!=*pChar)
			stTelNum.acTelNum[i] = *pChar;
		else
		{
			stTelNum.acTelNum[i] = 0;
			break;
		}
		++i;			
	}
	cx_printf("%d,acTelNum = %s\n",__LINE__, stTelNum.acTelNum);
	++pChar;
	stTelNum.ucTelType = StrStrToUnChar(&pChar,  acCharParam);
	cx_printf("%d,ucTelType = %d\n",__LINE__, stTelNum.ucTelType);
	pChar += strlen(",,,,");
	stTelNum.ucCliFlag = (unsigned char)(*pChar -'0');
	cx_printf("%d,ucCliFlag = %d\n",__LINE__, stTelNum.ucCliFlag);

	memcpy(g_stAtToOther.acText, (void*)&stTelNum, sizeof(stTelNum));
	/*Ҫ�ϱ�ҵ�����.*/
#ifdef CX_DEBUG_VERSION	
	msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, 0);
#else
	msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
#endif
	cx_printf("%s,%d,%s\n",__FILE__, __LINE__, __FUNCTION__);
	return;
}

/*****************************************************************************
 �� �� ��  : CallDial
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void CallDial(char *pszInfoBack)
{
	int lMsgId;
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	if ( NULL == strstr(pszInfoBack,  "\r\nOK\r\n") )
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_UNKNOW;

		g_stAtToOther.lMsgType = DIAL;
		lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
		cx_printf("%s,%d,%s(),lMsgId = %d\n", __FILE__,  __LINE__, __FUNCTION__, lMsgId);
		/*���ǿ���ģ���������Ϣ�����跴��*/
		if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
		{
            TTY_DEBUG("lMsgId is %d\n", lMsgId);
			msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
		}	
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	return;
}


/*****************************************************************************
 �� �� ��  : AutoConf
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��2��2��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void AutoConf(char *pszInfoBack)
{
    struct stAtSndMsg stAtFromOther;
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	//write(g_iPcuiFd, "AT^DDSETEX=2\r",strlen("AT^DDSETEX=2\r"));
    AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, DEX_SET,"AT^DDSETEX=2" );
    g_iCallOutFlag = VOICE_CALLOUT_AUTOCONF;
	/* BEGIN: Added by c106292, 2009/3/16   PN:Led light method changes*/
	g_ulTrafficFlag	|= CALL_ON_HSPA;
	sysLedCtrl(kLedHspa, kLedStateFastBlinkContinues);
	/* END:   Added by c106292, 2009/3/16 */
#if 0
	g_stAtToOther.lMsgType = DIAL;
	g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;
	g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_OK;
	msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, 0);
#endif

}

/*****************************************************************************
 �� �� ��  : AcceptCall
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void AcceptCall(char *pszInfoBack)
{
	int lMsgId;
    int i = 0;
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
    for( i = 0; i < 3; i++ )
    {
        if ( NULL == strstr(pszInfoBack, "\r\nOK\r\n") )
        {
        	memset(pszInfoBack, 0, AT_RCV_MSG_LENGTH);
		TTY_LOG("ATA: Find OK error,read pcui again\n");
        	read(g_iPcuiFd, pszInfoBack, AT_RCV_MSG_LENGTH);
            cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
        }
        else
        {
	/* BEGIN: Added by c106292, 2009/3/16   PN:Led light method changes*/
	g_ulTrafficFlag	|= CALL_ON_HSPA;
	sysLedCtrl(kLedHspa, kLedStateFastBlinkContinues);
	/* END:   Added by c106292, 2009/3/16 */
            break;
        }
    }

	if ( NULL != strstr(pszInfoBack, "\r\nOK\r\n") )
	{	
        g_iCallInFlag = VOICE_CALLIN_ATA;
        TTY_LOG("ATA: Find OK error,read pcui again\n");
	}
	else
	{
        g_stAtToOther.lMsgType = ACCEPT;
        g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;
        g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_UNKNOW;
        TTY_LOG("ATA: Find OK error,Send HSPA_ERROR_UNKNOW\n");
        msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
        g_iCallInFlag = VOICE_CALLIN_IDLE;
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	return;
}

/*****************************************************************************
 �� �� ��  : DExSet
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void DExSet(char *pszInfoBack)
{

    if( VOICE_CALLIN_CONN == g_iCallInFlag )
    {
    	g_stAtToOther.lMsgType = ACCEPT;
    	g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;
        
    	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
    	if ( NULL == strstr(pszInfoBack, "\r\nOK\r\n") )
    	{
    		memset(pszInfoBack, 0, AT_RCV_MSG_LENGTH);
    		read(g_iPcuiFd, pszInfoBack, AT_RCV_MSG_LENGTH);
    	}
    	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

    	if ( NULL != strstr(pszInfoBack, "\r\nOK\r\n") )
    	{	
    		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_OK;
    		TTY_LOG("DEXSET:SEND OK for ATA");
    	}
    	else
    	{
    		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_UNKNOW;
    		TTY_LOG("DEXSET:SEND NOK for ATA");
    	}
        
    	msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
        g_iCallInFlag = VOICE_CALLIN_IDLE;
    }
#if 1
    else if (VOICE_CALLOUT_AUTOCONF == g_iCallOutFlag)
    {
        g_stAtToOther.lMsgType = DIAL;
        g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;

    	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
    	if ( NULL == strstr(pszInfoBack, "\r\nOK\r\n") )
    	{
    		memset(pszInfoBack, 0, AT_RCV_MSG_LENGTH);
    		read(g_iPcuiFd, pszInfoBack, AT_RCV_MSG_LENGTH);
    	}
    	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

    	if ( NULL != strstr(pszInfoBack, "\r\nOK\r\n") )
    	{	
    		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_OK;
    		TTY_LOG("DEXSET:SEND OK for ATD");
    	}
    	else
    	{
    		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_UNKNOW;
    		TTY_LOG("DEXSET:SEND NOK for ATD");
    	}

    	msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
        g_iCallOutFlag = VOICE_CALLOUT_IDLE;

        
    }
#endif
    g_sndflg = 0;
	sem_post(&sem);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	return;
}


/*****************************************************************************
 �� �� ��  : EndCallReport
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  :  ^CEND:1,0,104,31--31����û�У��ϱ����͸�ֵ0��
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void EndCallReport(char *pszInfoBack)
{
	struct stVoice_Msg_Call_End stCallEnd;
	char *pChar;
	char acCharParam[COMMON_VERY_SHORT_STRING_LENGTH];
	g_stAtToOther.lMsgType = CALL_END_REPORT;
	stCallEnd.ucCmdFrom = AT_MODULE;
	stCallEnd.ucResult = HSPA_OK;
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
    g_iCallInFlag = VOICE_CALLIN_IDLE;

	/* BEGIN: Added by c106292, 2009/3/16   PN:Led light method changes*/
	g_ulTrafficFlag &=  ~CALL_ON_HSPA;
	if( 0  !=  g_stHspa.sDeviceCount)
	{
            /* BEGIN: Modified by y67514, 2009/12/8   PN:��δ��pin��״̬���������к�hspa�Ƴ���7��8��*/
            if ( ( HSPA_NO_SIM == g_stHspa.sSimExist ) ||( HSPA_INVALID_SIM == g_stHspa.sSimExist ) 
                || (HSPA_PIN_PUK_OK != g_stHspa.sPinPukNeed))
            {
                TTY_DEBUG("HSPA is invalid! \n");
                sysLedCtrl(kLedHspa, kLedStateSlowFlickerContinues);
            }
            else if( 2 == getWanStatus())
            {
                TTY_DEBUG("Data is on \n");
                sysLedCtrl(kLedHspa, kLedStateOff);
                sysLedCtrl(kLedHspa, kLedStateVerySlowBlinkContiunes);
            }
            else
            {
                TTY_DEBUG("Data is not ready \n");
                sysLedCtrl(kLedHspa, kLedStateOff);	  
                sysLedCtrl(kLedHspa, kLedStateOn);
            }
    	/* END:   Modified by y67514, 2009/12/8 */
	}
	/* END:   Added by c106292, 2009/3/16 */

	pChar = index(pszInfoBack, ':');
	if ( NULL == pChar )
		goto Exit1;
	++pChar;
	stCallEnd.ucCallId = *pChar -'0';
	cx_printf("%s, %d, ucCallId = %u\n",__FILE__, __LINE__, stCallEnd.ucCallId );
	/*������2������*/
	pChar = index(pChar, ',');
	if ( NULL == pChar )
		goto Exit1;
    //add by z67625 20080330 for cend, start
	stCallEnd.ulDuration = StrStrToInt(&pChar, acCharParam);
	cx_printf("%s, %d, ucEndCause = %u\n",__FILE__, __LINE__, stCallEnd.ucEndCause );
	/*������3������66  ^CEND:1,0,104,31 */
	stCallEnd.ucEndCause = StrStrToUnChar(&pChar, acCharParam);
    //add by z67625 20080330 for cend, end
	cx_printf("%s,%d, ulDuration = %u\n",__FILE__, __LINE__, stCallEnd.ulDuration );
	/*������4������66  ^CEND:1,0,104,31 */
	stCallEnd.ucCallCtlCause= StrStrToUnChar(&pChar, acCharParam);
	cx_printf("%s, %d, ucCallCtlCause = %u\n",__FILE__, __LINE__, stCallEnd.ucCallCtlCause );
Exit1:	
	memcpy(g_stAtToOther.acText, (void*)&stCallEnd, sizeof(stCallEnd));
#ifndef CX_DEBUG_VERSION	
	msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
#else
	msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
#endif
	return;
}

/*****************************************************************************
 �� �� ��  : CfunSet
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���
*****************************************************************************/
static void CfunSet(char *pszInfoBack)
{
	int lMsgId;
	cx_printf("%s,%d,pszInfoBack = %s", __FILE__,  __LINE__, pszInfoBack);

    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,pszInfoBack = %s", __FILE__,  __LINE__, pszInfoBack);
	if ( NULL != strstr(pszInfoBack,  "\r\nOK\r\n") )
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_OK;
	}
	else
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_UNKNOW;
	}
	g_stAtToOther.lMsgType = CFUN_SET;
	g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    g_sndflg = 0;
	sem_post(&sem);
	return;

}
/*****************************************************************************
 �� �� ��  : CfunQuery
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
����ģʽ��ѯ����
�����ʽCFUN_QUERY ---- AT+CFUN�� 
��Ӧ��ʽ��<CR><LF>+CFUN: 1<CR><LF><CR><LF>OK<CR><LF>��MS��ش���ʱ��<CR><LF>+CME ERROR: <err><CR><LF> 
 �޸���ʷ      :
  1.��    ��   : 2008��1��8��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void CfunQuery(char *pszInfoBack)
{
//	struct stMsg_Operation_Mode_Query stOpModeQuery;
	int lMsgId;
	char *pChar;
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	if ( NULL == strstr(pszInfoBack, "+CFUN:") )
	{
		memset(pszInfoBack, 0, AT_RCV_MSG_LENGTH);
		read(g_iPcuiFd, pszInfoBack, AT_RCV_MSG_LENGTH);
	}
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	pChar = strstr(pszInfoBack,  "+CFUN:");
	if ( (NULL != strstr(pszInfoBack,  "\r\nOK\r\n")) && (NULL != strstr(pszInfoBack,  "+CFUN:"))   )
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_OK;
		pChar = index(pChar, ':');
		if ( NULL != pChar )
		{
			++pChar;
			if(isdigit(*pChar))
			{
				g_stAtToOther.acText[2] = *pChar - '0';
			}
			else
			{
				++pChar;
				g_stAtToOther.acText[2] = *pChar - '0';
			}			
		}
	}
	else
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_UNKNOW;
	}
	g_stAtToOther.lMsgType = CFUN_QUERY;
	g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;

	/*���¹�������*/
	if ( HSPA_OK == g_stAtToOther.acText[HSPA_RESULT_POSITION] )
	{
		g_stHspa.sDeviceStatus = g_stAtToOther.acText[2];
        #if 0 //AU8D00583 SIP user��ע��Ȼ������ע�ᣬhspa����ʾ����ȷ������ -- ������
        if( (HSPA_PIN_PUK_OK != g_stHspa.sPinPukNeed) || (g_stHspa.sSimExist == HSPA_NO_SIM) )
            sysLedCtrl(kLedHspa, kLedStateFail);
        #endif

	}
	else
	{
		g_stHspa.sDeviceStatus = HSPA_UNKNOW;
	}
	/*���¹����ļ�*/
	lseek(g_ShareiFd, 0,SEEK_SET);
	write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
	fsync(g_ShareiFd);	

	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	
	return;
}
/*****************************************************************************
 �� �� ��  : AutoModeReport
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void AutoModeReport(char *pszInfoBack)
{
	struct stMsg_System_Mode stSysMode;
	char *pChar;
	int lLightFlag = 0;
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	g_stAtToOther.lMsgType = AUTO_SYSTEM_MODE_STATUS_REPORT;
	stSysMode.ucCmdFrom = AT_MODULE;
	stSysMode.ucResult = HSPA_OK;

	pChar = index(pszInfoBack, ':');
	++pChar;
	stSysMode.ucSysMode = *pChar -'0';
	pChar +=2;
	stSysMode.ucSubSysMode= *pChar -'0';
#if 0
	/*�ж��Ƿ��������״̬*/
	if ( ( g_stHspa.sSysMode < 4) && (stSysMode.ucSysMode >= 4 ) )
	{
		lLightFlag = 1;
	}
	else
	if ( (-1 ==  g_stHspa.sSysMode ) && (stSysMode.ucSysMode > 0 ) )
	{
		lLightFlag = 1;
	}
#endif

	/*���¹����ļ�*/
	g_stHspa.sSysMode = (short int)stSysMode.ucSysMode;
	g_stHspa.sSubSysMode = (short int)stSysMode.ucSubSysMode;
	lseek(g_ShareiFd, 0, SEEK_SET);
	write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
	fsync(g_ShareiFd);
	
	memcpy(&g_stAtToOther.acText, &stSysMode, sizeof(stSysMode));
#ifndef CX_DEBUG_VERSION	
//	msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, 0);
//	msgsnd(g_lModemMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, 0);
//	msgsnd(g_lTr069MsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, 0);
//	msgsnd(g_lLedMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, 0);
#else
	//msgsnd(g_lCmdMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, 0);
#endif

#if 0
	/*HSPA��������*/
	if (( 1 == lLightFlag) &&  ( 0 <  g_stHspa.sSysMode) 
		&& (HSPA_INVALID_SIM!= g_stHspa.sSimExist) && (HSPA_NO_SIM != g_stHspa.sSimExist))
	{
		if ( g_stHspa.sSysMode < 4)
		{
			cx_printf("%d, 2G����\n", __LINE__);
			sysLedCtrl(kLedHspa, kLedStateOn); 
			sysSetHspaTrafficMode(MODE_2G);
		}
		else
		if ( ( g_stHspa.sSysMode < 8) )	
		{
			cx_printf("%d, 3G����\n", __LINE__);
			sysLedCtrl(kLedHspa, kLedStateOn); 
			sysSetHspaTrafficMode(MODE_3G);
		}
	}
	
	if ( 0 == stSysMode.ucSysMode )
	{
		/*���*/
		cx_printf("%d,û��������������ƣ�������\n",__LINE__);
		sysSetHspaTrafficMode(MODE_NONE);
		sysLedCtrl(kLedHspa, kLedStateOff);
		sysLedCtrl(kLedHspa, kLedStateFail);
	}
	else
	if ( ( HSPA_INVALID_SIM ==  g_stHspa.sSimExist) ||  ( HSPA_NO_SIM ==  g_stHspa.sSimExist) )
	{
		/*���*/
		cx_printf("%d,û��SIM����SIM����Ч������ƣ�������\n",__LINE__);
		sysSetHspaTrafficMode(MODE_NONE);
		sysLedCtrl(kLedHspa, kLedStateOff);
		sysLedCtrl(kLedHspa, kLedStateFail);
	}
	/*û���豸���ص�
	��ֹ�����濪��ʱ�򣬱��źŴ�
	�������*/
	if ( HSPA_NO == g_stHspa.sDeviceCount)
	{
		cx_printf("%d,û�����ݿ��ص�\n",__LINE__);
		sysLedCtrl(kLedHspa, kLedStateOff);
		sysSetHspaTrafficMode(MODE_NONE); 	
	}
#endif
	/* start of VOICE ���������� by pengqiuming(48166), 2008/9/9 
	   for ��ttyUSB����ʱ������һ�߳�ÿ��10s����AT^SYSINFO,��������Ͳ���Ҫ��*/
	/*
	struct stAtSndMsg stAtFromOther;
	AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, SYSTEM_INFO_QUERY, "AT^SYSINFO");
	*/
	/* end of VOICE ���������� by pengqiuming(48166), 2008/9/9 */
	return;

}

/*****************************************************************************
 �� �� ��  : AutoServerStatus
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void AutoServerStatus(char *pszInfoBack)
{
	struct stMsg_Service_Status stSerStatus;
	char *pChar;
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	
	g_stAtToOther.lMsgType = AUTO_SERVICE_STATUS_REPORT;
	stSerStatus.ucCmdFrom = AT_MODULE;
	stSerStatus.ucResult = HSPA_OK;
	
	pChar = index(pszInfoBack, ':');
	++pChar;
	stSerStatus.ucSrvStatus = *pChar - '0';
	
	memcpy(&g_stAtToOther.acText, &stSerStatus, sizeof(stSerStatus));

	/*���¹����ļ�*/
	g_stHspa.sSrvStatus = (short int)stSerStatus.ucSrvStatus;
	lseek(g_ShareiFd, 0, SEEK_SET);
	write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
	fsync(g_ShareiFd);
	
#ifndef CX_DEBUG_VERSION	
//	msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, 0);
//	msgsnd(g_lModemMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, 0);
//	msgsnd(g_lTr069MsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, 0);
//	msgsnd(g_lLedMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, 0);
#else
	//msgsnd(g_lCmdMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, 0);
#endif

	return;
}

/*****************************************************************************
 �� �� ��  : AutoRing
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��14��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void AutoRing(char *pszInfoBack)
{
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	g_stAtToOther.lMsgType = RING_REPORT;
	g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;
	g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_OK;

#ifndef CX_DEBUG_VERSION	
	msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
#else
	msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
#endif
}
/*****************************************************************************
 �� �� ��  : AutoConn
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void AutoConn(char *pszInfoBack)
{
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
    struct stAtSndMsg stAtFromOther;

    if( VOICE_CALLIN_ATA == g_iCallInFlag )
    {
        g_iCallInFlag = VOICE_CALLIN_CONN;
        AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, DEX_SET,"AT^DDSETEX=2" );
    }
    else 
    {
    	g_stAtToOther.lMsgType = CONN_REPORT;
    	g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;
    	g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_OK;

#ifndef CX_DEBUG_VERSION	
	msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
#else
	msgsnd(g_lVoiceMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
#endif
    }
}

/*****************************************************************************
 �� �� ��  : OperationInfoQuery
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
#define 	OPERATION_QUERY 72
AT+COPS?

<CR><LF>+COPS: <mode>[,<format>,<oper>[,<rat>]]
<CR><LF>OK<CR><LF>
��MS��ش���ʱ��
<CR><LF>+CME ERROR: <err><CR><LF>
/*                                                                  
+COPS: 0,2,"46000",0                                                            
                                                                                
OK �޸���ʷ      :
  1.��    ��   : 2008��1��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void OperationInfoQuery(char *pszInfoBack)
{
	int lMsgId;
	char *pChar = NULL;
	int i = 0;
	struct stMsg_Operation_Info_Query stOperInfo;
	char acCharParam[COMMON_VERY_SHORT_STRING_LENGTH];
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	g_stAtToOther.lMsgType = OPERATION_QUERY;
	memset(&stOperInfo, 0, sizeof(stOperInfo));
	stOperInfo.ucCmdFrom = AT_MODULE;

    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	
	pChar = strstr(pszInfoBack, "+COPS:");
	if ( NULL != pChar )
	{
		stOperInfo.ucResult = HSPA_OK;
		pChar += strlen("+COPS:");
		cx_printf("%s,%d,%s(),pChar = %s\n", __FILE__,  __LINE__, __FUNCTION__, pChar);
		stOperInfo.ucChooseMode = StrStrToUnChar(&pChar,  acCharParam);
		if ( 0 != strncmp(pChar, "\r\n", 2 ) )
		{
			cx_printf("%s,%d,%s(),pChar = %s\n", __FILE__,  __LINE__, __FUNCTION__, pChar);
			stOperInfo.ucOperationFormat = StrStrToUnChar(&pChar,  acCharParam);
			pChar += strlen(",\"");
			cx_printf("%s,%d,%s(),pChar = %s\n", __FILE__,  __LINE__, __FUNCTION__, pChar);
			while(1)
			{
				if ( '"' == *pChar )
				{
					stOperInfo.acOperationInfo[i] = 0;
					break;
				}
				stOperInfo.acOperationInfo[i] = *pChar;
				++pChar;
				++i;
			}
			cx_printf("%s,%d,%s(),pChar = %s\n", __FILE__,  __LINE__, __FUNCTION__, pChar);
			++pChar;
			stOperInfo.ucWifiTech = StrStrToUnChar(&pChar,  acCharParam);
			cx_printf("%s,%d,%s(),pChar = %s\n", __FILE__,  __LINE__, __FUNCTION__, pChar);
		}
		
	}
	else
	{
		stOperInfo.ucResult = HSPA_ERROR_UNKNOW;
	}
	memcpy(g_stAtToOther.acText, &stOperInfo, sizeof(stOperInfo));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	return;
}
/*****************************************************************************
 �� �� ��  : CardModeQuery
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
	<CR><LF>^CARDMODE:<sim_type><CR><LF><CR><LF>OK <CR><LF> �޸���ʷ      :
  1.��    ��   : 2008��1��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void CardModeQuery(char *pszInfoBack)
{
	int lMsgId;
	char *pChar;
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	g_stAtToOther.lMsgType = CARDMODE_QUERY;
	g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;
	
    HspaMatchRead(pszInfoBack);

	pChar =  strstr(pszInfoBack, "^CARDMODE:");
	if ( NULL != pChar )
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_OK;
		pChar += strlen("^CARDMODE:");
		if ( isdigit(*pChar))
		{
			g_stAtToOther.acText[2] = *pChar - '0';			
		}
		else
		{
			++pChar;
			g_stAtToOther.acText[2] = *pChar - '0';			
		}
	}
	else
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_UNKNOW;
	}
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    
    g_sndflg = 0;
	sem_post(&sem);
	return;
}
/*****************************************************************************
 �� �� ��  : ChupBack
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void ChupBack(char *pszInfoBack)
{
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
    g_iCallInFlag = VOICE_CALLIN_IDLE;
    g_sndflg = 0;
	/* BEGIN: Added by c106292, 2009/3/16   PN:Led light method changes*/
	g_ulTrafficFlag	&= ~CALL_ON_HSPA;

	/*c106292*/

	if( 2 == getWanStatus())
	{
        sysLedCtrl(kLedHspa, kLedStateOff);
        sysLedCtrl(kLedHspa, kLedStateVerySlowBlinkContiunes);
	}
	else
	{
	    sysLedCtrl(kLedHspa, kLedStateOff);
	    sysLedCtrl(kLedHspa, kLedStateOn);
	}
	/* END:   Added by c106292, 2009/3/16 */
	sem_post(&sem);
	return;
}

/*****************************************************************************
 �� �� ��  : HexToNum
 ��������  : ��һ��16���Ƶ��ַ�ת��Ϊ��Ӧ���޷���
 				��ֵ
 �������  : ��
 �� �� ֵ  : unsigned long int 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��25��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
 unsigned long int HexToNum( char cChar)
 {
 	switch(cChar)
 	{
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case 'a':
			return 10;
		case 'A':
			return 10;
		case 'b':
			return 11;
		case 'B':
			return 11;
		case 'c':
			return 12;
		case 'C':
			return 12;
		case 'd':
			return 13;
		case 'D':
			return 13;
		case 'e':
			return 14;
		case 'E':
			return 14;
		case 'f':
			return 15;
		case 'F':
			return 15;
		default:
			return 0;
 	}
 }
/*****************************************************************************
 �� �� ��  : aHexToUl
 ��������  : ��һ��16�����ַ���ת��Ϊ��Ӧ���޷�������
 �������  : char * pszChar  
 �������  : ��
 �� �� ֵ  : unsigned long int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��1��25��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
unsigned long int  aHexToUl(char * pszChar)
 {
 	#define HEX_NUM_LENGHT 20
	#define LONG_UP	16
 	char *p = NULL;
	char acHex[HEX_NUM_LENGHT] = {0};
	int i ;
	int lLen = strlen(pszChar);
	unsigned long ulResult = 0;
	unsigned long ulCom = 0;
	if ( 0 == lLen)
	{
		return 0;
	}
	//�ж��Ƿ���16�������ַ���
	p = pszChar;
	for(i=0; i<lLen; ++i)
	{
		if ( 0 == isxdigit(*p))
			return 0;
		++p;
	}

	//�ҵ���������λ
	p = pszChar;
	for(i=0; i<lLen; ++i)
	{
		if ( '0' == *p)
		{
			++p;
			continue;
		}
		else
		{
			break;
		}
	}
	strcpy(acHex, p);
	lLen = strlen(acHex);
	if ( LONG_UP < lLen )
	{
		return 0;
	}
	for ( i=0; i < lLen; ++i)
	{
		ulCom = HexToNum( acHex[i] );
		ulCom <<= ((lLen-i-1)<<2);
		ulResult += ulCom;
	}
 	return ulResult;
 }

/*****************************************************************************
 �� �� ��  : AutoDataFlow
 ��������  : ���������ϱ�������16λ��û�м���
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
��ʽ:^DSFLOWRPT:00000004,00000000,00000000,0000000000000080,000000000000017C,0003E800,10106803
 �޸���ʷ      :
  1.��    ��   : 2008��1��25��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/

static void AutoDataFlow(char *pszInfoBack)
{
	unsigned long  ulQosTxRate = 0;
    unsigned long  ulQosRxRate = 0;
    unsigned long  ulTxRate = 0;
    unsigned long  ulRxRate = 0;
	char *pChar;
	int fd;
	char acCharParam[COMMON_SHORT_STRING_LENGTH] = {0};

	memset(acCharParam, 0, COMMON_SHORT_STRING_LENGTH);
	pChar = index(pszInfoBack, ':');
	if( NULL == pChar )
	{
		return;
	}
#if 0
	++pChar;
	strncpy(acCharParam, pChar, 8);
	g_stHspa.stDataFlowInfo.ulCurrDsTime = aHexToUl(acCharParam);

	pChar += 9;
	strncpy(acCharParam, pChar, 8);
	g_stHspa.stDataFlowInfo.ulTxRate = aHexToUl(acCharParam);

	pChar += 9;
	strncpy(acCharParam, pChar, 8);
	g_stHspa.stDataFlowInfo.ulRxRate = aHexToUl(acCharParam);
	pChar += 43;
#endif

/*start of HG_Support 2008.10.21 HG556AV100R001C01B020 AU8D00943 */
#if 1
        pChar += 10;
        strncpy(acCharParam, pChar, 8);
        ulTxRate = aHexToUl(acCharParam);
        //printf("TX:acCharParam = %s\t ulTxRate = %d\n",acCharParam,ulTxRate);
        if(ulTxRate != g_stHspa.stDataFlowInfo.ulTxRate)
        {
            g_stHspa.stDataFlowInfo.ulTxRate = ulTxRate;
            /*���¹����ļ�*/
            lseek(g_ShareiFd, 0, SEEK_SET);
            write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
            fsync(g_ShareiFd);
        }
        
        pChar += 9;
        strncpy(acCharParam, pChar, 8);
        ulRxRate = aHexToUl(acCharParam);
        //printf("RX:acCharParam = %s\t ulRxRate = %d\n",acCharParam,ulRxRate);
        if(ulRxRate != g_stHspa.stDataFlowInfo.ulRxRate)
        {
            g_stHspa.stDataFlowInfo.ulRxRate = ulRxRate;
            /*���¹����ļ�*/
            lseek(g_ShareiFd, 0, SEEK_SET);
            write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
            fsync(g_ShareiFd);
        }
#endif
/*end of HG_Support 2008.10.21 HG556AV100R001C01B020 AU8D00943 */


    pChar = index(pszInfoBack, ':');
	pChar += 62;
	strncpy(acCharParam, pChar, 8);
	ulQosTxRate = aHexToUl(acCharParam);
	//g_stHspa.stDataFlowInfo.ulQosTxRate = aHexToUl(acCharParam);
	if ( ulQosTxRate != g_stHspa.stDataFlowInfo.ulQosTxRate)
	{
		g_stHspa.stDataFlowInfo.ulQosTxRate = ulQosTxRate;
		lseek(g_ShareiFd, 0, SEEK_SET);
		write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
		fsync(g_ShareiFd);

		if( (fd = open( "/dev/brcmboard", O_RDWR )) != -1 )
	    {
	        ioctl( fd, BOARD_IOCTL_WAKEUP_MONITOR_TASK, NULL);
	        close(fd);
	    }
	}
	/*if ( g_stHspa.stDataFlowInfo.ulQosTxRate != ulQosTxRate )
	{
		int fd = -1;
		g_stHspa.stDataFlowInfo.ulQosTxRate = ulQosTxRate;
		if( (fd = open( "/dev/brcmboard", O_RDWR )) != -1 )
		{
		    ioctl( fd, BOARD_IOCTL_WAKEUP_MONITOR_TASK, NULL);
		    close(fd);
		}
	}*/

/* commented by tanyin 2009.5.20*/
#if 0
	/* BEGIN: Modified by c106292, 2009/4/15   PN: */
	if (  CALL_ON_HSPA & g_ulTrafficFlag )
	{   
        //�绰��
	    sysLedCtrl(kLedHspa, kLedStateFastBlinkContinues);
	}
	else if( (ulRxRate!=0) ||( ulTxRate!=0 ) ) 
	{
	    sysLedCtrl(kLedHspa, kLedStateVerySlowBlinkContiunes);
	}
	else
	{
	    sysLedCtrl(kLedHspa, kLedStateOff);
	    sysLedCtrl(kLedHspa, kLedStateOn);
	}
/* END:   Modified by c106292, 2009/4/15 */
#endif

#if 0
	pChar += 9;
	strncpy(acCharParam, pChar, 8);
	g_stHspa.stDataFlowInfo.ulQosRxRate = aHexToUl(acCharParam);
	/*���¹����ļ�*/
	lseek(g_ShareiFd, 0, SEEK_SET);
	write(g_ShareiFd, &g_stHspa, sizeof(g_stHspa));
	fsync(g_ShareiFd);
#endif
}
/*****************************************************************************
 �� �� ��  : ImeiInfoAnlysis
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
ָ��ͷ�����Ϣ��ʽ:
*IMEI_QUERY	74----AT^SN\r
*NORMAL:8----AT^SN<CR>,  25----<CR><LF>^SN: EV2AB10810301426<CR><LF><CR><LF>OK<CR><LF>
*UNNORMAL:<CR><LF>+CME ERROR: <err><CR><LF>
 �޸���ʷ      :
  1.��    ��   : 2008��1��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
static void SnQuery(char *pszInfoBack) 
{
 	struct stMsg_SN_Query stSn;
	int lMsgId;
	int lLen;
	memset(&stSn, 0, sizeof(stSn));
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	g_stAtToOther.lMsgType = SN_QUERY;/*������Ϣ��Ӧ������*/
	stSn.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	/*�ж�������Ժͷ�����Ϣ�Ƿ�ͬʱ����
	��������һ����Ҫ��ȡ������Ϣ*/
    HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	pszInfoBack = strstr(pszInfoBack, "\r\n");
       pszInfoBack = strstr(pszInfoBack, "^SN:"); /*����K3772 HSPA,ȥ��"^SN: "�еĿո�*/
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
    
	if ( (NULL != pszInfoBack) && (NULL != strstr(pszInfoBack, "\r\nOK\r\n")) )/*�жϷ�����Ϣ�Ƿ�ɹ�*/
	{
		lLen = strlen(pszInfoBack);
		stSn.ucResult = HSPA_OK;
        /*Begin:Modefid by luokunling l00192527,2012/2/2*/
		lLen = lLen-strlen("^SN:")-strlen("\r\n\r\nOK\r\n");  /*Ϊ��ȡ���кų��ȣ����ȥ�ַ���ͷ"^SN:"����
                                                                                 ֮ǰ��ȥĩβ"\r\n\r\n\r\nOK\r\n"���� ����, �޸�Ϊ��ȥ"\r\n\r\nOK\r\n"����*/
        /*End:Modefid by luokunling l00192527,2012/2/2*/
		if ( lLen >= sizeof(stSn.acSN) )
			lLen = sizeof(stSn.acSN);
        /*Begin:Modefid by luokunling l00192527,2012/2/2*/
		memcpy(stSn.acSN, pszInfoBack+strlen("^SN:"), lLen); /*����K3772 HSPA,ȥ��"^SN: "�еĿո�*/
        /*End:Modefid by luokunling l00192527,2012/2/2*/
	}	
	else
	{
		stSn.ucResult = HSPA_ERROR_UNKNOW;
	}	

	memcpy(g_stAtToOther.acText, (void*)&stSn, sizeof(stSn));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    g_sndflg = 0;
	sem_post(&sem);
	return;

}

static void CgactQuery(char *pszInfoBack) 
{
	struct stMsg_CGACT_Query stSn;
	int lMsgId;
	int lLen;
	char* pcTmp = 0;
	memset(&stSn, 0, sizeof(stSn));
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
	g_stAtToOther.lMsgType = CGACT_QUERY;/*������Ϣ��Ӧ������*/
	stSn.ucCmdFrom = AT_MODULE;/*������Ϣ��Դ*/
	/*�ж�������Ժͷ�����Ϣ�Ƿ�ͬʱ����
	��������һ����Ҫ��ȡ������Ϣ*/
    	HspaMatchRead(pszInfoBack);
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);

	pszInfoBack = strstr(pszInfoBack, "\r\n");
    	pszInfoBack = strstr(pszInfoBack, "+CGACT: ");
	cx_printf("%s,%d,%s(),pszInfoBack = %s\n", __FILE__,  __LINE__, __FUNCTION__, pszInfoBack);
    
	if ( (NULL != pszInfoBack) && (NULL != strstr(pszInfoBack, "\r\nOK\r\n")) )/*�жϷ�����Ϣ�Ƿ�ɹ�*/
	{
		lLen = strlen(pszInfoBack);
		stSn.ucResult = HSPA_OK;

/*
+CGACT: 1,0
OK
*/
		/* not support query cid */
		stSn.ucCid = 0;

		pcTmp = strstr(pszInfoBack, "\r\n");
		*pcTmp = '\0';
		pszInfoBack = strstr(pszInfoBack, ",");
        TTY_DEBUG("...........pszInfoBack is %s\n", pszInfoBack);
		
		stSn.ucState = atoi(pszInfoBack+1);

        TTY_DEBUG("...........ucCmdFrom %d ucResult %d ucCid %d ucState %d\n", stSn.ucCmdFrom, stSn.ucResult, stSn.ucCid, stSn.ucState);


	}	
	else
	{
		stSn.ucResult = HSPA_ERROR_UNKNOW;
	}	

	memcpy(g_stAtToOther.acText, (void*)&stSn, sizeof(stSn));
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    g_sndflg = 0;
	sem_post(&sem);
	return;

}

/*****************************************************************************
 �� �� ��  : VoiceEnableSet
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��8��15��
    ��    ��   : s48571
    �޸�����   : �����ɺ���
*****************************************************************************/
static void VoiceEnableSet(char *pszInfoBack)
{
	int lMsgId;

    HspaMatchRead(pszInfoBack);
	if ( NULL != strstr(pszInfoBack,  "\r\nOK\r\n") )
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_OK;
	}
	else
	{
		g_stAtToOther.acText[HSPA_RESULT_POSITION] = HSPA_ERROR_UNKNOW;
	}
	g_stAtToOther.lMsgType = VOICE_SET;
	g_stAtToOther.acText[HSPA_FROM_POSITION] = AT_MODULE;
	lMsgId = MsgIdCompute(g_stAtFromOther.acParam[HSPA_FROM_POSITION]);
	/*���ǿ���ģ���������Ϣ�����跴��*/
	if ( (-1 != lMsgId) && (g_lAtMsgId!= lMsgId) )
	{
        TTY_DEBUG("lMsgId is %d\n", lMsgId);
		msgsnd(lMsgId,  (void*)&g_stAtToOther,AT_RCV_MSG_LENGTH, IPC_NOWAIT);
	}
    g_sndflg = 0;
	sem_post(&sem);
	return;

}


void TTY_WriteDebug(char *pcTmp)
{
    FILE *fs = NULL;

    if(NULL == pcTmp)
    {
        return;
    }
    
    if(NULL == (fs = fopen(TTYDEBUGPATH, "a+")))
    {
        fprintf(stdout, "%s", pcTmp);
    }
    else
    {
        fprintf(fs, "%s", pcTmp);
        fclose(fs);
    }
    return ;
}

int TTY_Output(const char* fmt,...)
{
    va_list argptr;
    char pcTmp[2048];
    int fd = 0;
    
    memset(pcTmp, 0, sizeof(pcTmp));
    if(!debug)
    {
        return 0;
    }
    
    va_start(argptr, fmt);
    vsprintf(pcTmp, fmt, argptr);
    va_end(argptr);
  
    if(OutputTerm == 2)
    {
        printf("%s",pcTmp);
        return 1;
    }
    else if(OutputTerm == 1)
    {        
        TTY_WriteDebug(pcTmp);
        return 1;
    }
    else
    {
        printf("%s",pcTmp);
        return 1;
    }

}

int debugswitch(void)
{
    FILE *fp = NULL;
    char acCmd[16] = "";
    struct stAtSndMsg stAtFromOther;

    if(NULL == (fp = fopen(TTYDEBUG, "r")))
    {
        return;
    }
    fgets(acCmd, 16, fp);
    fclose(fp);

    if (debug == atoi(acCmd))
    {
    		AtSend(g_lAtMsgId, &stAtFromOther, AT_MODULE, RSSI_QUERY, "AT+CSQ");
		return;
    }

    debug = atoi(acCmd);
    if(debug)
    {
        OutputTerm = debug;
        debug = 1;
        printf("\r\TTY debug switch turn on!\n");
        TTY_LOG( "g_lAtMsgId = %d\n", g_lAtMsgId);
        TTY_LOG( "g_lModemMsgId = %d\n", g_lModemMsgId);
        TTY_LOG( "g_lVoiceMsgId = %d\n", g_lVoiceMsgId);
        TTY_LOG( "g_lTr069MsgId = %d\n", g_lTr069MsgId);
        TTY_LOG( "g_lHttpMsgId = %d\n", g_lHttpMsgId);
    }
    else
    {
        printf("\r\TTY debug switch turn off!\n");
    }
}

static int RegisterNtwk(void)
{
    FILE* fs = NULL;
    unsigned long ulConType = 0;
    int lLen = 0;
    char acChannel[AT_SND_MSG_LENGTH];
    char acSysCfgCmd[AT_SND_MSG_LENGTH];
    char acOperator[COMMON_SHORT_STRING_LENGTH];
    unsigned long ulVoiceEnable = ENABLE_VOICE;//Ĭ��voice���ܿ�����

    memset(acChannel, 0, sizeof(acChannel));
    memset(acSysCfgCmd, 0, sizeof(acSysCfgCmd));
    memset(acOperator, 0,  sizeof(acOperator));

    /*��ȡ����ע����Ϣ*/
    fs = fopen(HSPA_SYSCFG_FILE, "r");

    if (!fs)
    {
        printf("FILE %s : open hspasys.cfg failed. \r\n", __FILE__);
        return -1;
    }

    fscanf(fs, HSPA_SYS_CFG_FORMAT_R, &ulConType, acChannel, &ulVoiceEnable, acOperator);

    fclose(fs);

    printf("[TTYUSB]ConType:%d bands:%s Operator:%s\n", ulConType, acChannel, acOperator);

    if (acChannel[0] == 0)
    {
        strcpy(acChannel, "3FFFFFFF");
    }
    memset(acSysCfgCmd, 0, sizeof(acSysCfgCmd));
    /*У��������Ϣ��ȷ��*/
    if (ulConType == ATCFG_CONNTYPE_GPRS_FIRST)
    {
        sprintf(acSysCfgCmd, "AT^SYSCFG=2,1,%s,2,4\r", acChannel);
    }
    else if (ulConType == ATCFG_CONNTYPE_3G_FIRST)
    {
        sprintf(acSysCfgCmd, "AT^SYSCFG=2,2,%s,2,4\r", acChannel);
    }
    else if (ulConType == ATCFG_CONNTYPE_GPRS_ONLY)
    {
        sprintf(acSysCfgCmd, "AT^SYSCFG=13,3,%s,2,4\r", acChannel);
    }
    else if (ulConType == ATCFG_CONNTYPE_3G_ONLY)
    {
        sprintf(acSysCfgCmd, "AT^SYSCFG=14,3,%s,2,4\r", acChannel);
    }
    else if (ulConType == ATCFG_CONNTYPE_AUTO)
    {
        sprintf(acSysCfgCmd, "AT^SYSCFG=2,0,%s,2,4\r", acChannel);
    }

    /* ע������ */
    if (g_iPcuiFd && (acSysCfgCmd[0] != 0))
    {
        printf("[TTYUSB]Register network:%s\n", acSysCfgCmd);
        lLen = write(g_iPcuiFd, acSysCfgCmd, strlen(acSysCfgCmd));
        usleep(100);
		lLen = read(g_iPcuiFd, acSysCfgCmd, COMMON_LONG_STRING_LENGTH);
    }

    /* add operator selection. <tanyin 2009.4.9> */
    if (*acOperator != 0 && strcmp(acOperator, "auto") != 0)
    {
        if (atoi(acOperator)) {
            strcpy(acSysCfgCmd,"AT+COPS=1,2,\""); /* MCC/MNC */
        } else {
            strcpy(acSysCfgCmd,"AT+COPS=1,1,\""); /* Short network name */
        }
        strcat(acSysCfgCmd,acOperator);
        strcat(acSysCfgCmd,"\"");
    }
    else /* auto search and register */
    {
        strcpy(acSysCfgCmd,"AT+COPS=0");       
    }
    /* ѡ����Ӫ�� */
    if (g_iPcuiFd)
    {
        printf("[TTYUSB]Select operator:%s\n", acSysCfgCmd);
        lLen = write(g_iPcuiFd, acSysCfgCmd, strlen(acSysCfgCmd));
        sleep(5);
		lLen = read(g_iPcuiFd, acSysCfgCmd, COMMON_LONG_STRING_LENGTH);
        printf("[TTYUSB]result:%s\n", acSysCfgCmd);
        return 0;
    }

    return -1;
}

/*****************************************************************************
 �� �� ��  : VoiceJudge
 ��������  : 
 �������  : char *pszInfoBack  
 �������  : ��
 �� �� ֵ  : static void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��8��15��
    ��    ��   : s48571
    �޸�����   : �����ɺ���
*****************************************************************************/
static int VoiceJudge(void)
{
    FILE* fs = NULL;
    unsigned long ulConType = 0;
    unsigned long ulVoiceEnable = ENABLE_VOICE;//Ĭ��voice���ܿ�����
    int lLen = 0;
    char acChannel[AT_SND_MSG_LENGTH];
    char acCmd[COMMON_LONG_STRING_LENGTH];
    char acOperator[COMMON_SHORT_STRING_LENGTH];

    memset(acChannel, 0, sizeof(acChannel));
    memset(acCmd, 0, sizeof(acCmd));

    /*��ȡ����ע����Ϣ*/
    fs = fopen(HSPA_SYSCFG_FILE, "r");

    if (!fs)
    {
        printf("FILE %s : open hspasys.cfg failed. \r\n", __FILE__);
        return -1;
    }

    fscanf(fs, HSPA_SYS_CFG_FORMAT_R, &ulConType, acChannel,&ulVoiceEnable,acOperator);

    fclose(fs);

    printf("\nTTYUSB:VoiceEnable[%d]\n", ulVoiceEnable);

    if( -1 == g_iPcuiFd)
    {
        printf("\nTTYUSB:PCUI devie fd error\n");
        return -1;
    }
    /*У��������Ϣ��ȷ��*/
    if ( ENABLE_VOICE == ulVoiceEnable )
    {
        lLen = write(g_iPcuiFd, "AT^CVOICE=0\r", strlen("AT^CVOICE=0\r"));
        usleep(100);
        lLen = read(g_iPcuiFd, acCmd, COMMON_LONG_STRING_LENGTH);
    }
    else if (DISABLE_VOICE == ulVoiceEnable )
    {
        lLen = write(g_iPcuiFd, "AT^CVOICE=1\r", strlen("AT^CVOICE=1\r"));
        usleep(100);
        lLen = read(g_iPcuiFd, acCmd, COMMON_LONG_STRING_LENGTH);
    }

    return 0;
}



