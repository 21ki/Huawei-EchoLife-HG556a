/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2006, ��Ϊ�������޹�˾.
  �ļ���: dyinggasp.h
  ����: liuzhijie 00028714
  �汾: V1.0
  ��������: 2006-5-10
  �������: 
  ��������: 
      �������������û���ĺ궨�塢���ݽṹ�ͺ�������
  ��Ҫ�����б�: 
      
  �޸���ʷ��¼�б�: 
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    
  ��ע: 
************************************************************************/
#ifndef _USER_DYINGGASP_H_
#define _USER_DYINGGASP_H_

#if defined(__cplusplus)
extern "C" {
#endif

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

#define TAINT_PROPRIETARY_MODULE	(1<<0)
#define TAINT_FORCED_MODULE		(1<<1)
#define TAINT_UNSAFE_SMP		(1<<2)
#define TAINT_FORCED_RMMOD		(1<<3)

#define ST0_IE			0x00000001
#define ST0_EXL			0x00000002
#define ST0_ERL			0x00000004
#define ST0_KSU			0x00000018
#define KSU_USER		0x00000010
#define KSU_SUPERVISOR	0x00000008
#define KSU_KERNEL		0x00000000
#define ST0_UX			0x00000020
#define ST0_SX			0x00000040
#define ST0_KX 			0x00000080
#define ST0_DE			0x00010000
#define ST0_CE			0x00020000
#define  CAUSEB_EXCCODE		2
#define  CAUSEF_EXCCODE		((unsigned long)(31)  <<  2)

/**********************************************/
/* Structure definition                       */
/*                                            */

/* ����ÿ�ε��ȷ���ʱ�Ľ��̺� */
struct tag_DG_SCHED_LIST
{
    int           iPid;
};
typedef struct tag_DG_SCHED_LIST S_DG_SCHED_LIST;

/* ����ÿ���������һ�α����ȵ�ʱ�� */
struct tag_DG_SCHED_TIME
{
    int           iPid;
    unsigned long   ulTicks;
    struct tag_DG_SCHED_TIME *psNext;
};
typedef struct tag_DG_SCHED_TIME S_DG_SCHED_TIME;

/* ��¼���̵��Ȳ�����Ҫ��¼�����������е��������� */
struct  tag_DG_SCHED_INFO
{
    unsigned long   ulCurNum;                           /*ָ��stSchededList�����е�ǰ�ɼ�¼�Ŀ���λ��*/
    unsigned long   ulCurPID;                           /*��ǰ���еĽ���*/
    unsigned long   ulAllSchedNum;                      /*ϵͳ�ܵ��ȴ���*/
    S_DG_SCHED_LIST sSchededList[DG_MAX_SCHEDLISTRECORD]; /*��¼���̵��ȵ�˳�������*/
    S_DG_SCHED_TIME asSchedTime[DG_MAX_NOUSEPROCESS]; /* ��¼ÿ���������һ�α����ȵ�ʱ������� */
};
typedef struct tag_DG_SCHED_INFO S_DG_SCHED_INFO;

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
	int pid;
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
	int pid;
    long state;
    unsigned long threadinfo;
    unsigned long task_addr;
    S_DG_REG sReg;
    long alStack[DG_MAX_STACK_LEN];
    unsigned long aulTrace[DG_MAX_TRACE_LEN];
};
typedef struct tag_DG_PROCESS_CONTEXT S_DG_PROCESS_CONTEXT;

/* ���������ܽṹ�������ڱ����ڴ��д�Ÿ�ʽ������Ӧ�ò������л�web���ں��ж�ȡ */
struct tag_DYINGGASP_INFO
{
    unsigned long           ulChecksum;    //У���֣���RESERVED_SDRAM_CHECKSUM����
    long                    lTime;         //��¼ʱ��
    S_DG_SCHED_INFO         sSched;        //��¼���̵�����Ϣ
    S_DG_IN_PACK_INFO       sInputPackage; //��¼ϵͳ�յ��ı�������
    S_DG_WEB_OP_INFO sWebOp;            //��¼web����
    S_DG_CLI_OP_INFO sCliOp;            //��¼�����в���
    S_DG_ITF_STATE_INFO sItfStat;       //��¼�˿�״̬�ı�
    S_DG_CURRENT_PROCESS_INFO sCurProc; //��¼��ǰ������Ϣ���ڽ����쳣ʱ��¼
    S_DG_PROCESS_CONTEXT sProcContext[DG_MAX_PROCESS_NUM];  //��¼���н���������
};
typedef struct tag_DYINGGASP_INFO    S_DYINGGASP_INFO;

/**********************************************/
/* Function announcement                      */
/*                                            */
extern int dg_checkDyingGasp(S_DYINGGASP_INFO *psDyingGaspInfo);
extern void dg_outputDyingGasp(S_DYINGGASP_INFO *psDyingGaspInfo);

#if defined(__cplusplus)
}
#endif

#endif /* _USER_DYINGGASP_H_ */

