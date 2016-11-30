#ifndef __CWMPAPPAPI_H__
#define __CWMPAPPAPI_H__

#include "cfg_typedef.h"
#include "xml_interface.h"
#include "cwmpstkapi.h"

#define ENABLE_TRANSFER
#define _CWMP_DEBUG_

#define ATP_CBBID_CWMP_NAME             "cwmp"

//#define CFG_STATE_DATA_START            (256)
#define CWMP_STK_STATE_LEN              (2048)

#define CWMP_CERTIFICATE_DEFALUT_PATH   "/etc/certdefault.crt"
#define CWMP_CERTIFICATE_PATH           "/var/cert.crt"
//#define CFG_STATE_SSL_STAT              (4096)
//#define CWMP_SSL_CERT_LEN               (2048)

//#define UPG_INFO_STATUS                  (8)

#define CWMP_TRANSFER_CFGFILE_PATH_D        "/var/psi.xml"
#define CWMP_TRANSFER_FIRMWARE_PATH_D       "/var/firm.bin"
#define CWMP_TRANSFER_LOG_PATH_D            "/var/log.txt"
#define CWMP_TRANSFER_SSL_PATH_D            "/var/ssl.cert"
/*add by z67625 ����ǽ��־�ļ�·������ start*/
#define CWMP_TRANSFER_FWLOG_PATH_D          "/var/fw_log"
/*add by z67625 ����ǽ��־�ļ�·������ end*/
 /*start of ��der ��ʽ��֤��ת����pem��ʽ��֤�� by s53329  at  20080428 */
#define CWMP_DEM_SSL_PATH_D                          "/var/derssl.crt"
 /*end of ��der ��ʽ��֤��ת����pem��ʽ��֤�� by s53329  at  20080428 */
/*start of VDF 2008.5.24 V100R001C02B015 j00100803 AU8D00658 */
#define WAN_NUM_MAX 8
typedef struct tagWanIfc
{
    char szWanIfcName[256];
    char szWanIfcIp[16];
}WANIFCINFO;
extern WANIFCINFO gl_szWanInterface[WAN_NUM_MAX];
/*start of VDF 2008.5.24 V100R001C02B015 j00100803 AU8D00658 */
// �������ص��ļ�����ö�ٶ���
typedef enum tagATP_CWMP_TRANS_FILE_TYPE
{
    CWMP_FILE_TYPE_FIRMWARE_IMAGE          = 0,
    CWMP_FILE_TYPE_WEB_CONTENT             = 1,
    CWMP_FILE_TYPE_CFG_FILE                = 2,
    CWMP_FILE_TYPE_UPLOAD_CFG_FILE         = 3,
    CWMP_FILE_TYPE_UPLOAD_LOG_FILE         = 4,
    CWMP_FILE_TYPE_ENCRYPTED_CFG_FILE      = 5,
    CWMP_FILE_TYPE_SSL_CERTIFICATE         = 6,
    /*add by z67625 ����ǽ��־�ļ����Ͷ��� start*/
    CWMP_FILE_TYPE_UPLOAD_FWLOG_FILE       = 7,
    /*add by z67625 ����ǽ��־�ļ����Ͷ��� end*/
    CWMP_FILE_TYPE_NONE
} ATP_CWMP_TRANS_FILE_TYPE_EN;

/*Cwmp��������*/

typedef enum tagATP_CWMP_ERR_CODE
{
    ATP_ERR_CWMP_START ,
    ATP_ERR_CWMP_PARA_INVALID,
    ATP_ERR_CWMP_MEM_NOT_ENOUGH,
    ATP_ERR_CWMP_INVALID_FORMAT,
    ATP_ERR_CWMP_TR069_NOT_INITED,
    ATP_ERR_CWMP_SYSTEM_CALL,

    ATP_ERR_CWMP_RPC_NOT_FOUND,
    ATP_ERR_CWMP_INVALID_EVT,
    ATP_ERR_CWMP_TOO_MANY_EVTS,
    ATP_ERR_CWMP_INVALID_RPC,
    ATP_ERR_CWMP_SCHEMA_RPC,
    ATP_ERR_CWMP_END
} ATP_CWMP_ERR_CODE_EN;

