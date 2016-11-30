/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2006, ��Ϊ�������޹�˾.
  �ļ���: dyinggasp.h
  ����: liuzhijie 00028714
  �汾: V1.0
  ��������: 2006-5-10
  �������: 
  ��������: 
      �������������ں˲��ֵĺ궨�塢���ݽṹ�ͺ�������
  ��Ҫ�����б�: 
      
  �޸���ʷ��¼�б�: 
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    
  ��ע: 
************************************************************************/
#ifndef _KERNEL_DYINGGASP_H_
#define _KERNEL_DYINGGASP_H_

#include <linux/skbuff.h>

/**********************************************/
/* Macro definition                           */
/*                                            */

/* �����̵��ȴ���������¼���MAX_SCHEDLISTRECORD�ν��̵��ȹ��� */
#define DG_MAX_SCHEDLISTRECORD                  50

/* ���δ�ý�������������¼����û�б����õĽ��� */
#define DG_MAX_NOUSEPROCESS                     10

/* ��¼������ĸ��� */
#define DG_MAX_IPPACKAGENUM                     10

/* WEB������¼��󳤶� */
#define DG_MAX_WEB_OP_LEN                       256

/* WEB������¼�� */
#define DG_MAX_WEB_OP_NUM                       50

/* �����в�����¼��󳤶� */
#define DG_MAX_CLI_OP_LEN                       128

/* �����в�����¼�� */
#define DG_MAX_CLI_OP_NUM                       50

/* �ӿ�״̬�ı��¼�� */
#define DG_MAX_ITF_STATE_NUM                    100

/* �ӿ�����󳤶� */
#define DG_MAX_ITF_NAME_LEN                     10

/* ģ������󳤶� */
#define DG_MAX_MODULE_NAME                      32

/* ��¼���ģ���� */
#define DG_MAX_MODULES_NUM                      50

/* ��ǰ���̶�ջ��¼���� */
#define DG_MAX_STACK_LEN                        40

/* �������й켣��¼���� */
#define DG_MAX_TRACE_LEN                        10

/* ���̵�ǰִ����������ļ�¼���� */
#define DG_MAX_CODE_LEN                         9

/* ��¼������Ϣ�����Ŀ */
#define DG_MAX_PROCESS_NUM                      50

/* ����У���� */
#define DG_RESERVED_SDRAM_CHECKSUM              0x5AA5A55A

/* �ڴ������ַ */
/**********************ֻ������16M�ĵ�ַ����������Ҫ����*********************/
#if defined (CONFIG_BCM96348)
#define DG_SDRAM_END_ADDR                       0xA0FA0000
#elif defined (CONFIG_BCM96358)
#define DG_SDRAM_END_ADDR                       0xA2000000
#endif

/* �ڴ汣���ռ��ܴ�С */
#define DG_RESERVED_SDRAM_SIZE                  (sizeof(S_DYINGGASP_INFO))

/* �ڴ汣���ռ���ʼ��ַ */
#define DG_RESERVED_SDRAM_START_ADDR            (DG_SDRAM_END_ADDR - DG_RESERVED_SDRAM_SIZE)


/* �����Ƚ����Ƿ������ļ��ʱ�䣬��λtick */
#define CIRCLE_MONITOR_INTERVAL                 5 * HZ

#define DG_DISABLE_SCHED_MON                    0
#define DG_ENABLE_SCHED_MON                     1
/**********************************************/
/* Structure definition                       */
/*                                            */

/* ����ÿ�ε��ȷ���ʱ�Ľ��̺� */
struct tag_DG_SCHED_LIST
{
    pid_t           iPid;
};
typedef struct tag_DG_SCHED_LIST S_DG_SCHED_LIST;

/* ����ÿ���������һ�α����ȵ�ʱ�� */
struct tag_DG_SCHED_TIME
{
    pid_t           iPid;
    unsigned long   ulTicks;
    struct tag_DG_SCHED_TIME *psNext;
};
typedef struct tag_DG_SCHED_TIME S_DG_SCHED_TIME;

/* ��¼���̵��Ȳ�����Ҫ��¼�����������е��������� */
struct  tag_DG_SCHED_INFO_STATIC
{
    unsigned long   ulCurNum;                           /*ָ��stSchededList�����е�ǰ�ɼ�¼�Ŀ���λ��*/
    unsigned long   ulCurPID;                           /*��ǰ���еĽ���*/
    unsigned long   ulAllSchedNum;                      /*ϵͳ�ܵ��ȴ���*/
    S_DG_SCHED_LIST sSchededList[DG_MAX_SCHEDLISTRECORD]; /*��¼���̵��ȵ�˳�������*/
    S_DG_SCHED_TIME asSchedTime[DG_MAX_NOUSEPROCESS]; /* ��¼ÿ���������һ�α����ȵ�ʱ������� */
};
typedef struct tag_DG_SCHED_INFO_STATIC S_DG_SCHED_INFO_STATIC;

