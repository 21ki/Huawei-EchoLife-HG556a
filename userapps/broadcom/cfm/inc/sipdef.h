
/******************************************************************************

                  ��Ȩ���� (C), 2006-2016, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : sipdef.h
  �� �� ��   : V1.0
  ��    ��   : shuxuegang
  ��������   : 2006��11��6��
  ����޸�   :
  ��������   : ����SIPҵ����ز������壬��Щ������web���ô����psi���ݿ⴦���н�
               ���õ�
  �����б�   :
  �޸���ʷ   :

******************************************************************************/
#ifndef _SIP_DEF_H_
#define _SIP_DEF_H_

#include "typedef.h"
#include "tapi_def.h"
#include "macro.h"

// #define DEBUG_SIP

/*SIPҵ�����������ù����ҵ��ģ��֮����й����õ���Ŀ¼��*/
// ����ǰ׺
#define SIP_CFGDIR_SIP_SERVER    "/var/sip/server"
#define SIP_CFGDIR_SIP_BASIC     "/var/sip/basic"
#define SIP_CFGDIR_SIP_ADVANCED  "/var/sip/advanced"
#define SIP_CFGDIR_PSTN_PREFIX   "/var/sip/pstncallprefix"
#define SIP_CFGDIR_SIP_QOS       "/var/sip/qos"
#define SIP_CFGDIR_PHONE_COMMON  "/var/sip/phonecommon"
#define SIP_CFGDIR_SPEED_DIAL    "/var/sip/speeddial"
#define SIP_CFGDIR_CALL_FORWARD  "/var/sip/callforward"
// �ļ���
#define SIP_CFG_FILE             "/var/sip"
#define SIP_STATUS_FILE          "/var/sipstatus"
/* BEGIN: Added by p00102297, 2008/3/11   ���ⵥ��:VDF C02*/
#define SIP_FXS_STATUS_FILE      "/var/fxsstatus"
/* END:   Modified by p00102297, 2008/3/11 VDF C02*/

#define SIP_TINY_LEN            16
#define SIP_SMALL_LEN           32
#define SIP_MEDIUM_LEN          40
#define SIP_LARGE_LEN           264
#define SIP_GIANT_LEN           1024
#define SIP_MAX_IFNAME_STR_LEN  17


// ʵ������server�����ó���Ϊ2������,0����,1�Ǳ�
#define SIP_SRV_MAX         2 
#define SIP_SRV_PRIMARY     0
#define SIP_SRV_SECONDARY   1

// ֧�������û�, 0-phone1, 1-phone2
#define SIP_USER_MAX        2 
#define SIP_FIRST_USER      0
#define SIP_SECOND_USER     1

/*BEGIN guwenbin 20080705 AU8D00774  QuickDialNumber ������40λ����*/
#define SIP_MAX_DAILNUM_LEN                    41    /*�û�dial�������󳤶�  */
/*END guwenbin 20080705 AU8D00774  QuickDialNumber ������40λ����*/

/* BEGIN:   Added by hKF20513, 2009/11/12 */
#define SIP_LOCAL_PORT_START                   1024   
#define SIP_LOCAL_PORT_END                       65535   
/* END:   Added by hKF20513, 2009/11/12 */

/* BEGIN:   Modify by hKF20513, 2009/11/12 */
#define SIP_MAX_NAME_LEN                   65//68    /*�û����Ƶ���󳤶�*/
#define SIP_MAX_NUM_LEN                    65//68    /*�û��������󳤶�  */
#define SIP_MAX_PSW_LEN                    65//68    /*�û��������󳤶�*/
/* END:   Modify by hKF20513, 2009/11/12 */

#define SIP_MAX_DIAL_PLAN                  10   /*���ٲ�����Ŀ������*/
#define SIP_MAX_CALL_FWD                   2     /*����ǰת�����*/
#define SIP_MAX_DIGIT_MAP                  1024
#define SIP_MIN_NOREPLY_TIME               20
#define SIP_MAX_NOREPLY_TIME               60
//s00125931 �淶����PSTN_MSX_PREFIX ��ΪPSTN_MAX_PREFIX_LEN
#define PSTN_MAX_PREFIX_LEN                    9    /*���pstn���׳���*/


// SIP server default
#define  DEFAULT_PROXY_ADDR_STRING      ""
#define  DEFAULT_PROXY_PORT             5060
#define  DEFAULT_REG_ADDR_STRING        ""
#define  DEFAULT_REG_PORT               5060
#define  DEFAULT_DOMAIN_NAME_STRING     ""

