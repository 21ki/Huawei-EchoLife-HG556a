/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : tapi_def.h
  �� �� ��   : V1.21(�ð汾�ź�TAPI-Interworking document.doc�汾�ű�����һ��)
  ��    ��   : lilei 22608
  ��������   : 2004��9��21��
  ����޸�   :
  ��������   : TAPI����ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2004��9��21��
    ��    ��   : lilei 22608
    �޸�����   : ����
  2.��    ��   : 2004��10��27��
    ��    ��   : lilei 22608
    �޸�����   : �޸����ɶ���


******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifndef TAPI_DEF_H
#define  TAPI_DEF_H
#include "typedef.h"

#define SERVICE_MANAGE_TONE_BASE 61

typedef enum tagTONE_TYPE_E
{
    TONE_TYPE_BUSY_TONE = 1,                /*����1:æ��*/
    TONE_TYPE_HOWL_TONE,                    /*����2:������*/
    TONE_TYPE_DIAL_TONE,                    /*����3:������*/
    TONE_TYPE_RING_BACK_TONE,               /*����4:������*/
    TONE_TYPE_DUAL_DIAL_TONE,               /*����5:���β�����*/
    TONE_TYPE_SPECIAL_DIAL_TONE,            /*����6:���Ⲧ����*/
    TONE_TYPE_SHORT_BUSY_TONE,              /*����7:��æ��*/
    TONE_TYPE_CW_TONE,                      /*����8:����������*/
    TONE_TYPE_MUTE_TONE,                    /*����9:����*/
    TONE_TYPE_CAS,                          /*����10:CASͨ�������к�����ʾ*/
    /* Start of ouyangyueling+29638 2005-03-23 TS0001 V300R001B01D030 FNRD04333 */
    TONE_TYPE_SAS,                          /*����11:SAS*/
    /* End of ouyangyueling+29638 2005-03-23 TS0001 V300R001B01D030 FNRD04333 */
    TONE_TYPE_VOICEMAIL_ON_TONE,            /*����12:���*/
    TONE_TYPE_VOICEMAIL_OFF_TONE,           /*����13:Ϩ��*/
    /* Start of ouyangyueling+29638 2005-03-23 TS0001 V300R001B01D030 FNRD04333 */
    TONE_TYPE_CONGESTION_TONE,              /*����14:������ cg*/
    TONE_TYPE_CONFIRM_TONE,                 /*����15:ȷ����*/
    /* End of ouyangyueling+29638 2005-03-23 TS0001 V300R001B01D030 FNRD04333 */
    TONE_TYPE_RT_ALWAYS_TONE,               /*����16:����������*/
    TONE_TYPE_USER_DEFINE_TONE1,   /*����17:Ӧ�ò㶨�����������֣��ļ�1*/
    TONE_TYPE_USER_DEFINE_TONE2,   /*����18:Ӧ�ò㶨�����������֣��ļ�2*/
    TONE_TYPE_USER_DEFINE_TONE3,   /*����19:Ӧ�ò㶨�����������֣��ļ�3*/
    TONE_TYPE_USER_DEFINE_TONE4,   /*����20:Ӧ�ò㶨�����������֣��ļ�4*/
    TONE_TYPE_USER_DEFINE_TONE5,   /*����21:Ӧ�ò㶨�����������֣��ļ�5*/
    /* Start of ouyangyueling+29638 2005-01-17 IAD2000V300R001B01D010 FNRD03870 */
    TONE_TYPE_CW_CURRENT_TONE,              /*����22:ͨ������������*/
    TONE_TYPE_EPHONE_RING_TONE,             /*����23:EPHONE�е�����ӿ�*/
    /* End of ouyangyueling+29638 2005-01-17 IAD2000V300R001B01D010 FNRD03870 */
    /* Start of ouyangyueling+29638 2005-05-30 TS0001 V300R001B01D070 A41D00415 */
    TONE_TYPE_ALARM_TONE,                   /*����24:���ӽӿ�*/
    TONE_TYPE_ZERO_CONFIG_SHORT_ONE_TONE,   /*����25:0���ö̡��֡����ӿ�*/
    TONE_TYPE_ZERO_CONFIG_LONG_ONE_TONE,    /*����26:0���ó����֡����ӿ�*/
    TONE_TYPE_ZERO_CONFIG_THREE_TONE,       /*����27:0�����������֡����ӿ�*/  
    /* End of ouyangyueling+29638 2005-05-30 TS0001 V300R001B01D070 A41D00415 */
	/* BEGIN: Added by guwenbin, 2008/7/10	 Spain���ⶨ��*/
		TONE_TYPE_NORMAL_ALARM_TONE,			/*alarm tone*/
		TONE_TYPE_NORMAL_ACKNOWLEDGEMENT_TONE,	/*Positive Acknowledgement	tone*/
	/* END:   Added by guwenbin, 2008/7/10 */  
/* BEGIN: Added by chenzhigang, 2008/3/13   ���ⵥ��:CWʹ�ܹر����ն�������*/
    TONE_TYPE_ENABLE_CW_TONE = SERVICE_MANAGE_TONE_BASE,  /*�ļ���*/
    TONE_TYPE_DISABLE_CW_TONE,
    TONE_TYPE_CW_STATE_ENABLE_TONE,
    TONE_TYPE_CW_STATE_DISABLE_TONE,
/* END:   Added by chenzhigang, 2008/3/13 */    
    TONE_TYPE_TONE_BUTT                    /*�������ֵ*/
}TONE_TYPE_E;

/******************************************************************************************
TAPI_PortOperate��ض���
���·���������ΪPORT_CMD_DOWN_SET_TALKʱ��
�·�����pParam����ΪSET_TALK_PARA_S *����

���·���������ΪPORT_CMD_DOWN_KCʱ��
�·�����pParam����ΪSET_DOWN_KC_S *����

******************************************************************************************/
typedef struct tagSET_TALK_PARA_S
{
    VOS_UINT32   ulDspChannelID;
}SET_TALK_PARA_S;

/* Start of ouyangyueling+29638 2005-03-15 TS0001 V300R001B01D030 FNRD04333 */
typedef struct tagSET_DOWN_KC_S
{
    VOS_UINT32  ulSendCounts;                               /*����*/
    VOS_UINT32  ulBreadth;                                  /*��ֵ*/
}SET_DOWN_KC_S;
/* End of ouyangyueling+29638 2005-03-15 TS0001 V300R001B01D030 FNRD04333 */

