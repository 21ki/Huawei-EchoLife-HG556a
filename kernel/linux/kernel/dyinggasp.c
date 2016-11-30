/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2006, ��Ϊ�������޹�˾.
  �ļ���: dyinggasp.c
  ����: l28714
  �汾: V1.0
  ��������: 2006-2-16
  �������: 
  ��������: 
      ���������ں˲��ֵĺ���ʵ��
  ��Ҫ�����б�: 
      
  �޸���ʷ��¼�б�: 
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    
  ��ע: 
************************************************************************/
#include <linux/sched.h>
#include <linux/netdevice.h>
#include <linux/time.h>
#include "../../../../../bcmdrivers/opensource/include/bcm963xx/board.h"
#include <linux/dyinggasp.h>

extern void show_trace(struct task_struct *task, unsigned long *stack);

S_DG_NORMAL_INFO sDyinggasp;
/* 
 * ����������־
 *
 * ���HGW��˵��дflashʱ�ر������жϣ����̲��ᱻ���ȣ�
 * ������Ҫ���Ӵ˱�־��֪ͨ��ʱ�����̵��ȵĳ���
 * ������Ʒ���Ը����Լ�����Ҫ�����ô˱�־
 */
S_DG_SCHED_STATE sSchedulerState;

/*------------------------------------------------------------
  ����ԭ��:  void dg_init(void)
  ����: ��ʼ�������������ԵĽṹ
  ����: ��
  ���: stDyinggasp
        stSchedulerState
  ����ֵ: ��
-------------------------------------------------------------*/
void dg_init(void)
{
    memset(&sDyinggasp, 0, sizeof(S_DG_NORMAL_INFO));
    sDyinggasp.ulChecksum = DG_RESERVED_SDRAM_CHECKSUM;

    /* ���Ƚ���Ĭ��״̬Ϊ���� */
    sSchedulerState.ulState = 1;
    sSchedulerState.ulCount = 0;

    /* �����ͷ */
    sDyinggasp.sSched.psSchedTime = kmalloc(sizeof(S_DG_SCHED_TIME), GFP_ATOMIC);
    if (NULL == sDyinggasp.sSched.psSchedTime)
    {
        printk("Malloc memory for schedule record failed.\n");
    }
    memset(sDyinggasp.sSched.psSchedTime, 0, sizeof(S_DG_SCHED_TIME));
}

/***********************************************************
Function:       void dg_recordSchedInfo(task_t *task)
Description:    ÿ�ν��̵���ʱ����¼���̵�����Ϣ
Calls:          NULL
Called By:      schedule
Input:          ��Ҫ�����ȵĽ��̿��ƿ�ָ��
Output:         stDyinggasp.stSched    ��¼���̵�����Ϣ
Return:         NULL
Others:         NULL

----------------------------------------------------------------------------
ʵ�֣�
��¼������̵Ľ���ID�������ڴ�ռ䣨ѭ����ţ�ֻ�������N�����ݣ�
��¼���̵��ȵ�ʱ�䣨׼�����㳤ʱ��δ���ȵĽ�����Ϣ��
************************************************************/
void dg_recordSchedInfo(task_t *task)
{
    S_DG_SCHED_INFO_DYNAMIC *psSchedInfo;
    S_DG_SCHED_TIME *p, *psHead;
    int iPid = (int)task->pid;

    /* �������û�г�ʼ�����ȳ�ʼ�� */
    if (DG_RESERVED_SDRAM_CHECKSUM != sDyinggasp.ulChecksum)
    {
        dg_init();
    }

    psSchedInfo = &(sDyinggasp.sSched);

    /* ֻ�з�0���̲���Ҫ��¼������Ϣ */
    if (0 != iPid)
    {
        psSchedInfo->sSchededList[psSchedInfo->ulCurNum].iPid = iPid;
        psSchedInfo->ulCurNum = (psSchedInfo->ulCurNum + 1) % DG_MAX_SCHEDLISTRECORD;
        psSchedInfo->ulCurPID = iPid;

        psHead = psSchedInfo->psSchedTime;
        /* ��Ϊ�գ������ͷ */
        if (NULL == psHead->psNext)
        {
            psHead->psNext = kmalloc(sizeof(S_DG_SCHED_TIME), GFP_KERNEL);
            if (NULL == psHead->psNext)
            {
                printk("Malloc memory for schedule record failed.\n");
            }
            else
            {
                p = psHead->psNext;
                p->iPid = iPid;
                p->ulTicks = jiffies;
                p->psNext = NULL;
            }        
        }
        else
        {
            p = psHead;
            while (NULL != p->psNext)
            {
                p = p->psNext;
                
                if (iPid == p->iPid)
                {
                    p->ulTicks = jiffies;
                    break;
                }
            }

            if (NULL == p->psNext && p->iPid != iPid)
            {
                p->psNext = kmalloc(sizeof(S_DG_SCHED_TIME), GFP_KERNEL);
                if (NULL == p->psNext)
                {
                    printk("Malloc memory for schedule record failed.\n");
                }
                else
                {
                    p->psNext->iPid = iPid;
                    p->psNext->ulTicks = jiffies;
                    p->psNext->psNext = NULL;
                }        
            }
        }
    }

    /* ��¼ϵͳ�ܵ��ȴ�������Ϊ�ж�ϵͳ�Ƿ��������е����� */
    psSchedInfo->ulAllSchedNum++;

}

