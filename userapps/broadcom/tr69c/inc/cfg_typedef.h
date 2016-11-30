/***********************************************************************
    ��Ȩ��Ϣ: ��Ȩ����(C) 1988-2005, ��Ϊ�������޹�˾.
    �ļ���:
    ����: Diao.Meng
    �汾: HG550V200R001B02
    ��������: 2005-4-18
    �������:
    ��������:

    ��Ҫ�����б�:

    �޸���ʷ��¼�б�:
    <��  ��>    <�޸�ʱ��>  <�汾>  <�޸�����>

    ��ע:
************************************************************************/


#ifndef _CFG_TYPEDEF_H_
#define _CFG_TYPEDEF_H_

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef WIN32
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#ifdef WIN32
    #ifndef XINLINE
    #define XINLINE
    #endif
#else
    #define XINLINE inline
#endif

typedef enum {
    TRX_OK=0,
    TRX_REBOOT,
    TRX_ERR
}TRX_STATUS;

#ifndef VOID
#define VOID void
#endif

#ifndef CHAR
#define CHAR char
#endif

#ifndef UCHAR
#define UCHAR unsigned char
#endif

#ifndef SHORT
#define SHORT short
#endif

#ifndef USHORT
#define USHORT  unsigned short
#endif

#ifndef LONG
#define LONG long
#endif

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef BOOL
#define BOOL unsigned int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifdef NULL
#undef NULL
#endif

#define NULL    0
/*Some systems define NULL as (-1), in our system, NULL must be (0). */

typedef CHAR  DATE_TIME;

#define MAX_DATE_TIME_LEN   20      /* YYYY-MM-DDTHH:MM:SS */


#define STRING_LEN_6                     6
#define STRING_LEN_16                   16
#define STRING_LEN_32                   32
#define STRING_LEN_64                   64
#define STRING_LEN_128                 128
#define STRING_LEN_256                 256

/* HUAWEI HGW ��С�� 2005��6��30��
  */
/*
#ifdef _DEBUG
#define DBGONLY(x)  x
#else
#define DBGONLY(x)
#endif
*/

/*
 *  ���y > 0, x  ȡx  ��y ֮�����Сֵ
 */
#define MIN(x, y)   ( ((x) < (y)) ? (x) : (y) )
#define MAX(x, y)   ( ((x) < (y)) ? (y) : (x) )

#ifdef  __cplusplus
}
#endif

#endif /*End of the file.*/