// SIP basic default
#define  DEFAULT_SIP_ID_STRING          ""
#define  DEFAULT_USR_NAME_STRING        ""
#define  DEFAULT_USR_PASSWD_STRING      ""
#define  DEFAULT_SIP_LOCAL_PORT         6050
// #define  DEFAULT_CALL_FW_TBLID          1
#define  DEFAULT_USR1_CALL_FW_TBLID          1
#define  DEFAULT_USR2_CALL_FW_TBLID          2
#define  DEFAULT_SIP_CLIP_ENABLE		1			

// SIP Advanced default
#define  DEFAULT_EXPIRATION_DURATION      3600
#define  DEFAULT_REREGISTER_TIMER         10
#define  DEFAULT_SESSION_EXPIRES          32000  //default value modified by chenyong for RFC4028 
#define  DEFAULT_MIN_SE                   90    //default value modified by chenyong for RFC4028
/* BEGIN: Modified by p00102297, 2008/1/29 */
#define  DEFAULT_MAXRETRY_TIME            32
#define  DEFAULT_PRIMARY_CHECK_TIME       300
/* END:   Modified by p00102297, 2008/1/29 */
/* BEGIN: Added by chenzhigang, 2008/11/5   ���ⵥ��:MWI_SUPPORT*/
#define  DEFAULT_MWI_EXPIRE               3600
/* END:   Added by chenzhigang, 2008/11/5 */
#define  DEFAULT_RTP_START_PORT           50000
#define  DEFAULT_RTP_END_PORT             65535
/*start of modification by chenyong 2009-03-05*/
#define  DEFAULT_RTCP_MONITOR             0  //default this function is disabled
/*start of modification by chenyong 2009-03-05*/

#define  DTMF_TYPE_PCM                    0
#define  DTMF_TYPE_RFC2833                1
#define  DTMF_TYPE_SIP_INFO               2
#define  DEFAULT_VOICE_DTMF_TYPE          DTMF_TYPE_PCM
//BEGIN:Added by zhourongfei to config the PT type via HTTP, telnet and TR-069
#define	 DEFAULT_VOICE_PAYLOAD_TYPE		  101
//END:Added by zhourongfei to config the PT type via HTTP, telnet and TR-069

#define  FAX_TYPE_G711A_PASSTHROUGH       0
#define  FAX_TYPE_G711U_PASSTHROUGH       1
#define  FAX_TYPE_T38                     2
#define  FAX_TYPE_PASSTHROUGH             FAX_TYPE_G711A_PASSTHROUGH
#define  DEFAULT_FAX_TYPE                 FAX_TYPE_PASSTHROUGH
#define  DEFAULT_SIP_INTERFACE_STRING     "br0"
#define  DEFAULT_SIP_PSTN_PRFFIX_STRING   "0000"
#define  SIP_TEST_DISABLED                0
#define  SIP_TEST_ENABLED                 1
#define  DEFAULT_SIP_TEST_ENBL_FLAG       SIP_TEST_DISABLED
#define  DEFAULT_SIP_DSCP_MARK            46         /* Added by s00125931, 2008/9/16   ���ⵥ��:vhg556*/
/* BEGIN: Modified by p00102297, 2008/1/17 */
#define PACKET_PERIOD_FIRST               10
#define PACKET_PERIOD_SECOND              20
#define PACKET_PERIOD_THIRD               30
#define DEFAULT_VOICE_PACKET_PERIOD       PACKET_PERIOD_SECOND
#define DEFAULT_VOICE_RTCPREPEAT_INTERVAL 3000
/* END:   Modified by p00102297, 2008/1/17 */

/* BEGIN: Modified by p00102297, 2008/2/21 */
/* ժ����Ĭ��ʱ��OffHookTime */
#define DEFAULT_VOICE_OFFHOOK_TIME        250
/* �һ���Ĭ��ʱ��OnHookTime */
#define DEFAULT_VOICE_ONHOOK_TIME         250
/* VAD��Ĭ������ֵ */
#define DEFAULT_SIP_VAD_VALUE             SIP_VAD_DISABLE
/* EC��Ĭ������ֵ */
#define DEFAULT_SIP_EC_VALUE              SIP_EC_ENABLE
/* CNG��Ĭ������ֵ*/
#define DEFAULT_SIP_CNG_VALUE             SIP_CNG_DISABLE
/* ��������LstVol��Ĭ������ֵ */
#define DEFAULT_VOICE_LISTEN_VOL          0
/* ˵������SpkVol��Ĭ������ֵ */
#define DEFAULT_VOICE_SPEAK_VOL           0
/* �����ȴ�ʱ���Ĭ������ֵ����λ:100ms */
#define DEFAULT_VOICE_DIAL_INTERVAL       50
/* ժ�����ŵȴ�ʱ���Ĭ������ֵ����λ:100ms */
#define DEFAULT_VOICE_FIRST_DIAL          150
/* ��С�Ĳ�ʱ���Ĭ������ֵ */
#define DEFAULT_VOICE_MINHOOK_FLASH       80
/* ����Ĳ�ʱ���Ĭ������ֵ */
#define DEFAULT_VOICE_MAXHOOK_FLASH       250
/* ��ͼ���õ�Ĭ��ֵ */
#define DEFAULT_VOICE_DIGITMAP            "[X*#ABCD].T"