typedef enum  tagPORT_COMMAND_DOWN_E
{
    PORT_CMD_DOWN_SET_TALK,                      /*��ͨ������С��*/
    PORT_CMD_DOWN_REL_TALK,                      /*�ͷ�ͨ������С��*/
    /*START ADD:liujianfeng 37298 2005-04-10 for IAD104I*/
    PORT_CMD_DOWN_B_CH_CONN_REQ = PORT_CMD_DOWN_SET_TALK, 		    /*Bͨ����������L2->L1*/ 
    PORT_CMD_DOWN_B_CH_DISCONN_REQ= PORT_CMD_DOWN_REL_TALK,         /*Bͨ����������L2->L1*/ 
    PORT_CMD_DOWN_ACTIVATE_REQ,				    /*�˿ڼ�������L2->L1*/
    PORT_CMD_DOWN_DEACTIVATE_REQ,           	/*�˿�ȥ��������L2->L1*/
    PORT_CMD_DOWN_DATA_TX_REQ,				    /*Dͨ�����ݽ�������L2->L1*/
    PORT_CMD_DOWN_DATA_RX_RST_REQ,              /*��λͨ������*/
   /*END ADD:liujianfeng 37298 2005-04-10 for IAD104I*/
    PORT_CMD_DOWN_START_RING,                   /*����*/
    PORT_CMD_DOWN_RING_SPLASH,                  /*������*/
    PORT_CMD_DOWN_RING_SEND_TELNUM,             /*�����ͺ�,ΪAGԤ��*/
    PORT_CMD_DOWN_INIT_RING,                    /*��ʼ����*/
    PORT_CMD_DOWN_SEND_TELNUM,                  /*�ͺ�,ΪAGԤ��*/
    PORT_CMD_DOWN_STOP_RING,                    /*ͣ����*/
    PORT_CMD_DOWN_START_SERVICE,                /*����ҵ��,ΪAGԤ��*/ 
    PORT_CMD_DOWN_END_SERVICE,                  /*��ֹҵ��,ΪAGԤ��*/
    PORT_CMD_DOWN_LOCK,                         /*�˿�����*/
    PORT_CMD_DOWN_UNLOCK,                       /*�˿ڽ�����*/
    PORT_CMD_DOWN_ENABLE_ONHOOK_TX,             /*�һ�����ʹ��*/
    PORT_CMD_DOWN_REVERSE,                      /*����*/
    /* Start of ouyangyueling+29638 2005-03-15 TS0001 V300R001B01D030 FNRD04333 */
    PORT_CMD_DOWN_KC,                           /*12KC��16KC�Ʒ���Ϣ*/    
    /* End of ouyangyueling+29638 2005-03-15 TS0001 V300R001B01D030 FNRD04333 */
    PORT_CMD_DOWN_SET_TALK_REVERSE,             /*��ͨ�����ҷ���*/
    PORT_CMD_DOWN_SEND_AOCE,                    /*��������*/
    PORT_CMD_DOWN_CDI_CALL_OFFHOOK,             /*����ժ�� CDI*/
    PORT_CMD_DOWN_CDI_DIAL_NUM,                 /*���в��� CDI*/
    PORT_CMD_DOWN_CDI_CALL_ONHOOK,              /*���йһ� CDI*/
    PORT_CMD_DOWN_PORT_RESET,                   /* ��λָ���˿�����*/
    PORT_CMD_DOWN_SEND_TTX_PULSE,               /* �ͷ����������� */
    PORT_CMD_DOWN_DISC_FEEDBACK                 /*����ҵ�����������*/
}PORT_COMMAND_DOWN_E;
/******************************************************************************************/



/******************************************************************************************
TAPI_SendCallerID��ض���
���·���������ΪPORT_CMD_DOWN_SET_TALKʱ��
�·�����pParam����ΪSET_TALK_PARA_S *����
******************************************************************************************/
#define LENGTH_OF_DATE_BUF          5
#define LENGTH_OF_TIME_BUF          5
#define LENGTH_OF_CALLING_NUMBER    32
#define LENGTH_OF_CALLER_NAME       64
#define LENGTH_OF_DTMF_PADDING      3
typedef struct tagCALLER_ID_REPORT_S
{
    VOS_UINT8   aucDate[LENGTH_OF_DATE_BUF];                    /* The caller ID date field (ASCII - MMDD'\0') */
    VOS_UINT8   aucTime[LENGTH_OF_TIME_BUF];                    /* The caller ID time field (ASCII - HHMM'\0') */
    VOS_UINT8   aucNumber[LENGTH_OF_CALLING_NUMBER];            /* The calling directory number ,��'\0'Ϊ���� */
    VOS_UINT8   aucName[LENGTH_OF_CALLER_NAME];                 /* The caller ID name */
    VOS_UINT8   ucStatus;                                                     /* Caller ID message status��ֵ������δȷ�� */
    VOS_UINT8   ucNumberAvailability;                           /* enum CALLER_ID_AVAIL_REASON_E */
    VOS_UINT8   ucNameAvailability;                             /* enum CALLER_ID_AVAIL_REASON_E */
    VOS_UINT8   ucHookStat;                                                 /* CID��Ҫ�����Ƿ�ժ��̬,���趨��־��
                                                                                             �ŵĸ���,�����enum CALLERID_HOOK_STAT_E*/
}CALLER_ID_REPORT_S;

/*��ʶ�Ƿ������ʾ�������������������������IAD�Ĵ��붨�����*/
typedef enum tagCALLER_ID_AVAIL_REASON_E
{
     CALLERID_AVAIL     =   0,       /*������ʾ*/
     CALLERID_UNAVAIL   =   'O',     /*���ܻ�����к��룬������ydn069.pdf�涨*/
     CALLERID_PRIVATE   =   'P'      /*��������ʾ(����Ͳ��ܻ�����к�����ͬ)*/          
}CALLER_ID_AVAIL_REASON_E;

typedef enum tagCALLERID_HOOK_STAT_E
{
     CALLERID_ONHOOK,       /*�һ�̬�µ����к�����ʾ*/
     CALLERID_OFFHOOK,      /*ͨ��̬�µ����к�����ʾ*/
     CALLERID_BUTT
}CALLERID_HOOK_STAT_E;
/******************************************************************************************/


