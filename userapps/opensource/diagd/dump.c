/***********************************************************************
  ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2005, ��Ϊ�������޹�˾.
  �ļ���: dump.c
  ����: y45539
  �汾: V500R003
  ��������: 2005-8-16
  �������: 2005-8-16
  ��������: ��ӡ������Ϣ������
      
  ��Ҫ�����б�: 
    void show_usuage()
    ��ӡdiagd��������
    int diag_parse_lines(int argc, char **argv)
    ���������д������
    int send_to_net(char* acbuf, int irLen, int arg)
    �������ݵ�����
    int echo_to_net(char* acbuf, int iLen, int arg)
    ��MODEM,_PCUI��ʱ����ӡ��ʾ���ͻس���
�޸���ʷ��¼�б�: 
<��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>
    
  ��ע: 
************************************************************************/
#include "dump.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

void info (char *fmt, ...)
{
    char buf[1024];
    va_list pvar;

    /*��¼��־�ļ��ĳ���*/
    static long file_len=0;

#if defined(__STDC__)
    va_start(pvar, fmt);
#else
    char *fmt;
    va_start(pvar);
    fmt = va_arg(pvar, char *);
#endif
    vsprintf (buf, fmt, pvar);
    va_end(pvar);
    int err;
    err = write( g_logsys, buf, strlen(buf) );
    if (err < 0)
    {
       send_to_net("error logsys", sizeof ("error logsys"), 0);
    }
    else
    {
        file_len += sizeof(buf);
        if ((32*1024) <= file_len)
        {
            //�ص��ļ���ͷ����д�ļ�
            lseek( g_logsys, 0, SEEK_SET );
        }
    }
}

/*------------------------------------------------------------
  ����ԭ��: void show_usuage()
  ����: ��ӡdiagd�����ʹ�÷���
  ����: ��
  ���: ��
  ����ֵ: ��
-------------------------------------------------------------*/
void show_usuage( void )
{
     printf("%s Usage: \n %s\n%s\n%s\n%s\n", "diagd function",
                 "      diagd -d DevPortNum",
                 "      diagd -p NetPortNum",
                 "      Notice! NetPortNum 1024~65535");
}

/*------------------------------------------------------------
  ����ԭ��: int parse_lines(argc, argv)
  ����: ���������в���������򿪵Ķ˿���AT��PCUI�˿ڣ����Ӧ_AT,_PCUIö�ٱ���
        ��������Ӧ��͸��AT�������͸��DIAG������
  ����: int argc    ��������
        char **argv ��������
  ���:  ��
  ����ֵ:�˿�ö��ֵ 
         _MODEM   MODEMAT��
         _DIAG DIAG��
         _PCUI PCUI��
         _VOICE PCSC��
-------------------------------------------------------------*/
int diag_parse_lines (int argc, char **argv)
{
#if 0

    if (argc <= 1)
    {
        g_ePort = _DIAG;
        return OPER_SUCCESS;
    }
    else if (argc > 3)
    {
        show_usuage();
        return OPER_FAILURE;
    }

    if (!strcmp(argv[1],"-modem"))
    {
      g_ePort = _MODEM;
    }
    else if (!strcmp(argv[1],"-diag"))
    {
      g_ePort = _DIAG;
    }
    else if (!strcmp(argv[1],"-pcui"))
    {
      g_ePort = _PCUI;
    }
     if (argc == 3)
     {
        char* ptr = strchr(argv[2], '=');
        int     i = 0;
        char  bFound = 0;

        while ( (NULL != ptr) && (*(ptr+i) != '\0' ))
        {
            if ( !isdigit( *(ptr+i) ) && i >= 5 )
            {
                bFound = -1;
                break;
            }
            i++;
        }
            
        if( (NULL != ptr) && (bFound == 0) && (atoi( ptr ) > 1024) && (atoi( ptr ) < 65535))
        {
            g_Serverport = atoi( ptr );
        }
        else
        {
            return OPER_FAILURE;
        }
     }
#endif
    int opt;

    while ((opt = getopt(argc, argv, "d:p:")) > 0) {
		switch (opt) {
		case 'd':
			g_ePort = atoi(optarg);
			printf("g_ePort %d\r\n", g_ePort);
			break;
		case 'p':
			g_Serverport = atoi(optarg);
			printf("g_Serverport %d\r\n", g_Serverport);
			break;
		default:
			show_usuage();
			return OPER_FAILURE;
		}
    	}
 
    return OPER_SUCCESS;
}

/*------------------------------------------------------------
  ����ԭ��: int send_to_net(char* acbuf, int irLen, int arg)
  ����: �������ݵ����ڣ���������ܹ����������ݾͻ�ѭ�����ͣ�
               ֱ���Ѵ�diag�ڽ��յ�����ȫ�����ͳ�ȥ��
  ����: 
                char*    pbuf           ���ͻ���
                int      rcv_len        ���ͻ��峤��
                int      arg            TCP����ʱ���Ʋ���
  ��     �� : ��
  ����ֵ: �ɹ�����0 ʧ�ܷ��ظ���
-------------------------------------------------------------*/
int send_to_net(char* pbuf, int rcv_len, int arg)
{
    int   send_len = 0;
    char *pCur = pbuf;
    
    while( rcv_len != 0 )
    {
        send_len = send (g_netfd, pCur, rcv_len, arg);                
        
        if ( ( (send_len == SOCKET_ERROR) 
            && (errno != EINTR))
            ||(send_len == 0))
        {
            return OPER_FAILURE;
        }
        rcv_len -= send_len;
        pCur    += send_len;
    }
    return OPER_SUCCESS;
}

/*------------------------------------------------------------
  ����ԭ��: int echo_to_net(char* acNet_rcvbuf, int iNet_rlen, int arg)
  ����: ���Ե����⴮��
  ����: char* acNet_rcvbuf ���ͻ���
        int iNet_rlen     ���ͻ��峤��
        int arg           TCP����ʱ���Ʋ���
  ���: ��
  ����ֵ: �ɹ�����0 ʧ�ܷ��ظ���
-------------------------------------------------------------*/
int echo_to_net(char* acbuf, int iLen, int arg)
{
     int realLen = iLen;
     char echo_buf[BUFFER_LEN+10];
     strncpy(echo_buf, acbuf, iLen);
     static unsigned int alter = 0;

     if ((alter%2 == 1) && (acbuf[iLen-1] == _r || acbuf[iLen-1] == _n))
     {
         echo_buf [iLen-1] = _n;
         echo_buf [iLen] =   _r;
         echo_buf [iLen+1] = _start;
         realLen = iLen + 2;
     }
     alter++;     
     return send_to_net(echo_buf, realLen, arg);
}

