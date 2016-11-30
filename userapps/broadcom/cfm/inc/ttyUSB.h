/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   :ttyUSB.h
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2007��12��29��
  ����޸�   :
  ��������   : cmdTest.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2007��12��29��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/


#ifndef __TTYUSB_H__
#define __TTYUSB_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
#include <linux/types.h>
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
 
 /* start of VOICE ���������� by pengqiuming(48166), 2008/10/14 
   for ֻ��ʶ��2��3��4�ڲŲ���,Usb-Serial Port Num����һ���ڱ��4����, ���������Ϊ��QXDM͸��ץlog��Ϣ.
	   ԭ��port�����û��ǲ���:0��������,1��������,2����ATָ��,3����QXDM. 
	   E180��11.104.18.00.00�汾�ĳ���4��,E169��11.314.17.31.00�汾�ĳ�4��.*/
#define HSPA_MAX_AVALABLE       4         //HSPA������port num
#define HSPA_MIN_AVALABLE       2         //HSPA���ٿ���port num
#define HSPA_MIN_VOICE_AVALABLE 3         //HSPA֧������ʱ������Ҫ��port num
#define HSPA_MAX_LINE_LEN       64        //����г���
#define HSPA_PORT_NUM_STR       "Usb-Serial Port Num:%d"
/* end of VOICE ���������� by pengqiuming(48166), 2008/10/14 */

/*����ʹ�ܺ�*/
//#define CX_DEBUG_VERSION

/*��ʾ��ͬ��ģ��*/
/*
#define AT_MODULE 0
#define VOICE_MODULE 1
#define MODEM_MODULE 2
#define TR069_MODULE 3
#define HTTP_MODULE 4
#define LED_MODULE 5
#define CMD_MODULE 6
*/
#define AT_MODULE 1
#define VOICE_MODULE 2
#define MODEM_MODULE 3
#define TR069_MODULE 4
#define HTTP_MODULE 5
#define LED_MODULE 6
#define CMD_MODULE 7
#define PINMNGR_MODULE 8

/*��ʾ��ͬ��ģ���Ӧ����Ϣ����*/
#define MSG_AT_QUEUE  100
#define MSG_VOICE_QUEUE  101
#define MSG_MODEM_QUEUE  102
#define MSG_TR069_QUEUE  103
#define MSG_HTTP_QUEUE  104
#define MSG_LED_QUEUE  105
#define MSG_CMD_QUEUE  106
#define MSG_PINMNGR_QUEUE 107

/*������HSPA���ƽ��̵���Ϣ�ṹ��Ļ���������*/
#define	AT_SND_MSG_LENGTH 128

/*��������HSPA���ƽ��̵���Ϣ�ṹ��Ļ���������*/
#define	AT_RCV_MSG_LENGTH 252

/************************************************************/
#define	CMEE_SET	1
#define	CMEE_QUERY	2
#define	E_SET	3
#define	S3_SET	4
#define	S3_QUERY	5
#define	S4_SET	6
#define	S4_QUERY	7
#define	S5_SET	8
#define	S5_QUERY	9
#define	V_SET	10
#define	FACTORY_SET	11
#define	REPORT_PORT_SET	12
#define	REPORT_PORT_QUERY	13
#define	CFUN_SET	14
#define	CFUN_QUERY	15
#define	PRODUCT_NAME_QUERY	16
#define	FACTORY_INFO_QUERY	17
#define	FACTORY_INFO_QUERY_C	18
#define	SOFTWARE_VERSION_QUERY	19
#define	SOFTWARE_VERSION_QUERY_C	20
#define	HARDWARE_VERSION_QUERY	21
#define	HARDWARE_VERSION_QUERY_W	22
#define	EARPHONE_VOLUME_SET	23
#define	EARPHONE_VOLUME_QUERY	24
#define	EARPHONE_SILENCE_SET	25
#define	EARPHONE_SILENCE_QUERY	26
#define	IMSI_QUERY	27
#define	IMEI_QUERY	28
#define	SPN_QUERY	29
#define	SIM_USIM_MODE_QUERY	30
#define	RSSI_QUERY	31
#define	AUTO_REPORT_SET	32
#define	AUTO_REPORT_QUERY	33
#define	SYSTEM_INFO_QUERY	34
#define	NETWORK_REG	35
#define	NETWORK_REG_QUERY	36
#define	GPRS_NETWORK_REG	37
#define	GPRE_NETWORK_REG_QUERY	38
#define	SYSCFG_SET	39
#define	SYSCFG_QUERY	40
#define	SYSCONFIG_SET	41
#define	SYSCONFIG_QUERY	42
#define	FREQLOCK_SET	43
#define	FREQLOCK_QUERY	44
#define	PIN_PASSWORD_MODIFY	45
#define	PIN_MANAGE_PLUSS	46
#define	PIN_QUERY_PLUSS	47
#define	PIN_ENABLE	48
#define	PIN_MANAGE	49
#define	PIN_QUERY	50
#define	PDP_SET	51
#define	PDP_QUERY	52
#define	DSFLOW_CLEAR	53
#define	DSFLOW_QUERY	54
#define	DIAL	55
#define	DIAG_PORT_SET	56
#define	ACCEPT	57
#define	CHUP	58
#define	TELNUM_DISPLAY_SET	59
#define	TELNUM_DISPLAY_SET_QUERY	60
#define	CALL_STATUS_QUERY	61
#define	CALL_TIME_QUERY	62
#define	AUTO_SIM_STATUS_REPORT	63
#define	RING_REPORT	64
#define	CALL_NUM_REPORT	65
#define	CALL_END_REPORT	66
#define	AUTO_SERVICE_STATUS_REPORT	67
#define	AUTO_SYSTEM_MODE_STATUS_REPORT	68
#define 	PIN_PUK_TIMES_QUERY 69
#define 	PIN_ENABLE_QUERY	70
#define  	CONN_REPORT 71
#define 	OPERATION_QUERY 72
#define	CARDMODE_QUERY 73
/* HUAWEI HGW s48571 2008��2��15�� ��ȡSN���� add begin:*/
#define	SN_QUERY	74
/* HUAWEI HGW s48571 2008��2��15�� ��ȡSN���� add end.*/
#define DEX_SET     75
#define   CGACT_QUERY  76
/* HUAWEI HGW s48571 2008��8��16�� �ر��������� add begin:*/
#define VOICE_SET   77
/* HUAWEI HGW s48571 2008��8��16�� �ر��������� add end.*/

