/******************************************************************************
  ��Ȩ����  : 2007-2020����Ϊ�������޹�˾
  �� �� ��  : wlancmd.h
  ��    ��  : l69021
  ��    ��  : 1.0
  ��������  : 2007-8-22
  ��    ��  : �����ն����ƽ̨WLANCMD������صײ�ӿ�ͷ�ļ�
  �����б�  :

  ��ʷ��¼      :
   1.��    ��   : 2007-8-22
     ��    ��   : l69021
     �޸�����   : ��ɳ���

*********************************************************************************/

#ifndef __WLANCMD_H__
#define __WLANCMD_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "wlancmdadapter.h"

typedef struct cmd 
{
    char *name;
    cmd_func_t *func;
    char *help;
}wlan_cmd_t;

#define CMD_ERR -1  /* Error for command */
#define CMD_OPT 0   /* a command line option */
#define CMD_WL  1   /* the start of a wl command */

#define INT_FMT_DEC 0   /* signed integer */
#define INT_FMT_UINT    1   /* unsigned integer */
#define INT_FMT_HEX 2   /* hexdecimal */
#define TRUE    1
#define FALSE   0

#define DEV_TYPE_LEN 2 /* length for devtype 'wl'/'et' */

#if __cplusplus
#ifdef __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __WLANCMD_H__ */