/******************************************************************************************
TAPI_ChannelOpen��ض���
******************************************************************************************/
typedef enum tagWORK_MODE_DEFINE_E
{
    WORK_MODE_DTMF,         /*�պ�ģʽ�����б����־λ����ΪPARA_CHANGE_NO��0��*/
    WORK_MODE_VOICE,        /*����ģʽ*/
    WORK_MODE_FAX,          /*����ģʽ,����뷽ʽ��������ΪT.38��G.711 */
    WORK_MODE_MODEM,        /*modemģʽ������뷽ʽ��������ΪV.150��G.711*/
    WORK_MODE_BUTT
}WORK_MODE_DEFINE_E;
 

typedef struct tagDSP_CONFIG_PARA_S
{
    VOS_UINT32  ulSilence;                              /*����*/
    VOS_UINT32  ulEcho;                                 /*����*/
    /* BEGIN: Added by chenzhigang, 2008/1/25   ���ⵥ��:VDF SDP������ϸ���*/
    VOS_UINT32  ulComfortNoise;                         /*��������*/
    /* END:   Added by chenzhigang, 2008/1/25 */
    VOS_UINT32  ulDtmfTransferMode;                     /*DTMF����ģʽ*/
    VOS_UINT32  ulRemoteCode;                           /*Զ�˱���뷽ʽ*/
    VOS_UINT32  ulRemotePeriod;                         /*Զ�˴��ʱ�䳤�ȣ���λMS*/
    VOS_UINT32  ulRemotePlType;                         /*�Զ�PTֵ*/
    VOS_UINT32  ulLocalCode;                            /*���˱���뷽ʽ*/
    VOS_UINT32  ulLocalPlType;                          /*����PTֵ*/
    VOS_UINT32  ulLocalPeriod;                          /*���˴��ʱ�䳤�ȣ���λMS*/
    VOS_UINT32  ulMode;                                 /*����ģʽ*/
 /*Modify begin HG550V100R002C02B010 A36D03254 by Donald 2007-1-10*/
    VOS_UINT32  ulWorkMode;                             /*������ʽ WORK_MODE_DEFINE_E*/
 /*Modify end HG550V100R002C02B010 A36D03254 by Donald 2007-1-10*/
    VOS_INT32    lOutputGain;                           /* DSPͨ���������棬��λDB */
    VOS_INT32    lInputGain;                            /* DSPͨ���������棬��λDB */
}DSP_CONFIG_PARA_S;
/*ulSilence����д����*/
#define CHANNEL_PARA_SILENCE_DISABLE   0                    /*�رվ���*/
#define CHANNEL_PARA_SILENCE_ENABLE   1                     /*�򿪾���*/
/*ulEcho����д����*/
#define CHANNEL_PARA_EC_DISABLE   0                         /*�رջ�����ֹ*/
#define CHANNEL_PARA_EC_ENABLE    1                         /*�򿪻�����ֹ*/
/*ulDtmfTransferMode����д����*/
#define CHANNEL_PARA_DTMF_DIRECT_TRANS_MODE    0            /*͸��*/
#define CHANNEL_PARA_DTMF_2833_MODE                    1    /*2833*/
#define CHANNEL_PARA_DTMF_OUTBAND_MODE             2        /*����*/
/* BEGIN: Modified by tKF19817, 2010/1/3   PN:AU8D03807 726Э��*/
//#define CMDPARM_MEDIA_PLTYPE_G726_32               103      /*G726-32 ����Ϊ103*/
#define CMDPARM_MEDIA_PLTYPE_G726_32               104      
/* END:   Modified by tKF19817, 2010/1/3 */

typedef enum tagCMD_PARA_MEDIA_PROTOCAL_E
{
    CMDPARM_MEDIA_PROTOCOL_G711U        = 0,
    CMDPARM_MEDIA_PROTOCOL_G7231        = 4,
    CMDPARM_MEDIA_PROTOCOL_G723         = CMDPARM_MEDIA_PROTOCOL_G7231,
    CMDPARM_MEDIA_PROTOCOL_G711A        = 8,
    CMDPARM_MEDIA_PROTOCOL_G722,
    /* BEGIN: Modified by tKF19817, 2010/1/3   PN:AU8D03807 726Э��*/
   // CMDPARM_MEDIA_PROTOCOL_G726_32      = 103,
    CMDPARM_MEDIA_PROTOCOL_G726_16      = 102,
    CMDPARM_MEDIA_PROTOCOL_G726_24      = 103,
    CMDPARM_MEDIA_PROTOCOL_G726_32      = 104,
  /*  CMDPARM_MEDIA_PROTOCOL_G726         = 2,*/ /* Ĭ�ϵ�G726����32λ */
    CMDPARM_MEDIA_PROTOCOL_G726_40      = 105,
    /* END:   Modified by tKF19817, 2010/1/3 */    
    CMDPARM_MEDIA_PROTOCOL_G728         = 15,
    CMDPARM_MEDIA_PROTOCOL_G729         = 18,
    CMDPARM_MEDIA_PROTOCOL_G729A        = CMDPARM_MEDIA_PROTOCOL_G729,
    CMDPARM_MEDIA_PROTOCOL_G723LOW      = 20,       /*�Ǳ�׼����*/
    CMDPARM_MEDIA_PROTOCOL_R2198        = 96,
    CMDPARM_MEDIA_PROTOCOL_R2833        = 97,
    CMDPARM_MEDIA_PROTOCOL_T38          = 98,       /*����Э�飬CSMV/6ʹ��98*/
    CMDPARM_MEDIA_PROTOCOL_V150,
    CMDPARM_MEDIA_PROTOCOL_T38_ADD_2    = 256,      /*T.38+2����*/
    CMDPARM_MEDIA_PROTOCOL_BUTT
}CMD_PARA_MEDIA_PROTOCAL_E;

 /* ��ͨ������ģʽ */
typedef enum tagCHANNEL_STREAM_MODE_E
{
    MODE_INACTIVE   = 0,        /* InActive */
    MODE_SENDONLY   = 1,        /* SendOnly */
    MODE_RECVONLY   = 2,        /* ReceiveOnly */
    MODE_SENDRECV   = 3,        /* SendReceive */
    MODE_LOOPBACK   = 4,        /* LoopBack */
    MODE_CONFRNCE,     
    MODE_CONTTEST,     
    MODE_NETWLOOP,     
    MODE_NETWTEST,     
    MODE_DATA,     
    MODE_BUTT
}CHANNEL_STREAM_MODE_E;