/*------------------------------------------------------------
  ����ԭ��: void dg_recordInputPackage(struct sk_buff *skb)
  ����: ��¼ϵͳ�յ������MAX_IPPACKAGENUM������
  ����: skb     ����
  ���: sDyinggasp.sInputPackage     ��¼������Ϣ
  ����ֵ: ��
-------------------------------------------------------------*/
void dg_recordInputPackage(struct sk_buff *skb)
{
    S_DG_IN_PACK_INFO *psInPackInfo = &(sDyinggasp.sInPack);

    memcpy(psInPackInfo->sIpInputBuf[psInPackInfo->ulCurPosition].aucDevName, 
           skb->dev->name, 
           16);
    memcpy(psInPackInfo->sIpInputBuf[psInPackInfo->ulCurPosition].aucMacInfo, 
           skb->mac.ethernet, 
           14);
    memcpy(psInPackInfo->sIpInputBuf[psInPackInfo->ulCurPosition].aucData, 
           skb->data, 
           (skb->len > 1536 ? 1536 : skb->len));
    psInPackInfo->sIpInputBuf[psInPackInfo->ulCurPosition].ulLen = skb->len;
    psInPackInfo->ulCurPosition = (psInPackInfo->ulCurPosition + 1) % DG_MAX_IPPACKAGENUM;
}

/***************************************************************************
Function:       void dg_calcNoUseProcess(ST_SCHEDTIME *, ST_SCHEDTIME **)
Description:    �������δ���ȵĽ��̲���¼
Calls:          NULL
Called By:      dg_storeDyingGaspInfo
Input:          ��¼�����н��̵���ʱ������
Output:         �δ���ȵ�10����������
Return:         NULL
Others:         NULL

----------------------------------------------------------------------------
ʵ�֣�
���ݸ������ϴα����ȵ����ڵ�ʱ��ó��δ���ȵ�10�����̲���¼
***************************************************************************/
void dg_calcNoUseProcess(S_DG_SCHED_TIME *psBefore, S_DG_SCHED_TIME *psAfter)
{
    S_DG_SCHED_TIME *psMin, *p;
    int i, j;

    i = 0;

    /* ֻ��¼ǰ10������ */
    for (i = 0; i < DG_MAX_NOUSEPROCESS; i++)
    {
        /* ����ticks��̵����� */

        /* ��ȡ�ñ���δ��¼��һ��������Ϊ�ȽϵĲο�ֵ */
        p = psBefore;
        psMin = NULL;
        while (NULL != p->psNext)
        {
            p = p->psNext;
            for (j = 0; j < i; j++)
            {
                if (psAfter[j].iPid == p->iPid)
                {
                    break;
                }
            }

            /* ��ǰ���δ��¼ */
            if (j == i)
            {
                psMin = p;
                break;
            }
        }

        /* ��������ȫ����¼�� */
        if (NULL == psMin)
        {
            break;
        }
            
        p = psBefore;
        while (NULL != p->psNext)
        {
            p = p->psNext;
            
            if (time_before(p->ulTicks, psMin->ulTicks))
            {
                /* ȥ�����Ѿ���¼������ */
                for (j = 0; j < i; j++)
                {
                    if (psAfter[j].iPid == p->iPid)
                    {
                        break;
                    }
                }

                /* ���Ѽ�¼������δ�ҵ������� */
                if (j == i)
                {
                    psMin = p;
                }
            }
        }

        /* ��¼���� */
        psAfter[i].iPid = psMin->iPid;
        psAfter[i].ulTicks = (jiffies - psMin->ulTicks) / HZ;
        psAfter[i].psNext = NULL;
    }
}

