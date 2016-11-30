/***************************************************************************
 * Broadcom Corp. Confidential
 * Copyright 2001 Broadcom Corp. All Rights Reserved.
 *
 * THIS SOFTWARE MAY ONLY BE USED SUBJECT TO AN EXECUTED
 * SOFTWARE LICENSE AGREEMENT BETWEEN THE USER AND BROADCOM.
 * YOU HAVE NO RIGHT TO USE OR EXPLOIT THIS MATERIAL EXCEPT
 * SUBJECT TO THE TERMS OF SUCH AN AGREEMENT.
 *
 ***************************************************************************
 * File Name  : board_api.c
 *
 * Description: interface for board level calls: flash,
 *              led, soft reset, free memory page, and memory dump.
 *              Adapted form flash_api.c by Yen Tran.
 *
 * Created on : 02/20/2002  seanl
 *1������FLASH VA����by l39225 2006-5-8
 ***************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>      /* open */
#include <unistd.h>     /* exit */
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ioctl.h>  /* ioctl */

#include <memory.h>
#include <syslog.h>



#include <bcmtypes.h>
#include <board_api.h>
/* start of maintain dying gasp by liuzhijie 00028714 2006��5��9��" */
#include "dyinggasp.h"
/* end of maintain dying gasp by liuzhijie 00028714 2006��5��9��" */

int boardIoctl(int board_ioctl, BOARD_IOCTL_ACTION action, char *string, int strLen, int offset, char *buf)
{
    BOARD_IOCTL_PARMS IoctlParms;
    int boardFd = 0;

    boardFd = open("/dev/brcmboard", O_RDWR);
    if ( boardFd != -1 ) {
        IoctlParms.string = string;
        IoctlParms.strLen = strLen;
        IoctlParms.offset = offset;
        IoctlParms.action = action;
        IoctlParms.buf    = buf;
        ioctl(boardFd, board_ioctl, &IoctlParms);
        close(boardFd);
        boardFd = IoctlParms.result;
    } else
        printf("Unable to open device /dev/brcmboard.\n");

    return boardFd;
}


int sysSetCliPid(int iPid)
{
	return (boardIoctl(BOARD_IOCTL_SET_TELNETD_PID, 0, NULL, 0, iPid, NULL));
}

int sysGetKeyMode(void)
{
	return (boardIoctl(BOARD_IOCTL_GET_KEY_MODE, 0, NULL, 0, 0, NULL));
}


int sysGetEquipTestMode(void)
{
	return (boardIoctl(BOARD_IOCTL_GET_EQUIPTEST_MODE, 0, NULL, 0, 0, NULL));
}

#ifdef USE_ALL



/*****************************************************************************
* sysScratchPadGet - get the contents of persistent stratch pad flash memory
* INPUT:   tokenId -- token id, ASCIZ tokBuf (up to 15 char)
*          tokBuf  -- buf 
*          bufLen  -- buf len
* RETURNS: 0 - ok, -1 fail.
*/
int sysScratchPadGet(char *tokenId, char *tokBuf, int bufLen)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_READ, SCRATCH_PAD, tokenId, bufLen, bufLen, tokBuf));
}

/*****************************************************************************
* sysScratchPadSet - write the contents of persistent scratch pad flash memory
* INPUT:   tokenId -- token id, ASCIZ tokBuf (up to 15 char)
*          tokBuf  -- buf 
*          bufLen  -- buf len
* RETURNS: 0 - ok, -1 fail.
*/
int sysScratchPadSet(char *tokenId, char *tokBuf, int bufLen)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_WRITE, SCRATCH_PAD, tokenId, bufLen, bufLen, tokBuf));
}

/*****************************************************************************
* sysScratchPadClearAll  - wipeout the scratch pad
* RETURNS: 0 - ok, -1 fail.
*/
int sysScratchPadClearAll(void)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_WRITE, SCRATCH_PAD, "", -1, -1, ""));
}

/*****************************************************************************
* sysPersistentGet - get the contents of non-volatile RAM,
* RETURNS: OK, always.
*/
int sysNvRamGet(char *string, int strLen, int offset)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_READ, NVRAM, string, strLen, offset, ""));
}

/*****************************************************************************
* sysPersistentSet - write the contents of non-volatile RAM
* RETURNS: OK, always.
*/
int sysNvRamSet(char *string, int strLen, int offset)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_WRITE, NVRAM, string, strLen, offset, ""));
}

/*****************************************************************************
* sysNrPagesGet - returns number of free system pages.  Each page is 4K bytes.
* RETURNS: Number of 4K pages.
*/
int sysNrPagesGet(void)
{
    return (boardIoctl(BOARD_IOCTL_GET_NR_PAGES, 0, "", 0, 0, ""));
}

/*****************************************************************************
* sysDumpAddr - Dump kernel memory.
* RETURNS: OK, always.
*/
int sysDumpAddr(char *addr, int len)
{
    return (boardIoctl(BOARD_IOCTL_DUMP_ADDR, 0, addr, len, 0, ""));
}

/*****************************************************************************
* sysDumpAddr - Set kernel memory.
* RETURNS: OK, always.
*/
int sysSetMemory(char *addr, int size, unsigned long value )
{
    return (boardIoctl(BOARD_IOCTL_SET_MEMORY, 0, addr, size, (int) value, ""));
}

/*****************************************************************************
 * image points to image to be programmed to flash; size is the size (in bytes)
 * of the image.
 * if error, return -1; otherwise return 0
 */

#endif // USE_ALL
 /*start of  ����FLASH VA����porting by l39225 20060508*/