//add by z67625 for automatic call
/*�Զ�����ʱ����*/
#define DEFAULT_VOICE_AUTOCALLINTERVAL    50
/*�Զ����ź���*/
#define DEFAULT_VOICE_AUTOCALLNUM         ""
#define DEFAULT_VOICE_AUTOCALLENBL        0

/*BEGIN: Added by chenyong 65116 2008-09-15 web����*/
/* reInjection Ĭ��ֵ */
#define DEFAULT_VOICE_REINJECTION         VOICE_REINJECTION_ENABLE
/*END: Added by chenyong 65116 2008-09-15 web����*/
/*start of additon by chenyong 2008-10-28 for Inner call*/
#define DEFAULT_VOICE_INNERCALL           INNER_CALL_ENABLE
/*end of additon by chenyong 2008-10-28 for Inner call*/
#define DEFAULT_VOICE_MODEMSERVICE        MODEM_SERVICE_ENABLE
/*BEGIN: Added by l00180792 @20130307 for Spain STICK_NOT_DETECTED_new*/
#define DEFAULT_VOICE_VOICEPROMPTVISIBLE  VOICEPROMPTVISIBLE_ENABLE
/*END: Added by l00180792 @20130307 for Spain STICK_NOT_DETECTED_new*/

/* �������Ĭ������ֵ */
/* BEGIN: Added by guwenbin, 2008/7/17 Spain���ⶨ��*/	
//#define DEFAULT_SIP_REGION_LOCAL          SIP_ITALY
#define DEFAULT_SIP_REGION_LOCAL          SIP_SPAIN
/* END:	   Added by guwenbin, 2008/7/17 */
/* ��·ʹ��ֵ��Ĭ������ֵ */
#define DEFAULT_SIP_LINE_ENABLE           SIP_LINE_ENABLE
/* ��·���еȴ���Ĭ������ֵ */
#define DEFAULT_SIP_LINE_CALLWAIT         SIP_LINE_CALLWAIT_ENABLE

/*start of addition by chenyong 2009-03-19*/
#define DEFAULT_SIP_LINE_CALLHOLD         SIP_LINE_CALLHOLD_ENABLE
/*end of addition by chenyong 2009-03-19*/

/*BEGIN: Added by chenyong 65116 2008-09-15 web����*/
/* ��·����ͨ����Ĭ������ֵ */
#define DEFAULT_SIP_LINE_3WAY             SIP_LINE_3WAY_ENABLE
/* ��·����ת�Ƶ�Ĭ������ֵ */
#define DEFAULT_SIP_LINE_CALLTRANSFER     SIP_LINE_CALLTRANSFER_ENABLE
/*END: Added by chenyong 65116 2008-09-15 web����*/
/* BEGIN: Added by chenzhigang, 2008/11/4   ���ⵥ��:MWI_SUPPORT*/
#define DEFAULT_SIP_LINE_MWI              SIP_LINE_MWI_ENABLE
#define DEFAULT_SIP_MWIACCOUNT_STRING     ""
/* END:   Added by chenzhigang, 2008/11/4 */
/* �����ⷽ���Ĭ��ֵ */
#define DEFAULT_SIP_FAX_DETECT            SIP_FAX_DETECT_DIRECTION_BOTH

/* END:   Modified by p00102297, 2008/2/21 */

// SIP QoS default
#define  DEFAULT_QOS_TOS                  0
#define  DEFAULT_QOS_VLAN_ENBL            0
#define  DEFAULT_QOS_VLANID               0
#define  DEFAULT_QOS_PRIORITY             3

/* BEGIN: Added by y67514, 2009/11/16   PN:voice provision*/
#define  DEFAULT_PROV_DISABLE                  0
/* END:   Added by y67514, 2009/11/16 */

// 1.SIP���������� - b. SIP����������
// a.SIP Server����,����Registar��Proxy��
typedef struct tagSIP_WEB_ADDR_S
{
    VOS_UINT16  uwPort;                     /* PORT in local Byte Order */
    VOS_CHAR    szIPAddr[SIP_MAX_NAME_LEN]; /* IP address or FQHN */
}SIP_WEB_ADDR_S;

