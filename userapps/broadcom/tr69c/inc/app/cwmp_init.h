/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2005, ��Ϊ�������޹�˾.
  �ļ���:
  ����: Diao.Meng
  �汾: HG550V200R001B02
  ��������: 2005-5-6
  �������:
  ��������:

  ��Ҫ�����б�:

  �޸���ʷ��¼�б�:
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>

  ��ע:
************************************************************************/

#ifndef _CWMP_INIT_H_
#define _CWMP_INIT_H_

#include "cfg_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif

#define    TR069_ACSURL_LEN                            (256)
#define    TR069_ACSUSERNAME_LEN                       (256)
typedef  struct   tagTR069_WEBINFO
{
    CHAR     pcAcsURL[TR069_ACSURL_LEN];

    CHAR     pcAcsUseName[TR069_ACSUSERNAME_LEN];
    CHAR     pcAcsPassword[TR069_ACSUSERNAME_LEN];
    CHAR     pcConnectUserName[TR069_ACSUSERNAME_LEN];
    CHAR     pcConnectPassword[TR069_ACSUSERNAME_LEN];
    BOOL     bEnableInform;
    int         iInformInterval;
}TR069_CONFIG_INFO , *PTR069_CONFIG_INFO;

typedef  struct   tagTR069_CLIINFO
{
    int         iDbgType;
    int         iDbgLevel ;
    int         ulPortNum;
    char        acIP[32];
}TR069_CONFIG_CLIINFO ,      *PTR069_CONFIG_CLIINFO;

typedef enum tagCWMP_CLI_DBG_TYPE
{
    CWMP_DBG_TYPE_SWITCH          = 0,
    CWMP_DBG_TYPE_LEVEL           = 1,
    CWMP_DBG_TYPE_TRACER          = 2,
    CWMP_DBG_TYPE_RESTORE         = 3,
    CWMP_DBG_TYPE_SSLCERT         = 4,
    CWMP_DBG_TYPE_NONE            = 5,
} CWMP_CLI_DBG_TYPE_EN;


VOID CWMP_Init(VOID);

#ifdef __cplusplus
}
#endif

#endif  /* _CWMP_INIT_H_ */