#define DISABLE_VOICE 0
#define ENABLE_VOICE 1

/* l65130 hspa������Ϣ�ļ� */
#define HSPA_SYSCFG_FILE   "/var/hspasys.cfg"
#define HSPA_SYS_CFG_FORMAT_W "ConType:%d\nbands:%s\nVoiceEnable:%d\nOperator:%s\n"
#define HSPA_SYS_CFG_FORMAT_R "ConType:%d\nbands:%s\nVoiceEnable:%d\nOperator:%31[^\n]\n"

/*<CR><LF>^CARDMODE:<sim_type><CR><LF><CR><LF>OK <CR><LF>*/
struct stMsg_Card_Mode
{
	unsigned char ucCmdFrom;      
	unsigned char ucResult;
	/*<sim_type>��SIM�������ͣ�ȡֵ���£�
	0:  Unknown mode
	1:  SIM��
	2:  USIM��*/
	unsigned char ucCardMode;
	unsigned char ucExtern1;
};


/*#define 	OPERATION_QUERY 72
AT+COPS?

<CR><LF>+COPS: <mode>[,<format>,<oper>[,<rat>]]
<CR><LF>OK<CR><LF>
��MS��ش���ʱ��
<CR><LF>+CME ERROR: <err><CR><LF>
<mode>:   ����ѡ��ģʽ
0:     �Զ����������������У�modeȡֵΪ0ʱ�������������Ҫ����
1:     �ֶ�����
2:     ȥע������
3:     ��Ϊ+COPS?���������÷��صĸ�ʽ<format>
4:     �ֶ����Զ������ϣ�����ֶ�����ʧ�ܣ����Զ�ת���Զ�����ģʽ
<format>:  ��Ӫ����Ϣ<oper>�ĸ�ʽ   
0:	���ַ�����ʽ����Ӫ����Ϣ<oper>
1:	���ַ�����ʽ����Ӫ����Ϣ<oper>
2:	���ָ�ʽ����Ӫ����Ϣ<oper>
<oper>: ��Ӫ�̵���Ϣ��
<stat>:  �����״̬��ʶ��ȡֵ���£�  
0:	δ֪��
1:	���õ�
2:	��ǰע��
3:	��ֹ
<rat>�����߽��뼼����ȡֵ���£�
 0:	GSM/GPRS ��ʽ
 2:       WCDMA��ʽ*/

/*                                                                  
+COPS: 0,2,"46000",0                                                            
                                                                                
OK*/

/*+COPS: (2,"CHINA  MOBILE","CMCC","46000",0),
(1,"","","46019",2 ),
(3,"CHN-CUGSM","CU-GSM","46001",0),

,(0,1,3,4),(0,1,2)  */
struct stMsg_Operation_Info_Query
{
	#define HSPA_OPERATION_INFO_LENGTH	23
	unsigned char ucCmdFrom;      
	unsigned char ucResult;
	/*<mode>:   ����ѡ��ģʽ
	0:     �Զ����������������У�modeȡֵΪ0ʱ�������������Ҫ����
	1:     �ֶ�����
	2:     ȥע������
	3:     ��Ϊ+COPS?���������÷��صĸ�ʽ<format>
	4:     �ֶ����Զ������ϣ�����ֶ�����ʧ�ܣ����Զ�ת���Զ�����ģʽ	*/
	unsigned char ucChooseMode;
	/*<format>:  ��Ӫ����Ϣ<oper>�ĸ�ʽ   
	0:	���ַ�����ʽ����Ӫ����Ϣ<oper>
	1:	���ַ�����ʽ����Ӫ����Ϣ<oper>
	2:	���ָ�ʽ����Ӫ����Ϣ<oper>	*/
	unsigned char ucOperationFormat;
	/*<oper>: ��Ӫ�̵���Ϣ����಻����20�ַ�*/
	char acOperationInfo[HSPA_OPERATION_INFO_LENGTH];
	/*	<rat>�����߽��뼼����ȡֵ���£�
	 0:	GSM/GPRS ��ʽ
 	2:       WCDMA��ʽ*/
	unsigned char ucWifiTech;
};


/*ͨ���ַ����鳤��*/
#define	COMMON_LONG_STRING_LENGTH	128
#define	COMMON_STRING_LENGTH	64
#define	COMMON_SHORT_STRING_LENGTH	32
#define	COMMON_VERY_SHORT_STRING_LENGTH	16
/*�ṹ�����ַ�������*/
#define	HSPA_FACINFO_LENGTH	60	
#define	HSPA_SOFTWARE_LENGTH	60
#define	HSPA_HARDWARE_LENGTH	60
#define	HSPA_PRODUCT_NAME_LENGTH	60

#define	HSPA_IMSI_LENGTH	16
#define	HSPA_IMEI_LENGTH	16
/* HUAWEI HGW s48571 2008��2��15�� ��ȡSN���� add begin:*/
#define	HSPA_SN_LENGTH 32
/* HUAWEI HGW s48571 2008��2��15�� ��ȡSN���� add end.*/
#define	HSPA_BAND_LENGTH	12
#define	HSPA_PROTOCOL_KIND_LENGTH	8
#define	HSPA_APN_NAME_LENGTH	112
#define	HSPA_DOMAIN_ADDR_LENGTH	20
#define	HSPA_TELNUM_LENGTH	24

