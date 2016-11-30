/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2006, ��Ϊ�������޹�˾.
  �ļ���: autoupgrade.h
  ����: zhangliang 60003055
  �汾: V1.0
  ��������: 2006-5-8
  �������: 
  ��������: 
      
  ��Ҫ�����б�: 
      
  �޸���ʷ��¼�б�: 
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    
  ��ע: 
************************************************************************/
#ifndef AUTOUPGRADE_H
#define AUTOUPGRADE_H

#ifndef AUG_DBG
#define AUG_DBG 0
#endif

#include "bcmTag.h"

#ifndef TRUE
#define TRUE                       1
#endif

#ifndef FALSE
#define FALSE                      0
#endif

#define SUCCESS                    0
#define SELECT_TIMEOUT             0
#define ROUTE_PATH_OK              1

//#define FAILURE_TAG_LEN            10
#define FAILURE_TAG                "5aa5a55a"
#define FAILURE_LOC                8

#define SHORT_CMD_LEN              30
#define LONG_CMD_LEN               512

#define FILE_BUFFER1K              1024
#define PRODUCT_NAME_LEN           20
#define SOFT_VERSION_LEN           20
#define UPGRADE_SERVER_NAME_LEN    128
#define UPGRADE_FILE_NAME_LEN      128
#define GENERALFILE_MAX_SIZE       1024
#define IMAGE_MINI_SIZE            1024000

#define UPGRADE_FLAG_LEN           2
#define ROOTPATH_MAX_LEN           256
#define TIMEOUT_MAX_LEN            6
#define MACADDR_LEN_INBYTE         6
#define MACADDR_LAST_3_BYTES       24

#define WRITE_VAR_INTERVAL         6 
#define RANDTIME_SCOPE_2HOURS      7200
#define T_24_HOURS_INSECOND        86400

#if defined(__cplusplus)
extern "C" {
#endif

int NeedToUpgrade(void);
void UpdateImage(void);
void Monitor_AutoUpgrade(void);


#if defined(__cplusplus)
}
#endif

#endif /* AUTOUPGRADE_H */