/*************************************************
  Function:        sysVariableGet
  Description:    get va data
  Input:            string : ָ�������ݵ��ڴ�����
                       strLen: ���ݳ���
                       offset:  ƫ��ֵ
  Output:           ��
  Return:          -1 : ʧ��
                       >0 :�ɹ�
  Others:          ��
 *************************************************/
int sysVariableGet(char *string, int strLen, int offset)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_READ, VARIABLE, string, strLen, offset, ""));
}

/*************************************************
  Function:        sysFixSet
  Description:    set fix data
  Input:            string : ָ�������ݵ��ڴ�����
                       strLen: ���ݳ���
                       offset:  ƫ��ֵ
  Output:           ��
  Return:          -1 : ʧ��
                       >0 :�ɹ�
  Others:          ��
 *************************************************/
int sysFixSet(char *string, int strLen, int offset)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_WRITE, FIX, string, strLen, offset, ""));
}

 /*************************************************
   Function:        sysFixGet
   Description:    get fix data
   Input:            string : ָ�������ݵ��ڴ�����
                        strLen: ���ݳ���
                        offset:  ƫ��ֵ
   Output:           ��
   Return:          -1 : ʧ��
                        >0 :�ɹ�
   Others:          ��
  *************************************************/
 int sysFixGet(char *string, int strLen, int offset)
 {
     return (boardIoctl(BOARD_IOCTL_FLASH_READ, FIX, string, strLen, offset, ""));
 }

 /*************************************************
   Function:        sysAvailGet
   Description:    get avail data
   Input:            string : ָ�������ݵ��ڴ�����
                        strLen: ���ݳ���
                        offset:  ƫ��ֵ
   Output:           ��
   Return:          -1 : ʧ��
                        >0 :�ɹ�
   Others:          ��
  *************************************************/
 int sysAvailGet(char *string, int strLen, int offset)
 {
     return (boardIoctl(BOARD_IOCTL_FLASH_READ, AVAIL, string, strLen, offset, ""));
 }
 
 /*************************************************
  Function:        sysAvailSet
  Description:    set avail data
  Input:            string : ָ�������ݵ��ڴ�����
                       strLen: ���ݳ���
                       offset:  ƫ��ֵ
  Output:           ��
  Return:          -1 : ʧ��
                       >0 :�ɹ�
  Others:          ��
 *************************************************/
int sysAvailSet(char *string, int strLen, int offset)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_WRITE, AVAIL, string, strLen, offset, ""));
}


/*****************************************************************************
 �� �� ��  : sysFirewallLogGet
 ��������  : ��÷���ǽ��־��FLASH
 �������  : char *string  
             int strLen    
             int offset    
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2008��9��11��
    ��    ��   : c00131380
    �޸�����   : �����ɺ���

*****************************************************************************/
int sysFirewallLogGet(char *string, int strLen, int offset)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_READ, FIREWALL_LOG, string, strLen, offset, ""));
}

 /*****************************************************************************
  �� �� ��  : sysFirewallLogSet
  ��������  : �������ǽ��־��FLASH
  �������  : char *string  
              int strLen    
              int offset    
  �������  : ��
  �� �� ֵ  : int
  ���ú���  : 
  ��������  : 
  
  �޸���ʷ      :
   1.��    ��   : 2008��9��11��
     ��    ��   : c00131380
     �޸�����   : �����ɺ���
 
 *****************************************************************************/
 int sysFirewallLogSet(char *string, int strLen, int offset)
 {
     return (boardIoctl(BOARD_IOCTL_FLASH_WRITE, FIREWALL_LOG, string, strLen, offset, ""));
 }


 /*****************************************************************************
  �� �� ��  : sysSystemLogGet
  ��������  : ��ȡϵͳ��־��FLASH
  �������  : char *string  
              int strLen    
              int offset    
  �������  : ��
  �� �� ֵ  : int
  ���ú���  : 
  ��������  : 
  
  �޸���ʷ      :
   1.��    ��   : 2008��9��26��
     ��    ��   : c00131380
     �޸�����   : �����ɺ���
 
 *****************************************************************************/
 int sysSystemLogGet(char *string, int strLen, int offset)
 {
     return (boardIoctl(BOARD_IOCTL_FLASH_READ, SYSTEM_LOG, string, strLen, offset, ""));
 }

 /*****************************************************************************
  �� �� ��  : sysSystemLogSet
  ��������  : ����ϵͳ��־��FLASH
  �������  : char *string  
              int strLen    
              int offset    
  �������  : ��
  �� �� ֵ  : int
  ���ú���  : 
  ��������  : 
  
  �޸���ʷ      :
   1.��    ��   : 2008��9��26��
     ��    ��   : c00131380
     �޸�����   : �����ɺ���
 
 *****************************************************************************/
 int sysSystemLogSet(char *string, int strLen, int offset)
 {
     return (boardIoctl(BOARD_IOCTL_FLASH_WRITE, SYSTEM_LOG, string, strLen, offset, ""));
 }

 /*************************************************
   Function:        sysVariableSet
   Description:    set va data
   Input:            string : ָ�������ݵ��ڴ�����
                        strLen: ���ݳ���
                        offset:  ƫ��ֵ
   Output:           ��
   Return:          -1 : ʧ��
                        >0 :�ɹ�
   Others:          ��
  *************************************************/
 int sysVariableSet(char *string, int strLen, int offset)
 {
     return (boardIoctl(BOARD_IOCTL_FLASH_WRITE, VARIABLE, string, strLen, offset, ""));
 }

 
/*****************************************************************************
* sysPersistentGet - get the contents of persistent flash memory
* RETURNS: OK, always.
*/
int sysPersistentGet(char *string, int strLen, int offset)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_READ, PERSISTENT, string, strLen, offset, ""));
}

