/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : cwmpstkapi.h
  ��    ��  : handy
  ��    ��  : V1.0
  ��������  : 2007-12-30
  ��    ��  : CWMPЭ��ջ����ͷ�ļ�
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

*********************************************************************************/

#ifndef __CWMP_STK_API_H__
#define __CWMP_STK_API_H__
#if 0
#include "atperrdef.h"
#include "atputil.h"
#endif
#include "xml_interface.h"

#ifdef ATP_CWMP_ENABLE_SSL
#include <openssl/x509v3.h>
#include <openssl/ssl.h>
#endif
#ifdef USE_CWMP_TRACE
VOS_UINT32 ATP_CWMP_Outputf(const CHAR *app, ULONG level,
                                     ULONG type, ULONG logno,
                                     const CHAR *file, ULONG line,
                                     const CHAR *fmt, ...);

#define ATP_CWMP_TRACE              ATP_CWMP_Outputf

#ifdef DEBUG
#define ATP_CWMP_DEBUG              ATP_CWMP_Outputf
#else
#define ATP_CWMP_DEBUG(x...)
#endif

#ifdef DEBUG
VOS_UINT32 ATP_CWMP_Outputf(const CHAR *app, ULONG level,
                                     ULONG type, ULONG logno,
                                     const CHAR *file, ULONG line,
                                     const CHAR *fmt, ...);

#define ATP_CWMP_DEBUG              ATP_CWMP_Outputf
#else
#define ATP_CWMP_DEBUG(x...)
#endif

#endif


#define ATP_CWMP_ENABLE_HTTP_AUTH

/* log level */
typedef enum vtopLOG_E_LEVEL
{
    LOG_LEVEL_FATAL = 0,    /* just for compatibility with previous version */
    LOG_LEVEL_ALERT,        /* action must be taken immediately             */
    LOG_LEVEL_CRIT,         /* critical conditions                          */
    LOG_LEVEL_ERROR,        /* error conditions                             */
    LOG_LEVEL_WARNING,      /* warning conditions                           */
    LOG_LEVEL_NOTICE,       /* normal but significant condition             */
    LOG_LEVEL_INFO,         /* informational                                */
    LOG_LEVEL_DEBUG,        /* debug-level                                  */
    LOG_LEVEL_BUTT
}LOG_E_LEVEL;

/* log type */
typedef enum vtopLOG_E_TYPE
{
    LOG_TYPE_OPERATION = 1,
    LOG_TYPE_SECURATY  = 3,
    LOG_TYPE_SYSTEM    = 4,
    LOG_TYPE_DEBUG     = 23,
    LOG_TYPE_BUTT
}LOG_E_TYPE;

/*
 *  Э��ջ������
 */
#if 0 
typedef enum tagATP_CWMP_STK_ERR_CODE
{
    ATP_ERR_CWMP_STK_START= 0; 
    ATP_ERR_CWMP_STK_PARA_INVALID,
    ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH,
    ATP_ERR_CWMP_STK_INVALID_FORMAT,
    ATP_ERR_CWMP_STK_TR069_NOT_INITED,
    ATP_ERR_CWMP_STK_ALREADY_STARTED,
    ATP_ERR_CWMP_STK_NOT_STARTED,
    ATP_ERR_CWMP_STK_UNRESOLVED_HOST,

    ATP_ERR_CWMP_STK_RPC_NOT_FOUND,
    ATP_ERR_CWMP_STK_INVALID_EVT,

    ATP_ERR_CWMP_STK_NET_SOCK 
    ATP_ERR_CWMP_STK_NET_CONN,
    ATP_ERR_CWMP_STK_NET_DNS,
    ATP_ERR_CWMP_STK_NET_RECV,
    ATP_ERR_CWMP_STK_NET_SEND,
    ATP_ERR_CWMP_STK_NET_HEADER,
    ATP_ERR_CWMP_STK_NET_HTTP,
    ATP_ERR_CWMP_STK_NET_SEC,
    ATP_ERR_CWMP_STK_NET_SSL,
    ATP_ERR_CWMP_STK_NET_AUTH,
    ATP_ERR_CWMP_STK_SOAP_XML,
    ATP_ERR_CWMP_STK_SOAP_ID,
    ATP_ERR_CWMP_STK_SOAP_FAULT
} ATP_CWMP_STK_ERR_CODE_EN;
#endif
/*
 *  CWMP�����ÿ�ֲ�������
 */
typedef enum
{
    ATP_CWMP_VALUE_TYPE_ULONG       = 0,
    ATP_CWMP_VALUE_TYPE_LONG        = 1,
    ATP_CWMP_VALUE_TYPE_BOOL        = 2,
    ATP_CWMP_VALUE_TYPE_DATE        = 3,
    ATP_CWMP_VALUE_TYPE_BASE64      = 4,
    ATP_CWMP_VALUE_TYPE_CHAR        = 5
} ATP_CWMP_ATTR_VALUE_TYPE_E;

/*
typedef enum
{
    VALUE_TYPE_MASK        = 0xFF00,
    VALUE_TYPE_ULONG       = 0x100,
    VALUE_TYPE_LONG        = 0x200,
    VALUE_TYPE_BOOL        = 0x300,
    VALUE_TYPE_DATE        = 0x400, //�ַ������͵�ö��ֵ�������VALUE_TYPE_DATE֮��
    VALUE_TYPE_BASE64      = 0x500,
    VALUE_TYPE_CHAR        = 0x600
}ATTR_VALUE_TYPE_E;
 */
/* TSP CFM���������Ͷ����CWMP��������֮���ת���� */
#define TSP_VALUETYPE_TO_CWMP(x)    (((x)>>8) - 1)


/*
 *  CWMPЭ��ջ��ע����ⲿ���亯���ӿ�
 */

/* ��Flash��ȡЭ��ջ״̬��Ϣ���ⲿ������
   Э��ջ������ulLength���ȵ��ַ�����
   pulUpgResָ���ϴ�������(VOS_TRUE)��(VOS_FALSE)�ɹ� */
typedef VOS_VOID (*PFAtpCwmpReadStkState)(VOS_CHAR *pcState,
                                                        VOS_UINT32 ulLength,
                                                        VOS_UINT32 *pulUpgRes);

/* ��Э��ջ״̬��Ϣд�뵽Flash���ⲿ������Э��ջ��������ַ����ͳ��� */
typedef VOS_VOID (*PFAtpCwmpWriteStkState)(VOS_CHAR *pcState,
                                                        VOS_UINT32 ulLength);

/* Inform��չ��������Э��ջ����TR069Э�鹹���Inform���ĺ�
   ��Ʒע��˺������Զ�Inform������չ */
typedef VOS_UINT32 (* PFAtpCwmpInformHook)(NODEPTR hInformSoap);

/*Start of ά�������� 2008-3-20 15:31 for C02��ǿSession���ƹ��� by handy*/
#if 0
/* �Ự������������Э��ջ���жϸô������ķ���ֵ���ɴ��������� */
typedef VOS_UINT32 (*PFCwmpOnSessionEnd)(VOS_VOID *pvArg1,
                                                      VOS_VOID *pvArg2);
