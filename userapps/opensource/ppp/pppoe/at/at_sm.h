/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2005, ��Ϊ�������޹�˾.
  �ļ���: at_sm.h
  ����: lichangqing 45517
  �汾: V500R003
  ��������: 2005-8-8
  �������: 2005-8-10
  ��������: ͷ�ļ�
      
  ��Ҫ�����б�: ��
      
  �޸���ʷ��¼�б�: 
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    l45517      20050816    0.0.1    ��ʼ���
  ��ע: 
************************************************************************/

#ifndef __AT_SM_H
#define __AT_SM_H

// Ϊÿ��AT�����������
#define AT_RSSI_QUERY    			 0	 // RSSI��ѯ��������			 RSSI_QUERY
#define AT_SYSINFO_QUERY			 1	 // ϵͳ��Ϣ��ѯ				 SYSTEM_INFO_QUERY
#define AT_SYSCFG_SET  				 2 	//������������				SYSCFG_SET
#define AT_PDP_SET					 3	//����PDP����					PDP_SET
#define AT_AUTO_SERVICE_REPORT 	 4	//����״̬�仯ָʾ			AUTO_SERVICE_STATUS_REPORT
#define AT_AUTO_SYSTEM_REPORT		 5	//ϵͳģʽ�仯�¼�ָʾ 	AUTO_SYSTEM_MODE_STATUS_REPORT
#if 0
#define AT_0         0 // ��������         AT       <CR><LF>OK<CR><LF>*/
#define AT_GMR_1     1 // ����汾�ţ�     +GMR     <CR><LF>+GMR:<softversion><CR><LF>   ��MS��ش���ʱ��<CR><LF>ERROR<CR><LF>
#define AT_HWVER_2   2 // Ӳ���汾�Ų�ѯ   ^HWVER   <CR><LF>^HWVER:<hardversion><CR><LF> ��MS��ش���ʱ��<CR><LF>ERROR<CR><LF>
#define AT_ESN_3     3 // ESN��ѯ����      +GSN     <CR><LF>+GSN: <ESN><CR><LF>          ��MS��ش���ʱ��<CR><LF>ERROR<CR><LF>
#define AT_SYSINFO_4 4 // ϵͳ��Ϣ��ѯ     ^SYSINFO <CR><LF>^SYSINFO:< srv_status >,< srv_domain >,< roam_status >,< sys_mode >,< sim_state ><CR><LF>
#define AT_CSQ_5     5 // RSSI��ѯ�������� +CSQ     <CR><LF>+CSQ: <rssi>,<ber><CR><LF><CR><LF>OK<CR><LF> ��MS��ش���ʱ��<CR><LF>+CME ERROR: <err><CR><LF>
#define AT_HDRCSQ_6  6 // evdo RSSI ��ѯ   ^HDRCSQ  <CR><LF>^HDRRSSI: <rssi><CR><LF>
#define AT_NETMODE   7//  ����ģʽ����   

/*add by sxg*/
#define AT_PDP_CONTEXT 8//����PDP����
#define AT_OPERATOR    9 //������Ӫ��ѡ��ģʽ: �Զ�/�ֶ�
#define AT_SYSCFG   10 //������������: GPRS/3G,����ͨ����������
#endif



int at_sm_is_active();
int at_sm_submit_at(int at_num,  int* is_report_err, char* param);
//int at_sm_initialize();
//int at_sm_destroy();
int at_sm_modem_submit_at(int modem_fd, char* at_dial_str);

#define SYSMODE_WCDMA       5
#define SYSMODE_CDMA        2
#define SYSMODE_NO_SERVICES 0

#define LOOP_TIMES 5

#define MAX_CMD_LEN    9  // �ϱ���������ƥ�䳤��
#define MAX_REPORT_NUM 7  // �����ϱ�����Ŀ
#define READ_SIZE      64 // ÿ����ͼ�Ӵ��ڶ�64���ַ�
#define AT_REPORT_OK   1
#define AT_REPORT_ERR  0
#define PARSE_OK		1
#define PARSE_ERROR		0
// A064D00428 EC506 ADD (by l45517 2005��11��21?) BEGIN
#define AT_REPORT_BUSY AT_REPORT_OK
// A064D00428 EC506 ADD (by l45517 2005��11��21?) END

#if 0
typedef struct AT_HANDLE_S
{
    char* at_cmd_str;
    char* at_report_str;
    char* at_report_err_str;
    void  (*create_at_cmd)(char* buf, char* param);
    int  (*at_sm_report_handler)(char* report_buf);
}AT_HANDLE_T;
#endif
typedef struct AT_HANDLE_S
{
	int at_cmd_id;
	void* at_cmd_struct;
	int  (*at_cmd_report_handler)(struct stAtRcvMsg *pstAtRcvMsg);
	char* at_cmd_str;
	void  (*create_at_cmd)(char* buf, char* param);
}AT_HANDLE_T;

 
#endif // __AT_SM_H

