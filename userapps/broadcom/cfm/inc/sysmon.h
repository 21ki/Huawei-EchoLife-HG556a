/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2006, ��Ϊ�������޹�˾.
  �ļ���: syswatch.h
  ����: l28714
  �汾: V1.0
  ��������: 2006-4-21
  �������: 
  ��������: 
      
  ��Ҫ�����б�: 
      
  �޸���ʷ��¼�б�: 
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    
  ��ע: 
************************************************************************/
#ifndef __SYS_MON__
#define __SYS_MON__

#if defined(__cplusplus)
extern "C" {
#endif

/***********************************************************************
*                             �궨��                                   *
************************************************************************/
#define NO_ERROR            0
#define ERR_OPEN_FILE       1
#define ERR_NULL_POINTER    2
#define ERR_INVALID_VALUE   3
#define ERR_OPEN_DIR        4
#define MAX_ERRNO           5

#define NEED_DISPLAY        1
#define NO_DISPLAY          0
/***********************************************************************
*                            ���ݽṹ                                  *
************************************************************************/
typedef struct tag_ERROR_NUMBER
{
    int iErrNo;
    char *pchErrInfo;
}S_ERROR_NUMBER;

/* ���������Ϣ������PID������״̬���ڴ�ռ�ô�С��CPUռ���ʣ����������� */
typedef struct tag_SYSMON_PROCESS_STATUS
{
    int		iPid;       /* ����ID */
    char	achState[4];    /* ����״̬ */
    long	lMem;       /* �����ڴ�ռ�ô�С(��λKB) */
    int		iCpu;       /* CPUռ����(��λǧ��֮һ) */
    char	achCmd[16]; /* ����ִ�е�����(����������) */
    int     iDispFlag;  /* �����Ƿ���Ҫ��ʾ�ı�־ */
    struct tag_SYSMON_PROCESS_STATUS *psNext;
} S_SYSMON_PROCESS_STATUS, *PS_SYSMON_PROCESS_STATUS; 

/* ����ϵͳ��Դռ����� */
typedef struct tag_SYSMON_TOTAL_USAGE
{
    int iTotal;     /* ���ڴ�(��λKB) */
    int iUsed;      /* ʹ�õ��ڴ�(��λKB) */
    int iFree;      /* �����ڴ�(��λKB) */
    int iBuffers;   /* ������ռ�õ��ڴ�(��λKB) */
    int iUCpu;      /* �û�̬CPUռ����(��λǧ��֮һ) */
    int iSCpu;      /* �ں�̬CPUռ����(��λǧ��֮һ) */
}S_SYSMON_TOTAL_USAGE, *PS_SYSMON_TOTAL_USAGE;

/***********************************************************************
*                            ��������                                  *
************************************************************************/
extern void sysmon_main(void);
extern void init_Hertz_value(void);
extern S_SYSMON_PROCESS_STATUS * procps_scan(void);

#if defined(__cplusplus)
}
#endif

#endif /* __SYS_MON__ */