#else
typedef enum tagATP_CWMP_SESS_CTRL_TYPE_EN
{
	ATP_CWMP_SESS_START,		// �Ự��ʼ�ص�����
	ATP_CWMP_SESS_END,		    // �Ự����ʱ�ص�����(���۳ɹ�����ʧ��)
	ATP_CWMP_SESS_MAX
} ATP_CWMP_SESS_CTRL_TYPE_EN;

/* �Ự���ƺ�����Э��ջ���жϸô������ķ���ֵ */
typedef VOS_UINT32 (*PFCwmpSessionCtrlFunc)(
                                            VOS_UINT32  ulErrCode,
                                            VOS_VOID    *pvArg1,
                                            VOS_VOID    *pvArg2);
#endif
/*End of ά�������� 2008-3-20 15:31 for C02��ǿSession���ƹ��� by handy*/

/* ��Ʒ���Ƶ�������������������ֵ��ʾ��(VOS_TRUE)��(VOS_FALSE)�����ɹ� */
typedef VOS_BOOL (*PFCwmpNameResolveFunc)(
                    const VOS_CHAR  *pcName,        // Э��ջ������������
                    VOS_UINT32      ulLocalIP,      // Э��ջ���󶨵�IP
                    VOS_UINT8       **ppucIPs,      // �������IP��ַ
                    VOS_UINT16      *pusIPCnt);     // �������IP��ַ����

/*
 *  Э��ջ������������
 */
/******************************************************************************
  ��������  : ATP_CWMP_StkInit
  ��������  : Э��ջ��ʼ���ӿ�
  �������  :
              1. pfReadState    :   �ⲿʵ�ֵĶ�ȡFlash�ӿ�
              2. pfWriteState   :   �ⲿʵ�ֵ�дFlash�ӿ�
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32  ATP_CWMP_StkInit(PFAtpCwmpReadStkState           pfReadState,
                                   PFAtpCwmpWriteStkState    pfWriteState);

/******************************************************************************
  ��������  : ATP_CWMP_StkStart
  ��������  : ����CWMPЭ��ջ������ǰ��������Э��ջ�󶨵�IP
  �������  :
              ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_ALREADY_STARTED

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32  ATP_CWMP_StkStart();

/******************************************************************************
  ��������  : ATP_CWMP_StkStop
  ��������  : ֹͣCWMPЭ��ջ
  �������  :
              ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_NOT_STARTED

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32  ATP_CWMP_StkStop();

/*
 *  Э��ջSession���ƽӿ�
 */
/******************************************************************************
  ��������  : ATP_CWMP_StkIsBusy
  ��������  : �ж�Э��ջ��ǰ�Ƿ��лỰ
  �������  :
              ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_TRUE:  ��ǰ�Ự��û�н���
              VOS_FALSE: ��ǰû�лỰ

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_BOOL  ATP_CWMP_StkIsBusy();

/******************************************************************************
  ��������  : ATP_CWMP_StkStartSession
  ��������  : ����һ�λỰ���ú���һ����ɹ������Э��ջæ����û������ʱ��
              ��Э��ջ��æ���������󣬻�����һ�λỰ
  �������  :
              ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : VOS_OK

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32  ATP_CWMP_StkStartSession();

/******************************************************************************
  ��������  : ATP_CWMP_PauseSession
  ��������  : ��ͣЭ��ջ��Session�����øýӿں�Э��ջ����ɵ�ǰ�Ự��
              �����ٷ����κλỰ��������⣬�벻Ҫ����ʹ�ñ��ӿڡ�
  �������  :
              ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-1-31
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_VOID ATP_CWMP_PauseSession();

/******************************************************************************
  ��������  : ATP_CWMP_ResumeSession
  ��������  : �ָ�Э��ջ��Session�����øýӿں�����лỰ����ͣ��
              Э��ջ�����������µĻỰ�����ATP_CWMP_PauseSession����ʹ�á�
  �������  :
              ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-1-31
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_VOID ATP_CWMP_ResumeSession();

/*
 *  Э��ջIP��ַ�����ӿ�
 */
/******************************************************************************
  ��������  : ATP_CWMP_SetLocalIP
  ��������  : ����Э��ջ�󶨵ı���IP��ַ����ע�����ⲿ��������ʱ��
              ���ӿڻ����ȵ����ⲿע��Ľ�����������IP
              ���û��ע���ⲿ���������������ⲿ������������ʧ�ܣ�
              ���ʹ��Ĭ��·�ɽ�������������Խ���ʧ�ܣ�
              �򷵻�ATP_ERR_CWMP_STK_UNRESOLVED_HOST
  �������  :
              1. pcIP:  ��ָ�ʽ��IP��ַ
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_UNRESOLVED_HOST

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32  ATP_CWMP_SetLocalIP(const VOS_CHAR *pcIP);

/******************************************************************************
  ��������  : ATP_CWMP_GetLocalIP
  ��������  : ��ȡ���ص�IP��IP��ַ�������ֽ�����
  �������  :
              1. pulLocalIP :   ��Ž����󱾵�IP��ַ��ָ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_GetLocalIP(VOS_UINT32           *pulLocalIP);

/******************************************************************************
  ��������  : ATP_CWMP_SetConReqPort
  ��������  : ��ȡ���صĻ����˿�
  �������  :
              1. usLocalPort    :   �����õĻ����˿�
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID

  �޸���ʷ      :
   1.��    ��   : 2008-5-29
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetConReqPort(VOS_UINT16           usLocalPort);

/******************************************************************************
  ��������  : ATP_CWMP_GetRemoteIP
  ��������  : ��ȡACS�������������IP��ַ�������ֽ���
  �������  :
              1. pulRemoteIP    :   ���ACS������������IP��ַ��ָ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_GetRemoteIP(VOS_UINT32           *pulRemoteIP);

/*
 *  ע��ص������ӿ�
 */
/*Start of ά�������� 2008-3-20 15:32 for C02��ǿ�Ự���ƹ��� by handy*/
#if 0
/******************************************************************************
  ��������  : ATP_CWMP_RegEndSessionFunc
  ��������  : ע��Ự������������ע��ĺ���ֻ������һ��
  �������  :
              1. pfFunc :   �Ự�����ص�����ָ�룬����Ϊ�ա�
              2. pvArg1 :   �ص������Ĳ�������ע�����Լ������ڴ棬
                            ���ڻص��������ͷ��ڴ档
              3. pvArg2 :   �ص������Ĳ�������ע�����Լ������ڴ�,
                            ���ڻص��������ͷ��ڴ档
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32  ATP_CWMP_RegEndSessionFunc(PFCwmpOnSessionEnd pfFunc,
                                                   VOS_VOID           *pvArg1,
                                                   VOS_VOID           *pvArg2);
#else
/******************************************************************************
  ��������  : ATP_CWMP_RegSessionCtrlFunc
  ��������  : ע��Ự���ƺ����������Ự��ʼ�ͻỰ����������
  �������  :
              1. pfFunc             :   �Ự���ƻص�����ָ�룬����Ϊ�ա�
              2. enFuncType         :   �Ự���ƺ���������
              3. bDeleteByCurSess   :   �ûỰ���ƺ����Ƿ���Ҫ����ǰ�Ựɾ��
              2. pvArg1             :   �ص������Ĳ�������ע�����Լ������ڴ棬
                                        ���ڻص��������ͷ��ڴ档
              3. pvArg2             :   �ص������Ĳ�������ע�����Լ������ڴ�,
                                        ���ڻص��������ͷ��ڴ档
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32  ATP_CWMP_RegSessionCtrlFunc(
                            PFCwmpSessionCtrlFunc           pfFunc,
                            ATP_CWMP_SESS_CTRL_TYPE_EN      enFuncType,
                            VOS_BOOL                        bDeleteByCurSess,
                            VOS_VOID                        *pvArg1,
                            VOS_VOID                        *pvArg2);

/******************************************************************************
  ��������  : ATP_CWMP_RegEndSessionFunc
  ��������  : ע��Ự������������ע��ĺ���ֻ������һ��
  �������  :
              1. pfFunc :   �Ự�����ص�����ָ�룬����Ϊ�ա�
              2. pvArg1 :   �ص������Ĳ�������ע�����Լ������ڴ棬
                            ���ڻص��������ͷ��ڴ档
              3. pvArg2 :   �ص������Ĳ�������ע�����Լ������ڴ�,
                            ���ڻص��������ͷ��ڴ档
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
#define ATP_CWMP_RegEndSessionFunc(pfFunc, pvArg1, pvArg2)  ATP_CWMP_RegSessionCtrlFunc((pfFunc), ATP_CWMP_SESS_END, VOS_TRUE, (pvArg1), (pvArg2))
#endif
/*End of ά�������� 2008-3-20 15:32 for C02��ǿ�Ự���ƹ��� by handy*/