/***************************************************************************
Function:       void dg_recordWebOp(char *pchStr, int iStrLen)
Description:    �����û�web����
Calls:          NULL
Called By:      board_ioctl
Input:          NULL
Output:         NULL
Return:         NULL
Others:         NULL

***************************************************************************/
void dg_recordWebOp(char *pchStr, int iStrLen)
{
    memset(sDyinggasp.sWebOp.achWebOp[sDyinggasp.sWebOp.ulCurPosition],
           0,
           DG_MAX_WEB_OP_LEN);

    /* �������ַ�����������ֻ����ǰN���ַ� */
    if (iStrLen >= DG_MAX_WEB_OP_LEN)
    {
        memcpy(sDyinggasp.sWebOp.achWebOp[sDyinggasp.sWebOp.ulCurPosition],
               pchStr,
               DG_MAX_WEB_OP_LEN - 1);
    }
    else
    {
        strcpy(sDyinggasp.sWebOp.achWebOp[sDyinggasp.sWebOp.ulCurPosition],
               pchStr);
    }
    
    sDyinggasp.sWebOp.ulCurPosition = (sDyinggasp.sWebOp.ulCurPosition + 1) % DG_MAX_WEB_OP_NUM;
}

/***************************************************************************
Function:       void dg_recordCliOp(char *pchStr, int iStrLen)
Description:    �����û������в���
Calls:          NULL
Called By:      board_ioctl
Input:          NULL
Output:         NULL
Return:         NULL
Others:         NULL

***************************************************************************/
void dg_recordCliOp(char *pchStr, int iStrLen)
{
    memset(sDyinggasp.sCliOp.achCliOp[sDyinggasp.sCliOp.ulCurPosition],
           0,
           DG_MAX_CLI_OP_LEN);

    /* �������ַ�����������ֻ����ǰN���ַ� */
    if (iStrLen >= DG_MAX_CLI_OP_LEN)
    {
        memcpy(sDyinggasp.sCliOp.achCliOp[sDyinggasp.sCliOp.ulCurPosition],
               pchStr,
               DG_MAX_CLI_OP_LEN - 1);
    }
    else
    {
        strcpy(sDyinggasp.sCliOp.achCliOp[sDyinggasp.sCliOp.ulCurPosition],
               pchStr);
    }
    
    sDyinggasp.sCliOp.ulCurPosition = (sDyinggasp.sCliOp.ulCurPosition + 1) % DG_MAX_CLI_OP_NUM;
}

/***************************************************************************
Function:       void dg_recordIntfState(char *dev_name, int state)
Description:    ��¼���ӿ�UP/DOWN��Ϣ
Calls:          NULL
Called By:      bcm63xx_enet_poll_timer
                xDSL�ӿ�״̬��⺯��
Input:          �ӿ������ӿ�״̬
Output:         NULL
Return:         NULL
Others:         NULL

----------------------------------------------------------------------------
ʵ�֣�
��¼��Ӧ�ӿڵ�״̬�͵�ǰϵͳʱ��
***************************************************************************/
void dg_recordIntfState(char *dev_name, int state)
{
    int i;
    /* ����˿�״̬���ϴ�״̬һ�£����ټ�¼ */
    for (i = (sDyinggasp.sItfStat.ulCurPosition + DG_MAX_ITF_STATE_NUM - 1) % DG_MAX_ITF_STATE_NUM;
         i != sDyinggasp.sItfStat.ulCurPosition;
         i = (i + DG_MAX_ITF_STATE_NUM - 1) % DG_MAX_ITF_STATE_NUM)
    {
        if (0 == strcmp(dev_name, sDyinggasp.sItfStat.asItfStat[i].achItfName))
        {
            if (sDyinggasp.sItfStat.asItfStat[i].iItfState == state)
            {
                return;
            }
            else
            {
                break;
            }
        }
    }
    memset(&(sDyinggasp.sItfStat.asItfStat[sDyinggasp.sItfStat.ulCurPosition]),
           0,
           sizeof(S_DG_ITF_STATE));

    strncpy(sDyinggasp.sItfStat.asItfStat[sDyinggasp.sItfStat.ulCurPosition].achItfName,
            dev_name,
            DG_MAX_ITF_NAME_LEN - 1);

    sDyinggasp.sItfStat.asItfStat[sDyinggasp.sItfStat.ulCurPosition].iItfState = state;

    sDyinggasp.sItfStat.asItfStat[sDyinggasp.sItfStat.ulCurPosition].iTime = xtime.tv_sec;

    sDyinggasp.sItfStat.ulCurPosition = (sDyinggasp.sItfStat.ulCurPosition + 1) % DG_MAX_ITF_STATE_NUM;
}
EXPORT_SYMBOL(dg_recordIntfState);