/*ִ�н������ȷ��*/
#define	HSPA_OK	0
#define	HSPA_ERROR_NO_E172	1
#define	HSPA_ERROR_NO_SIM		2
#define	HSPA_BAD_COMMAND 	3
#define	HSPA_ERROR_PIN_NEED	4
#define	HSPA_ERROR_PUK_NEED	5
#define	HSPA_ERROR_OPERATION_REFUSED	6
#define	HSPA_ERROR_COMMAND_REFUSED	7
#define	HSPA_ERROR_INCORRECT_PASSWORD	8
#define	HSPA_ERROR_PIN_NEED	9
#define	HSPA_ERROR_PUK_NEED  10

#define HSPA_PIN_PUK_NEEDED 101


/*Ԥ����8~31*/
#define	HSPA_UNKNOW	(-1)
#define	HSPA_ERROR	(-1)
#define	HSPA_UNKNOW_VALUE	250
#define	HSPA_OFFSET	1
#define	HSPA_EXIT	125
#define	HSPA_WRITE_ENABLE		1
#define	HSPA_WRITE_DISABLE	0
#define	HSPA_ERROR_UNKNOW	100

/*��Ϣ��Դ��Ӧ�����±��*/
#define	HSPA_FROM_POSITION 0
#define	HSPA_RESULT_POSITION 1

/*���ݿ������޺�*/
#define	HSPA_NO	 0
#define	HSPA_EXIST	1
#define	HSPA_UNKNOWN	2


/*SIM�����ں�*/
#define  HSPA_NO_SIM	255

/*SIM����Ч��*/
#define  HSPA_INVALID_SIM	 0
/*SIM����Ч��*/
#define  HSPA_VALID_SIM	 1
/*��ҪPIN��У���*/

#define	HSPA_PIN_NEED	1
/*��ҪPUK��У���*/
#define	HSPA_PUK_NEED	2
/*����ҪPIN/PUK��У���*/
#define	HSPA_PIN_PUK_OK	0

/*�����Ƿ�д���豸��
����ǰ�������ʾ����
Ϣ��������ȡ����Ϣ*/
#define 	HSPA_WRITE_OK	1
#define	HSPA_WRITE_NO	0
#define	HSPA_WRITE_UNKNOW (-1)


/*
<curr_ds_time>��
00000000��FFFFFFFF��8λ16����������ʾ��ǰ���DS������ʱ�䣬��λΪ�롣
<ltx_rate>��
		00000000��FFFFFFFF��8λ16����������ʾ��ǰ�ķ������ʣ���λΪ�ֽ�ÿ�롣
<rx_rate>��
		00000000��FFFFFFFF��8λ16����������ʾ��ǰ�Ľ������ʣ���λΪ�ֽ�ÿ�롣
<curr_tx_flow>��
0000000000000000��FFFFFFFFFFFFFFFF ��16λ16����������ʾ��ǰ���DS�ķ�����������λΪ�ֽڡ�
<curr_rx_flow>��
0000000000000000��FFFFFFFFFFFFFFFF ��16λ16����������ʾ��ǰ���DS�Ľ�����������λΪ�ֽڡ�
<qos_tx_rate>��
		00000000��FFFFFFFF��8λ16���������������Э�̺�ȷ����PDP���ӷ������ʣ���λΪ�ֽ�ÿ�롣
<qos_rx_rate>��
		00000000��FFFFFFFF��8λ16���������������Э�̺�ȷ����PDP���ӽ������ʣ���λΪ�ֽ�ÿ�롣

*/
struct stDataFlow
{
	unsigned long int ulCurrDsTime;
	unsigned long int ulTxRate;
	unsigned long int ulRxRate;
	unsigned long int ulQosTxRate;
	unsigned long int ulQosRxRate;
	/*char acCurrDsTime[COMMON_SHORT_STRING_LENGTH];
	char acTxRate[COMMON_SHORT_STRING_LENGTH];
	char acRxRate[COMMON_SHORT_STRING_LENGTH];
	char acCurrTxFlow[COMMON_SHORT_STRING_LENGTH];
	char acCurrRxFlow[COMMON_SHORT_STRING_LENGTH];
	char acQosTxRate[COMMON_SHORT_STRING_LENGTH];
	char acQosRxRate[COMMON_SHORT_STRING_LENGTH];*/
};
#define HSPA_SHARE_FILE_PATH "/var/HspaStatus"
struct stHspaInfo
{
	/*HSPA���ƽ���pid*/
	int lPid;
	/*USB-SERIAL�豸�ӿ���Ŀ
	-1��״̬δ֪
	0�����豸
	3������	*/
	short int sDeviceCount;
	/*	-1��״̬δ֪
	0������ΪLPM���͹��ģ�ģʽ��MS֮ǰ�����ñ���Ϊ��offlineģʽ��
	1������Ϊonlineģʽ��Ĭ��ֵ����MS֮ǰ�����ñ���Ϊ��offlineģʽ��
	4������Ϊofflineģʽ��MS֮ǰ�����ñ���Ϊ��FTMģʽ��
	5������ΪFTMģʽ��MS֮ǰ�����ñ���Ϊonlineģʽ��
	6������MS��MS֮ǰ�����ñ���Ϊofflineģʽ��	*/
	short int sDeviceStatus;
	/*SIM��״̬��ȡֵ���£�
	-1��״̬δ֪
	0�� USIM��״̬��Ч
	1:  USIM��״̬��Ч  
	2�� USIM��CS����Ч��EW��
	3�� USIM��PS����Ч(EW)
	4�� USIM��PS+CS�¾���Ч(EW)
	240��ROMSIM�汾
	255�� USIM�������� 	
	250:��ʾδ��ȡ����ֵ��������*/
	short int sSimExist;
	/*	��ʾSIM��״̬
	-1��״̬δ֪��
	0��	   SIM��δ��SIMLOCK��������
	1��	   SIM����SIMLOCK��������*/
	short int sSimStatus;
	/*ϵͳģʽ��ȡֵ���£�
	-1��״̬δ֪��
	0���޷���
	1��AMPSģʽ���ݲ�ʹ�ã�
	2��CDMAģʽ���ݲ�ʹ�ã�
	3��GSM/GPRS ģʽ
	4��HDRģʽ
	5��WCDMAģʽ
	6��GPSģʽ*/
	short int sSysMode;
	/*ϵͳģʽ����ģʽ��ȡֵ���£�
	-1��״̬δ֪��
	0���޷���
	1��GSMģʽ
	2��GPRSģʽ
	3��EDGEģʽ
	4��WCDMAģʽ
	5��HSDPAģʽ
	6��HSUPAģʽ
	7��HSDPA&HSUPAģʽ*/
	short int sSubSysMode;
	/*	-1��״̬δ֪��
	0:  �޷���
	1:  �����Ʒ���
	2:  ������Ч	
	3:  �����Ƶ��������
	4:  ʡ�����˯״̬	*/
	short int sSrvStatus;
	/*-1��״̬δ֪��
	0:����Ҫ�κ���֤
	1:PIN��֤
	2:PUK��֤
	*/
	short int sPinPukNeed;
	char acIMEI[HSPA_IMEI_LENGTH]; //�洢�ڿ��е�IMSIֵ��Ϊһ��0��9��ʮ���������ɵ��ַ���
	char acPinCode[HSPA_IMEI_LENGTH];
	char acOldIMEI[HSPA_IMEI_LENGTH];
	struct stDataFlow stDataFlowInfo;

 };
