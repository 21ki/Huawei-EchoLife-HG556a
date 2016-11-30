/*                                                                           */
/*                Copyright 2004 - 2050, Huawei Tech. Co., Ltd.              */
/*                           ALL RIGHTS RESERVED                             */
/*                                                                           */
/* FileNmae: os_tmr_interface.h                                              */
/* Version : 1.0                                                             */
/*                                                                           */
/* Description:                                                              */
/*                                                                           */
/* History:                                                                  */
/* Data            Author            Modification                            */
/*****************************************************************************/
#ifndef __OS_TMR_INTERFACE_H__
#define __OS_TMR_INTERFACE_H__

#include <time.h>
#include "os_def.h"
#include "os_tim_interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

typedef VOS_VOID * VTOP_TIMER_ID;

/*timer type*/
typedef enum _VTOP_E_TimerAttr
{
    VTOP_TIMER_LOOP  = 0,       /* the timer start loop */
    VTOP_TIMER_ONE_SHOOT ,      /* the timer start once and then stop */
    VTOP_TIMER_AUTO_FREE        /* the timer start once and then free */
}VTOP_E_TimerAttr;

/* ������ʱ��API*/
extern VOS_INT VTOP_StartTimerTask(void * pthread, const void * pattr);
#ifdef VTOP_OS_WIN32
#define VTOP_InitTimer() VTOP_StartTimerTask(VOS_NULL_PTR, VOS_NULL_PTR)
#endif

/*������ʱ��API*/
extern VTOP_TIMER_ID VTOP_CreateAbsTimerM(VOS_VOID (*timeout)(void * arg), VTOP_E_TimerAttr attr, VOS_UINT32 ulLineNo, const VOS_CHAR *strFileName);
#define VTOP_CreateAbsTimer(timeout, attr) VTOP_CreateAbsTimerM(timeout, attr, __LINE__, __FILE__) 
extern VTOP_TIMER_ID VTOP_CreateRelTimerM(VOS_VOID (*timeout)(void * arg), VTOP_E_TimerAttr attr, VOS_UINT32 ulLineNo, const VOS_CHAR *strFileName);
#define VTOP_CreateRelTimer(timeout, attr) VTOP_CreateRelTimerM(timeout, attr, __LINE__, __FILE__) 

/*������ʱ��API*/
extern VOS_INT VTOP_StartAbsTimer(VTOP_TIMER_ID timer, const VTOP_S_TM * tm, VOS_VOID * arg);
extern VOS_INT VTOP_StartRelTimer(VTOP_TIMER_ID timer, VOS_UINT32 msec, VOS_VOID * arg);
extern VOS_INT VTOP_StartRelTimer_Simple(VOS_UINT32 msec, VOS_VOID (*timeout)(void * arg),VOS_VOID * arg);

/*ֹͣ��ʱ��API*/
extern VOS_INT VTOP_StopAbsTimer_Safe(VTOP_TIMER_ID timer, VOS_VOID(* finallize)(VOS_VOID *), VOS_VOID * arg);
#define VTOP_StopAbsTimer(timer)  VTOP_StopAbsTimer_Safe(timer, VOS_NULL_PTR,VOS_NULL_PTR)

extern VOS_INT VTOP_StopRelTimer_Safe(VTOP_TIMER_ID timer, VOS_VOID(* finallize)(VOS_VOID *), VOS_VOID * arg);
#define VTOP_StopRelTimer(timer)  VTOP_StopRelTimer_Safe(timer, VOS_NULL_PTR, VOS_NULL_PTR)

extern VOS_INT VTOP_StopAbsTimer_SafeEx(VTOP_TIMER_ID timer, VOS_VOID(* finallize)(VOS_VOID *), VOS_VOID * parg, VOS_VOID ** pparg);
#define VTOP_StopAbsTimerEx(timer, pparg)  VTOP_StopAbsTimer_SafeEx(timer, VOS_NULL_PTR,VOS_NULL_PTR, pparg)