typedef enum
{
    CWMP_DEBUG_NO,		/* ������κε�����Ϣ */
    CWMP_DEBUG_IMPORT,	/* �����Ҫ�ĵ�����Ϣ */
    CWMP_DEBUG_NORMAL,	/* �����Ҫ/��Ҫ�ĵ�����Ϣ */
    CWMP_DEBUG_ALL 		/* �����Ҫ/��Ҫ/һ����Ե�����Ϣ */
} CWMP_DEBUG_LEVEL;

#ifdef _CWMP_DEBUG_
#define CWMP_DEBUG(x)  x;
/*------------------------------------------------------------
  ����ԭ��: VOID CWMP_Debug(CWMP_DEBUG_LEVEL DebugLev,CHAR *pcDbgFileName,
                                                            ULONG ulDbgLineNo, CHAR const* pcFmtStr, ...);
  ����:
  ����:
  ���:
  ����ֵ:
-------------------------------------------------------------*/
VOID CWMP_Debug(CWMP_DEBUG_LEVEL DebugLev,CHAR *pcDbgFileName,
                                                            ULONG ulDbgLineNo, CHAR const* pcFmtStr, ...);

#else
#define CWMP_DEBUG(x)
#endif

/* �ļ�����Э�� */
typedef enum tagATP_CWMP_TRANS_TYPE_EN
{
    ATP_TRANS_TYPE_HTTPS,
    ATP_TRANS_TYPE_HTTP,
    ATP_TRANS_TYPE_FTP,
    ATP_TRANS_TYPE_TFTP,
    ATP_TRANS_TYPE_MAX
} ATP_CWMP_TRANS_TYPE_EN;

/* �ļ����䷽�� */
typedef enum tagATP_CWMP_TRANS_DIR_EN
{
	 ATP_TRANS_DIR_UPLOAD,
	 ATP_TRANS_DIR_DOWNLOAD,
	 ATP_TRANS_DIR_BOTH
} ATP_CWMP_TRANS_DIR_EN;

#define ATP_TRANS_OK                (0)
#define ATP_TRANS_TIMEOUT           (0xF1)
#define ATP_TRANS_FILE_ERR          (0xF2)
#define ATP_TRANS_SYS_ERR           (0xF3)
#define ATP_TRANS_AUTH_ERR          (0xF4)

/* ϵͳ�����ⲿ���� */
typedef VOID (*PFAtpCwmpReboot)();

/* �ָ������ⲿ���� */
typedef VOID (*PFAtpCwmpFactoryReset)();

/* ���������ļ����� */
typedef ULONG (*PFATPCWMPUpgradeImage)(
                                                        const CHAR *pcImage,
                                                        ULONG ulImageLen);
/* ������־�ļ����� */
typedef ULONG (*PFATPCWMPLogGen)(const CHAR *pcLogFileName);


ULONG ATP_CWMP_SetUpgradeImageFunc(
                                            PFATPCWMPUpgradeImage pfUpgrade);

ULONG ATP_CWMP_SetLogGenFunc(PFATPCWMPLogGen pfLogGen);


ULONG ATP_CWMP_Init(PFAtpCwmpReboot			    pfReboot, PFAtpCwmpFactoryReset	pfReset,
                            PFAtpCwmpReadStkState	pfReadState,PFAtpCwmpWriteStkState	pfWriteState);

ULONG ATP_CWMP_Uninit();

ULONG ATP_CWMP_Start();

ULONG ATP_CWMP_RegExtClients(
                                const CHAR          *pcClient,
                                ATP_CWMP_TRANS_TYPE_EN  eTransType,
                                ATP_CWMP_TRANS_DIR_EN   eTransDir);

ULONG ATP_CWMP_GetVersion(
                                CHAR                *pszVersion,
                                ULONG              ulLen);

/* BEGIN: Modified by w00135358, 2010/02/21: TR069 FIRMWARE UPGRADE Fault Code-ACS���������ϱ�9010����*/
ULONG CwmpWriteLoadInfoToFlash(CHAR *pcCmdkey, ULONG ulErrCode);
/* END: Modified by w00135358, 2010/02/21: TR069 FIRMWARE UPGRADE Fault Code-ACS���������ϱ�9010����*/

#endif

LONG ATP_CWMP_VerifyCert(CHAR *certfile, ULONG ulLen);