/*2833���ܵ������Կ����*/
#define MAX_2833_KEY_LEN 256

typedef struct tagRFC2833_CONFIG_PARA_S
{
    VOS_UINT32 ulPtFor2833;                         /*2833��PTֵ��������Ϊ96����������Ϊ97*/
    VOS_UINT32 ulReSendCount;                       /*�ط�����*/
    VOS_UINT32 ulSupport2833;                       /*�Ƿ�֧��2833 ��1-֧�֣�0-��֧��*/
    VOS_UINT32 ulSupportEny;                        /*�Ƿ�֧�ּ��ܣ�1-֧�֣�0-��֧��*/
    VOS_UINT32 ul2833KeyLen;
    VOS_UCHAR uc2833Key[MAX_2833_KEY_LEN + 1];      /*2833��Կ,��֧�ֿ���������д*/
}RFC2833_CONFIG_PARA_S;

#define RFC2833_PARA_REDUN                96        /*����*/
#define RFC2833_PARA_UNREDUN            97          /*������*/

typedef struct tagRFC2198_CONFIG_PARA_S
{
    VOS_UINT32 ulSupport2198;                       /*�Ƿ�֧��2198*/
    VOS_UINT32 ulRFC2198RedCount;                   /*�������*/
    VOS_UINT32 ulBlockPtFor2198;                    /*����pt���ͣ�������Ϊ96����������Ϊ97*/
    VOS_UINT32 ulPayloadPtFor2198;                  /*�����pt���ͣ�������Ϊ96����������Ϊ97*/
}RFC2198_CONFIG_PARA_S;

#define RFC2198_PARA_UNSUPPORT          0           /*��֧��*/
#define RFC2198_PARA_SUPPORT              1         /*֧��*/

#define RFC2198_PARA_REDUN                   96     /*����*/
#define RFC2198_PARA_UNREDUN              97        /*������*/

typedef struct tagRTP_CONFIG_PARA_S
{
    VOS_UINT32  ulRtpRemoteIP;      /*Զ��IP*/
    VOS_UINT32  ulRtpRemotePort;    /*Զ�˶˿�*/
    VOS_UINT32  ulRtpLocalPort;     /*���˶˿�*/
    VOS_UINT32  ulJitterBuffer;     /* JitterBUffer����λMS */
}RTP_CONFIG_PARA_S;

typedef struct tagRTCP_CONFIG_PARA_S
{
    VOS_UINT32 ulRemoteRtcpPort;
    VOS_UINT32 ulLocalRtcpPort;
}RTCP_CONFIG_PARA_S;

typedef struct tagMODEM_CONFIG_PARA_S
{
    VOS_UINT32 ulModemTransferMode;   /*MODEM�Ĵ���ģʽ,��ʱ��ʹ��*/
}MODEM_CONFIG_PARA_S;

typedef struct tagFAX_CONFIG_PARA_S
{
    /*ѵ��ģʽ������������ʡ�ECM֧��*/
    VOS_UINT32 ulMaxRate;               /* ��������ٶ�, voipFaxRate_EN */
    VOS_UINT32 ulTrainMode;             /* ѵ����ʽ, 0-�˵���, 1-���� */
    VOS_UINT32 ulECM;                   /* ECM��ʽ��0-��֧��, 1-֧�� */  
}FAX_CONFIG_PARA_S;

typedef enum tagIPP_FAX_MAX_RATE_E
{
     FAX_RATE_NULL  = 0,
     NO_LIMIT	    = 1,         	/* û����������(ȱʡ) */
     FAX_RATE_2400	= 2,	        /* 2,400bps */
     FAX_RATE_4800	= 3,        	/* 4,800bps */
     FAX_RATE_7200	= 4,	        /* 7,200bps */
     FAX_RATE_9600	= 5,	        /* 9,600bps */
     FAX_RATE_12000	= 6,	        /* 12,000bps */
     FAX_RATE_14400	= 7	            /* 14,400bps */
}IPP_FAX_MAX_RATE_E;

#define FAX_PARA_PPP_TRAIN       0          /*PPP*/
#define FAX_PARA_LOCAL_TRAIN     1          /*LOCAL*/

#define FAX_PARA_ECM_UNSUPPORT     0        /*��֧��*/
#define FAX_PARA_ECM_SUPPORT          1     /*֧��*/