/*****************************************************************************
* sysPersistenSet - write the contents of persistent Scrach Pad flash memory
* RETURNS: OK, always.
*/
int sysPersistentSet(char *string, int strLen, int offset)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_WRITE, PERSISTENT, string, strLen, offset, ""));
}

/* begin --- Add Persistent storage interface backup flash partition by w69233 */
/*****************************************************************************
* sysPsiBackupGet - get the contents of psi backup flash memory
* RETURNS: OK, always.
*/
int sysPsiBackupGet(char *string, int strLen, int offset)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_READ, PSI_BACKUP, string, strLen, offset, ""));
}

/*****************************************************************************
* sysPsiBackupSet - write the contents of psi backup flash memory
* RETURNS: OK, always.
*/
int sysPsiBackupSet(char *string, int strLen, int offset)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_WRITE, PSI_BACKUP, string, strLen, offset, ""));
}
/* end ----- Add Persistent storage interface backup flash partition by w69233 */

//********************************************************************************
// Get PSI size
//********************************************************************************
int sysGetPsiSize( void )
{
    return( boardIoctl(BOARD_IOCTL_GET_PSI_SIZE, 0, "", 0, 0, "") );
}


int sysFlashLog(void)
{
#define SHM_LOGKEY 0x414e4547
#define SHM_LOGSIZE 16*1024

	int logid = -1;
       int semId = -1;
    	char* pcBuf = NULL;
	int iRet = 0;
	struct sembuf SMrup[1] = {{0, -1, IPC_NOWAIT | SEM_UNDO}}; // set SMrup
	struct sembuf SMrdn[2] = {{1, 0}, {0, +1, SEM_UNDO}}; // set SMrdn

	if ((logid = shmget(SHM_LOGKEY, 0, 0)) == -1)
    	{
        	printf("Syslog disabled or log buffer not allocated. err %d line %d\n", errno, __LINE__);
		iRet = -1;
                return iRet;
    	}
	 
	 if ( (pcBuf = shmat(logid, 0x58800000, SHM_RND)) == NULL) 
	{
	    printf("Can't get access to circular buffer from syslogd\n");
	    iRet = -1;
            return iRet;
	}

	 if ( (semId = semget(SHM_LOGKEY, 0, 0)) == -1) 
	{
	    printf("Can't get access to semaphone(s) for circular buffer err %d\n", errno);
	    iRet = -1;
	    goto output_end;
	}
	 
	if ( semop(semId, SMrdn, 2) == -1 )
	{
		iRet = -1;
		goto output_end;
	}
	sysSystemLogSet((char*)pcBuf, SHM_LOGSIZE, 0);

	if ( semop(semId, SMrup, 1) == -1 )
	{	
		iRet = -1;
	}
output_end:
	if (logid != -1)
	{
	    shmdt(pcBuf);
	}

	return iRet;
}


int sysFlashImageSet(void *image, int size, int addr,
    BOARD_IOCTL_ACTION imageType)
{

    int result;
    
    result = boardIoctl(BOARD_IOCTL_FLASH_WRITE, imageType, image, size, addr, "");

    return(result);
}

/*****************************************************************************
 * Get flash size 
 * return int flash size
 */
int sysFlashSizeGet(void)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_READ, FLASH_SIZE, "", 0, 0, ""));
}

#ifdef WIRELESS
extern char **environ;
int bcmSystem (char *command) {
   int pid = 0, status = 0;

   if ( command == 0 )
      return 1;

   pid = fork();
   if ( pid == -1 )
      return -1;

   if ( pid == 0 ) 
   {
      char *argv[4];
      argv[0] = "sh";
      argv[1] = "-c";
      argv[2] = command;
      argv[3] = 0;
      printf("sysMipsSoftReset: %s\r\n", command);
      execve("/bin/sh", argv, environ);
      exit(127);
   }

   /* wait for child process return */
   do 
   {
      if ( waitpid(pid, &status, 0) == -1 ) 
      {
         if ( errno != EINTR )
         {
            return -1;
         }
      }
      else
      {
         return status;
      }
   } while ( 1 );

   return status;
}
#endif


/*****************************************************************************
* kerSysMipsSoftReset - soft reset the mips. (reboot, go to 0xbfc00000)
* RETURNS: NEVER
*/
void sysMipsSoftReset(void)
{ 
    boardIoctl(BOARD_IOCTL_MIPS_SOFT_RESET, 0, "", 0, 0, "");
}

/* start of y42304 added 20060907: ֧��ͨ������λ��10���ڿ���/�ر�wlan */
#ifdef WIRELESS   
int sysCheckWirelessStatus(void)
{    
    return (boardIoctl(BOARD_IOCTL_CHECK_WLAN_STATUS, 0, "", 0, 0, ""));
}
/*add by aizhanlong 00169427 for DTS2011102004063 wifi can't enable by wifi button before disable wifi using web*/
int sysSetWirelessStatus(int status)
{  
    if ( status == 1 )
    {
        return (boardIoctl(BOARD_IOCTL_CHECK_WLAN_STATUS, 17, "", 0, 0, ""));
    }
    return (boardIoctl(BOARD_IOCTL_CHECK_WLAN_STATUS, 18, "", 0, 0, ""));
}
/*end by aizhanlong 00169427 for DTS2011102004063 wifi can't enable by wifi button before disable wifi using web*/
/*start of ���� WPS ״̬��� by s53329 at  20090722*/
int sysCheckWirelessWpsStatus(void)
{    
    return (boardIoctl(BOARD_IOCTL_CHECK_WLAN_WPS_STATUS, 0, "", 0, 0, ""));
}
/*end of ���� WPS ״̬��� by s53329 at  20090722*/
#endif 
/* end of y42304 added 20060907 */