/* ��¼���̵��Ȳ�����Ҫ��¼�����������е��������� */
struct  tag_DG_SCHED_INFO_DYNAMIC
{
    unsigned long   ulCurNum;                           /*ָ��stSchededList�����е�ǰ�ɼ�¼�Ŀ���λ��*/
    unsigned long   ulCurPID;                           /*��ǰ���еĽ���*/
    unsigned long   ulAllSchedNum;                      /*ϵͳ�ܵ��ȴ���*/
    S_DG_SCHED_LIST sSchededList[DG_MAX_SCHEDLISTRECORD]; /*��¼���̵��ȵ�˳�������*/
    S_DG_SCHED_TIME *psSchedTime;                      /* ��¼ÿ���������һ�α����ȵ�ʱ������� */
};
typedef struct tag_DG_SCHED_INFO_DYNAMIC S_DG_SCHED_INFO_DYNAMIC;

/* ��¼ϵͳ�յ��ı������� */
struct  tag_DG_IN_PACK_ENTRY
{
    unsigned char   aucDevName[16];
    unsigned char   aucMacInfo[16];
    unsigned char   aucData[1536];
    unsigned long   ulLen;
};
typedef struct  tag_DG_IN_PACK_ENTRY  S_DG_IN_PACK_ENTRY;

/* ѭ������������¼ϵͳ�յ�����󼸸����� */
struct  tag_DG_IN_PACK_INFO
{
    unsigned long           ulCurPosition;
    S_DG_IN_PACK_ENTRY      sIpInputBuf[DG_MAX_IPPACKAGENUM];
};
typedef struct  tag_DG_IN_PACK_INFO S_DG_IN_PACK_INFO;

/* ѭ������������¼ϵͳ�յ�����󼸸�http���� */
struct  tag_DG_WEB_OP_INFO
{
    unsigned long           ulCurPosition;
    char                    achWebOp[DG_MAX_WEB_OP_NUM][DG_MAX_WEB_OP_LEN];
};
typedef struct  tag_DG_WEB_OP_INFO S_DG_WEB_OP_INFO;

/* ѭ������������¼ϵͳ�յ�����󼸸������в��� */
struct  tag_DG_CLI_OP_INFO
{
    unsigned long           ulCurPosition;
    char                    achCliOp[DG_MAX_CLI_OP_NUM][DG_MAX_CLI_OP_LEN];
};
typedef struct  tag_DG_CLI_OP_INFO S_DG_CLI_OP_INFO;

enum E_DG_ITF_STATE
{
    DG_ITF_UP = 0,
    DG_ITF_DOWN,
    DG_XDSL_TRAINING_G992_EXCHANGE,
    DG_XDSL_TRAINING_G992_CHANNEL_ANALYSIS,
    DG_XDSL_TRAINING_G992_STARTED,
    DG_XDSL_TRAINING_G994,
    DG_XDSL_G994_NONSTDINFO_RECEIVED,
    DG_XDSL_BERT_COMPLETE,
    DG_XDSL_ATM_IDLE,
    DG_XDSL_EVENT,
    DG_XDSL_G997_FRAME_RECEIVED,
    DG_XDSL_G997_FRAME_SENT
};

/* �˿�״̬��¼ */
struct tag_DG_ITF_STATE
{
    char achItfName[DG_MAX_ITF_NAME_LEN];   //�ӿ���
    int iItfState;                          //�ӿ�״̬��0:up 1:down
    int iTime;                              //�ӿ�״̬�ı�ʱ��(���ϵͳ������ʱ��)
};
typedef struct tag_DG_ITF_STATE S_DG_ITF_STATE;

/* ѭ��������¼��󼸴ζ˿�״̬�ı� */
struct tag_DG_ITF_STATE_INFO
{
    unsigned long   ulCurPosition;
    S_DG_ITF_STATE  asItfStat[DG_MAX_ITF_STATE_NUM];    
};
typedef struct tag_DG_ITF_STATE_INFO S_DG_ITF_STATE_INFO;

struct tag_DG_REG
{
    unsigned long cpu;
	/* Saved main processor registers. */
	unsigned long regs[32];

	/* Saved special registers. */
	unsigned long cp0_status;
	unsigned long lo;
	unsigned long hi;
	unsigned long cp0_badvaddr;
	unsigned long cp0_cause;
	unsigned long cp0_epc;
    int tainted;
    unsigned long prid;
};    
typedef struct tag_DG_REG S_DG_REG;