typedef struct tagCHANNEL_PARA_BIT_CHANGE_FLAG_S
{
    /*DSP���������־*/
    VOS_UINT16  bSilence:1;                 /*0�����������־*/
    VOS_UINT16  bEcho:1;                    /*1�����������־*/
    VOS_UINT16  bDtmfTransferMode:1;        /*2��DTMF����ģʽ�����־*/
    VOS_UINT16  bRemoteCode:1;              /*3��Զ�˱���뷽ʽ�����־*/
    VOS_UINT16  bRemotePtType:1;            /*4��Զ��ploadType�ı��־*/
    VOS_UINT16  bRemotePeriod:1;            /*5��Զ�˴��ʱ�䳤�ȱ����־*/
    VOS_UINT16  bLocalCode:1;               /*6�����˱���뷽ʽ�����־*/
    VOS_UINT16  bLocalPtType:1;             /*7������ploadType�ı��־*/
    VOS_UINT16  bLocalPeriod:1;             /*8�����˴��ʱ�䳤�ȱ����־*/
    VOS_UINT16  bMode:1;                    /*9������ģʽ�����־*/
    VOS_UINT16  bWorkMode:1;                /*10������ģʽ�����ʶ*/
    VOS_UINT16  bOutputGain:1;              /*11��DSPͨ��������������־*/
    VOS_UINT16  bInputGain:1;               /*12��DSPͨ��������������־*/
    /* BEGIN: Added by chenzhigang, 2008/1/25   ���ⵥ��:VDF SDP������ϸ���*/
    VOS_UINT16  bComfortNoise:1;            /*13��������������*/
    VOS_UINT16  bDspFill:3;                 /*���*/
    /* END:   Added by chenzhigang, 2008/1/25 */

    /*2833���������־, 2833��2198�ϲ�����������*/
    VOS_UINT16  bPtFor2833:1;               /*0��PTֵ�����־*/
    VOS_UINT16  bReSendCount:1;             /*1��2833�ش����������־*/
    /*2198���������־*/
    VOS_UINT16  bSupport2198:1;             /*2��֧�ֱ����־*/
    VOS_UINT16  bRFC2198RedCount:1;         /*3���������*/
    VOS_UINT16  bBlockPtFor2198:1;          /*4������pt���ͣ�������Ϊ96����������Ϊ97*/
    VOS_UINT16  bPayloadPtFor2198:1;        /*5�������pt���ͣ�������Ϊ96����������Ϊ97*/
    VOS_UINT16  bRFCFill:10;                /*���*/

    /*RTP���������־, RTP��RTCP�ϲ��������е�����RTCP����*/
    VOS_UINT8   bRtpRemoteIP:1;             /*0��Զ��IP�����־*/
    VOS_UINT8   bRtpRemotePort:1;           /*1��Զ�˶˿ڱ����־*/
    VOS_UINT8   bRtpLocalPort:1;            /*2�����˶˿ڱ����־*/
    VOS_UINT8   bJitterBuffer:1;            /*3��JitterBUffer�������־*/
    VOS_UINT8   bRemoteRtcpPort:1;          /*4��Զ�˶˿ڱ����־*/
    VOS_UINT8   bLocalRtcpPort:1;           /*5�����ض˿ڱ����־*/
    VOS_UINT8   bRtpFill:2;                 /*���*/


    /*Modem���������־*/
    VOS_UINT8  bModemTransferMode:1;        /*0������ģʽ�����־*/
    VOS_UINT8  bModemFill:7;                /*���*/

    /*FAX���������־*/
    VOS_UINT8  bMaxRate:1;                  /*0����������ٶ�, voipFaxRate_EN */
    VOS_UINT8  bTrainMode:1;                /*1��ѵ����ʽ, 0-�˵���, 1-���� */
    VOS_UINT8  bECM:1;                      /*2��ECM��ʽ��0-��֧��, 1-֧�� */
    VOS_UINT8  bFaxFill:5;                  /*���*/
                  
    VOS_UINT8 ucFill;                       /*��֤�ṹ4�ֽڶ��룬���ӱ�־ʱ��Ҫͬ���޸�*/
}CHANNEL_PARA_BIT_CHANGE_FLAG_S;

#define   PARA_CHANGE_YES      1            /*�������*/
#define   PARA_CHANGE_NO       0            /*δ���*/

typedef struct tagCHANNEL_PARA_BLOCK_CHANGE_FLAG_S
{
    VOS_UINT16  uwDspParaChange;            /*DSP���������־����Ϊ0�����־�������*/
    VOS_UINT16  uwRFCParaChange;            /*2833���������־ 2198���������־����Ϊ0�����־�������*/
    VOS_UINT8    ucRtpParaChange;           /*RTP���������־����Ϊ0�����־�������*/
    VOS_UINT8    ucModemParaChange;         /*Modem���������־����Ϊ0�����־�������*/
    VOS_UINT8    ucFaxParaChange;           /*FAX���������־����Ϊ0�����־�������*/
    VOS_UINT8    ucFill;                    /*��֤�ṹ4�ֽڶ��룬���ӱ�־ʱ��Ҫͬ���޸�*/
}CHANNEL_PARA_BLOCK_CHANGE_FLAG_S;

typedef struct tagCHANNEL_OPEN_PARA_S
{ 
    union
    {
        CHANNEL_PARA_BIT_CHANGE_FLAG_S       stParaBitChangeFlag;
        CHANNEL_PARA_BLOCK_CHANGE_FLAG_S  stParaBlockChangeFlag;
    }stParaChangeFlag;
    DSP_CONFIG_PARA_S                stDspCommonPara;
    RFC2833_CONFIG_PARA_S         stRFC2833Para;
    RFC2198_CONFIG_PARA_S         stRFC2198Para;
    RTP_CONFIG_PARA_S                stRtpPara; 
    RTCP_CONFIG_PARA_S              stRtcpPara;
    MODEM_CONFIG_PARA_S           stModemPara;
    FAX_CONFIG_PARA_S                stFaxPara;
}CHANNEL_OPEN_PARA_S;
/******************************************************************************************/


/******************************************************************************************
TAPI_GetRtpStatistic��ض���
******************************************************************************************/
typedef struct tagRTP_STATISTIC_DATA_S
{
    VOS_UINT32      ulSndPktNum;                /*���͵�RTP������*/
    VOS_UINT32      ulSndOctetNum;              /*���͵�RTP�ֽ�����*/
    VOS_UINT32      ulRecvPktNum;               /*�յ���RTP������*/
    VOS_UINT32      ulRecvOctetNum;             /*�յ���RTP�ֽ�����*/
    VOS_UINT32      ulCumLost;                  /*��ʧ�İ��������Ǳ���Ӧ����ʵ�ʽ��յİ��ĸ�����ƫ��*/
    VOS_UINT32      ulJitter;                   /*�����ƽ��ƫ� ��λMS*/
    VOS_UINT32      ulLoop;                     /*�������������ڼ�Ļ�·ʱ��,��λMS*/
    VOS_UINT32      ulFractionLost;             /*�������������ڼ�Ķ�������*/
    VOS_UINT32      ulDuration;                 /*����ʱ��nt/duration*/
}RTP_STATISTIC_DATA_S;
/******************************************************************************************/

/******************************************************************************************
TAPI_SendDtmf��ض���
******************************************************************************************/
typedef enum tagDTMF_TONE_VALUE_E
{
    DTMF_CODE_0,                /*��ʾ0*/
    DTMF_CODE_1,                /*��ʾ1*/
    DTMF_CODE_2,                /*��ʾ2*/
    DTMF_CODE_3,                /*��ʾ3*/
    DTMF_CODE_4,                /*��ʾ4*/
    DTMF_CODE_5,                /*��ʾ5*/
    DTMF_CODE_6,                /*��ʾ6*/
    DTMF_CODE_7,                /*��ʾ7*/
    DTMF_CODE_8,                /*��ʾ8*/
    DTMF_CODE_9,                /*��ʾ9*/
    DTMF_CODE_A,                /*��ʾ**/
    DTMF_CODE_B,                /*��ʾ#*/
    DTMF_CODE_C,                /*��ʾA*/
    DTMF_CODE_D,                /*��ʾB*/
    DTMF_CODE_E,                /*��ʾC*/
    DTMF_CODE_F,                /*��ʾD*/
    DTMF_CODE_BUTT

}DTMF_TONE_VALUE_E;
/******************************************************************************************/