// b. SIP����������
typedef struct tagSIP_WEB_SERVER_S
{
    SIP_WEB_ADDR_S  stSipRegAddr;                         /* ����registar  */
    SIP_WEB_ADDR_S  stSipProxyAddr;                       /* ����proxy  */
    VOS_CHAR        acDomainName[SIP_MAX_NAME_LEN];            /* �������, gvt�Խ�Ҫ��  */
}SIP_WEB_SERVER_S, *PSIP_WEB_SERVER_S;

/*���Ա任(���ӻ����ǰ׺��)�ĵ绰�����������*/
#define SIP_MAX_DAILNUM_CHANGE_NUM 	16

/*BEGIN guwenbin 20080705 AU8D00774  ��stAppandPrefixNum��Ӧ��
   SIP_MAX_MATCHNUM_SIZE��Ϊ64+1*/
 /*���ӻ���ٵ�ǰ��׺��������ַ�����*/
//#define SIP_MAX_ASIDENUM_SIZE		(16)
#define SIP_MAX_ASIDENUM_SIZE		(42+1)

/*��ƥ����������ַ�����*/
//#define SIP_MAX_MATCHNUM_SIZE		(32)
#define SIP_MAX_MATCHNUM_SIZE		(64+1)
/*END guwenbin 20080705 AU8D00774  */

typedef struct tagSIP_NUM_TRAN_S
{
    VOS_CHAR szAsideNum[SIP_MAX_ASIDENUM_SIZE]; /*����Ҫ�ӻ����ǰ��׺��*/
    VOS_CHAR szMatchNum[SIP_MAX_MATCHNUM_SIZE]; /*��ƥ��ĺ���*/
}SIP_NUM_TRAN_S;

/*���Ϊ������汾 zhoubaiyun 64446 for ITALY*/
#ifdef SUPPORT_CUSTOMER_ITALY
#if 0
#define SIP_APPAND_DIAL_NUM        16     /*֧�����������Ӹ���*/
#define SIP_STRIP_DIAL_NUM         16     /*֧����������ٸ���*/
#endif
/*BEGIN guwenbin 20080705 AU8D00774  ��SIP_MAX_NUM_LEN/4��ΪSIP_MAX_DAILNUM_LEN notice!!*/
// #define SIP_PRENUM_SIZE			   (SIP_MAX_NUM_LEN/4)
// #define SIP_MATCHNUM_SIZE		   (SIP_MAX_NUM_LEN*3/4+1)
#define SIP_PRENUM_SIZE			   (SIP_MAX_DAILNUM_LEN)
#define SIP_MATCHNUM_SIZE		   (SIP_MAX_MATCHNUM_SIZE)
/*END guwenbin 20080705 AU8D00774  QuickDialNumber ������40λ����*/
#if 0
typedef struct tagSIP_NUM_TRAN_S
{
    VOS_CHAR szPrefixNum[SIP_PRENUM_SIZE];     /*����Ҫ�ӻ��ǰ׺*/
    VOS_CHAR szMatchNum[SIP_MATCHNUM_SIZE];    /*��ƥ��ĺ���*/
}SIP_NUM_TRAN_S;
#endif
#endif

// 3. SIP Advanced - d.SIP�߼�����
// a.��������صĸ߼�����
typedef struct tagSIP_WEB_SVR_CFG_S
{
    /* BEGIN: Added by y67514, 2009/11/4   PN:��������tr069������*/
    VOS_UINT32  ulRegPeriod;
    /* END:   Added by y67514, 2009/11/4 */
    VOS_UINT32  ulExpiration;
    VOS_UINT32  ulReRegister;  
    VOS_UINT32  SessionExpires;
    VOS_UINT32  MinSE;
    /* BEGIN: Modified by p00102297, 2008/1/17 */ 
    VOS_UINT32  ulMaxRetryTime;          /*�μ�RFC3261 64T1�ش�ʱ��*/
    VOS_UINT32  ulPrimaryCheckTime;      /*���ڼ����������ʱ��*/
    /* END:   Modified by p00102297, 2008/1/17 */
    /* BEGIN: Added by chenzhigang, 2008/11/5   ���ⵥ��:MWI_SUPPORT*/
    VOS_UINT32 ulMWIExpire;
    /* END:   Added by chenzhigang, 2008/11/5 */
}SIP_WEB_SVR_CFG_S;

// b. RTP����,��ֹ�˿ں�
typedef struct tagSIP_WEB_RTP_CFG_S
{
    VOS_UINT32  ulStartPort;
    VOS_UINT32  ulEndPort;
    /* BEGIN: Modified by p00102297, 2008/1/18 */
    VOS_UINT32  ulTxRepeatInterval;
    /* END:   Modified by p00102297, 2008/1/18 */
    /*start of modification by chenyong 2009-03-05*/
	VOS_UINT32  ulRtcpMonitor;
	/*end of modification by chenyong 2009-03-05*/
}SIP_WEB_RTP_CFG_S;