//#define	FACTORY_INFO_QUERY 17
//#define	FACTORY_INFO_QUERY_C 18
/*������Ϣ��ѯ 
�����ʽ AT+GMI 
��Ӧ��ʽ��<CR><LF>+GMI: <manufacturer><CR><LF><CR><LF>OK<CR><LF> ��MS��ش���ʱ��<CR><LF>ERROR<CR><LF>*/
/*������Ϣ��ѯ 
�����ʽ��AT+CGMI 
��Ӧ��ʽ��CR><LF>+GMI: <manufacturer><CR><LF><CR><LF>OK<CR><LF> 
��MS��ش���ʱ��<CR><LF>ERROR<CR><LF>*/
struct stMsg_Factory_Info_Query
{ 
      unsigned char ucCmdFrom;      
      unsigned char ucResult;
      unsigned char ucExtern1;
      unsigned char ucExtern2;
      char acFactoryInfo[HSPA_FACINFO_LENGTH];//������Ϣ��ȡֵΪ�ַ�����     
};


//#define	SOFTWARE_VERSION_QUERY	19
//#define	SOFTWARE_VERSION_QUERY_C 20
/*����汾�Ų�ѯ 
�����ʽ��AT+GMR <CR><LF>+GMR:<softversion><CR><LF><CR><LF>OK<CR><LF>
��MS��ش���ʱ��<CR><LF>ERROR<CR><LF>*/
/*����汾�Ų�ѯ 
�����ʽ��AT+CGMR <CR><LF>+GMR:<softversion><CR><LF><CR><LF>OK<CR><LF>
��MS��ش���ʱ��<CR><LF>ERROR<CR><LF>*/
struct stMsg_Software_Version_Query
{
	unsigned char ucCmdFrom;      
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
	char acSoftWareVersion[HSPA_SOFTWARE_LENGTH]; //����汾��    
};


//#define	HARDWARE_VERSION_QUERY	21
//#define	HARDWARE_VERSION_QUERY_W  22
/*Ӳ���汾�Ų�ѯ 
�����ʽ��AT^HVER 
<CR><LF>^HWVER:<hardversion><CR><LF><CR><LF>OK<CR><LF>��MS��ش���ʱ��<CR><LF>+CME ERROR: <err><CR><LF>*/
/*Ӳ���汾�Ų�ѯ 
�����ʽ��AT^HWVER 
��Ӧ��ʽ��<CR><LF>^HWVER:<hardversion><CR><LF><CR><LF>OK<CR><LF>
��MS��ش���ʱ��<CR><LF>+CME ERROR: <err><CR><LF>*/
struct stMsg_Hardware_Version_Query
{
	unsigned char ucCmdFrom; 
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
	char acHardWareVersion[HSPA_HARDWARE_LENGTH];  //Ӳ���汾��   
};

	
//#define	IMSI_QUERY	27
/*IMSI��ѯ 
�����ʽ��AT+CIMI 
��Ӧ��ʽ��<CR><LF><IMSI><CR><LF><CR><LF>OK<CR><LF>��MS��ش���ʱ��<CR><LF>+CME ERROR: <err><CR><LF>*/
struct stMsg_IMSI_Query
{
	unsigned char ucCmdFrom; 
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
	char acIMSI[HSPA_IMSI_LENGTH]; //�洢�ڿ��е�IMSIֵ��Ϊһ��0��9��ʮ���������ɵ��ַ���,  
};

/*#define	IMEI_QUERY	28
IMEI��ѯ 
�����ʽ��AT+CGSN 
��Ӧ��ʽ��<CR><LF><IMEI><CR><LF><CR><LF>OK<CR><LF>��MS��ش���ʱ��<CR><LF>+CME ERROR: <err><CR><LF>*/
struct stMsg_IMEI_Query
{ 
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
	char acIMEI[HSPA_IMEI_LENGTH];//�����IMEIֵ,Ϊһ�ַ���   
};