//********************************************************************************
// Get Chip Id
//********************************************************************************
int sysGetChipId( void )
{
    return( boardIoctl(BOARD_IOCTL_GET_CHIP_ID, 0, "", 0, 0, "") );
}

//********************************************************************************
// Wakeup monitor task
//********************************************************************************
void sysWakeupMonitorTask(void)
{
    boardIoctl(BOARD_IOCTL_WAKEUP_MONITOR_TASK, 0, "", 0, 0, "");
}

#ifdef USE_ALL
//********************************************************************************
// LED status display:  ADSL link: DOWN/UP, PPP: DOWN/STARTING/UP
//********************************************************************************
void sysLedCtrl(BOARD_LED_NAME ledName, BOARD_LED_STATE ledState)
{
    boardIoctl(BOARD_IOCTL_LED_CTRL, 0, "", (int)ledName, (int)ledState, "");
}


//********************************************************************************
// Get board id
//********************************************************************************
int sysGetBoardIdName(char *name, int length)
{
    return( boardIoctl(BOARD_IOCTL_GET_ID, 0, name, length, 0, "") );
}


//********************************************************************************
// Get MAC Address
//********************************************************************************
int sysGetMacAddress( unsigned char *pucaAddr, unsigned long ulId )
{
    return(boardIoctl(BOARD_IOCTL_GET_MAC_ADDRESS, 0, pucaAddr, 6, (int) ulId, ""));
}


//********************************************************************************
// Release MAC Address
//********************************************************************************
int sysReleaseMacAddress( unsigned char *pucaAddr )
{
    return( boardIoctl(BOARD_IOCTL_RELEASE_MAC_ADDRESS, 0, pucaAddr, 6, 0, "") );
}


//********************************************************************************
// Get SDRAM size
//********************************************************************************
int sysGetSdramSize( void )
{
    return( boardIoctl(BOARD_IOCTL_GET_SDRAM_SIZE, 0, "", 0, 0, "") );
}


/*****************************************************************************
* sysGetBooline - get bootline
* RETURNS: OK, always.
*/
int sysGetBootline(char *string, int strLen)
{
    return (boardIoctl(BOARD_IOCTL_GET_BOOTLINE, 0, string, strLen, 0, ""));
}

/*****************************************************************************
* sysSetBootline - write the bootline to nvram
* RETURNS: OK, always.
*/
int sysSetBootline(char *string, int strLen)
{
    return (boardIoctl(BOARD_IOCTL_SET_BOOTLINE, 0, string, strLen, 0, ""));
}

//********************************************************************************
// Get MAC Address
//********************************************************************************
int sysGetBaseMacAddress( unsigned char *pucaAddr )
{
    return(boardIoctl(BOARD_IOCTL_EQUIPMENT_TEST, GET_BASE_MAC_ADDRESS, pucaAddr, 6, 0, ""));
}

//********************************************************************************
// Get number of Ethernet phys
//********************************************************************************
int sysGetNumEnet(void)
{
    return(boardIoctl(BOARD_IOCTL_GET_NUM_ENET, 0, "", 0, 0, ""));
}

//********************************************************************************
// Get CFE vesion info
//********************************************************************************
int sysGetCFEVersion(char *string, int strLen)
{
    return (boardIoctl(BOARD_IOCTL_GET_CFE_VER, 0, string, strLen, 0, ""));
}

//********************************************************************************
// Get board Ethernet configuration
//********************************************************************************
int sysGetEnetCfg(char *string, int strLen)
{
    return(boardIoctl(BOARD_IOCTL_GET_ENET_CFG, 0, string, strLen, 0, ""));
}

//********************************************************************************
// Set monitor loop file descriptor
//********************************************************************************
int sysSetMonitorFd(int fd)
{
    return (boardIoctl(BOARD_IOCTL_SET_MONITOR_FD, 0, "", 0, fd, ""));
}

//******************************************************************************
// Get VCOPE board information: cs, gpio, board revision
//******************************************************************************
int sysGetVcopeInfo(int info_type)
{
    return(boardIoctl(BOARD_IOCTL_GET_VCOPE_GPIO, 0, "", 0, info_type, ""));
}

//******************************************************************************
// Configure Chip Select, by setting given parameter to a passed value
//******************************************************************************
int sysConfigCs (int cs_number, void *cs_info)
{
    return(boardIoctl(BOARD_IOCTL_SET_CS_PAR, 0, "", 0, cs_number, cs_info));
}

//******************************************************************************
// Set up PLL clock register according to the passed values
//******************************************************************************
int  sysSetPllClockRegister(mask, value)
{
    return(boardIoctl(BOARD_IOCTL_SET_PLL, 0, "", mask, value, ""));
}

//******************************************************************************
// Configure GPIO bit according to the passed values
//******************************************************************************
int  sysSetGpioBit (int gpio_bit, GPIO_STATE_t flag)
{
    return(boardIoctl(BOARD_IOCTL_SET_GPIO, 0, "", gpio_bit, flag, ""));
}

/**************************************************************************
 * ������  :   sysGetPCBVersion
 * ����    :   Through GPIO get PCB version
 *
 * �������:   ��           
 * �������:   ��
 *
 * ����ֵ  :   PCB �汾
 * ����    :   yuyouqing42304
 * �޸���ʷ:   2006-05-16����  
 ***************************************************************************/

int sysGetPCBVersion(void)
{    
    return (boardIoctl(BOARD_IOCTL_EQUIPMENT_TEST, GET_PCB_VERSION, "", 0, 0, ""));    
}

typedef enum  {
	E_RALINK_WLAN = 0,        
	E_ATHEROS_WLAN = 1,
	E_BCM_WLAN = 2,
}E_WLAN_VENDOR_TYPE;