/******************************************************************************************
TAPI_NetOperate��ض���
******************************************************************************************/
typedef enum tagNET_OPER_TYPE_E
{
    FORWARD_CONNECT,            /* ǰ������   */
    BACKWARD_CONNECT,           /* ��������   */
    DUAL_CONNECT,               /* ˫������   */
    DISCONNECT,                 /* ��������� */
    NET_OPERATION_BUTT
}NET_OPER_TYPE_E;

typedef enum tagNET_CHANNEL_TYPE_E
{
    CHAN_TYPE_DSP,
    CHAN_TYPE_RTP,
    CHAN_TYPE_HANDLE,           /*������"HW/TS"*/
    CHAN_TYPE_BUTT
}NET_CHANNEL_TYPE_E;
/******************************************************************************************/


/******************************************************************************************
TAPI_UserPortEventReport��ض���
******************************************************************************************/
typedef enum tagUSER_PORT_EVENT_REPORT_E
{
    USER_CALL_OFFHOOK,                  /* 0 �û��¼�:����ժ��*/
    USER_CALLED_OFFHOOK,                /* 1 �û��¼�:����ժ��        */
    USER_HOOKFLASH,                     /* 2 �û��¼�:�Ĳ�*/
    USER_ONHOOK,                        /* 3 �û��¼�:�һ�*/
    USER_DISCONNECT,                    /* 4 �û��¼�: �����������*/  
    
    /* Start of ouyangyueling+29638 2005-01-17 IAD2000V300R001B01D010 FNRD03870 */
    USER_TRANSFER,                      /* 5�û��¼�:ת��*/
    USER_CONFERENCE,                    /* 6 �û��¼�:����*/
    USER_CANCEL,                        /* 7�û��¼�:�ܾ�*/
    /* End of ouyangyueling+29638 2005-01-17 IAD2000V300R001B01D010 FNRD03870 */

    USER_START_SERVICE,                 /* 8�û��¼�: ����ҵ��*/
    USER_END_SERVICE,                   /* 9�û��¼�: ��ֹҵ��*/
    USER_PORT_FAULT,                    /* 10�û��¼�:�˿ڹ���*/
    USER_PORT_NORMAL,                   /* 11�û��¼�:�˿ڻָ�����*/

    USER_PULSE_CODE,                    /* 12��������ϱ�*/

    /* Start of ouyangyueling+29638 2005-01-17 IAD2000V300R001B01D010 FNRD03870 */
    USER_CODE_KEY,                      /* 13��������,һ������,���ַ���'\0'����; ��ʹ�ô��ֶ�ʱpReport�����CODE_KEY_S*/    
    /* End of ouyangyueling+29638 2005-01-17 IAD2000V300R001B01D010 FNRD03870 */

    USER_FSK_READY,                     /* 14FSK���� */
    USER_CDI_SEIZURE_ACK,               /* 15ժ����Ӧ  CDI */ 
    USER_CDI_CALLED_OFFHOOK,            /* 16����ժ��  CDI */
    USER_CDI_CALLED_ONHOOK,             /* 17���йһ�  CDI */ 
    USER_CDI_SEND_NUM_OK,               /* 18�ͺ�OK  CDI */
    USER_CDI_BLOCK,                     /* ����  CDI */
    USER_CDI_UNBLOCK,                   /* ������  CDI */

    /* BEGIN: Added by chenzhigang, 2008/5/16   ���ⵥ��:æ��*/
    USER_SERVICEMANTONE_FINISH,
    /* END:   Added by chenzhigang, 2008/5/16 */
    USER_BUTT
} USER_PORT_EVENT_REPORT_E;


/* Start of ouyangyueling+29638 2005-01-17 IAD2000V300R001B01D010 FNRD03870 */
/*���ϱ��¼�����ΪUSER_CODE_KEYʱ,pReport����*/
typedef struct tag_CODE_KEY_S 
{
    VOS_UINT8 aucNumber[LENGTH_OF_CALLING_NUMBER];
}CODE_KEY_S;
/* End of ouyangyueling+29638 2005-01-17 IAD2000V300R001B01D010 FNRD03870 */

/******************************************************************************************/

/******************************************************************************************
TAPI_DspEventReport��ض���
******************************************************************************************/
typedef enum tagCHANNEL_EVENT_REPORT_E
{
    CHANNEL_FAULT,                      /*����*/
    CHANNEL_FAX_START,                  /*���濪ʼ*/
    CHANNEL_FAX_END,                    /*�������*/
    CHANNEL_MODEM_START,                /*MODEM��ʼ*/
    CHANNEL_DETECT_DTMF,                /*DTMF���*/
    CHANNEL_DETECT_FSK,                 /*FSK���*/
    CHANNEL_DETECT_BUSY_TONE,           /*æ�����*/
    CHANNEL_RTCP_ALARM,                 /*RTCP�澯*/
    CHANNEL_OPEN_FAILED                 /*��ͨ��ʧ��*/
}CHANNEL_EVENT_REPORT_E;
/******************************************************************************************/

/******************************************************************************************
TAPI_DevEventReport��ض���
******************************************************************************************/
typedef enum tagDEV_TYPE_DEFINE_E
{
    DEV_USER_BOARD,
    DEV_DSP_CHIP,
    DEV_LSW_CHIP,           /*AG����*/
    DEV_FPGA,               /*AG����*/
    DEV_FE,                 /*AG����*/
    DEV_BUTT
}DEV_TYPE_DEFINE_E;

typedef enum tagDEV_EVENT_TYPE_E
{
    FAULT,
    NORMAL,
}DEV_EVENT_TYPE_E;

typedef struct tagBOARD_DATA_S
{
    VOS_UINT32 ulPortType;     /*�ݲ�ʹ��*/
    VOS_UINT32 ulStartUserPort;
    VOS_UINT32 ulEndUserPort;
} BOARD_DATA_S;
 	
typedef struct tagDSP_INDEX_S
{
    VOS_UINT32 ulStartDspIndex;
    VOS_UINT32 ulEndDspIndex;
}DSP_INDEX_S;
 	