/*#define	RSSI_QUERY	31
RSSI��ѯ 
�����ʽ��AT+CSQ 
��Ӧ��ʽ��<CR><LF>+CSQ: <rssi>,<ber><CR><LF><CR><LF>OK<CR><LF> */
struct stMsg_RSSI_Query
{
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucRSSI;/*�����ź�ǿ��ָʾ
	                        0   ���ڻ�С�� 113 dBm
	                        1   111 dBm
	                        2   ...30  109...  53 dBm
	                        31 ���ڻ���� 51 dBm
	                        99 δ֪�򲻿ɲ�*/
	unsigned char ucBer;//��ʱ��֧�� 
};

/*#define	SYSTEM_INFO_QUERY	 34
ϵͳ����Ϣ��ѯ 
�����ʽ��AT^SYSINFO 
��Ӧ��ʽ��<CR><LF>^SYSINFO:< srv_status >,< srv_domain >,< roam_status >,< sys_mode >,< sim_state >[,<lock_state>,<sys_submode>]<CR><LF><CR><LF>OK<CR><LF> <CR><LF>^SYSINFO:< srv_status >,< srv_domain >,< roam_status >,< sys_mode >,< sim_state >[,<lock_state>,<sys_submode>]<CR><LF><CR><LF>OK<CR><LF>*/
struct stMsg_System_Info_Query
{
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucSrvStatus;/*ϵͳ����״̬��ȡֵ���£�
	                              0:  �޷���
	                              1:  �����Ʒ���
	                              2:  ������Ч 
	                              3:  �����Ƶ��������
	                              4:  ʡ�����˯״̬   */
	unsigned char ucSrvDomain;/*ϵͳ������ȡֵ���£�
	                              0��    �޷���
	                              1��    ��CS����
	                              2��    ��PS����
	                              3��    PS+CS����
	                              4��    CS��PS��δע�ᣬ����������״̬
	                              255��CDMA��֧�֡�

					250:��ʾδ��ȡ����ֵ��������
	                              */
	unsigned char ucRoamStatus;/*����״̬��ȡֵ���£�
	                              0��  ������״̬
	                              1��  ����״̬ */
	unsigned char ucSysMode;/*ϵͳģʽ��ȡֵ���£�
							0���޷���
							1��AMPSģʽ���ݲ�ʹ�ã�
							2��CDMAģʽ���ݲ�ʹ�ã�
							3��GSM/GPRS ģʽ
							4��HDRģʽ
							5��WCDMAģʽ
							6��GPSģʽ
							*/	
	unsigned char ucSimState;  /*SIM��״̬��ȡֵ���£�
	                              0�� USIM��״̬��Ч
	                              1:  USIM��״̬��Ч  
	                              2�� USIM��CS����Ч��EW��
	                              3�� USIM��PS����Ч(EW)
	                              4�� USIM��PS+CS�¾���Ч(EW)
	                              240��ROMSIM�汾
	                              255�� USIM��������   

					250:��ʾδ��ȡ����ֵ��������

	                              */
	unsigned char ucLockState;/*SIM����LOCK״̬��ȡֵ���£�
	                               0��    SIM��δ��SIMLOCK��������
	                               1��    SIM����SIMLOCK��������   */
	unsigned char ucSysSubMode;/*    0���޷���
							              1��GSMģʽ
							              2��GPRSģʽ
							              3��EDGEģʽ
							              4��WCDMAģʽ
							              5��HSDPAģʽ
							              6��HSUPAģʽ
							7��HSDPA&HSUPAģʽ
							 */
	unsigned char ucExtern1;
	unsigned char ucExtern2;
	unsigned char ucExtern3;
};

//#define	SYSCFG_SET	39
/*ϵͳ���òο����� 
�����ʽ��AT^SYSCFG<mode>,<acqorder>,<band>,<roam>,<srvdomain> 
��Ӧ��ʽ��<CR><LF>OK<CR><LF> */
struct stMsg_Syscfg_Set
{
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
};

//#define	SYSCFG_QUERY	40
/*ϵͳ���òο����ò�ѯ 
//�����ʽ��AT^SYSCFG? 
//��Ӧ��ʽ��
<CR><LF>^SYSCFG:<mode>,<acqorder>,<band>,<roam>,<srvdomain><CR><LF><CR><LF>OK<CR><LF>*/
struct stMsg_Syscfg_Query
{
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucMode;/*ϵͳģʽ�ο��� 
	                      2 �Զ�ѡ��
	                      13     GSM ONLY
	                      14     WCDMA ONLY
	                      16     �ޱ仯*/
	unsigned char ucAcqorder;/*����������ο���
	                         0    �Զ�
	                         1    ��GSM��WCDMA
	                         2    ��WCDMA��GSM
	                         3     �ޱ仯*/
	char acBand[HSPA_BAND_LENGTH];/*Ƶ��ѡ�񣬺�ģʽ��ѡ������صģ�����ȡ���ڵ������ܣ������Ϊ16���ƣ�
	                      ȡֵΪ���и�����������0x3FFFFFFF��0x40000000����ĸ���������ֵ��
	                        80��CM_BAND_PREF_GSM_DCS_1800��               GSM DCS systems
	                        100��CM_BAND_PREF_GSM_EGSM_900��             Extended GSM 900
	                        200��CM_BAND_PREF_GSM_PGSM_900��             Primary GSM 900
	                        200000��CM_BAND_PREF_GSM_PCS_1900��            GSM PCS
	                        400000��CM_BAND_PREF_WCDMA_I_IMT_2000��   WCDMA IMT 2000
	                        3FFFFFFF��CM_BAND_PREF_ANY��           �κ�Ƶ��
	                        40000000��CM_BAND_PREF_NO_CHANGE��     Ƶ�����仯     */
	                         
	unsigned char ucRoam;/*����֧�֣�
	                         0 ��֧��
	                         1 ��������
	                         2   �ޱ仯*/
	unsigned char usSrvDomain;/*�����ã�
	                         0 CS_ONLY
	                         1 PS_ONLY
	                         2         CS_PS
	                         3         ANY
	                         4         �ޱ仯*/
	unsigned char ucExtern1;
	unsigned char ucExtern2;
};