#define HW_VERSION_A "HG556AVDFA"
#define HW_VERSION_B "HG556BVDFA"
#define HW_VERSION_C "HG556CVDFA"

int bcmGetWlanChipType(int bMute)
{
#define KRNL_DECTECTED_DEVICES  "/proc/bus/pci/devices"
#define BCM_VERNDOR_ID          0x14e4
#define ATHEROS_VERNDOR_ID      0x168c
#define RALINK_VERNDOR_ID       0x1814

/* seq_printf(m, "%02x%02x\t%04x%04x\t%x", dev->bus->number, dev->devfn, */ 
#define SKIP_TAG                 "\t"     //skip bus number and devfn

    char  acLineBbuf[512];
    char  acVendorStr[][12] = {"RaLink", "Atheros", "Broadcom"};
    char  *pStr = NULL;
    int iVendorProduct = 0;
    int iVendorID = 0;
    int iVendorType = E_RALINK_WLAN;
    FILE *fp = NULL;
    
    if (NULL == (fp = fopen(KRNL_DECTECTED_DEVICES, "r")))
    {
        printf("Open Device Failed\n");
        return E_RALINK_WLAN;
    }

    if (fgets(acLineBbuf, 512, fp) != NULL)
    {    
        pStr = strstr(acLineBbuf, SKIP_TAG);
        if (NULL == pStr)
        {        
            return E_RALINK_WLAN;
        }
        
        sscanf(pStr+1, "%x", &iVendorProduct);
        
        iVendorID = iVendorProduct >> 16;
        switch (iVendorID)
        {
            case BCM_VERNDOR_ID:
                iVendorType = E_BCM_WLAN;
                break;
            case ATHEROS_VERNDOR_ID:
                iVendorType = E_ATHEROS_WLAN;
                break;
            case RALINK_VERNDOR_ID:
                iVendorType = E_RALINK_WLAN;
                break;
            default:
                printf("getWlanChipType Unkown Vendor Type = <0x%x>.\n", iVendorType);
                break;
        }

        if (!bMute)
        {
            printf("Sean@WLAN CHIP Vendor=<%s>, ProductId=<0x%x>\n", acVendorStr[iVendorType], iVendorProduct & 0xFFFF);
        }
        
    }

    fclose(fp);

    return iVendorType;
}

int GetHwVersino(char* cHwVer)
{
    FILE* fp = NULL;
    char buf[512];
    int iFxo = 0;
    if ( NULL == cHwVer )
    {
        return FALSE;
    }

    memset(buf,0,sizeof(buf));
	
    sysFixGet(cHwVer, HARDWARE_VERSION_LEN, HARDWARE_VERSION_OFFSET);
    if (strlen(cHwVer) == 0 || strlen(cHwVer) > HARDWARE_VERSION_VALID_LEN)
    {
        if (E_ATHEROS_WLAN == bcmGetWlanChipType(TRUE))
        {             
            if ( fp = fopen("/proc/fxo","r") )
            {
                fgets(buf,512,fp);
                iFxo = atoi(buf);
                if ( iFxo)
                {
                    strcpy(cHwVer, HW_VERSION_A);
                }
                else
                {
                    strcpy(cHwVer, HW_VERSION_B);
                }
                fclose(fp);
            }
            else
            {
                strcpy(cHwVer, HW_VERSION_A);
                //printf("==HWVER:%s:%s:%d:open fxo file erro==\n",__FILE__,__FUNCTION__,__LINE__);
            }
        }
        else
        {            
            strcpy(cHwVer, HW_VERSION_C);
        }
    }
    //printf("%s\r\n", cHwVer);
    return TRUE;
}

/**************************************************************************
 * ������  :   sysGetBoardVersion
 * ����    :   Through GPIO get Board version
 *
 * �������:   ��           
 * �������:   ��
 *
 * ����ֵ  :   Board �汾
 * ����    :   yuyouqing42304
 * �޸���ʷ:   2006-05-16����  
 ***************************************************************************/
int sysGetBoardVersion(unsigned char *string)
{    
    //return (boardIoctl(BOARD_IOCTL_EQUIPMENT_TEST, GET_BOARD_VERSION, string, 0, 0, ""));  
    return (GetHwVersino(string));  
}

int sysGetBoardManufacturer(unsigned char *string)
{
    return (boardIoctl(BOARD_IOCTL_EQUIPMENT_TEST, GET_BOARD_MANUFACTURER_NAME, string, 0, 0, ""));    
}


/* start of EquipTest added by y42304 20060519: telnet�޸����ڵ�ַ */
int sysSetBaseMacAddress(unsigned char *string, int strLen)
{
    return (boardIoctl(BOARD_IOCTL_EQUIPMENT_TEST, SET_BASE_MAC_ADDRESS, string, strLen, 0, ""));
}

int sysGetMacNumbers(void)
{    
    return (boardIoctl(BOARD_IOCTL_EQUIPMENT_TEST, GET_MAC_AMOUNT, "", 0, 0, ""));    
}

int sysSetMacNumbers(int macNumbers)
{
    return (boardIoctl(BOARD_IOCTL_EQUIPMENT_TEST, SET_MAC_AMOUNT, "", 0, macNumbers, ""));
}

int sysSetBoardId(unsigned char *string, int strLen)
{    
    return (boardIoctl(BOARD_IOCTL_EQUIPMENT_TEST, SET_BOARD_ID, string, strLen, 0, ""));    
}

int sysGetBoardId(unsigned char *string)
{
    return (boardIoctl(BOARD_IOCTL_EQUIPMENT_TEST, GET_BOARD_ID, string, 0, 0, ""));            
}