typedef struct tagDEV_INFO_S
{
    VOS_UINT32 ulBoardIndex;
} DEV_INFO_S;
/******************************************************************************************/




/******************************************************************************************
TAPI_GetDecodeCapability��ض���
ulCodecCap�����CMD_PARA_MEDIA_PROTOCAL_E����
ulCapPara�������£�
 	**��Ӧ���������������
           ���ʱ������λms
 	**��Ӧ��T38��RFC2198��RFC2833  
           ���������
 	**��Ӧ��V.150
           �ݲ�����
******************************************************************************************/
#define MAX_DEV_CAPABILITY_NUM     32
/* BEGIN: Added by chenzhigang, 2008/1/7   ���ⵥ��:VDF SDP������ϸ���*/
//G.729���������
#define MAX_DEV_CAPABILITY_G729_NUM      2
/* END:   Added by chenzhigang, 2008/1/7 */

typedef struct tagCAPABILITY_ITEM_S
{
    VOS_UINT32  ulCodecCap;
    VOS_UINT32  ulCapPara;
}CAPABILITY_ITEM_S;

typedef struct tagDEV_CAPABILITY_S
{
    CAPABILITY_ITEM_S  stDevCap[MAX_DEV_CAPABILITY_NUM];
}DEV_CAPABILITY_S;

typedef enum tagGET_CAP_SELECT_CODE_E
{
    GET_DEV_CAP_ALL     = 0,        /*��ȡ�����������������ȼ�����������֧�ֵĴ��ʱ���ͱ�������ϡ���ͬһ�ֱ���룬��ͬ���ʱ������£��������뽫��Щ�������������������У������մ��ʱ����������*/
    GET_DEV_CAP_CALL    = 1,        /*���뷵������֧�ֵı����������ÿ������ֻ�ṩһ�ִ��ʱ����������ϵͳ�ṩ�ı�������ȼ�����0�����������ȣ�*/
    GET_DEV_CAP_FAX     = 2,        /*���ش����������������ȼ����򡣷��������������G.711����ϴ��ʱ����ֻ����һ����¼���� T.38��������������ֵ���� T.38+2��������������ֵ��*/
    GET_DEV_CAP_2198    = 3,
    GET_DEV_CAP_2833    = 4,
    GET_DEV_CAP_BUFF
}GET_CAP_SELECT_CODE_E;




/******************************************************************************************
TAPI_GetPortStatus��ض���
******************************************************************************************/
typedef enum tagPORT_STATUS_E
{
    PORT_STATUS_IDLE,
    PORT_STATUS_BUSY,
    PORT_STATUS_FAULT,
    PORT_STATUS_LOCKED,
    PORT_STATUS_BLOCK_OAM,
    PORT_STATUS_BLOCK_PEER,
    PORT_STATUS_BLOCK_FAULT,
    PORT_STATUS_TESTING,
    PORT_STATUS_LOOP,                     /*����,���ں�ͨ��ʹ��״̬���ڲ��еĵ�λ���ն˲�ʹ��������塣AG�п���ʹ��**/
    PORT_STATUS_NULL,                     /*δ��װ*/
    PORT_STATUS_BUFF
}PORT_STATUS_E;



/* Start of ouyangyueling+29638 2005-01-17 IAD2000V300R001B01D010 FNRD03870 */
/******************************************************************************************
TAPI_CIPReceiveCallstatus��ض���
******************************************************************************************/
typedef enum tagCIP_CALL_STATUS_E
{
    CIP_CALLSTATUS_IDLE = 0 ,
    /* Start of ouyangyueling+29638 2005-03-15 TS0001 V300R001B01D030 FNRD04333 */
    CIP_CALLER_DAILING_OVER,        /* ��Ϊ���У�����������*/
    /* End of ouyangyueling+29638 2005-03-15 TS0001 V300R001B01D030 FNRD04333 */
    CIP_CALLER_START,               /* ��Ϊ���У����п�ʼ,���ųɹ���ʼ����*/
    CIP_CALLED_START,               /* ��Ϊ���У����п�ʼ,��ʼ����*/
    CIP_CALLER_END,                 /* ��Ϊ���У����н������һ�*/
    CIP_CALLED_END,                 /* ��Ϊ���У����н�������ͨǰ�Է��һ�*/    
    CIP_CALL_PEER_BUSY,             /* ��Ϊ���У����жԷ�æ*/ 
    CIP_CALL_PEER_NO_RES,           /* ��Ϊ���У�������Ӧ��, �Է��ýв�Ӧ*/ 
    CIP_TALK_START,                 /* ͨ����ʼ*/ 
    CIP_TALK_END,                   /* ͨ������,����ֻҪ��һ���һ���ͨ������������ͨ������Ҫ��������*/
    CIP_BUTT,
}CIP_CALL_STATUS_E;
/* End of ouyangyueling+29638 2005-01-17 IAD2000V300R001B01D010 FNRD03870 */




/* Start of ouyangyueling+29638 2005-01-24 IAD2000V300R001B01D020 FNRD03949 */
/******************************************************************************************
TAPI_FXOStateChange/TAPI_FXOStateGet��ض���
******************************************************************************************/
typedef enum tag_FXO_STATE_E
{
    FXO_IDLE,
    FXO_BUSY,
    FXO_ALERT,
    FXO_NULL
}FXO_STATE_E;




/******************************************************************************************
TAPI_FXOSwitch��ض���
******************************************************************************************/
typedef enum tagSWITCH_DIRECT_E
{
    POTS_TO_FXO,   /* switch  POTS port to   FXO port */
    POTS_TO_FXS    /* switch  POTS port to   FXS port */
}SWITCH_DIRECT_E;
/* End of ouyangyueling+29638 2005-01-24 IAD2000V300R001B01D020 FNRD03949 */


/* Start of ouyangyueling+29638 2005-03-15 TS0001 V300R001B01D030 FNRD04333 */
/******************************************************************************************
TAPI_SetTalkForPhone��ض���
******************************************************************************************/
typedef enum tagTEL_DIRE_E
{
    TEL_HANDSET_ON,                     /*����ͨ������Ͳ*/
    TEL_HANDFREE_ON,                    /*����ͨ��������*/
    TEL_DEFAULT,                        /*ȱʡ����*/
    TEL_BUTT,                           
}TEL_DIRECT_E;