//#define	SYSCONFIG_SET	41
/*ϵͳ���òο����� 
�����ʽ��AT^SYSCONFIG =<mode>,<acqorder>,<roam>,<srvdomain> 
��Ӧ��ʽ��<CR><LF>OK<CR><LF>*/ 
struct stMsg_Sysconfig_Set
{
	unsigned char ucCmdFrom;  
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
};

//#define	SYSCONFIG_QUERY	42
/*ϵͳ���òο����ò�ѯ 
�����ʽ��AT^SYSCONFIG? 
��Ӧ��ʽ��
<CR><LF>^SYSCONFIG:<mode>,<acqorder>,<roam>,<srvdomain><CR><LF><CR><LF>OK<CR><LF>*/
struct stMsg_Sysconfig_Query
{   
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucMode;/*ϵͳģʽ�ο��� 
	                       <2>    �Զ�ѡ��
	                       <13>   GSM ONLY
	                       <14>   WCDMA ONLY
	                       <16 >  �ޱ仯*/
	unsigned char ucAcqorder;/*����������ο���
	                        <0>   �Զ�
	                        <1>    ��GSM��WCDMA
	                        <2>  ��WCDMA��GSM
	                        <3>    �ޱ仯 */
	unsigned char ucRoam;/*����֧�֣�
	                       <0> ��֧��
	                       <1> ��������
	                       <2>  �ޱ仯*/
	unsigned char usSrvDomain;/*�����ã�
	                       <0> CS_ONLY
	                       <1> PS_ONLY
	                       <2>   CS_PS
	                       <3>   ANY
	                       <4>   �ޱ仯 */
	unsigned char ucExtern1;
	unsigned char ucExtern2;
};

//#define	PDP_SET	51
/*PDP���û��� 
�����ʽ��
AT+CGDCONT=<cid>[,<PDP_type>[,<APN>[,<PDP_addr>[,<d_comp>[,<h_comp>]]]]] 
��Ӧ��ʽ��<CR><LF>OK<CR><LF> */
struct stMsg_PDP_Set
{
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
};

//#define	PDP_QUERY	52
/*PDP���û�����ѯ
�����ʽ��AT+CGDCONT? 
��Ӧ��ʽ��<CR><LF>+CGDCONT: <cid>, <PDP_type>, <APN>,<PDP_addr>, <d_comp>, <h_comp>[<CR><LF>+CGDCONT: <cid>, <PDP_type>, <APN>,<PDP_addr>, <d_comp>, <h_comp>[...]]<CR><LF><CR><LF>OK<CR><LF> */
struct stMsg_PDP_Query
{      
	unsigned char ucCmdFrom;      
	unsigned char ucResult;//0-�ɹ���������ʧ�ܣ�������Ϣ�ṹ�嶼�����
	unsigned char ucCidIndex;//1��16��PDP���û���������ֵ������PDP��ص��������ͨ��������ֵ�����ñ�������á�
	unsigned char ucDComp;//����ֵ������PDP���ݵ�ѹ����  0����ѹ����  1��ѹ����
	unsigned char ucHComp;//����ֵ������PDPͷ��ѹ���� 0����ѹ����  1��ѹ����
	unsigned char ucExtern1;
	unsigned char ucExtern2;
	unsigned char ucExtern3;
	char acProtocolKind[HSPA_PROTOCOL_KIND_LENGTH];//�ַ���ֵ����ʾ������Э�����͡�
	char acAPN[HSPA_APN_NAME_LENGTH];//�ַ���ֵ����ʾ����GGSN���ⲿ���Ľ����������
	char acDomainName[HSPA_DOMAIN_ADDR_LENGTH];//�ַ���ֵ����ʾMS�ĵ�ַ��
};

//#define	PIN_PASSWORD_MODIFY	45
/*PIN�޸����� 
�����ʽ��AT+CPWD=<fac>,<oldpwd>,<newpwd>
��Ӧ��ʽ��<CR><LF>OK<CR><LF>
��MS��ش���ʱ��<CR><LF>+CME ERROR: <err><CR><LF>*/
struct stMsg_Pin_Password_Modify
{
	unsigned char ucCmdFrom;      
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
};

//#define	PIN_ENABLE	48
/*PIN��ʹ�� 
�����ʽ��AT+ CLCK =<fac>,<mode>[,<passwd>[,<class>]]
��Ӧ��ʽ���� <mode>=2������ɹ�ִ��ʱ��<CR><LF>+CLCK: <status><CR><LF><CR><LF>OK<CR><LF>
��<mode>��2��ִ�гɹ�ʱ��<CR><LF>OK<CR><LF>
��MS��ش���ʱ��<CR><LF>+CME ERROR: <err><CR><LF>*/
struct stMsg_Pin_Password_Verification
{
	unsigned char ucCmdFrom;      
	unsigned char ucResult;
	unsigned char ucStatus;// 0��δ���� 1������ other:��Ч��ֵ�������
	unsigned char ucExtern1;

};

/*add by w00135351 20080908*/
/*PIN��ʹ�� 
�����ʽ��AT+ CLCK =<fac>,<mode>[,<passwd>[,<class>]]
��<mode>��2��ִ�гɹ�ʱ��
<CR><LF>OK<CR><LF>
��MT��ش���ʱ��
<CR><LF>+CME ERROR: <err><CR><LF>
mode=0 ����;mode=1 ��
*/
struct stMsg_Pin_Enable   /*��ʱֻ��������Ԫ�أ��Ժ������Ҫ��������*/
{
	unsigned char ucCmdFrom;      
	unsigned char ucResult;
};
/*end by w00135351 20080908*/