/*------------------------------------------------------------
  ����ԭ��: void dg_recordRegs(struct pt_regs *regs)
  ����: ��¼��ǰ���̼Ĵ�����Ϣ
  ����: regs    �Ĵ�������
  ���: NULL
  ����ֵ: NULL
-------------------------------------------------------------*/
void dg_recordRegs(struct pt_regs *regs)
{
    int i;
    S_DG_REG *psReg = &(sDyinggasp.sCurProc.sReg);

    /*
     * Saved processor id
     */
    psReg->cpu = smp_processor_id();

    /*
     * Saved main processor registers
     */
    for (i = 0; i < 32; ) {
        psReg->regs[i] = regs->regs[i];
    }

    psReg->hi = regs->hi;
    psReg->lo = regs->lo;

    /*
     * Saved cp0 registers
     */
    psReg->cp0_epc = regs->cp0_epc;
    psReg->tainted = tainted;
    psReg->cp0_status = regs->cp0_status;
    psReg->cp0_cause = regs->cp0_cause;
    psReg->prid = read_c0_prid();
}

/*------------------------------------------------------------
  ����ԭ��: void dg_recordModules(void)
  ����: ��¼��ǰ���ص�ģ����Ϣ�������ں�ģ������ָ���Ǿ�̬������ֻ�ðѼ�¼��������module.c�е���
  ����: NULL
  ���: NULL
  ����ֵ: NULL
-------------------------------------------------------------*/
void dg_recordModules(struct list_head *modules)
{
	struct module *mod;
    int i = 0;
 
	list_for_each_entry(mod, modules, list)
	{
		strcpy(sDyinggasp.sCurProc.achModules[i], mod->name);
        i++;
        if (i >= DG_MAX_MODULES_NUM)
        {
            break;
        }
	}
}

/*------------------------------------------------------------
  ����ԭ��: void dg_recordStack(struct task_struct *task, unsigned long *sp)
  ����: ��¼��ǰ���̶�ջ
  ����: task    ��ǰ���̿��ƿ�
        sp      sp�Ĵ���
  ���: NULL
  ����ֵ: NULL
-------------------------------------------------------------*/
void dg_recordStack(struct task_struct *task, unsigned long *sp, long *plStack)
{
	long stackdata;
	int i;

	sp = sp ? sp : (unsigned long *) &sp;

	i = 0;
	while ((unsigned long) sp & (PAGE_SIZE - 1)) {
		if (__get_user(stackdata, sp++)) {
			break;
		}

        plStack[i] = stackdata;
		i++;
	}
}

/*------------------------------------------------------------
  ����ԭ��: void dg_recordTrace(struct task_struct *task, unsigned long *stack)
  ����: ��¼��ǰ�������й켣
  ����: task    ���̿��ƿ�
        stack   sp�Ĵ���
  ���: NULL
  ����ֵ: NULL
-------------------------------------------------------------*/
void dg_recordTrace(struct task_struct *task, unsigned long *stack, unsigned long *pulTrace)
{
	unsigned long addr;
    int i;

	if (!stack)
		stack = (unsigned long*)&stack;

    i = 0;
	while (!kstack_end(stack)) {
		addr = *stack++;
		if (__kernel_text_address(addr)) {
            pulTrace[i] = addr;
		}
        i++;
	}
}

/*------------------------------------------------------------
  ����ԭ��: void dg_recordCode(unsigned int *pc)
  ����: ��¼��ǰִ�����ǰ��Ĵ���
  ����: pc  ��ǰ���д���ָ��
  ���: NULL
  ����ֵ: NULL
-------------------------------------------------------------*/
void dg_recordCode(unsigned int *pc, unsigned long *pulCode)
{
	long i;
	unsigned int insn;

	for(i = -3 ; i < 6 ; i++) {
		if (__get_user(insn, pc + i)) {
			break;
		}
		pulCode[i + 3] = insn;
	}
}