/******************************************************************************
  ��������  : ATP_CWMP_RegNameResolveFunc
  ��������  : ע����������Ļص���������ε��������һ��Ϊ׼
  �������  :
              1. pfFunc :   ����Ϊ�գ�Ϊ��ʱ��ʹ��Ĭ��·�ɽ�����������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32  ATP_CWMP_RegNameResolveFunc(PFCwmpNameResolveFunc pfFunc);

/******************************************************************************
  ��������  : ATP_CWMP_RegInformHooks
  ��������  : ע��Inform���Ĵ���Ļص���������ε��������һ��Ϊ׼
  �������  :
              1. pfInformHook   :   ���Ĵ�����ָ�룬����Ϊ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : VOS_OK

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_RegInformHooks(
                                PFAtpCwmpInformHook     pfInformHook);

/*
 *  ����������
 */
/******************************************************************************
  ��������  : ATP_CWMP_StkSetAcsUrl
  ��������  : ����ACS��ַ
  �������  :
              1. pcAcsUrl   :   ACS��ַ������Ϊhttp��https��ʽ�����ܳ���255�ֽ�
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_NET_SSL
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_StkSetAcsUrl(const VOS_CHAR *pcAcsUrl);

/******************************************************************************
  ��������  : ATP_CWMP_SetUsername
  ��������  : ����ACS��֤CPEʱ��CPEʹ�õ���֤�û���
  �������  :
              1. pcUsername :   ���õ��û�����������Ϊ�գ���������256�ֽ�
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetUsername(const VOS_CHAR *pcUsername);

/******************************************************************************
  ��������  : ATP_CWMP_SetPassword
  ��������  : ����ACS��֤CPEʱ��CPEʹ�õ���֤����
  �������  :
              1. pcPassword :   ���õ����룬������Ϊ�գ���������256�ֽ�
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetPassword(const VOS_CHAR *pcPassword);

/******************************************************************************
  ��������  : ATP_CWMP_SetConReqUsername
  ��������  : ����CPE��֤ACSʱʹ�õ���֤�û���
  �������  :
              1. pcUsername :   ���õ��û�����������Ϊ�գ���������256�ֽ�
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetConReqUsername(const VOS_CHAR *pcUsername);

/******************************************************************************
  ��������  : ATP_CWMP_SetConReqPassword
  ��������  : ����CPE��֤ACSʱʹ�õ���֤����
  �������  :
              1. pcPassword :   ���õ����룬������Ϊ�գ���������256�ֽ�
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetConReqPassword(const VOS_CHAR *pcPassword);

#ifdef ATP_CWMP_ENABLE_SSL
/******************************************************************************
  ��������  : ATP_CWMP_SetCertEnable
  ��������  : �����Ƿ�����SSL�ͻ���֤����֤
  �������  :
              1. bCertEnable    :   �Ƿ�����
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetCertEnable(VOS_BOOL bCertEnable);

/******************************************************************************
  ��������  : ATP_CWMP_SetCertificate
  ��������  : ���ÿͻ�����֤��֤��·��
  �������  :
              1. pcCertPath :   ���õ�֤��·����������Ϊ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetCertificate(const VOS_CHAR *pcCertPath);
#endif

/******************************************************************************
  ��������  : ATP_CWMP_SetInformEnable
  ��������  : ����ʹ������Inform
  �������  :
              1. bInformEnable  :   �Ƿ���������Inform
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : VOS_OK

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetInformEnable(VOS_BOOL bInformEnable);

/******************************************************************************
  ��������  : ATP_CWMP_SetInformPeriod
  ��������  : ����Inform������
  �������  :
              1. ulInformPeriod :   ���õ�����
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : VOS_OK

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetInformPeriod(VOS_UINT32 ulInformPeriod);

/*
 *  ��ȡ������
 */
/******************************************************************************
  ��������  : ATP_CWMP_GetAcsUrl
  ��������  : ��ȡAcs��ַ��ִ�к�ppcAcsUrlָ��Э��ջ�е��ڴ棬ʹ���߲����ͷ�
  �������  :
              1. ppcAcsUrl  :   ָ��ACS��ַ��ָ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_GetAcsUrl(const VOS_CHAR **ppcAcsUrl);

/******************************************************************************
  ��������  : ATP_CWMP_GetUsername
  ��������  : ��ȡ��֤�û�����ִ�к�ppcUsernameָ��Э��ջ�е��ڴ棬ʹ���߲����ͷ�
  �������  :
              1. ppcUsername    :   ָ����֤�û�����ַ��ָ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_GetUsername(const VOS_CHAR **ppcUsername);

/******************************************************************************
  ��������  : ATP_CWMP_GetPassword
  ��������  : ��ȡ��֤���룬ִ�к�ppcPasswordָ��Э��ջ�е��ڴ棬ʹ���߲����ͷ�
  �������  :
              1. ppcPassword    :   ָ����֤�����ַ��ָ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_GetPassword(const VOS_CHAR **ppcPassword);

/******************************************************************************
  ��������  : ATP_CWMP_GetConReqURL
  ��������  : ����ConnectionRequestURL��ֵ
  �������  : ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ConnectionRequestURL��ֵ��ʹ���߲����ͷŸ�ֵ

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
const VOS_CHAR *ATP_CWMP_GetConReqURL(VOS_VOID);

/******************************************************************************
  ��������  : ATP_CWMP_GetConReqUsername
  ��������  : ��ȡ��֤�û�����ִ�к�ppcUsernameָ��Э��ջ�е��ڴ棬
              ʹ���߲����ͷ�
  �������  :
              1. ppcUsername    :   ָ����֤�û�����ַ��ָ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_GetConReqUsername(const VOS_CHAR **ppcUsername);

/******************************************************************************
  ��������  : ATP_CWMP_GetConReqPassword
  ��������  : ��ȡ��֤���룬ִ�к�ppcPasswordָ��Э��ջ�е��ڴ棬ʹ���߲����ͷ�
  �������  :
              1. ppcPassword    :   ָ����֤�����ַ��ָ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_GetConReqPassword(const VOS_CHAR **ppcPassword);

#ifdef ATP_CWMP_ENABLE_SSL
/******************************************************************************
  ��������  : ATP_CWMP_GetCertEnable
  ��������  : ��ȡSSL֤����֤�Ƿ�ʹ�ܱ�־
  �������  :
              1. pbCertEnable   :   ����Ƿ�ʹ��SSL֤����֤��ָ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_GetCertEnable(VOS_BOOL *pbCertEnable);
#endif

/******************************************************************************
  ��������  : ATP_CWMP_GetInformEnable
  ��������  : ��ȡ�Ƿ�ʹ������Inform��־
  �������  :
              1. pbInformEnable :   ����Ƿ�ʹ������Inform��ָ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_GetInformEnable(VOS_BOOL *pbInformEnable);

/******************************************************************************
  ��������  : ATP_CWMP_GetInformPeriod
  ��������  : ��ȡInform������
  �������  :
              1. pulInformPeriod    :   ���Inform���ڵ�ָ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_GetInformPeriod(VOS_UINT32 *pulInformPeriod);


/*
 *  Э��ջ���ֿռ�ӿ�
 */