typedef struct tagSIP_WEB_VOICE_CODEC_LIST_S
{
    VOS_UINT32  ulCodecCap;      /*codec����,��ϸ��CMD_PARA_MEDIA_PROTOCAL_E*/
	VOS_UINT32  ulPTime;         /*codec���ʱ��*/
	VOS_UINT32  ulPriority;      /*codec ���ȼ�,������SIP_CODEC_TYPE_MAX*/
	VOS_BOOL    bEnable;         /*codec ʹ��*/
}SIP_WEB_VOICE_CODEC_LIST_S;

/* BEGIN: Added by chenzhigang, 2007/12/28   ���ⵥ��:VDF SDP������ϸ���*/
// c. ��ѡ�����˳��
typedef struct tagSIP_WEB_VOICE_CFG_S
{
    SIP_WEB_VOICE_CODEC_LIST_S stCodecList[SIP_MAX_VOIP_CODE_TYPE];
    VOS_UINT32  ulDtmfType;      /*DTMF���䷽ʽ*/
	VOS_UINT32	ulPayloadType;	/*add by zhourongfei to config the PT type via HTTP, telnet and TR-069*/
}SIP_WEB_VOICE_CFG_S;
/* END:   Added by chenzhigang, 2007/12/28 */

// d.SIP�߼�����
typedef struct tagSIP_WEB_ADV_CFG_S
{
    SIP_WEB_SVR_CFG_S    stServerCfg;
    SIP_WEB_RTP_CFG_S    stRTPCfg;
//    SIP_WEB_VOICE_CFG_S  stVoiceCfg;
//    VOS_UINT32           ulFaxType;
    VOS_CHAR             szIfName[SIP_MAX_IFNAME_STR_LEN];          /* Interface name */
    VOS_UINT32           ulSipLocalPort;
//s00125931 �淶����PSTN_MSX_PREFIX ��ΪPSTN_MAX_PREFIX_LEN
    VOS_CHAR             szPstnPrefix[PSTN_MAX_PREFIX_LEN];          /* Interface name */    
    VOS_UINT32           ulEnblSipTest;
    VOS_UINT32           ulDscp;                                 /*Added by s00125931, 2008/9/10   ���ⵥ��:vhg556*/
}SIP_WEB_ADV_CFG_S, *PSIP_WEB_ADV_CFG_S;

// 4. SIP QOS
typedef struct tagSIP_WEB_QOS_CFG_S
{
    VOS_UINT32    ulTos;
    VOS_UINT32    ulEnblVlan;
    VOS_UINT32    ulVlan;
    VOS_UINT32    ulPriority;
}SIP_WEB_QOS_CFG_S, *PSIP_WEB_QOS_CFG_S;

/* BEGIN: Added by y67514, 2009/11/16   PN:voice provision*/
// 5. SIP provisioned
typedef struct tagSIP_WEB_PROV_CFG_S
{
    VOS_UINT16   usCliStatusProv; 
    VOS_UINT16   usLineCallHoldEnableProv;
    VOS_UINT16   usLineCWEnableProv;
    VOS_UINT16   usLine3WayEnableProv;  
    VOS_UINT16   usLineCallTransferEnableProv;
    VOS_UINT16   usLineMWIEnableProv;
    VOS_UINT16   usLineAutoCallEnableProv;
}SIP_WEB_PROV_CFG_S, *PSIP_WEB_PROV_CFG_S;
/* END:   Added by y67514, 2009/11/16 */

#define SIP_MAX_DIGIT_MAP                  1024
//Phoneҳ�棺
typedef struct tagSIP_WEB_PHONE_CFG_S
{
    VOS_INT32     lSpkVol;
    VOS_INT32     lLstVol;
    VOS_UINT32    ulDialInterval;
    //VOS_UINT32    ulVad;
    VOS_UINT32    ulRegion;
    VOS_UINT32    minHookFlash;    /* �Ĳ����� */
    VOS_UINT32    maxHookFlash;    /* �Ĳ����� */
    VOS_UINT8     szDigitMap[SIP_MAX_DIGIT_MAP];
    /* BEGIN: Modified by p00102297, 2008/1/17 */
    //VOS_UINT32    ulEC;
    //VOS_UINT32    ulCNG;
    VOS_UINT32    ulOffHookTime;
    VOS_UINT32    ulOnHookTime;
    VOS_UINT32    ulFstDialTime;
    /* END:   Modified by p00102297, 2008/1/17 */
	/*BEGIN: Added by chenyong 65116 2008-09-15 web����*/
	VOS_UINT32    ulReInjection;     /*��ϸ��VOICE_REINJECTION_E , Added by s00125931, 2008/9/10   ���ⵥ��:vhg556*/
	/*END: Added by chenyong 65116 2008-09-15 web����*/
	/*start of additon by chenyong 2008-10-28 for Inner call*/
	VOS_UINT32    ulInnerCall;      /*Ĭ��ΪINNER_CALL_ENABLE,ͨ�������ļ�����*/
	/*end of additon by chenyong 2008-10-28 for Inner call*/
	VOS_UINT32    ulModemService;   /*Ĭ��ΪMODEM_SERVICE_ENABLE,ͨ�������ļ�����*/
    /*BEGIN: Added by l00180792 @20130307 for Spain STICK_NOT_DETECTED_new*/
    VOS_BOOL      bVoicePromptVisible;
    /*END: Added by l00180792 @20130307 for Spain STICK_NOT_DETECTED_new*/
    //add by z67625 for automatic call
    VOS_INT32     lAutoEnbl;        /*�Զ����ſ���*/
    VOS_UINT32    ulAutoInterval;   /*�Զ�����ʱ��*/    
    VOS_CHAR      acAutoDialNum[SIP_SMALL_LEN]; /*�Զ����ź���*/
}SIP_WEB_PHONE_CFG_S;