int sysGetAllBoardIdName(char *string, int strLen)
{
    return (boardIoctl(BOARD_IOCTL_EQUIPMENT_TEST, GET_ALL_BOARD_ID_NAME, string, strLen, 0, ""));                
}

int sysGetBoardSerialNumber(char *string, int strLen)
{
    return (boardIoctl(BOARD_IOCTL_EQUIPMENT_TEST, GET_SERIAL_NUMBER, string, strLen, 0, ""));
}

int sysSetBoardSerialNumber(char *string, int strLen)
{
    return (boardIoctl(BOARD_IOCTL_EQUIPMENT_TEST, SET_SERIAL_NUMBER, string, strLen, 0, ""));
}

int sysGetChipRevisionID(void)
{    
    return( boardIoctl(BOARD_IOCTL_EQUIPMENT_TEST, GET_CPU_REVISION_ID, "", 0, 0, "") );    
}
/* end of EquipTest added by y42304 20060519 */

/* start of y42304 added 20060814: �ṩ��ȡflash FILE_TAG�Ľӿڸ�Ӧ��*/
/**************************************************************************
 * ������  :   sysGetFileTagFromFlash
 * ����    :   �ṩ��Ӧ�ô�FLASH��ȡFILE_TAG����Ϣ(256 bytes)
 *
 * �������:   pstTag:  �洢����FILE_TAG����ָ��
 * �������:   ��
 *
 * ����ֵ  :   0:   ��ȷ
               -1:  ����
 * ����    :   yuyouqing42304
 * �޸���ʷ:   2006-08-15����  
 ***************************************************************************/
int sysGetFileTagFromFlash(char * pstTag)
{    
    return( boardIoctl(BOARD_IOCTL_FLASH_READ, GET_FILE_TAG_FROM_FLASH, pstTag, 0, 0, "") );    
}


/**************************************************************************
 * ������  :   sysGetFsImageFromFlash
 * ����    :   �ṩ��Ӧ�ô�FLASH��ȡfs�ļ�ϵͳ
 *
 * �������:   string:  �洢���ݵ�buf
               strLen:  buf���ֽڳ���
               offSet:  ���ļ�ϵͳ��ʼλ�õ�ƫ����
 * �������:   ��
 *
 * ����ֵ  :   0:   ��ȷ
               -1:  ����
 * ����    :   yuyouqing42304
 * �޸���ʷ:   2006-08-15����  
 ***************************************************************************/
int sysGetFsImageFromFlash(char *string, int strLen, int offSet)
{    
    return (boardIoctl(BOARD_IOCTL_FLASH_READ, BCM_IMAGE_FS, string, strLen, offSet, ""));
}

/**************************************************************************
 * ������  :   sysGetKernelImageFromFlash
 * ����    :   �ṩ��Ӧ�ô�FLASH��ȡfs�ļ�ϵͳ
 *
 * �������:   string:  �洢���ݵ�buf
               strLen:  buf���ֽڳ���
               offSet:  ��kernel��ʼλ�õ�ƫ����
 * �������:   ��
 *
 * ����ֵ  :   0:   ��ȷ
               -1:  ����
 * ����    :   yuyouqing42304
 * �޸���ʷ:   2006-08-15����  
 ***************************************************************************/

int sysGetKernelImageFromFlash(char *string, int strLen, int offSet)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_READ, BCM_IMAGE_KERNEL, string, strLen, offSet, ""));   
}


/**************************************************************************
 * ������  :   sysGetFsAndKernleImageFromFlash
 * ����    :   �ṩ��Ӧ�ô�FLASH��ȡbin�ļ�(��ʼλ��CFE+FILE_TAG)
 *
 * �������:   string:  �洢���ݵ�buf
               strLen:  buf���ֽڳ���
               offSet:  ���ļ�ϵͳ��ʼλ�õ�ƫ����
 * �������:   ��
 *
 * ����ֵ  :   0:   ��ȷ
               -1:  ����
 * ����    :   yuyouqing42304
 * �޸���ʷ:   2006-05-16����  
 ***************************************************************************/

#define EACH_READ_SIZE  (64*1024)
int sysGetFsAndKernleImageFromFlash(char *string, int strLen, int offSet)
{
    int i = 0, count = 0, remainder = 0;
    int iRet = 0;
    char tmpBuf[EACH_READ_SIZE];

    count     = strLen / EACH_READ_SIZE;
    remainder = strLen % EACH_READ_SIZE;

    if (string == NULL)
    {
        return -1;
    }
    
    memset(string, 0, strLen);

    /* ÿ�ζ�ȡ��FLASH �ļ�ϵͳ��ʼ��λ�����ζ�ȡ64K���� */
    for (i = 0; i < count; i++)
    {        
        memset(tmpBuf, 0, EACH_READ_SIZE);
        iRet = sysGetFsImageFromFlash(tmpBuf, EACH_READ_SIZE, offSet+i*EACH_READ_SIZE);
        if (0 != iRet)
        {
            return iRet;
        }
        memcpy(string+i*EACH_READ_SIZE, tmpBuf, EACH_READ_SIZE);          
    }

    if (remainder != 0)
    {        
        memset(tmpBuf, 0, EACH_READ_SIZE);
        iRet = sysGetFsImageFromFlash(tmpBuf, remainder, offSet+i*EACH_READ_SIZE);
        if (0 != iRet)
        {
            return iRet;
        }        
        memcpy(string+i*EACH_READ_SIZE, tmpBuf, remainder);       
    }      
    return iRet;
}