/*�������ֿռ�ṹ*/
#define ATP_CWMP_NAMESPACE_LEN_D    (32)

typedef struct tagATP_CWMP_NAME_SAPCE_ST
{
    VOS_CHAR      acSoapEnv[ATP_CWMP_NAMESPACE_LEN_D];
    VOS_CHAR      acSoapEnc[ATP_CWMP_NAMESPACE_LEN_D];
    VOS_CHAR      acSoapXsd[ATP_CWMP_NAMESPACE_LEN_D];
    VOS_CHAR      acSoapXsi[ATP_CWMP_NAMESPACE_LEN_D];
    VOS_CHAR      acCwmp[ATP_CWMP_NAMESPACE_LEN_D];
} ATP_CWMP_NAME_SAPCE_ST;

/******************************************************************************
  ��������  : ATP_CWMP_SetNs
  ��������  : ����Э��ջ��SOAP�����ֿռ�
  �������  :
              1. pstNs  :   ���ֿռ�ṹ��ָ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetNs(const ATP_CWMP_NAME_SAPCE_ST *pstNs);

/******************************************************************************
  ��������  : ATP_CWMP_GetNs
  ��������  : ��ȡЭ��ջ��ǰ��ʹ�õ����ֿռ�ṹ��ָ��
  �������  : ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : Э��ջ��ǰ��ʹ�õ����ֿռ�ṹ��ָ��

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
const ATP_CWMP_NAME_SAPCE_ST *ATP_CWMP_GetNs(VOS_VOID);


/*
 *  RPCע��ӿ�
 */

/* RPC����������ָ�� */
typedef VOS_UINT32 (*PFATPCWMPRpcMethod)(
                        const ATP_CWMP_NAME_SAPCE_ST  *pstNs,
                        NODEPTR                       hRpcRequest,
                        NODEPTR                       *phRpcResponse);

/*Start of ATP 2008-12-3 for AU4D01174 by z65940: CPE��GetRPCMethod response�ķ���ֵ������Э���׼*/
#if 1
/******************************************************************************
  ��������  : ATP_CWMP_RegRPC
  ��������  : ע���ⲿRPC�������ⲿ����
  �������  :
              1. pcRPCName  :   RPC�������ƣ�����Ϊ�գ����ܳ���256�ֽ�
              2. pfMethod   :   RPC����������
              3. bDirection :   RPC��������VOS_TRUE��ʾCPE�ṩ��RPC��
              				VOS_FALSE��ʾACS�ṩ��RPC
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2008-12-3
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_RegRPCEx(const VOS_CHAR				*pcRPCName,
									   PFATPCWMPRpcMethod   		pfMethod,
									   VOS_BOOL						bDirection);
#endif
/*End of ATP 2008-12-3 for AU4D01174 by z65940*/

/******************************************************************************
  ��������  : ATP_CWMP_RegRPC
  ��������  : ע���ⲿRPC�������ⲿ����
  �������  :
              1. pcRPCName  :   RPC�������ƣ�����Ϊ�գ����ܳ���256�ֽ�
              2. pfMethod   :   RPC����������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-7-16
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
#define ATP_CWMP_RegRPC(pcRPCName, pfMethod) ATP_CWMP_RegRPCEx((pcRPCName), (pfMethod), VOS_TRUE)

/*
 *  Inform��Խӿ�
 */

/*
 *  ����DeviceID�ӿ�
 */
typedef struct tagATP_CWMP_DEVICEID_ST
{
    VOS_CHAR            *pcManufacturer;
    VOS_CHAR            *pcOUI;
    VOS_CHAR            *pcProductClass;
    VOS_CHAR            *pcSerialNumber;
} ATP_CWMP_DEVICEID_ST;

/******************************************************************************
  ��������  : ATP_CWMP_SetDeviceID
  ��������  : ����Inform�ϱ���DeviceID�ṹֵ
  �������  :
              1. pstDeviceID    :   ���õ�ֵ
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetDeviceID(const ATP_CWMP_DEVICEID_ST *pstDeviceID);

/*
 *  Inform�ϱ������б�ӿ�
 */