// 2. SIP�û���Ϣ - SIP Basic
typedef struct tagSIP_WEB_USER_INFO_S
{
    /* BEGIN: Modified by p00102297, 2008/2/19 */
    SIP_WEB_VOICE_CFG_S  stVoiceCfg;
    VOS_UINT32    ulVad;
    VOS_UINT32    ulEC;
    VOS_UINT32    ulCNG;
    VOS_UINT32    ulFaxType;
    /* END:   Modified by p00102297, 2008/2/19 */
    VOS_CHAR    szId[SIP_MAX_NUM_LEN];      /* �û���ʶ */
    VOS_CHAR    szName[SIP_MAX_NAME_LEN];   /* �û����� */
    VOS_CHAR    szPsw[SIP_MAX_PSW_LEN];     /* ע����Կ */
    VOS_UINT16  uwPort;			            /* PORT in local Byte Order */
    VOS_UINT32  ulFwdTbl;                   /* ����ǰת��id:1 or 2 */

/*���Ϊ������汾 zhoubaiyun 64446 for ITALY*/
#ifdef SUPPORT_CUSTOMER_ITALY
    VOS_UINT16   usCliStatus;              /* TRUE:�������к���;FALSE:�������к��� */
    /* BEGIN: Added by lihua 68693, 2007/10/20   ���ⵥ��:A36D03793*/
    //VOS_UINT16   usCliPrivacyType;         /* ���к�������ʱ�����ط�ʽ */
    /* END:   Added by lihua 68693, 2007/10/20 */
    /* BEGIN: Modified by p00102297, 2008/2/3 */
    VOS_UINT16   usLineEnable;
    /* END:   Modified by p00102297, 2008/2/3 */
	/*start of addition by chenyong 2009-03-19*/
    VOS_UINT16   usLineCallHoldEnable;
	/*end of addition by chenyong 2009-03-19*/
    /* BEGIN: Modified by p00102297, 2008/3/14   ���ⵥ��:VDF C02*/
    VOS_UINT16   usLineCWEnable;
    /* END:   Modified by p00102297, 2008/3/14 */
	/*BEGIN: Added by chenyong 65116 2008-09-15 web����*/
    VOS_UINT16   usLine3WayEnable;              /*��������ͨ������,��SIP_LINE_3WAY_E*/
    VOS_UINT16   usLineCallTransferEnable;      /*����ת�ƹ���,    ��SIP_LINE_CALLTRANSFER_E*/
	/*END: Added by chenyong 65116 2008-09-15 web����*/
    /* BEGIN: Added by chenzhigang, 2008/11/4   ���ⵥ��:MWI_SUPPORT*/
    VOS_UINT16   usLineMWIEnable;                  /*����MWI,��SIP_LINE_MWI_ENABLE*/
    VOS_CHAR     szLineMWIAccount[SIP_MAX_NUM_LEN];
    /* END:   Added by chenzhigang, 2008/11/4 */

    /* BEGIN: Modified by p00102297, 2008/4/7   ���ⵥ��:VDF C02*/
    VOS_UINT16   usLineFaxDetect;
    /* END:   Modified by p00102297, 2008/4/7 */
    
    SIP_NUM_TRAN_S		   stAppandPrefixNum[SIP_MAX_DAILNUM_CHANGE_NUM];	/*����ǰ׺*/
	SIP_NUM_TRAN_S		   stStripPrefixNum[SIP_MAX_DAILNUM_CHANGE_NUM];	/*����ǰ׺*/
	SIP_NUM_TRAN_S		   stAppandSuffixNum[SIP_MAX_DAILNUM_CHANGE_NUM];	/*���Ӻ�׺*/
	SIP_NUM_TRAN_S		   stStripSuffixNum[SIP_MAX_DAILNUM_CHANGE_NUM];	/*���ٺ�׺*/
#if 0
    SIP_NUM_TRAN_S         stAppNum[SIP_APPAND_DIAL_NUM];       /*ƥ�����Ӻ���ļ�¼*/
    SIP_NUM_TRAN_S         stStripNum[SIP_STRIP_DIAL_NUM]; 	    /*ƥ����ٺ���ļ�¼*/
#endif
#endif  
}SIP_WEB_USER_INFO_S, *PSIP_WEB_USER_INFO_S;