//#define	PIN_MANAGE	49
/*PIN����
�����ʽ��
AT+CPIN=<pin>[,<newpin>] 
AT^CPIN=<pin>[,<newpin>] 
��Ӧ��ʽ��<CR><LF>OK<CR><LF>
��MS��ش���ʱ��
<CR><LF>+CME ERROR: <err><CR><LF>*/
struct stPin_Verfication_Modify
{
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
};

//#define	DIAL	55
/*���з��� 
�����ʽ��ATD[digits][I/i][;] 
��Ӧ��ʽ��<CR><LF>OK<CR><LF>ִ�д���<CR><LF>NO CARRIER<CR><LF>*/
struct stVoiceMsg_Dial
{
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
};

//#define	DIAG_PORT_SET	56
/*DIAG���л����� 
�����ʽ��AT^DDSETEX=2     
��Ӧ��ʽ��<CR><LF>OK<CR><LF>*/
struct stVoice_Msg_Diag_Port_Exchange
{
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
};
//#define	ACCEPT	57
/*���� 
�����ʽ��ATA 
��Ӧ��ʽ��<CR><LF>OK<CR><LF>*/ 
struct stVoiceMsg_Accept
{
	unsigned char ucCmdFrom; 
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
};

//#define	CHUP	58
/*���йҶ� 
�����ʽ��AT+CHUP 
��Ӧ��ʽ��<CR><LF>OK<CR><LF>*/ 
struct stVoiceMsg_Hung
{   
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
};

//#define	CALL_TIME_QUERY	62
/*ͨ��ʱ����ѯ 
�����ʽ��AT^CDUR=<idx> 
��Ӧ��ʽ��<CR<LF>^cdur:<idx>,<duration><CR<LF><CR<LF>OK<CR<LF>*/
struct stVoiceMsg_C
{
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
	unsigned long ulDuration;/*ͨ��ʱ������sΪ��λ*/
};

//#define	AUTO_SIM_STATUS_REPORT	63
/*SIM ״̬�仯ָʾ  
��Ӧ��ʽ��<CR><LF>^SIMST:<sim_state>[,<lock_state>]<CR><LF>*/
struct stMsg_Sim_State
{
	unsigned char ucCmdFrom;
	unsigned char ucResult; 
	unsigned char ucSimState;/*USIM��״̬��ȡֵ���£�
	                             0��     USIM��״̬��Ч
	                             1:      USIM��״̬��Ч  
	                             2��     USIM��CS����Ч
	                             3��     USIM��PS����Ч
	                             4��     USIM��PS+CS�¾���Ч
	                             255�� USIM�������ڣ�����USIM��ȷʵ�����ں���Ϊ��SIMLOCK��ס�����������������
	                                    ��ʱ��Ӧͨ��ucLockState��ȡֵ�ж�USIM������ʵ״̬��*/
	unsigned char ucLockState;/*SIM����LOCK״̬��ȡֵ���£�
	                               0��    SIM��δ��SIMLOCK��������
	                               1��    SIM����SIMLOCK�������� */
};

//#define	RING_REPORT	 64
/*��������ָʾ  
��Ӧ��ʽ��<CR><LF>RING<CR><LF>*/
struct stVoiceMsg_Ring
{  
	unsigned char ucCmdFrom;
	unsigned char ucResult;
};

//#define	CALL_NUM_REPORT	65
/*���������ʾ  
��Ӧ��ʽ��<CR><LF>+CLIP:<number>,<type>,,,,<CLI validity><CR><LF> */
struct stVoiceMsg_Call_Num
{
	unsigned char ucCmdFrom;
	unsigned char ucResult; 
	unsigned char ucTelType;/*�������ͣ�����145��ʾ���ʺ��룬����ȡֵ�ɲμ������ŷ������� +CMGS��һ���У�
	                           SC�����е�type_addr�����ľ��嶨�塣255,CDMA��֧��*/
	unsigned char ucCliFlag;/*0 CLI ��Ч
	                            1 CLI �����з��𷽾ܾ�.
	                            2 ���ڷ�����������ƻ������������CLI�����á�
	                            <type>��<CLI validity>֮�䱣���������ֶΡ�*/
	char acTelNum[HSPA_TELNUM_LENGTH];/*������룬ASCII �ַ����Ϸ����ַ�����������0�� �C ��9��, ��*��, ��//#��*/
};

//#define	CALL_END_REPORT	66
/*ͨ������ָʾ  
//��Ӧ��ʽ��
<CR><LF>^CEND:<call_x>,< duration >,<end_status>[,<cc_cause>]<CR><LF>*/
struct stVoice_Msg_Call_End
{
      unsigned char ucCmdFrom;
      unsigned char ucResult;
      unsigned char ucCallId;/*����ID��ECȡֵ0��6*/
      unsigned char ucEndCause;/*���н���ԭ��ECȡֵ255*/
      unsigned long ulDuration;/*ͨ��ʱ������sΪ��λ*/
      unsigned char ucCallCtlCause;/*���п�����Ϣ ECȡֵ255
                                     ע�����Ϊ���������ĺ��н����Ż����ֶ�<cc_cause>���ϱ���
                                     ������ط����ĺ��У���û�еõ��������Ӧ��
                                     ���оͽ�������ʱ�Ͳ�����<cc_cause>�ϱ���*/
};

//#define	CFUN_SET	14
/*����ģʽ��������
�����ʽ AT+CFUN=<fun> 
��Ӧ��ʽ��<CR><LF>OK<CR><LF>��MS��ش���ʱ��<CR><LF>+CME ERROR: <err><CR><LF>
*/

struct stMsg_Operation_Mode_Set
{
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
};
//#define	CFUN_QUERY	15
/*����ģʽ��ѯ����
�����ʽ AT+CFUN�� 
��Ӧ��ʽ��<CR><LF>+CFUN: <fun><CR><LF><CR><LF>OK<CR><LF>��MS��ش���ʱ��<CR><LF>+CME ERROR: <err><CR><LF>*/