/******************************************************************************
  ��������  : ATP_CWMP_AddParameterEx
  ��������  : ��ʼ��ʱ����ϱ�������ÿ�λỰ�����ϱ��ò�����ֻ���ڳ�ʼ��ʱʹ��
  �������  :
              1. pcParaName     :   �ϱ��Ĳ�������
              2. pcParaValue    :   �ϱ��Ĳ���ֵ
              3. enValueType    :   �ϱ�����ֵ������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-8-20
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_AddParameterEx(
                                const VOS_CHAR              *pcParaName,
                                const VOS_CHAR              *pcParaValue,
                                ATP_CWMP_ATTR_VALUE_TYPE_E  enValueType);

/******************************************************************************
  ��������  : ATP_CWMP_AddParameterEx
  ��������  : ��ʼ��ʱ����ϱ�������ÿ�λỰ�����ϱ��ò�����ֻ���ڳ�ʼ��ʱʹ��
  �������  :
              1. pcParaName     :   �ϱ��Ĳ�������
              2. pcParaValue    :   �ϱ��Ĳ���ֵ
              3. enValueType    :   �ϱ�����ֵ������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-8-20
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
#define ATP_CWMP_AddParameter(pcParaName, pcParaValue) \
            ATP_CWMP_AddParameterEx((pcParaName), (pcParaValue), ATP_CWMP_VALUE_TYPE_CHAR)

/******************************************************************************
  ��������  : ATP_CWMP_AddParaValueChgEx
  ��������  : �����ı�ʱ������ϱ�������ֻ�ڸûỰ���ϱ���һ���ϱ���ɣ�������
  �������  :
              1. pcParaName     :   �ϱ��Ĳ�������
              2. pcParaValue    :   �ϱ��Ĳ���ֵ
              3. enValueType    :   �ϱ�����ֵ������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-8-20
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_AddParaValueChgEx(
                                const VOS_CHAR              *pcParaName,
                                const VOS_CHAR              *pcParaValue,
                                ATP_CWMP_ATTR_VALUE_TYPE_E  enValueType);

/******************************************************************************
  ��������  : ATP_CWMP_AddParaValueChg
  ��������  : ���Inform�ϱ���������Ӻ�ֻ���ϱ�һ�Σ��ϱ�����Զ���ɾ��
              Ĭ���ϱ��Ĳ���ֵ����Ϊstring
  �������  :
              1. pcParaName     :   �ϱ��Ĳ������ƣ�����Ϊ�գ���Ҫ����256�ֽ�
              2. pcParaValue    :   �ϱ��Ĳ���ֵ������Ϊ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
#define ATP_CWMP_AddParaValueChg(pcParaName, pcParaValue) \
            ATP_CWMP_AddParaValueChgEx((pcParaName), (pcParaValue), ATP_CWMP_VALUE_TYPE_CHAR)

/******************************************************************************
  ��������  : ATP_CWMP_SetConnectionIP
  ��������  : ���TR069���󶨵�IPConnection��PPPConnection�����ƺ�IPֵ��
              �ṩ���ӿ���Ϊ�˱�֤�ϱ�ʱ��ÿ��TR069�󶨵�IP���ϱ������б��ж���
              ���һ�����ֲ�SupportSoft ACS��Bug
  �������  :
              1. pcParaName     :   TR069���󶨵����ƣ���Ҫ����256�ֽ�
              2. pcParaValue    :   IPֵ
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetConnectionIP(const VOS_CHAR *pcParaName,
                                              const VOS_CHAR *pcParaValue);

/******************************************************************************
  ��������  : ATP_CWMP_DelParameter
  ��������  : ��ÿ�ζ���Ҫ�ϱ��Ĳ����б���ɾ���ϱ��Ĳ���
  �������  :
              1. pcParaName :   ��ɾ���Ĳ�������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_VOID ATP_CWMP_DelParameter(const VOS_CHAR   *pcParaName);

/*
 *  Inform�ϱ��¼��ӿ�
 */

/* �¼����� */
#define ATP_CWMP_EVT_BOOTSTRAP_STR_D     "0 BOOTSTRAP"
#define ATP_CWMP_EVT_BOOT_STR_D          "1 BOOT"
#define ATP_CWMP_EVT_PERIODIC_STR_D      "2 PERIODIC"
#define ATP_CWMP_EVT_SCHEDULED_STR_D     "3 SCHEDULED"
#define ATP_CWMP_EVT_VALUECHANGE_STR_D   "4 VALUE CHANGE"
#define ATP_CWMP_EVT_KICKED_STR_D        "5 KICKED"
#define ATP_CWMP_EVT_CONNREQ_STR_D       "6 CONNECTION REQUEST"
#define ATP_CWMP_EVT_TRANSCOMPLETE_STR_D "7 TRANSFER COMPLETE"
#define ATP_CWMP_EVT_DIAGCOMPLETE_STR_D  "8 DIAGNOSTICS COMPLETE"
#define ATP_CWMP_EVT_REQDOWNLOAD_STR_D   "9 REQUEST DOWNLOAD"
#define ATP_CWMP_EVT_MREBOOT_STR_D       "M Reboot"
#define ATP_CWMP_EVT_MSCHEDULE_STR_D     "M ScheduleInform"
#define ATP_CWMP_EVT_MDOWNLOAD_STR_D     "M Download"
#define ATP_CWMP_EVT_MUPLOAD_STR_D       "M Upload"

/* �Ự���Բ��� */
typedef enum tagATP_CWMP_EVT_RETRY_POLICY_EN
{
    ATP_CWMP_EVT_RETRY_EXCLUSIVE,       // �������ԣ��Ҳ��������¼����棬
                                        // ��O BOOTSTRAP�¼�
    ATP_CWMP_EVT_RETRY_FOREVER,         // ��ʹ��������Ҳ��Ҫ���Ե��¼�
    ATP_CWMP_EVT_RETRY_ALIVE,           // �������Ҫ���Ե��¼�
    ATP_CWMP_EVT_RETRY_NO,              // ����Ҫ���Ե��¼�
    ATP_CWMP_EVT_RETRY_ARBITRARY        // û�ж�����¼�
} ATP_CWMP_EVT_RETRY_POLICY_EN;

/******************************************************************************
  ��������  : ATP_CWMP_RegEvent
  ��������  : ��Э��ջ����һ���û���չ�¼�
  �������  :
              1. pcEvtName          :   �¼����ƣ�������Ϊ�գ���Ҫ����256�ֽ�
              2. pcConfirmRPC       :   �¼�ȷ�ϱ�ACS���յ�RPC��������
              3. pcRequestRPC       :   �¼����������Ҫ���͸�ACS����������
              4. pcRelEvt           :   ����¼�������
              5. eRetryPolicy       :   �¼����Բ���
              6. ucSingleOrMultiple :   �¼��Ƿ�����ۻ�
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              1. VOS_OK                             : ע��ɹ�
              2. ATP_ERR_CWMP_STK_PARA_INVALID      : ����Ϊ��ָ��
              3. ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH    : �ڴ����
              4. ATP_ERR_CWMP_STK_RPC_NOT_FOUND     : RPCû���ҵ�
              5. ATP_ERR_CWMP_STK_INVALID_EVT       : �Ƿ��Ĺ����¼�

  �޸���ʷ      :
   1.��    ��   : 2007-7-4
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_RegEvent(
                                const VOS_CHAR          *pcEvtName,
                                const VOS_CHAR          *pcConfirmRPC,
                                const VOS_CHAR          *pcRequestRPC,
                                const VOS_CHAR          *pcRelEvt,
                                ATP_CWMP_EVT_RETRY_POLICY_EN    eRetryPolicy,
                                VOS_BOOL                bSingleOrMultiple);

/******************************************************************************
  ��������  : ATP_CWMP_AddEvt
  ��������  : TR069Э��ջ������ʱ����¼�����Щ�¼����ϱ���ACS��
              ������¼�������RPC���򻹻ᷢ��RPC����
  �������  :
              1. pcEvtName      :   �¼�����
              2. pcCmdKey       :   ���¼���CommandKey������Ϊ�գ�������32�ֽ�
              3. hstRPCArg      :   ��Ӧ��RPC����������DOM��
              4. bActiveInform  :   �Ƿ���������Ự
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              1. VOS_OK                             : ��ӳɹ�
              2. ATP_ERR_CWMP_STK_PARA_INVALID      : �����Ƿ�
              3. ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH    : �ڴ����
              4. ATP_ERR_CWMP_STK_INVALID_EVT       : �¼�û��ע��

  �޸���ʷ      :
   1.��    ��   : 2007-7-4
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_AddEvent(
                                const VOS_CHAR          *pcEvtName,
                                const VOS_CHAR          *pcCmdKey,
                                NODEPTR                 hstRPCArg,
                                VOS_BOOL                bActiveInform);

/*
 *  RPC DOM�����캯��
 */