extern VOS_INT VTOP_StopRelTimer_SafeEx(VTOP_TIMER_ID timer, VOS_VOID(* finallize)(VOS_VOID *), VOS_VOID * parg, VOS_VOID ** pparg);
#define VTOP_StopRelTimerEx(timer, pparg)  VTOP_StopRelTimer_SafeEx(timer, VOS_NULL_PTR, VOS_NULL_PTR, pparg)

/*�ͷŶ�ʱ��API*/
extern VOS_INT VTOP_FreeAbsTimer(VTOP_TIMER_ID timer);
extern VOS_INT VTOP_FreeRelTimer(VTOP_TIMER_ID timer);

/*�޸�ϵͳʱ��ӿ�*/
extern VOS_INT VTOP_SetSysTime(const VTOP_S_TM * pstime);

/*��ʱ��������Ϣ*/
/*��ʱ������*/
#define VTOP_ABS_TIMER  0x1
#define VTOP_REL_TIMER  0x2

/*��ʱ������״̬*/
#define VTOP_TIMER_RUN  0x1
#define VTOP_TIMER_STOP 0x2

/*��ʱ��ά����Ϣ���ݽṹ*/
typedef struct VTOP_TmInfo
{
    VOS_UINT32 ulTmFlag;     /*���/���Զ�ʱ����־��VTOP_ABS_TIMERΪ���Զ�ʱ����VTOP_REL_TIMERΪ��Զ�ʱ�� */
    VOS_VOID * pulTimerID;   /*��ʱ��ָ��*/
    VOS_UINT32 ulTmAttr;     /*��ʱ�����ͣ����ڣ�VTOP_TIMER_LOOP���Զ��ͷţ�VTOP_TIMER_AUTO_FREE��ֹͣ�����ͷţ�VTOP_TIMER_ONE_SHOOT */
    VOS_UINT32 ulTmRunInfo; /*��ʱ������״̬�����У�VTOP_TIMER_RUN��ֹͣ��VTOP_TIMER_STOP*/
    union
    {
        struct tm  stAbsTime; /*���Զ�ʱ����ʱʱ��*/
        VOS_UINT32 ulRelTmLen;/*��Զ�ʱ��ʱ��*/
    }unTime;
    VOS_UINT32 ulLineNo;      /*���붨ʱ�����ļ��к�*/
    const VOS_CHAR * pstrFileName;  /*���붨ʱ�����ļ�����ָ��*/
}VTOP_stTmInfo;

typedef struct VTOP_TmNum
{
    VOS_UINT32 ulTotalAbsTm;  /*�ܹ�����ľ��Զ�ʱ�������������Ѿ��ͷŵ�*/
    VOS_UINT32 ulTotalRelTm;  /*�ܹ��������Զ�ʱ�������������Ѿ��ͷŵ�*/
    VOS_UINT32 ulCurAbsTm;    /*��ǰ����ľ��Զ�ʱ�������������Ѿ��ͷŵ�*/
    VOS_UINT32 ulCurRelTm;    /*��ǰ�������Զ�ʱ�������������Ѿ��ͷŵ�*/
    VOS_UINT32 ulRunAbsTm;    /*��ǰ�������еľ��Զ�ʱ����*/   
    VOS_UINT32 ulRunRelTm;    /*��ǰ�������еľ��Զ�ʱ����*/
} VTOP_stTmNum;

/*��ʱ����Ϣ����*/
extern VTOP_stTmInfo astTmInfo[];

/* ��ʱ������API*/
/*��ö�ʱ����Ϣ����ά����Ϣ�ӿ�*/
extern VTOP_stTmInfo * VTOP_GetTmInfo(VTOP_stTmNum * pstTmNum);
/*��ӡ��ʱ����Ϣ����ά����Ϣ�ӿ�*/
extern VOS_VOID VTOP_PrintTmInfo(const VTOP_stTmInfo* astTimerInfo, const VTOP_stTmNum * pstTmNum);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* __TMR_INTERFACE_H__ */