struct stMsg_Operation_Mode_Query
{
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucOperationMode;
	unsigned char ucExtern1;
};
//#define	PRODUCT_NAME_QUERY	16
/*��Ʒ���Ʋ�ѯ
�����ʽ��AT+CGMM
��Ӧ��ʽ��<CR><LF> < production_ name><CR><LF><CR><LF>OK<CR><LF>��MS��ش���ʱ��<CR><LF>+CME ERROR: <err><CR><LF>*/
struct stMsg_Product_Name_Query
{
	unsigned char ucCmdFrom; 
	unsigned char ucResult;
	char acProductName[HSPA_PRODUCT_NAME_LENGTH];  //��Ʒ��   
	unsigned char ucExtern1;
	unsigned char ucExtern2;
};
//#define	AUTO_SERVICE_STATUS_REPORT	67
/*����״̬�仯ָʾ ^SRVST��EW ������
�Զ��ϱ���ʽ�� <CR><LF>^SRVST:<srv_status><CR><LF>
������״̬�����仯ʱ��MS�����ϱ��µķ���״̬��TE��*/
struct stMsg_Service_Status
{ 
	unsigned char ucCmdFrom;
	//��Ϣ����
	unsigned char ucResult;
	unsigned char ucSrvStatus;/*ϵͳ����״̬��ȡֵ���£�
	                       0:  �޷���
	                       1:  �����Ʒ���
	                       2:  ������Ч 
	                       3:  �����Ƶ��������
	                       4:  ʡ�����˯״̬*/
	unsigned char ucExtern1;
};
//#define	AUTO_SYSTEM_MODE_STATUS_REPORT	68
/*ϵͳģʽ�仯�¼�ָʾ ^MODE
��Ӧ��ʽ��^MODE:<sys_mode>[,<sys_submode>]*/
struct stMsg_System_Mode
{ 
	unsigned char ucCmdFrom;
	//��Ϣ����
	unsigned char ucResult;
	unsigned char ucSysMode;/*ϵͳģʽ
		                       0���޷���
		                       1��AMPSģʽ���ݲ�ʹ�ã�
		                       2��CDMAģʽ���ݲ�ʹ�ã�
		                       3��GSM/GPRS ģʽ
		                       4��HDRģʽ
		                       5��WCDMAģʽ
		                       6��GPSģʽ*/
	unsigned char ucSubSysMode;/*ϵͳģʽ����ģʽ��ȡֵ���£�
				      0���޷���
				      1��GSMģʽ
				      2��GPRSģʽ
				      3��EDGEģʽ
				      4��WCDMAģʽ
				      5��HSDPAģʽ
				      6��HSUPAģʽ
				      7��HSDPA&HSUPAģʽ*/
};
/*PIN��������AT^CPIN?
��Ӧ��ʽ��^CPIN: <code>,[<times>],<puk_times>,<pin_times>,<puk2_times>,<pin2_times><CR><LF><CR><LF>OK<CR><LF>*/
struct stMsg_Pin_Time_Query
{ 
      unsigned char ucCmdFrom;
      unsigned char ucResult;
    unsigned char ucType;
    unsigned char ucTimes;/*ʣ����������������PIN��PIN2��������������3�Σ�����PUK��PUK2������������10�Ρ�
˵�������������������������<times>�ֶλ������ǰ��ҪУ��������ʣ��У��������������������������������ֶ�Ϊ�ա�
*/
    unsigned char ucPukTimes; /*<puk_times>��ʣ���PUK���������������������10�Ρ�*/
    unsigned char ucPinTimes; /*<pin_times>��ʣ���PIN���������������������3��*/
    unsigned char ucPuk2Times; /*<puk2_times>��ʣ���PUK2���������������������10�Ρ�*/
    unsigned char ucPin2Times;  /*<pin2_times>��ʣ���PIN2���������������������3��*/
};
/*
<CR><LF>^CARDMODE:<sim_type><CR><LF><CR><LF>OK <CR><LF>
<sim_type>��SIM�������ͣ�ȡֵ���£�
0:  Unknown mode
1:  SIM��
2:  USIM��*/
struct stMsg_Sim_Type
{
      unsigned char ucCmdFrom;
      unsigned char ucResult;
      unsigned char ucSimType;
	unsigned char ucExtern1;
};

/* HUAWEI HGW s48571 2008��2��15�� ��ȡSN���� add begin:*/
/*
SN��ѯ 
�����ʽ��AT^SN 
��Ӧ��ʽ��<CR><LF><SN><CR><LF><CR><LF>OK<CR><LF>��MS��ش���ʱ��<CR><LF>+CME ERROR: <err><CR><LF>*/
struct stMsg_SN_Query
{ 
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucExtern1;
	unsigned char ucExtern2;
	char acSN[HSPA_SN_LENGTH];//�����SNֵ,Ϊһ�ַ���   
};

/* HUAWEI HGW s48571 2008��2��15�� ��ȡSN���� add end.*/
struct stMsg_CGACT_Query
{
	unsigned char ucCmdFrom;
	unsigned char ucResult;
	unsigned char ucCid;
	unsigned char ucState;
};


/*������HSPA���ƽ��̵���Ϣ�ṹ��*/
struct stAtSndMsg
{
	long int lMsgType;//��������Ӧ�ĺ�	
	char acParam[AT_SND_MSG_LENGTH];
};
/*��������HSPA���ƽ��̵���Ϣ�ṹ��*/
struct stAtRcvMsg
{
	long int lMsgType; 
	char acText[AT_RCV_MSG_LENGTH];
};

int AtRcv(int lMsgKey, struct stAtRcvMsg *pstAtRcvMsg, int lMsgType);
int AtSend(int lMsgKey, struct stAtSndMsg *pstAtSndMsg, char cFrom,  int lMsgType,  const char *pszParam);




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __TTYUSB_H__ */