/*------------------------------------------------------------
  ����ԭ��: void show_registers(struct pt_regs *regs)
  ����: ��¼��ǰ��������
  ����: ��ǰ���̼Ĵ���
  ���: NULL
  ����ֵ: NULL
-------------------------------------------------------------*/
void dg_recordRegisters(struct pt_regs *regs)
{
	dg_recordRegs(regs);
    strcpy(sDyinggasp.sCurProc.comm ,current->comm);
    sDyinggasp.sCurProc.pid = current->pid;
    sDyinggasp.sCurProc.threadinfo = (unsigned long)current_thread_info();
    sDyinggasp.sCurProc.task_addr = (unsigned long)current;
	dg_recordStack(current, (long *) regs->regs[29], sDyinggasp.sCurProc.alStack);
	dg_recordTrace(current, (long *) regs->regs[29], sDyinggasp.sCurProc.aulTrace);
	dg_recordCode((unsigned int *) regs->cp0_epc, sDyinggasp.sCurProc.aulCode);
}

/***************************************************************************
Function:       void dg_recordProcessInfo(void)
Description:    ���浱ǰ���н���������
Calls:          NULL
Called By:      dg_storeDyingGaspInfo
Input:          NULL
Output:         NULL
Return:         NULL
Others:         NULL

----------------------------------------------------------------------------
ʵ�֣�
���浱ǰ���н��̵Ľ��̿��ƿ飬���̶�ջ��Ϣ����ǰ����״̬
***************************************************************************/
void dg_recordProcessInfo(S_DYINGGASP_INFO *psInfo)
{
    struct task_struct *task;
    int i;
    
    i = 0;
    for_each_process(task) {
        strcpy(psInfo->sProcContext[i].comm ,task->comm);
        psInfo->sProcContext[i].pid = task->pid;
        psInfo->sProcContext[i].state = task->state;
        psInfo->sProcContext[i].sReg.regs[16] = task->thread.reg16;
        psInfo->sProcContext[i].sReg.regs[17] = task->thread.reg17;
        psInfo->sProcContext[i].sReg.regs[18] = task->thread.reg18;
        psInfo->sProcContext[i].sReg.regs[19] = task->thread.reg19;
        psInfo->sProcContext[i].sReg.regs[20] = task->thread.reg20;
        psInfo->sProcContext[i].sReg.regs[21] = task->thread.reg21;
        psInfo->sProcContext[i].sReg.regs[22] = task->thread.reg22;
        psInfo->sProcContext[i].sReg.regs[23] = task->thread.reg23;
        psInfo->sProcContext[i].sReg.regs[29] = task->thread.reg29;
        psInfo->sProcContext[i].sReg.regs[30] = task->thread.reg30;
        psInfo->sProcContext[i].sReg.regs[31] = task->thread.reg31;
        
        psInfo->sProcContext[i].threadinfo = (unsigned long)(task->thread_info);
        psInfo->sProcContext[i].task_addr = (unsigned long)task;

    	dg_recordStack(task, (long *) task->thread.reg29, psInfo->sProcContext[i].alStack);
    	dg_recordTrace(task, (long *) task->thread.reg29, psInfo->sProcContext[i].aulTrace);

        i++;
        if (i >= DG_MAX_PROCESS_NUM)
        {
            break;
        }
    }
    /* end of maintain dying gasp debug by liuzhijie 00028714 2006��5��11�� */
}