//Phone Regionҳ�棺
typedef enum tagSIP_PHONE_REGION_E
{
    SIP_BELGIUM = 0,
    SIP_BRAZIL,
    SIP_CHILE,
    SIP_CHINA,
    SIP_DENMARK,
    SIP_ETSI,           // 5
    SIP_FINLAND,
    SIP_FRANCE,
    SIP_GERMANY,
    SIP_HUNGARY,
    SIP_ITALY,          // 10
    SIP_JAPAN,
    SIP_NETHERLANDS,
    SIP_NORTH_AMERICA,
    SIP_SPAIN,
    SIP_SWEDEN,         // 15
    SIP_SWITZERLAND,
    SIP_TR57,
    SIP_UK,              // 18
    SIP_SINGAPORE		//19
}SIP_WEB_PHONE_REGION_E;


//Speed Dial����
typedef struct tagSIP_WEB_DIAL_PLAN_S
{
/*BEGIN guwenbin 20080705 AU8D00774  ��SIP_MAX_NUM_LEN/4��ΪSIP_MAX_DAILNUM_LEN*/
   //VOS_UINT8 szDialNum[SIP_MAX_NUM_LEN/4];
   VOS_UINT8 szDialNum[SIP_MAX_DAILNUM_LEN];
/*END guwenbin 20080705 AU8D00774  QuickDialNumber ������40λ����*/
    VOS_UINT8 szName[SIP_MAX_NUM_LEN];
    VOS_UINT8 szRealNum[SIP_MAX_NUM_LEN];
}SIP_WEB_DIAL_PLAN_S;

//����ǰת��
typedef enum tagSIP_WEB_FWD_TYPE_E
{
    SIP_WEB_UNCON_FWD,
    SIP_WEB_NOREP_FWD,
    SIP_WEB_ONBSY_FWD,
    SIP_WEB_BUUTT_FWD
}SIP_WEB_FWD_TYPE_E;


/*Privacy's Tokens*/
typedef enum _EN_SIP_PRIVACY_HDR_TOKEN_ID
{
    SIP_PRIVACY_TOKEN_HEADER = 1,
    SIP_PRIVACY_TOKEN_USER,
    SIP_PRIVACY_TOKEN_SESSION,
    SIP_PRIVACY_TOKEN_NONE,
    SIP_PRIVACY_TOKEN_CRITICAL,
    SIP_PRIVACY_TOKEN_ID,
    SIP_PRIVACY_TOKEN_OTHER,
    SIP_PRIVACY_TOKEN_BUTT

}EN_SIP_PRIVACY_HDR_TOKEN_ID;

/* BEGIN: Modified by p00102297, 2008/2/3 */
typedef enum tag_SIP_LINE_ENABLE_EN
{
    SIP_LINE_ENABLE    = 0,
    SIP_LINE_DISABLE   = 1,
    SIP_LINE_QUIESCENT = 2
}EN_SIP_LINE_ENABLE_ST;

typedef enum tag_SIP_LINECLIP_EN
{
    SIP_LINECLIP_DISABLE = 0,
    SIP_LINECLIP_ENABLE  = 1
}SIP_LINECLIP_EN;

typedef enum tag_SIP_VAD_EN
{
    SIP_VAD_DISABLE = 0,
    SIP_VAD_ENABLE  = 1
}SIP_VAD_EN;

typedef enum tag_SIP_EC_EN
{
    SIP_EC_DISABLE = 0,
    SIP_EC_ENABLE  = 1
}SIP_EC_EN;

typedef enum tag_SIP_CNG_EN
{
    SIP_CNG_DISABLE = 0,
    SIP_CNG_ENABLE  = 1
}SIP_CNG_EN;

typedef enum tag_SIP_PREFIX_TYPE_EN
{
    SIP_PREFIX_APPEND = 0,
    SIP_PREFIX_STRIP  = 1,
    SIP_SUFFIX_APPEND = 2,
    SIP_SUFFIX_STRIP  = 3
}SIP_PREFIX_TYPE_EN;