/**************************************************************************
 * ������  :   sysGetPsiActualSizeFromFlash
 * ����    :   ��ȡFLASH��PSI����ʵ�����ݵĴ�С
 * �������:   ��
 * �������:   ��
 *
 * ����ֵ  :   0:   ��ȷ
               -1:  ����
 * ����    :   yuyouqing42304
 * �޸���ʷ:   2006-05-16����  
 ***************************************************************************/

#define XML_PSI_START_TAG      "<psitree>"
#define XML_PSI_END_TAG        "</psitree>"
int sysGetPsiActualSizeFromFlash(void)
{
    unsigned long ulLen = 0;    
    char profileBuffer[64*1024];
    char *pc = NULL;
    
    ulLen = sysGetPsiSize();
    
    sysPersistentGet(profileBuffer, ulLen, 0);

    ulLen = 0;    
    // if <psitree> cannot be found then return
    if ( 0 == memcmp(profileBuffer, XML_PSI_START_TAG, strlen(XML_PSI_START_TAG))) 
    {
        pc = strstr(profileBuffer, XML_PSI_END_TAG);   
        if ( pc != NULL )
        {
            ulLen = pc - profileBuffer + strlen(XML_PSI_END_TAG);
        }        
        else
        {
            ulLen = 0;
        }
    }
    return ulLen;    
}
/* end of y42304 added 20060814*/


/* start of l68693 added 20081227: ReInection�Ż��޸� */ 
/* ���FXO���� */
int sysDiagDaaParam( void )
{
   REINJECTION_CMD_PARAM stParam;
   int retVal = 0;
   int nfdEndpt = -1;

   nfdEndpt = open("/dev/bcmendpoint0", O_RDWR);
   if (nfdEndpt <= 0)
   {
       printf( "bcmendpoint0 : open error %d\n", errno );
   }

   stParam.size = sizeof(REINJECTION_CMD_PARAM);
   if ( ioctl( nfdEndpt, ENDPOINTIOCTL_CONFIG_DAA_PARAM, &stParam ) != 0 )
   {
      printf("sysDiagDaaParam: error during ioctl");
   }
   else
   {
      retVal = stParam.epStatus;
   }

   close(nfdEndpt);

   return( retVal );
}
/* end of l68693 added 20081227: ReInection�Ż��޸� */ 


/* start of l68693 added 20090222: ֧��װ��ReInection���� */ 

/* ReInjectionװ������, ��������  */
/*
REINJECTION_CTL_RELAY_ON = 0,
REINJECTION_CTL_RELAY_OFF = 1,
REINJECTION_CTL_RESET = 2,   
*/

int sysDiagControlReInjection ( int cmd )
{
   REINJECTION_CMD_PARAM stParam;
   int retVal = 0;
   int nfdEndpt = -1;

   nfdEndpt = open("/dev/bcmendpoint0", O_RDWR);
   if (nfdEndpt <= 0)
   {
       printf( "bcmendpoint0 : open error %d\n", errno );
   }

   stParam.size = sizeof(REINJECTION_CMD_PARAM);
   stParam.cmd = cmd;
   if ( ioctl( nfdEndpt, ENDPOINTIOCTL_CONTROL_REINJECTION, &stParam ) != 0 )
   {
      printf("sysDiagControlReInjection: error during ioctl");
   }
   else
   {
      retVal = stParam.epStatus;
   }

   close(nfdEndpt);

   return( retVal );
}
/* end of l68693 added 20090222: ֧��װ��ReInection���� */ 


#if defined(SUPPORT_SLIC_3210) || defined(SUPPORT_SLIC_3215)
/**************************************************************************
 * ����    :   ����slic 3210/3215�Ĵ���
 * �������:   ��
 * �������:   ��
 *
 * ����ֵ  :   0:   ��ȷ
               -1:  ����
 * ����    :   yuyouqing42304
 * �޸���ʷ:   2006-11-16����  
 ***************************************************************************/
int sysDiagSlic(int command, int lineId, unsigned char ucBeginRegAddr, unsigned short usOption)
{
    SLIC_CMD_PARM stparam;    
    int       fileHandle;
 
    fileHandle = open("/dev/bcmendpoint0", O_RDWR);
    if (fileHandle == -1)
    {        
        printf("Unable to open device /dev/bcmendpoint0.\n");
        return -1;
    }
    
    stparam.cmd = command;
    stparam.lineId = lineId;
    stparam.size = sizeof(SLIC_CMD_PARM);    
    stparam.arg[0] = (int)ucBeginRegAddr;
    stparam.arg[1] = (int)usOption;
    stparam.epStatus = EPSTATUS_SUCCESS;

    if ( ioctl( fileHandle, ENDPOINTIOCTL_CONTROL_SLIC, &stparam ) != 0 )
    {
       printf("sysDiagSlic: error during ioctl" );
    }
    
    close(fileHandle);
    return stparam.retVal;
}
#elif defined(SUPPORT_SLIC_LE88221)
int sysDiagSlic(int command, int lineId, unsigned char ucBeginRegAddr, unsigned short usOption, unsigned char *pucRegValue)
{
    SLIC_CMD_PARM stparam;    
    int       fileHandle;
 
    fileHandle = open("/dev/bcmendpoint0", O_RDWR);
    if (fileHandle == -1)
    {        
        printf("Unable to open device /dev/bcmendpoint0.\n");
        return -1;
    }
    
    stparam.cmd = command;
    stparam.lineId = lineId;
    stparam.size = sizeof(SLIC_CMD_PARM);    
    stparam.arg[0] = (int)ucBeginRegAddr;
    stparam.arg[1] = (int)usOption;

    if( usOption < 16 && NULL != pucRegValue)
        memcpy(stparam.regValue, pucRegValue, usOption);
    
    stparam.epStatus = EPSTATUS_SUCCESS;

    if ( ioctl( fileHandle, ENDPOINTIOCTL_CONTROL_SLIC, &stparam ) != 0 )
    {
       printf("sysDiagSlic: error during ioctl" );
    }
    
    close(fileHandle);
    return stparam.retVal;
}