typedef enum tagATP_CWMP_CPE_FAULT_CODES_EN
{
    ATP_CWMP_RPC_CPE_OK                                 = 0,
    ATP_CWMP_RPC_CPE_METHOD_NOT_SPT                     = 9000,
    ATP_CWMP_RPC_CPE_REQ_DENIED                         = 9001,
    ATP_CWMP_RPC_CPE_INTER_ERROR                        = 9002,
    ATP_CWMP_RPC_CPE_INVALID_ARG                        = 9003,
    ATP_CWMP_RPC_CPE_RES_EXCD                           = 9004,
    ATP_CWMP_RPC_CPE_INVALID_PARAM_NAME                 = 9005,
    ATP_CWMP_RPC_CPE_INVALID_PARAM_TYPE                 = 9006,
    ATP_CWMP_RPC_CPE_INVALID_PARAM_VALUE                = 9007,
    ATP_CWMP_RPC_CPE_ATMPT_SET_NON_WRITE_PARAM          = 9008,
    ATP_CWMP_RPC_CPE_NOTFY_REQ_REJ                      = 9009,
    ATP_CWMP_RPC_CPE_DOWNLOAD_FAIL                      = 9010,
    ATP_CWMP_RPC_CPE_UPLOAD_FAIL                        = 9011,
    ATP_CWMP_RPC_CPE_FILE_TRANF_AUTH_FAIL               = 9012,
    ATP_CWMP_RPC_CPE_UNSPTED_PROT_FOR_FILE_TRAN         = 9013,

    ATP_CWMP_RPC_CPE_INVALID_SCHEMA                     = 9037,     // Extended
    ATP_CWMP_RPC_CPE_MAX_MASK                           = 0XFFFF
} ATP_CWMP_CPE_FAULT_CODES_EN;

/******************************************************************************
  ��������  : ATP_CWMP_GetErrStringByCode
  ��������  : ���ݴ������ȡ���������ַ���
  �������  :
              1. ulErrCode  :   ������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : �����ַ�����û���ҵ�ʱ������"Unknown Error"

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
const VOS_CHAR *ATP_CWMP_GetErrStringByCode(
                                VOS_UINT32             ulErrCode);

/******************************************************************************
  ��������  : ATP_CWMP_BuildErrorNode
  ��������  : ����SOAP����DOM������ʽ����:
      <soap:Fault>
          <faultcode>Server</faultcode>
          <faultstring>CWMP fault</faultstring>
          <detail>
              <cwmp:Fault>
                  <FaultCode>8003</FaultCode>
                  <FaultString>Invalid arguments</FaultString>
              </cwmp:Fault>
          </detail>
      </soap:Fault>
  �������  :
              1. ulErrCode  :   ������
              2. pstNs      :   ���ֿռ�
  ���ú���  :
  ��������  :
  �������  :
              1. phError    :   ��Ź����DOM����ָ��
  �� �� ֵ  :
              VOS_OK
              TSP XML���������

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_BuildErrorNode(
                                   NODEPTR                        *phError,
                                   VOS_UINT32                     ulErrCode,
                                   const ATP_CWMP_NAME_SAPCE_ST   *pstNs);

/******************************************************************************
  ��������  : ATP_CWMP_ErrorAddPara
  ��������  : ������õ�SOAP����DOM������Ӵ������
            <cwmp:Fault>
                      <FaultCode>9003</FaultCode>
                      <FaultString>Invalid arguments</FaultString>
                      <SetParameterValuesFault>
                          <ParameterName>
                              InternetGatewayDevice.Time.LocalTimeZone
                          </ParameterName>
                          <FaultCode>9012</FaultCode>
                          <FaultString>...</FaultString>
                      </SetParameterValuesFault>
             _________<SetParameterValuesFault>
            |             <ParameterName>
          ��|  pcErrPara:     InternetGatewayDevice.Time.LocalTimeZoneName
          ��|             </ParameterName>
          ��|  ulErrCode: <FaultCode>9012</FaultCode>
          ��|             <FaultString>...</FaultString>
            |_________</SetParameterValuesFault>
                  </cwmp:Fault>
          </detail>
          </soap:Fault>

  �������  :
              1. hError     :   �Ѿ�����õĴ���DOM��
              2. pcErrPara  :   �������������Ϊ�գ���Ҫ����256�ֽ�
              3. ulErrCode  :   ������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              TSP XML���������

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_ErrorAddPara(NODEPTR             hError,
                            const VOS_CHAR           *pcErrPara,
                            VOS_UINT32               ulErrCode);

typedef enum tagCWMP_NEXT_LEVEL_TYPE_EN
{
    CWMP_NEXT_LEVEL_TYPE_NONE,
    CWMP_NEXT_LEVEL_TYPE_STATUS_0,
    CWMP_NEXT_LEVEL_TYPE_STATUS_1,
    CWMP_NEXT_LEVEL_TYPE_PARALIST
} CWMP_NEXT_LEVEL_TYPE_EN;

/******************************************************************************
  ��������  : ATP_CWMP_CreateRpcTopNode
  ��������  : �������DOM�������ʽ����:
CWMP_NEXT_LEVEL_TYPE_STATUS_0/CWMP_NEXT_LEVEL_TYPE_STATUS_1:
    <cwmp:SetParameterValuesResponse>       // phTopNode
          <Status>0</Status>                // phNextLevelNode
    </cwmp:SetParameterValuesResponse>

CWMP_NEXT_LEVEL_TYPE_NONE:
    <cwmp:SetParameterAttributesResponse>   // phTopNode
    </cwmp:SetParameterAttributesResponse>

CWMP_NEXT_LEVEL_TYPE_PARALIST:
    <cwmp:GetParameterValuesResponse>       // phTopNode
        <ParameterList>                     // phNextLevelNode
        </ParameterList>
    </cwmp:GetParameterValuesResponse>

  �������  :
              1. pcRpcName      :   RPC���ƣ�����Ϊ�գ���Ҫ����256�ֽ�
              2. bResponse      :   �Ƿ�ΪResponse
              3. ulNextLevelType:   �������ͣ�������
              4. pstNs          :   ���ֿռ䣬���ܳ���256�ֽ�
  ���ú���  :
  ��������  :
  �������  :
              1. phTopNode      :   ������DOM��ָ���ָ�룬����Ϊ��
              2. phNextLevelNode:   ����ڲ�DOM��ָ���ָ�룬����Ϊ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH
              TSP XML���������

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_CreateRpcTopNode(
                        const VOS_CHAR                  *pcRpcName,
                        VOS_BOOL                        bResponse,
                        VOS_UINT32                      ulNextLevelType,
                        const ATP_CWMP_NAME_SAPCE_ST    *pstNs,
                        NODEPTR                         *phTopNode,
                        NODEPTR                         *phNextLevelNode);

/******************************************************************************
  ��������  : ATP_CWMP_SetArrayTypeAttr
  ��������  : ����ParameterList����
    <cwmp:GetParameterValuesResponse>
                      |pcTypeNs|                             |-ulArrayNum-|
        <ParameterList soapenc:arrayType="cwmp:ParameterValueStruct[1]">
                       |------��Ӳ���---------|                  |---|
            <ParameterValueStruct>             |----pcArrayType---|
                <Name>InternetGatewayDevice.NumberOfWANDevices</Name>
                <Value></Value>
            </ParameterValueStruct>
        </ParameterList>
    </cwmp:GetParameterValuesResponse>

  �������  :
              1. hParaList      :   �Ѿ�����õ�ParameterList��DOMָ��
              2. pcTypeNs       :   ParameterList��ÿ���������͵����ֿռ䣬
                                    ��Ҫ����256�ֽ�
              3. pcArrayType    :   ParameterList��ÿ������������
              4. ulArrayNum     :   ParameterList�в����ĸ���
              5. pstNs          :   ���ֿռ�
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH
              TSP XML���������

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetArrayTypeAttr(
                        NODEPTR                         hParaList,
                        const VOS_CHAR                  *pcTypeNs,
                        const VOS_CHAR                  *pcArrayType,
                        VOS_UINT32                      ulArrayNum,
                        const ATP_CWMP_NAME_SAPCE_ST    *pstNs);

/******************************************************************************
  ��������  : ATP_CWMP_AppendParaInfoNode
  ��������  : ��ParameterList����Ӳ���
    <cwmp:GetParameterNamesResponse>
        <ParameterList SOAP-ENC:arrayType="cwmp:ParameterInfoStruct[1]">
        _
     �� |   <ParameterInfoStruct>
     �� |       <Name>InternetGatewayDevice.WANDevice</Name>    // pcParaName
     �� |       <Writable>0</Writable>                          // bWritable
     �� |   </ParameterInfoStruct>
        -
        </ParameterList>
    </cwmp:GetParameterNamesResponse>

  �������  :
              1. hParaList  :   �Ѿ�����õ�ParameterList��DOMָ��
              2. pcParaName :   ����Ӳ��������ƣ�����Ϊ�գ���Ҫ����256�ֽ�
              3. bWritable  :   ����Ӳ����Ƿ��д
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH
              TSP XML���������

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_AppendParaInfoNode(
                                        NODEPTR         hParaList,
                                        const VOS_CHAR  *pcParaName,
                                        VOS_BOOL        bWritable);

/******************************************************************************
  ��������  : ATP_CWMP_AppendParaValueStructNode
  ��������  : ���ParameterValueStruct�ṹ
  <cwmp:GetParameterValuesResponse>
      <ParameterList soapenc:arrayType="cwmp:ParameterValueStruct[1]">
      _
   �� | <ParameterValueStruct>
   �� |   <Name>InternetGatewayDevice.NumberOfWANDevices</Name> // pcParaName
   �� |   <Value>1</Value>                                      // pcParaValue
   �� | </ParameterValueStruct>
      -
      </ParameterList>
  </cwmp:GetParameterValuesResponse>

  �������  :
              1. hParaList      :   �Ѿ�����õ�ParameterList��DOMָ��
              2. pcParaName     :   ����ӵĲ������ƣ�����Ϊ�գ���Ҫ����256�ֽ�
              3. pcParaValue    :   ����ӵĲ���ֵ
              4. enValueType    :   ����ֵ������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH
              TSP XML���������

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_AppendParaValueStructNode(
                                NODEPTR                     hParaList,
                                const VOS_CHAR              *pcParaName,
                                const VOS_CHAR              *pcParaValue,
                                ATP_CWMP_ATTR_VALUE_TYPE_E   enValueType);

/******************************************************************************
  ��������  : ATP_CWMP_BuildTransCompleteNodes
  ��������  : ����TransferComplete DOM��
    <cwmp:TransferComplete>
        <CommandKey></CommandKey>           // pcCmdKey
        <FaultStruct>
            <FaultCode></FaultCode>         // ulErrCode
            <FaultString></FaultString>
        </FaultStruct>
        <StartTime></StartTime>             // pcStartTime
        <CompleteTime></CompleteTime>       // pcEndTime
    </cwmp:TransferComplete>

  �������  :
              1. pcCmdKey       :   RPC��CommandKey
              2. pcStartTime    :   ���俪ʼʱ�䣬����Ϊ��
                                    Ϊ��ʱ��ʹ��Ĭ��ʱ��"0001-01-01T00:00:00Z"
              3. pcEndTime      :   �������ʱ�䣬����Ϊ��
                                    Ϊ��ʱ��ʹ��Ĭ��ʱ��"0001-01-01T00:00:00Z"
              4. ulErrCode      :   ���������
              5. pstNs          :   ���ֿռ�
  ���ú���  :
  ��������  :
  �������  :
              1. hOutNode       :   ����õ�DOM��ָ��
  �� �� ֵ  :
              VOS_OK
              ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_MEM_NOT_ENOUGH
              TSP XML���������

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_BuildTransCompleteNodes(
                                    const VOS_CHAR  *pcCmdKey,
                                    const VOS_CHAR  *pcStartTime,
                                    const VOS_CHAR  *pcEndTime,
                                    VOS_UINT32      ulErrCode,
                                    NODEPTR         *hOutNode,
                                    const ATP_CWMP_NAME_SAPCE_ST  *pstNs);

/*
 *  CWMP������Ϣ�ӿ�
 */