typedef enum tag_SIP_LINE_CALLWAIT_E
{
    SIP_LINE_CALLWAIT_DISABLE = 0,
    SIP_LINE_CALLWAIT_ENABLE
}SIP_LINE_CWEN_E;
/* END:   Modified by p00102297, 2008/2/3 */

/*start of addition by chenyong 2009-03-19*/
typedef enum tag_SIP_LINE_CALLHOLD_E
{
    SIP_LINE_CALLHOLD_DISABLE = 0,
    SIP_LINE_CALLHOLD_ENABLE
}SIP_LINE_CALLHOLD_E;
/*end of addition by chenyong 2009-03-19*/

/* BEGIN: Added by chenzhigang, 2008/11/4   ���ⵥ��:MWI_SUPPORT*/
typedef enum tag_SIP_LINE_MWI_E
{
    SIP_LINE_MWI_DISABLE = 0,
    SIP_LINE_MWI_ENABLE
}SIP_LINE_MWI_E;
/* END:   Added by chenzhigang, 2008/11/4 */
/*BEGIN: Added by chenyong 65116 2008-09-15 web����*/
typedef enum tag_SIP_LINE_3WAY_E
{
    SIP_LINE_3WAY_DISABLE = 0,
    SIP_LINE_3WAY_ENABLE
}SIP_LINE_3WAY_E;

typedef enum tag_SIP_LINE_CALLTRANSFER_E
{
    SIP_LINE_CALLTRANSFER_DISABLE = 0,
    SIP_LINE_CALLTRANSFER_ENABLE
}SIP_LINE_CALLTRANSFER_E;

typedef enum tag_VOICE_REINJECTION_En
{
    VOICE_REINJECTION_DISABLE = 0,
    VOICE_REINJECTION_ENABLE,
    VOICE_REINJECTION_FORCE
}VOICE_REINJECTION_E;
/*END: Added by chenyong 65116 2008-09-15 web����*/

/* start: add by y00183561 for automatic call tr069 node 2011/10/12 */
typedef enum tag_AUTO_CALL_ENABLE_E
{
    AUTO_CALL_DISABLE = 0,
    AUTO_CALL_ENABLE
}AUTO_CALL_ENABLE_E;
/* end: add by y00183561 for automatic call tr069 node 2011/10/12 */

/* BEGIN: Modified by p00102297, 2008/3/13   ���ⵥ��:VDF C02*/
typedef enum tag_Voice_FXS_Status
{
    VOICE_FXS_STAT_START = -1,
    VOICE_FXS_STAT_ONHOOK = 0,
	VOICE_FXS_STAT_OFFHOOK,
	VOICE_FXS_STAT_BUTT
}VOICE_FXS_STATUS_E;

/* BEGIN: Added by chenzhigang, 2008/4/7   ���ⵥ��:���������з�����*/
typedef enum tag_SIP_FAX_DETECT_DIRECTION_TYPE_E
{
    SIP_FAX_DETECT_DIRECTION_DISABLE = 0,
    SIP_FAX_DETECT_DIRECTION_CALLEE,
    SIP_FAX_DETECT_DIRECTION_CALLER,
    SIP_FAX_DETECT_DIRECTION_BOTH,
    SIP_FAX_DETECT_DIRECTION_BUTT
}SIP_FAX_DETECT_DIRECTION_TYPE;
/* END:   Added by chenzhigang, 2008/4/7 */

/* END:   Modified by p00102297, 2008/3/13 */

/*   T A G   S   I   P _   A   D   V   A   N   C   E   D _   F   W   D   */
/*-------------------------------------------------------------------------
    ����ǰת���ýṹ
-------------------------------------------------------------------------*/
typedef struct tagSIP_WEB_FWD_NUM_S
{
    VOS_BOOL       bIsActive;
    VOS_UINT8      szIncoming[SIP_MAX_NUM_LEN];
    VOS_UINT8      szForward[SIP_MAX_NUM_LEN];
    SIP_WEB_FWD_TYPE_E enFwdType; 
}SIP_WEB_FWD_NUM_S;

#define SIP_WEB_FWD_ADVANCED_NUM  10
#define SIP_WEB_FWD_BASIC_NUM      3
/*   T A G   S   I   P _   F   W   D _   N   U   M _   S   */
/*-------------------------------------------------------------------------
    ����ǰת��
-------------------------------------------------------------------------*/
typedef struct tagSIP_WEB_FWD_S
{
    SIP_WEB_FWD_NUM_S  pstAdvance[SIP_WEB_FWD_ADVANCED_NUM];    /*10��*/
    SIP_WEB_FWD_NUM_S  pstBasic[SIP_WEB_FWD_BASIC_NUM];         /*3��*/
    VOS_UINT32         ulNoReplyTime;
}SIP_WEB_FWD_S;

#endif