#endif

#endif // USE_ALL

/* start of maintain dying gasp by liuzhijie 00028714 2006��5��9��" */
int sysGetDyingGaspInfo(char * bufs)
{
    return (boardIoctl(BOARD_IOCTL_GET_DYINGGASP, 0, "", 0, 0, bufs));
}

int sysPutWebOperate(char *string, int strLen)
{
    return (boardIoctl(BOARD_IOCTL_PUT_WEB_OP, 0, string, strLen, 0, ""));
}

int sysPutCliOperate(char *string, int strLen)
{
    return (boardIoctl(BOARD_IOCTL_PUT_CLI_OP, 0, string, strLen, 0, ""));
}

int sysPutItfState(char *itf_name, int state)
{
    return (boardIoctl(BOARD_IOCTL_PUT_ITF_STATE, 0, itf_name, state, 0, ""));
}
/* end of maintain dying gasp by liuzhijie 00028714 2006��5��9��" */

int sysGetVoipServiceStatus(void)
{
	return 0;
    //return (boardIoctl(BOARD_IOCTL_GET_VOIP_SERVICE, 0, "", 0, 0, ""));    
}
int sysSetVoipServiceStatus(unsigned char ucStatus)
{    
    return (boardIoctl(BOARD_IOCTL_SET_VOIP_SERVICE, 0, "", 0, ucStatus, ""));
}

/* HUAWEI HGW s48571 2008��1��19�� Hardware Porting add begin:*/
int sysSetHspaTrafficMode(int hspaMode)
{
    return (boardIoctl(BOARD_IOCTL_SET_TRAFFIC_MODE, 0, "", 0, hspaMode, ""));
}
/* HUAWEI HGW s48571 2008��1��19�� Hardware Porting add end.*/

/* HUAWEI HGW s48571 2008��2��20�� װ������״̬��־��� add begin:*/
int sysSetEquipTestMode(int equipTestMode)
{
    return (boardIoctl(BOARD_IOCTL_SET_EQUIPTEST_MODE, 0, "", 0, equipTestMode, ""));
}

/* HUAWEI HGW s48571 2008��2��20�� װ������״̬��־��� add end.*/

/* start of ����MAC��ַ���书�� by c00131380 at 080910*/

 /*************************************************
  Function:        sysAvailMacAddrGet
  Description:     ���FLASH�е�MAC��ַ
  Input:            string : ָ�������ݵ��ڴ�����
                       strLen: ���ݳ���
                       offset:  ƫ��ֵ
  Output:           ��
  Return:          -1 : ʧ��
                       >0 :�ɹ�
  Others:          ��
 *************************************************/
 int sysAvailMacAddrGet(unsigned char *string, int strLen, int offset)
 {
     return (boardIoctl(BOARD_IOCTL_FLASH_READ, AVAIL, string, strLen, offset, ""));
 }
 
 /*************************************************
  Function:        sysAvailMacAddrSet
  Description:     ����FLASH�е�MAC��ַ
  Input:            string : ָ�������ݵ��ڴ�����
                       strLen: ���ݳ���
                       offset:  ƫ��ֵ
  Output:           ��
  Return:          -1 : ʧ��
                       >0 :�ɹ�
  Others:          ��
 *************************************************/
int sysAvailMacAddrSet(unsigned char *string, int strLen, int offset)
{
    return (boardIoctl(BOARD_IOCTL_FLASH_WRITE, AVAIL, string, strLen, offset, ""));
}

int sysGetMemMacInUsedStatus(int macID)
{
    return (boardIoctl(BOARD_IOCTL_GET_MEMMACINUSESTATUS, 0, "", 0, macID, ""));
}

int sysSetMemMacInUsedStatus(int macID,int inUseStatus)
{
    return (boardIoctl(BOARD_IOCTL_SET_MEMMACINUSESTATUS, 0, "", inUseStatus, macID, ""));
}

int sysSetMemMacAddrInfo(unsigned char *string,int macID)
{
    return (boardIoctl(BOARD_IOCTL_SET_MEMMACADDRESS, 0, string, MAC_ADDRESS_LEN, macID, ""));
}

int sysGetMemMacAddrInfo(unsigned char *string,int macID)
{
    return (boardIoctl(BOARD_IOCTL_GET_MEMMACADDRESS, 0, string, MAC_ADDRESS_LEN, macID, ""));
}

int sysGetNumofUnUsedMac(void)
{
    return (boardIoctl(BOARD_IOCTL_GET_NUMOFUNUSEDMACADDR, 0, "", 0, 0, ""));
}

int sysDeleteMemMacAddr(int macID)
{
    return (boardIoctl(BOARD_IOCTL_DEL_EXTENDMACADDR, 0,"", 0, macID, ""));
}

/* end of ����MAC��ַ���书�� by c00131380 at 080910*/

/* start of ����igmp proxy syslog���ع��� by l129990 2008,9,28*/
int sysSetIGMPLogMode(int mode)
{
    return (boardIoctl(BOARD_IOCTL_SET_IGMPLOG_MODE, 0, "", 0, mode, ""));
}
/* end of ����igmp proxy syslog���ع��� by l129990 2008,9,28*/
/* j00100803 save fwlog to flash for HG556A */
int sysGetRebootBtnFlag(void)
{
	return (boardIoctl(BOARD_IOCTL_GET_VAR_REBOOT_BUTTON, 0, "", 0, 0, ""));
}
/* j00100803 save fwlog to flash for HG556A */
