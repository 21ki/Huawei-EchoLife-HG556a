/*
<:copyright-broadcom 
 
 Copyright (c) 2002 Broadcom Corporation 
 All Rights Reserved 
 No portions of this material may be reproduced in any form without the 
 written permission of: 
          Broadcom Corporation 
          16215 Alton Parkway 
          Irvine, California 92619 
 All information contained in this document is Broadcom Corporation 
 company private, proprietary, and trade secret. 
 
:>
*/
/****************************************************************************
 *
 * G997.h 
 *
 * Description:
 *	This file contains the exported functions and definitions for G97Framer
 *
 *
 * Copyright (c) 1993-1998 AltoCom, Inc. All rights reserved.
 * Authors: Ilya Stomakhin
 *
 * $Revision: 1.1.2.1 $
 *
 * $Id: G997.h,v 1.1.2.1 2009/11/19 06:39:16 l43571 Exp $
 *
 * $Log: G997.h,v $
 * Revision 1.1.2.1  2009/11/19 06:39:16  l43571
 * ��������ࡿ
 * �����ⵥ�š�
 * ������������
 * ���޸�˵����
 *
 * Revision 1.1  2008/08/25 06:41:05  l65130
 * ��������ࡿ��������
 * �����ⵥ�š�
 * ������������
 * ���޸�˵����
 *
 * Revision 1.1  2008/06/20 06:21:24  z67625
 * *** empty log message ***
 *
 * Revision 1.1  2008/01/14 02:47:27  z30370
 * *** empty log message ***
 *
 * Revision 1.2  2007/12/16 10:09:56  z45221
 * ������:
 * s48571
 *
 * �������ݣ�
 * ADSL�������°汾��A2x020h
 * ADSLfirmware�汾�����汾��a2pb023k/b2pb023k
 *
 * �޸��ļ��б�
 * /bcmdrivers/broadcom/char/adsl/impl1/AdslCore.c
 * /bcmdrivers/broadcom/char/adsl/impl1/AdslCore.h
 * /bcmdrivers/broadcom/char/adsl/impl1/AdslCoreFrame.c
 * /bcmdrivers/broadcom/char/adsl/impl1/AdslCoreFrame.h
 * /bcmdrivers/broadcom/char/adsl/impl1/AdslCoreMap.h
 * /bcmdrivers/broadcom/char/adsl/impl1/AdslDrvVersion.h
 * /bcmdrivers/broadcom/char/adsl/impl1/AdslFile.c
 * /bcmdrivers/broadcom/char/adsl/impl1/AdslFile.h
 * /bcmdrivers/broadcom/char/adsl/impl1/AdslSelfTest.c
 * /bcmdrivers/broadcom/char/adsl/impl1/AdslSelfTest.h
 * /bcmdrivers/broadcom/char/adsl/impl1/BcmAdslCore.c
 * /bcmdrivers/broadcom/char/adsl/impl1/BcmAdslCore.h
 * /bcmdrivers/broadcom/char/adsl/impl1/BcmAdslDiag.h
 * /bcmdrivers/broadcom/char/adsl/impl1/BcmAdslDiagLinux.c
 * /bcmdrivers/broadcom/char/adsl/impl1/BcmAdslDiagNone.c
 * /bcmdrivers/broadcom/char/adsl/impl1/BcmAdslDiagRtems.c
 * /bcmdrivers/broadcom/char/adsl/impl1/BcmOs.h
 * /bcmdrivers/broadcom/char/adsl/impl1/Makefile
 * /bcmdrivers/broadcom/char/adsl/impl1/adsl.c
 * /bcmdrivers/broadcom/char/adsl/impl1/adsldrv.c
 * /bcmdrivers/broadcom/char/adsl/impl1/adslcore6348/adsl_defs.h
 * /bcmdrivers/broadcom/char/adsl/impl1/adslcore6348/adsl_lmem.c
 * /bcmdrivers/broadcom/char/adsl/impl1/adslcore6348/adsl_lmem.h
 * /bcmdrivers/broadcom/char/adsl/impl1/adslcore6348/adsl_sdram.c
 * /bcmdrivers/broadcom/char/adsl/impl1/adslcore6348/adsl_sdram.h
 * /bcmdrivers/broadcom/char/adsl/impl1/adslcore6348B/adsl_defs.h
 * /bcmdrivers/broadcom/char/adsl/impl1/adslcore6348B/adsl_lmem.c
 * /bcmdrivers/broadcom/char/adsl/impl1/adslcore6348B/adsl_lmem.h
 * /bcmdrivers/broadcom/char/adsl/impl1/adslcore6348B/adsl_sdram.c
 * /bcmdrivers/broadcom/char/adsl/impl1/adslcore6348B/adsl_sdram.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/AdslCoreDefs.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/AdslMib.c
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/AdslMib.gh
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/AdslMib.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/AdslMibObj.c
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/AdslMibOid.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/AdslXfaceData.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/BlockUtil.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/CircBuf.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/DslFramer.c
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/DslFramer.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/Flatten.c
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/Flatten.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/G992p3OvhMsg.c
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/G992p3OvhMsg.gh
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/G992p3OvhMsg.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/G997.c
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/G997.gh
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/G997.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/HdlcByte.c
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/HdlcFramer.c
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/HdlcFramer.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/MathUtil.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/MipsAsm.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/MiscUtil.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/SoftAtmVc.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/SoftDsl.gh
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/SoftDsl.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/SoftDslFrame.c
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/SoftModem.h
 * /bcmdrivers/broadcom/char/adsl/impl1/softdsl/SoftModemTypes.h
 *
 * /bcmdrivers/broadcom/includeAdslMibDef.h
 * /bcmdrivers/broadcom/includeDiagDef.h
 * /bcmdrivers/broadcom/includeadsldrv.h
 * /bcmdrivers/broadcom/includebcmadsl.h
 *
 * /userapps/broadcom/adslctl/adslctl.c
 * /userapps/broadcom/adslctl/adsluser.c
 *
 * /userapps/broadcom/cfm/html/statsadsl.html
 * /userapps/broadcom/cfm/html/countrysupport/italy_vdf/info.html
 * /userapps/broadcom/cfm/inc/syscall.h
 * /userapps/broadcom/cfm/main/adslcfgmngr/adslctlapi.c
 * /userapps/broadcom/cfm/util/system/syscall.c
 * /userapps/broadcom/cfm/web/cgimain.c
 * /userapps/broadcom/cfm/web/cgimain.h
 *
 * Revision 1.3  2003/07/18 18:56:59  ilyas
 * Added support for shared TX buffer (for ADSL driver)
 *
 * Revision 1.2  2002/07/20 00:51:41  ilyas
 * Merged witchanges made for VxWorks/Linux driver.
 *
 * Revision 1.1  2001/12/13 02:28:27  ilyas
 * Added common framer (DslPacket and G997) and G997 module
 *
 *
 *
 *****************************************************************************/

#ifndef	G997FramerHeader
#define	G997FramerHeader

extern Boolean  G997Init(
		void					*gDslVars, 
		bitMap					setup, 
		ulong					rxBufNum,
		ulong					rxBufSize,
		ulong					rxPacketNum,
		upperLayerFunctions		*pUpperLayerFunctions,
		dslCommandHandlerType	g997PhyCommandHandler);

extern void		G997Close(void *gDslVars);
extern void		G997Timer(void *gDslVars, long timeQ24ms);
extern Boolean	G997CommandHandler (void *gDslVars, dslCommandStruct *cmd);
extern void		G997StatusSnooper (void *gDslVars, dslStatusStruct *status);

extern	int		G997SendFrame(void *gDslVars, void *pVc, ulong mid, dslFrame *pFrame);
extern  int		G997ReturnFrame(void *gDslVars, void *pVc, ulong mid, dslFrame *pFrame);

extern  Boolean G997SetTxBuffer(void *gDslVars, ulong len, void *bufPtr);
extern	void *	G997GetFramePoolHandler(void *gDslVars);

#endif	/* G997FramerHeader */