/* ��¼�����쳣��Ϣ */
struct tag_DG_CURRENT_PROCESS_INFO
{
    //�ѼĴ�������ջ�Ƚṹ����һ�ݣ����û�̬������ʾ�����Բο��ں���ʾ�ķ�ʽ
	char comm[16];
	pid_t pid;
    unsigned long threadinfo;
    unsigned long task_addr;
    S_DG_REG sReg;
    char achModules[DG_MAX_MODULES_NUM][DG_MAX_MODULE_NAME];
    long alStack[DG_MAX_STACK_LEN];
    unsigned long aulTrace[DG_MAX_TRACE_LEN];
    unsigned long aulCode[DG_MAX_CODE_LEN];
};
typedef struct tag_DG_CURRENT_PROCESS_INFO S_DG_CURRENT_PROCESS_INFO;

struct tag_DG_PROCESS_CONTEXT
{
    //������������ݣ�����״̬���Ĵ�������ջ
	char comm[16];
	pid_t pid;
    long state;
    unsigned long threadinfo;
    unsigned long task_addr;
    S_DG_REG sReg;
    long alStack[DG_MAX_STACK_LEN];
    unsigned long aulTrace[DG_MAX_TRACE_LEN];
};
typedef struct tag_DG_PROCESS_CONTEXT S_DG_PROCESS_CONTEXT;

/* ϵͳ��������ʱ��Ҫ��¼������������Ϣ���ڸ�λ֮ǰ���浽ָ���ڴ��� */
struct tag_DG_NORMAL_INFO
{
    unsigned long           ulChecksum;    //У���֣���RESERVED_SDRAM_CHECKSUM����
    S_DG_SCHED_INFO_DYNAMIC sSched;     //��¼���̵�����Ϣ
    S_DG_IN_PACK_INFO sInPack;          //��¼ϵͳ�յ��ı�������
    S_DG_WEB_OP_INFO sWebOp;            //��¼web����
    S_DG_CLI_OP_INFO sCliOp;            //��¼�����в���
    S_DG_ITF_STATE_INFO sItfStat;       //��¼�˿�״̬�ı�
    S_DG_CURRENT_PROCESS_INFO sCurProc; //��¼��ǰ������Ϣ���ڽ����쳣ʱ��¼
    S_DG_PROCESS_CONTEXT sProcContext[DG_MAX_PROCESS_NUM];  //��¼���н���������
};
typedef struct tag_DG_NORMAL_INFO S_DG_NORMAL_INFO;
    
/* ���������ܽṹ�������ڱ����ڴ��д�Ÿ�ʽ������Ӧ�ò������л�web���ں��ж�ȡ */
struct tag_DYINGGASP_INFO
{
    unsigned long           ulChecksum;    //У���֣���RESERVED_SDRAM_CHECKSUM����
    long                    lTime;         //��¼ʱ��
    S_DG_SCHED_INFO_STATIC  sSched;        //��¼���̵�����Ϣ
    S_DG_IN_PACK_INFO       sInputPackage; //��¼ϵͳ�յ��ı�������
    S_DG_WEB_OP_INFO sWebOp;            //��¼web����
    S_DG_CLI_OP_INFO sCliOp;            //��¼�����в���
    S_DG_ITF_STATE_INFO sItfStat;       //��¼�˿�״̬�ı�
    S_DG_CURRENT_PROCESS_INFO sCurProc; //��¼��ǰ������Ϣ���ڽ����쳣ʱ��¼
    S_DG_PROCESS_CONTEXT sProcContext[DG_MAX_PROCESS_NUM];  //��¼���н���������
};
typedef struct tag_DYINGGASP_INFO    S_DYINGGASP_INFO;

/* ����״̬��־ */
struct tag_DG_SCHED_STATE
{
    unsigned long       ulState;        //���Ƚ���״̬��Ĭ��ΪTRUE������ΪFALSE���򲻶�ʱ��ؽ����Ƿ����ڵ���
    unsigned long       ulCount;          //���ü���������ж���������øñ�־ΪFALSE����������Щ������ֶ����ñ�־ΪTRUE���Ž��м��
};
typedef struct tag_DG_SCHED_STATE S_DG_SCHED_STATE;

/**********************************************/
/* Variable announcement                      */
/*                                            */

/**********************************************/
/* Function announcement                      */
/*                                            */
extern void dg_init(void);
extern void dg_recordSchedInfo(task_t *task);
extern void dg_checkResetStatus(int iTimes);
extern void dg_recordInputPackage(struct sk_buff *skb);
extern void dg_checkSchedulerAlive(unsigned long psecs);
extern void dg_storeDyingGaspInfo(void);
extern void dg_recordWebOp(char *pchStr, int iStrLen);
extern void dg_recordCliOp(char *pchStr, int iStrLen);
extern void dg_recordIntfState(char *dev_name, int state);
extern void dg_recordRegisters(struct pt_regs *regs);


#endif /* _KERNEL_DYINGGASP_H_ */

