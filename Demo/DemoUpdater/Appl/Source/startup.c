/*****************************************************************************
* Project Name: Vector Basic Runtime System for CANbedded
*    File Name: startup.C
*
*  Description: Startup code for different NEC V85x derivatives 
*
*-----------------------------------------------------------------------------
*               C O P Y R I G H T
*-----------------------------------------------------------------------------
* Copyright (c) 2003 by Vector Informatik GmbH.      All rights reserved.
*
*                    Alle Rechte an der Software verbleiben bei der
*                    Vector Informatik GmbH.
*                    Vector Informatik GmbH räumt dem Lizenznehmer das unwider-
*                    rufliche, geographisch und zeitlich nicht beschränkte,
*                    jedoch nicht ausschließliche Nutzungsrecht innerhalb des
*                    Lizenznehmers ein. Die Weitergabe des Nutzungsrechts
*                    durch den Lizenznehmers ist auf dessen Zulieferer
*                    beschränkt. Die Zulieferer sind zu verpflichten, die
*                    Software nur im Rahmen von Projekten für den Lizenznehmer
*                    zu verwenden; weitere Rechte der Zulieferer sind
*                    auszuschließen.
*-----------------------------------------------------------------------------
*               A U T H O R   I D E N T I T Y
*-----------------------------------------------------------------------------
* Initials     Name                      Company
* --------     ---------------------     ------------------------------------
* Ljn          Jan Lutterbeck            Vector Informatik GmbH
*-----------------------------------------------------------------------------
*               R E V I S I O N   H I S T O R Y
*-----------------------------------------------------------------------------
* Date           Version  Author  Description
* ----------     -------  ------  -----------------------------------------------
* 2012-07-12     1.00.00    Ljn   Initial version for RH850
*****************************************************************************/

#include "v_cfg.h"

/* 
--===========================================================================--
-- Enviroment : Devices    : V850E/FJ3
--              Assembler  :
--              C-Compiler :
--              Linker     :
--              Debugger   :
--===========================================================================--
-- Version    Author    	Description :
--            LemetteilM	
--
-- 1.00 (31/10/05)  : - Initial release of this file
--===========================================================================--
*/

/*
--------------------------------------------------------------------------
-------------  Basic Initialisation of the controller	        		   
--------------------------------------------------------------------------
*/

#define IRAM_START              0xFEBE0000
#define IRAM_END                0xFEBFFFFF

# pragma asm
--===========================================================================--
-- Definition of external functions 
--===========================================================================--

-- reset
--.globl _RESET
.globl __usr_init

-- used for GHS entry
--.extern __start
.extern __usr_init

-- external routines
--.extern __emergency

--===========================================================================--
-- Basic Initialisation of the controller 
--===========================================================================--
.section ".text"



--===========================================================================--
-- _RESET
--
-- Description:	The main initialization of the MCU. Clock generator and
--		PLL are setup. First, we work with the 8MHz Ring-Osc,
--		then we switch to the Main-Oscilator so that periphericals
--		can be used. Eventually, the PLL is launched.
		
--===========================================================================--

--_RESET:
__usr_init:

   -- Initialize CPU register to avoid Compare Unit Mismatch Error,
   -- when saved to stack by ghs_ind_crt0
   mov     r0,     r1
   mov     r0,     r2
   mov     r0,     r3
   mov     r0,     r4
   mov     r0,     r5
   mov     r0,     r6
   mov     r0,     r7
   mov     r0,     r8
   mov     r0,     r9
   mov     r0,     r10
   mov     r0,     r11
   mov     r0,     r12
   mov     r0,     r13
   mov     r0,     r14
   mov     r0,     r15
   mov     r0,     r16
   mov     r0,     r17
   mov     r0,     r18
   mov     r0,     r19
   mov     r0,     r20
   mov     r0,     r21
   mov     r0,     r22
   mov     r0,     r23
   mov     r0,     r24
   mov     r0,     r25
   mov     r0,     r26
   mov     r0,     r27
   mov     r0,     r28
   mov     r0,     r29
   mov     r0,     r30
   mov     r0,     r31

  -- =========================================================================
  -- == Initialisation of some SYSTEM PARAMETERS for GHS				======
  -- =========================================================================
  
	-- Initialisation of the global pointer								   
	movhi	hi(___ghsbegin_sdabase),zero,gp								   
	movea	lo(___ghsbegin_sdabase),gp,gp								   

	-- Initialisation of the text pointer								   
	movhi	hi(___ghsbegin_robase),zero,tp								   
	movea	lo(___ghsbegin_robase),tp,tp								   

	-- Initialisation of the stack pointer								   
	movhi	hi(___ghsend_stack-4),zero,sp								   
	movea	lo(___ghsend_stack-4),sp,sp								       
	mov -4,r1                                                              
	and r1,sp

	-- Set INTBP base address for interrupt handler table								   
	mov	    0x00,r6	-- exeception handler base address
	ldsr    r6, 4, 1	-- INTBP
	stsr    4, r6, 1 	-- INTBP
	mov   	zero, r6
	ldsr	  r6,2,1
	stsr	  2, r6, 1
	mov   	zero, r6
	ldsr	  r6,3,1
	stsr	  3, r6, 1

	-- Check for 1 in POF
	-- If flag is set, clear iRAM and clear flag
	ld.w      -480240[r0], r18
	andi      1, r18, r18
	addi      -1, r18, r18
	bnz       clear_iRAM_done

	-- Clear flag in POFC
	mov       zero, r18
	addi      1, r18, r18
	st.w      r18, -480236[r0]

	-- Clear RAM with value of 0
    mov     IRAM_START, r6                          -- r6 = Start address
    mov     (IRAM_END +1 - IRAM_START) >> 2, r7     -- r7 = Word(32-Bit) count
clear_iRAM:
    st.w    r0, 0[r6]                               -- store 0 to address which is in r6
    add     4, r6                                   -- increment address
    loop    r7, clear_iRAM                          -- loop until 512KB written

clear_iRAM_done:

  -- All clock initialization is moved to BRS_hw.c
  
    -- Jump to the Initialisation functions of the GHS "crt.o" library !
    jr __start

-------------------------------------------------------------------------------
# pragma endasm