/*------------------------------------------------------------
  ����ԭ��: void dg_storeDyingGaspInfo(void)
  ����: ϵͳ�쳣ʱ��������������
  ����: ��
  ���: sDyinggasp
  ����ֵ: ��
-------------------------------------------------------------*/
void dg_storeDyingGaspInfo(void)
{
    S_DYINGGASP_INFO *psSdram;
    int i;

    psSdram = (S_DYINGGASP_INFO *)DG_RESERVED_SDRAM_START_ADDR;
    memset(psSdram, 0, sizeof(S_DYINGGASP_INFO));

    psSdram->ulChecksum = DG_RESERVED_SDRAM_CHECKSUM;
    psSdram->lTime = xtime.tv_sec;

    psSdram->sSched.ulCurNum = sDyinggasp.sSched.ulCurNum;
    psSdram->sSched.ulCurPID = sDyinggasp.sSched.ulCurPID;
    psSdram->sSched.ulAllSchedNum = sDyinggasp.sSched.ulAllSchedNum;
    memcpy(&(psSdram->sSched.sSchededList), 
           &(sDyinggasp.sSched.sSchededList), 
           sizeof(S_DG_SCHED_LIST) * DG_MAX_SCHEDLISTRECORD);

    dg_calcNoUseProcess(sDyinggasp.sSched.psSchedTime,
                        psSdram->sSched.asSchedTime);

    /* ��¼��ǰ���н��������� */
    dg_recordProcessInfo(psSdram);

    /* ��¼�쳣���̼Ĵ��� */
    memcpy(&(psSdram->sCurProc), 
           &(sDyinggasp.sCurProc), 
           sizeof(S_DG_CURRENT_PROCESS_INFO));

    /* ����ϵͳ�յ��ı��� */
    memcpy(&(psSdram->sInputPackage), 
           &(sDyinggasp.sInPack), 
           sizeof(S_DG_IN_PACK_INFO));

    /* ����web������־ */
    memcpy(&(psSdram->sWebOp), &(sDyinggasp.sWebOp),
           sizeof(S_DG_WEB_OP_INFO));

    /* ���������в�����־ */
    memcpy(&(psSdram->sCliOp), &(sDyinggasp.sCliOp),
           sizeof(S_DG_CLI_OP_INFO));

    /* ����˿�״̬�ı��¼ */
    memcpy(&(psSdram->sItfStat), &(sDyinggasp.sItfStat),
           sizeof(S_DG_ITF_STATE_INFO));

}

/*------------------------------------------------------------
  ����ԭ��: void dg_setScheduleState(int iState)
  ����: ���õ���״̬
  ����: iState      ����״̬
  ���: sSchedulerState
  ����ֵ: ��
-------------------------------------------------------------*/
void dg_setScheduleState(int iState)
{
    if (0 == iState)
    {
        if (1 == sSchedulerState.ulState)
        {
            sSchedulerState.ulState = 0;
        }
        sSchedulerState.ulCount++;
    }
    else if (1 == iState)
    {
        if (0 == sSchedulerState.ulState)
        {
            sSchedulerState.ulCount--;

            if (0 == sSchedulerState.ulCount)
            {
                sSchedulerState.ulState = 1;
            }
        }
    }
    else
    {
        return;
    }
}

/*------------------------------------------------------------
  ����ԭ��: void dg_checkSchedulerAlive(unsigned long psecs)
  ����: ��ʱ�����Ƚ����Ƿ����ڵ��ȣ���û�н��е��ȣ�����ϵͳ�쳣�������ֳ���Ϣ����λ
  ����: psecs       ��ǰtick��
  ���: ��
  ����ֵ: ��
-------------------------------------------------------------*/
void dg_checkSchedulerAlive(unsigned long psecs)
{
    static unsigned long lastMonTime = 0;  //�ϴμ��ʱ��
    static unsigned long lastMonCount = 0; //�ϴμ��ʱ���̵��ܵ�����
    S_DG_SCHED_TIME *p, *q;
    struct task_struct *task;

    if (0 == sSchedulerState.ulState)
    {
        lastMonTime = psecs;
        return;
    }

    if (psecs - lastMonTime >= CIRCLE_MONITOR_INTERVAL) {
        if (lastMonCount != sDyinggasp.sSched.ulAllSchedNum) {
            lastMonCount = sDyinggasp.sSched.ulAllSchedNum;
        }
        else {
            printk("Schedule process dead. Current %s Time %d \n", current->comm, psecs - lastMonTime);
            dg_storeDyingGaspInfo();
            printk("\nReset Board!\n");
            kerSysMipsSoftReset();            
        }
        lastMonTime = psecs;

        /* ��ʱ�Ѽ�¼�������һ�ε���ʱ��Ľṹ���Ѿ��˳��Ľ�������ɾ���� */
        p = sDyinggasp.sSched.psSchedTime;
        while (NULL != p->psNext)// || NULL != p)//������һ�����ݱ�ɾ������p�п���Ϊ��
        {
            for_each_process(task) 
            {
                if (task->pid == p->psNext->iPid)
                {
                    break;
                }
            }
            if (task == &init_task && 1 != p->psNext->iPid)
            {
                q = p->psNext;
                p->psNext = p->psNext->psNext;
                kfree(q);
            }
            else
            {
                p = p->psNext;
            }
        }
    }
}    