/******************************************************************************************
TAPI_GetHardwareType��ض���
******************************************************************************************/
typedef enum tagHARDWARE_TYPE_E
{
    HARDWARE_TYPE_IAD101H,                    
    HARDWARE_TYPE_IAD101S,                     
    HARDWARE_TYPE_IAD102H,                    
    HARDWARE_TYPE_IAD104H,    
    HARDWARE_TYPE_IAD102S,    
    HARDWARE_TYPE_BUFF
}HARDWARE_TYPE_E;


/******************************************************************************************
TAPI_RegLedDisplay��ض���
******************************************************************************************/
typedef enum tagLED_STATUS_E
{
    LED_TURNOFF,                        /*���� */
    LED_LIGHT,                          /*ժ��ָʾ�ƣ�����*/
    LED_FALSH,                          /*���Ի�δ������ָʾ��,��˸*/
    LED_BUTT                            /*��Ч����*/
}LED_STATUS_E;

/* End of ouyangyueling+29638 2005-03-15 TS0001 V300R001B01D030 FNRD04333 */

/* Start of lidongyun+39160 2005-06-21 TS0001 TS0401V300R001B01D060 A41D00612*/
typedef struct tagRING_CHANNEL_PARA_S
{
    VOS_UINT8  ucRingParaType;  //(L/r0---L/r7) ,����ʱ���0xFF
    VOS_UINT8  ucDspIndex;
    VOS_UINT8  ucBuffer[2];     //����ʱ���0xFFFFFF
} RING_CHANNEL_PARA_S;

typedef enum tagRING_PARA_E
{
    RING_R0 = 0,     // �����������(L/r0)
    RING_R1,         // �����������(L/r1)
    RING_R2,         // �����������(L/r2)
    RING_R3,         // �����������(L/r3)
    RING_R4,         // �����������(L/r4)
    RING_R5,         // �����������(L/r5)
    RING_R6,         // �����������(L/r6)
    RING_R7,         // �����������(L/r7)
    RING_RG,
    RING_BUTT        // ����������� 
}RING_PARA_E;
/*End of lidongyun+39160 2005-06-21 TS0001 TS0401V300R001B01D060 A41D00612*/

/* Start of zhuweiping 30370 2005-07-28 TS0401 TS0401V300R001B01D090 A41D00849*/
/*�˽ṹ���ΪMGCP MIB���߼��˿ڱ�ʹ��.�����ǰ�к��У�����ʹ�õ�ǰ���еĲ���;��֮����ȡ���������õ�ȱʡֵ*/
/*ע1:�������Ѿ��������������У���ʱ�޸����������ã�������������Ե�ǰ���е�Ϊ׼.*/
/*ע2:���ڴ����MODEMҵ��Ҳ�Ե�ǰ���е�Ϊ׼��������������������*/
#define CHANNEL_PARA_COMFORT_NOISE_DISABLE   0  /*��ʾ��֧������������Ŀǰ��������Ϊ��֧�֣���ʱ����,Ϊ���������ýӿ�*/
#define CHANNEL_PARA_COMFORT_NOISE_ENABLE    1  /*��ʾ֧����������*/

typedef struct tag_TAPI_MIB_DSP_CHANNEL_PARA_S
{
    VOS_UINT32  ulEcho;          /*ȱʡON.���ú궨��CHANNEL_PARA_EC_DISABLE��ʾOFF;CHANNEL_PARA_EC_ENABLE��ʾON,�������궼������TAPI_DEF.H�У�����ֱ��ʹ��*/
    VOS_UINT32  ulSilence;       /*ȱʡOFF.���ú궨��CHANNEL_PARA_SILENCE_DISABLE��ʾOFF;CHANNEL_PARA_SILENCE_ENABLE��ʾON,�������궼������TAPI_DEF.H�У�����ֱ��ʹ��*/
    VOS_UINT32  ulComfortNoise;  /*ȱʡON.���ú궨��CHANNEL_PARA_COMFORT_NOISE_DISABLE��ʾOFF;CHANNEL_PARA_COMFORT_NOISE_ENABLE��ʾON,�������궼������TAPI_DEF.H�У�����ֱ��ʹ��*/
}TAPI_MIB_DSP_CHANNEL_PARA_S;
/* End of zhuweiping 30370 2005-07-28 TS0401 TS0401V300R001B01D090 A41D00849*/

typedef enum tagTAPICallStateE
{
    CS_CALL_IDLE = 0,           /*����*/
    CS_CALLER_DIAL_TONE,        /*�������в�����*/
    CS_CALLER_START,            /*����������룬��ʼ�������*/
    CS_CALLER_RINGBACK,         /*���к��гɹ�����ʼ���Ż�����*/
    CS_CALLER_CONXT,            /*����ժ�������гɹ�����*/
    CS_CALLER_NOREPLY,          /*�ýв�Ӧ*/
    CS_CALLER_BUSY,             /*����æ*/
    CS_CALL_INI_ONHOOK,         /*���˹һ���������гɹ�*/
    CS_CALL_PAS_ONHOOK,         /*�Զ˹һ���������гɹ�*/
    CS_CALLEE_ALERTING,         /*��ʼ�����壬��ʱ���к����Ѿ��·�����*/
    CS_CALLEE_CONXT,            /*����ժ���ϱ����Զ˽��ܺ��У����п�ʼ*/
    CS_CALL_HOLD_START,         /*���б��ֳɹ�����ʼ�Ŷ��β�����*/
    CS_CALL_HOLD_END,           /*���б��ֽ�����ֹͣ�Ŷ��β�����*/
    CS_CALL_WAITING_START,      /*���º��н��룬���ź��еȴ��������еȴ���ʼ*/
    CS_CALL_TWOCR_SWITCH,       /*��ҵ���������û��л�*/
    CS_CALL_RELEASE_ONECR,      /*��ҵ���н���һ�����У���������һ������*/
    CS_CALL_CONF_START,         /*����ͨ����ʼ*/
    CS_CALL_CONF_END,           /*����ͨ������*/
    CS_CALL_END,                /*���н���*/
}TAPICallStateE;
typedef enum channel_ACTION_E
{
    TAIP_CHANNEL_INIT = 0,
    TAIP_CHANNEL_MDFY ,
    TAPI_CHANNEL_CLOSE
}CHANNEL_ACTION_E;

#endif  /*TAPI_DEF_H*/

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