/******************************************************************************
  ��������  : ATP_CWMP_StkGetVersion
  ��������  : ��ȡCWMPЭ��ջ����İ汾��
  �������  :
              1. pszVersion :   ��Ű汾���ַ�����ָ�룬������Ϊ��
              2. ulLen      :   ����������
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-1-3
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_StkGetVersion(
                            VOS_INT8        *pszVersion,
                            VOS_UINT32      ulLen);

#define USE_CWMP_TRACE

#ifdef USE_CWMP_TRACE
/******************************************************************************
  ��������  : ATP_CWMP_Outputf
  ��������  : ������Ϣ��ӡ��������ͨ�������ض���
  �������  :
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2007-12-30
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_Outputf(const VOS_INT8 *app, VOS_UINT32 level,
                                     VOS_UINT32 type, VOS_UINT32 logno,
                                     const VOS_INT8 *file, VOS_UINT32 line,
                                     const VOS_INT8 *fmt, ...);

#define ATP_CWMP_TRACE              ATP_CWMP_Outputf

#ifdef DEBUG
#define ATP_CWMP_DEBUG              ATP_CWMP_Outputf
#else
#define ATP_CWMP_DEBUG(x...)
#endif

#else

#define ATP_CWMP_TRACE              VTOP_LOG_Outputf

#ifdef DEBUG
ULONG ATP_CWMP_Outputf(const VOS_INT8 *app, VOS_UINT32 level,
                                     VOS_UINT32 type, VOS_UINT32 logno,
                                     const VOS_INT8 *file, VOS_UINT32 line,
                                     const VOS_INT8 *fmt, ...);

#define ATP_CWMP_DEBUG              ATP_CWMP_Outputf
#else
#define ATP_CWMP_DEBUG(x...)
#endif

#endif

#define ATP_CWMP_ENABLE_HTTP_AUTH

/*
 *  C02 ��������
 */
#ifdef ATP_CWMP_ENABLE_SSL

/* ֤����֤ģʽ */
typedef enum tagATP_CWMP_SSL_MODE
{
    ATP_CWMP_SSL_CERT_AUTH_NONE,        // ������֤����֤
    ATP_CWMP_SSL_CERT_AUTH_PEER,        // ��Ҫ�ԶԷ�����֤����֤��
                                        // �����ṩ�������εĸ�֤��
    ATP_CWMP_SSL_CERT_AUTH_BY_PEER, // ��Ҫ���Զ���֤��
                                        // ��Ҫ�ṩ���ص�֤���Լ�˽Կ
    ATP_CWMP_SSL_CERT_AUTH_BOTH,        // ˫����֤����Ҫ�ṩ��������֤��
    ATP_CWMP_SSL_CERT_AUTH_MAX
} ATP_CWMP_SSL_MODE;

