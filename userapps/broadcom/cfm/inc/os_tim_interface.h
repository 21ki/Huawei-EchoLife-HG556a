/**************************************************************************/
/*                                                                        */
/*              Copyright  2005-2009, Huawei Tech. Co., Ltd.              */
/*                      ALL RIGHTS RESERVED                               */
/*                                                                        */
/*                                                                        */
/*  File Name     :  vtop_time.h                               */
/*                                                                        */
/*  Author         : �Ŷ��� 36904                                            */
/*  Create Date    : 2005-10-21                                          */
/*  Description     : about time define                                       */
/*  Others         :                                                      */
/*  Version        : 1.0                                                  */
/*------------------------------------------------------------------------*/
/*  Function List   :                                                     */
/*  Name                Function                                          */
/*                                                                        */
/*------------------------------------------------------------------------*/
/*  History         :                                                     */
/*  Data            Author      Modification                              */
/*                                                                        */
/**************************************************************************/

#include "os_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifndef __OS_TIM_INTERFACE_H__
#define __OS_TIM_INTERFACE_H__

typedef struct _VTOP_S_TM {
     VOS_INT     tm_sec;         /* seconds */
     VOS_INT     tm_min;         /* minutes */
     VOS_INT     tm_hour;        /* hours */
     VOS_INT     tm_mday;        /* day of the month */
     VOS_INT     tm_mon;         /* month */
     VOS_INT     tm_year;        /* year */
     VOS_INT     tm_wday;        /* day of the week */
     VOS_INT     tm_yday;        /* day in the year */
     VOS_INT     tm_isdst;       /* daylight saving time */
}VTOP_S_TM;

typedef struct vtop_timeval
  {
    VTOP_TIME_T tv_sec;            /* Seconds.  */
    VTOP_TIME_T tv_usec;      /* Microseconds.  */
  }VTOP_S_TimeVal;

typedef struct vtop_timespec
  {
    VTOP_TIME_T tv_sec;            /* Seconds.  */
    VOS_INT tv_nsec;           /* Nanoseconds.  */
  }VTOP_S_Timespec;



/*��time_t timep ת��Ϊtm*/
VTOP_S_TM *VTOP_LocalTime(const VTOP_TIME_T *timep );
VTOP_SIZE_T VTOP_StrFtime(VOS_CHAR*s, VTOP_SIZE_T max, const VOS_CHAR *format,  const VTOP_S_TM *time);
VOS_CHAR *VTOP_Ctime(const VTOP_TIME_T *ptime);
/*��ȡ�����*/
VOS_UINT32 VTOP_Random(VOS_VOID);
/*��ȡϵͳʱ�䣬��ϵͳ�������ĺ�����*/
VOS_UINT32 VTOP_GetSystemTime(VOS_VOID);
/*��ȡϵͳʱ�䣬(00:00:00 UTC, January 1, 1970) ����*/
VOS_UINT32 VTOP_GetSysTimeSec(VOS_VOID);


 
/*��ȡ��ǰ��ϵͳʱ�䣬�꣬����ʱ�������*/
VOS_VOID VTOP_GetTime(VTOP_S_TM* curdate);

#endif /* __OS_TIM_INTERFACE_H__ */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