/* SSL�汾 */
typedef enum tagATP_CWMP_SSL_VERSION
{
    ATP_CWMP_SSL_ANY_EXCLUDE_V2,        // �ų�V2������Ӧʹ��V3����TLS����ȫ�Ը���
    ATP_CWMP_SSL_ANY,                   // ����Ӧʹ��V2��V3����TSL
    ATP_CWMP_SSL_TLS,                   // TLS
    ATP_CWMP_SSL_V3,
    ATP_CWMP_SSL_MAX
} ATP_CWMP_SSL_VERSION;


/* SSL֤��ṹ�� */
typedef struct tagATP_CWMP_SSL_CERTS_ST
{
	VOS_CHAR		*pcRootCertPath;		// ���ظ�֤��·��
	VOS_CHAR		*pcCertPath;			// ����֤��·��
	VOS_CHAR		*pcKeyPath;				// ����֤���Ӧ��˽Կ·��
	VOS_CHAR		*pcKeyPassword;			// ����˽Կ�ļ������룬���˽Կ�м��ܣ�
	                                        // ������ṩ����
} ATP_CWMP_SSL_CERTS_ST;

/* SSL���ýṹ�� */
typedef struct tagATP_CWMP_SSL_CONFIG_ST
{
	VOS_UINT8	ucClientAuthMode;	// CWMP��Ϊ�ͻ��˵���֤ģʽ��ȡATP_CWMP_SSL_MODEö��ֵ
	VOS_UINT8	ucServerAuthMode;	// CWMP��Ϊ����������֤ģʽ��ȡATP_CWMP_SSL_MODEö��ֵ
	VOS_UINT8	ucClientSSLVersion;		// �ͻ���SSL�汾��ȡATP_CWMP_SSL_VERSIONö��ֵ
	VOS_UINT8	ucServerSSLVersion;		// ������SSL�汾��
	ATP_CWMP_SSL_CERTS_ST	*pstClientCerts;	// �ͻ��˵�֤��·��
	ATP_CWMP_SSL_CERTS_ST	*pstServerCerts;	// ��������֤��·��������Ϳͻ�����ͬ�������Ϊ��
} ATP_CWMP_SSL_CONFIG_ST;

/* ����ģʽ */
typedef enum tagATP_CWMP_CON_REQ_MODE
{
    ATP_CWMP_CON_REQ_MODE_HTTP,     // HTTP��ʽ����
    ATP_CWMP_CON_REQ_MODE_HTTPS,        // HTTPS��ʽ����
    ATP_CWMP_CON_REQ_MODE_BOTH,     // ���߶�֧�֣����ϱ�����HTTP��ʽ��URL
    ATP_CWMP_CON_REQ_MODE_MAX
} ATP_CWMP_CON_REQ_MODE;

/******************************************************************************
  ��������  : ATP_CWMP_RefreshSSLContext
  ��������  : ����SSL�������ģ�����Ҫ����֤��ʱ����Ҫʹ�ô˽ӿ�������֤��
  �������  :
              ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ATP_ERR_CWMP_STK_NET_SSL
              ATP_ERR_CWMP_STK_PARA_INVALID
              VOS_OK

  �޸���ʷ      :
   1.��    ��   : 2008-4-15
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_RefreshSSLContext();

/******************************************************************************
  ��������  : ATP_CWMP_ConfigSSL
  ��������  : ����CWMP����Ŀͻ��˺ͷ�������SSL����
  �������  :
              1. pstConfig  :   SSL���Խṹ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ATP_ERR_CWMP_STK_NET_SSL
              ATP_ERR_CWMP_STK_PARA_INVALID
              VOS_OK

  �޸���ʷ      :
   1.��    ��   : 2008-4-15
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_ConfigSSL(ATP_CWMP_SSL_CONFIG_ST *pstConfig);

#ifdef ATP_CWMP_ENABLE_SERVER_SSL
/******************************************************************************
  ��������  : ATP_CWMP_SetConReqMode
  ��������  : ����CWMP����ķ������Ļ�����ʽ
  �������  :
              1. enMode :   ������ʽֵ
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : ATP_ERR_CWMP_STK_PARA_INVALID
              ATP_ERR_CWMP_STK_NET_SOCK
              ATP_ERR_CWMP_STK_NET_SSL

  �޸���ʷ      :
   1.��    ��   : 2008-4-15
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_SetConReqMode(ATP_CWMP_CON_REQ_MODE enMode);
#endif

/*
 * ֤����֤����������VOS_TRUE��ʾ֤����֤ͨ��������VOS_FALSE��ʾ֤����֤ʧ��
 * bClient��ʾ�������ǿͻ��˻��Ƿ�������VOS_TRUE��ʾ�ͻ��ˡ�VOS_FALSE��ʾ������
 * ok��ʾOpenSSL���õ�֤��У���Ƿ��Ѿ�ͨ��
 * store��SSL֤��ṹ�����Ը��ݸý����ȡ�Զ�֤�飬����OpenSSL�ӿ���У�顣
 */
typedef VOS_INT32 (*PFATPCWMPSSLCertCheck)(VOS_BOOL bClient, VOS_INT32 ok,
                                                         X509_STORE_CTX *store);

/******************************************************************************
  ��������  : ATP_CWMP_RegSSLCertCheckFunc
  ��������  : ע��֤����֤��չ��������ε���ʱ�������һ�ε�Ϊ׼
  �������  :
              1. pfCheckCert    :   ��չ����֤����ָ��
  ���ú���  :
  ��������  :
  �������  : ��
  �� �� ֵ  : VOS_OK

  �޸���ʷ      :
   1.��    ��   : 2008-4-15
     ��    ��   : handy
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32 ATP_CWMP_RegSSLCertCheckFunc(PFATPCWMPSSLCertCheck pfCheckCert);

#endif

extern VOS_UINT16 g_usLocalPortMin;
extern VOS_UINT16 g_usLocalPortRng;

/*
 * TR069Э��ջ����socketʱ�������socket��������
 *      �����ڴ˻ص������м���QoS��ǩ
 * lSockDesc : �´�����socket������
 * usPort       : �´�����socket���󶨵Ķ˿ڣ�û�а�ʱΪ0
 */
typedef VOS_VOID (*PFCwmpExtraSocketOp)(
                                VOS_INT32       lSockDesc,
                                VOS_UINT16      usPort);

/******************************************************************************
  ��������  : ATP_CWMP_RegExtraSockOp
  ��������  : ע��TR069Э��ջ����socketʱ�Ķ��⴦����
  �������  : 
              1.  pfFunc : ������Ļص�����
  ���ú���  : 
  ��������  : 
  �������  : ��
  �� �� ֵ  : ��

  �޸���ʷ      :
   1.��    ��   : 2008-9-26
     ��    ��   : z65940
     �޸�����   : ��ɳ���

******************************************************************************/
VOS_UINT32  ATP_CWMP_RegExtraSockOp(PFCwmpExtraSocketOp pfFunc);
/*End of MNT 2008-10-25 10:52 for A36D05726 by z65940*/

#endif

