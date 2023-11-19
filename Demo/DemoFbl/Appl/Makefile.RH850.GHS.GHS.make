#######################################################################################################################
# File Name  : Makefile.RH850.GHS.GHS.make                                                                            #
# Description: Linker/Symbol Preprocessor command file generation                                                     #
# Project    : Vector Basic Runtime System                                                                            #
# Module     : BrsHw for Platform Renesas RH850                                                                       #
#              and Compiler GreenHills                                                                                #
#                                                                                                                     #
#---------------------------------------------------------------------------------------------------------------------#
# COPYRIGHT                                                                                                           #
#---------------------------------------------------------------------------------------------------------------------#
# Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved. #
#                                                                                                                     #
#---------------------------------------------------------------------------------------------------------------------#
# AUTHOR IDENTITY                                                                                                     #
#---------------------------------------------------------------------------------------------------------------------#
# Name                          Initials      Company                                                                 #
# ----------------------------  ------------  ------------------------------------------------------------------------#
# Jan Lutterbeck                visljn        Vector Informatik GmbH                                                  #
# Steffen Frank                 visfsn        Vector Informatik GmbH                                                  #
# Alexander Becker              visabc        Vector Informatik GmbH                                                  #
# Benjamin Walter               visbwa        Vector Informatik GmbH                                                  #
# Michael Tiran                 vismin        Vector Informatik GmbH                                                  #
# Nika Hamidi                   visnhi        Vector Informatik GmbH                                                  #
# Sascha Mauser                 vismaa        Vector Informatik GmbH                                                  #
#---------------------------------------------------------------------------------------------------------------------#
# REVISION HISTORY                                                                                                    #
#---------------------------------------------------------------------------------------------------------------------#
# Version   Date        Author  Description                                                                           #
# --------  ----------  ------  --------------------------------------------------------------------------------------#
# 01.00.00  2012-09-03  visljn  Initial version                                                                       #
# 01.01.00  2013-07-25  visfsn  Merged changes from V850 Makesupport 2.02.35, cleaned up Linker sections              #
# 01.02.00  2013-08-12  visfsn  Added SafeCtx Support                                                                 #
# 01.03.00  2013-08-14  visljn  Compiler independent device description                                               #
# 01.04.00  2013-09-20  visabc  P1x added                                                                             #
# 01.05.00  2013-11-11  visljn  R1M added                                                                             #
# 01.06.00  2013-11-27  visfsn  Added update of ".redirect_tools" in Multi dir, SAFE_CONTEXT_OS_USED changed to       #
#                               OS_USECASE, removed GenericRH850                                                      #
# 01.07.00  2013-06-10  visfsn  Moved linker file generation to separate files,                                       #
#                               switched CPU_TYPE to $(CPU_FAMILY_$(DERIVATIVE))                                      #
# 01.08.00  2013-06-17  visfsn  Linker file generation suppressed for custom LF                                       #
# 01.08.01  2015-02-18  visbwa  Added connection script for P1H-C (701325)                                            #
# 01.08.02  2015-12-01  visvnn  Added ($(DERIVATIVE_GROUP_$(DERIVATIVE)),P1H)                                         #
# 01.08.03  2015-12-21  visfsn  Added ADDITIONAL_COMMAND_D1M                                                          #
# 01.08.04  2016-04-21  vismin  Added rule to generate and delete python script, added build target 'cs'              #
# 01.09.00  2016-06-20  visbwa  Added support for Makefile.RH850.GHS.GHS.LinkerScriptOsGen7,                          #
#                               adapted file header according to Brs_Template 2.00.01                                 #
# 01.10.00  2016-09-09  visnhi  Added support for Makefile.RH850.GHS.GHS.LinkerScriptMultiCore                        #
# 01.10.01  2017-04-28  visbwa  Added $(PROJECT_NAME).rc to multi target to prevent problems with activated           #
#                               USER_LINKER_COMMAND_FILE                                                              #
# 01.10.02  2017-05-05  vismaa  Extracted hex target to Makefile                                                          #
#######################################################################################################################

include Makefile.derivative.memorymap

##########################################################################################################
# MULTI CONNECT SCRIPT GENERATION
##########################################################################################################
ifeq ($(DEBUGGER),MINICUBE)
  REMOTE_ESERV = 850eserv2 -rh850 -minicube -df=$(DEVICE_FILE_FOLDER)\$(DEVICE_FILE_NAME)".dvf" -id ffffffffffffffffffffffff -noiop
endif

ifeq ($(DEBUGGER),E1)
  ifeq ($(DERIVATIVE_GROUP_$(DERIVATIVE)),P1H)
    REMOTE_ESERV = 850eserv2 -rh850 -e1lpd4=16500 -df=$(DEVICE_FILE_FOLDER)\$(DEVICE_FILE_NAME).dvf -cfapw ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff -dfapw ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff -id ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff -dclock=$(MAIN_OSC_CLK)000,0,swoff -no_trace -noiop
  else
    REMOTE_ESERV = 850eserv2 -rh850 -e1lpd4=11000 -dclock=$(MAIN_OSC_CLK)000,0,swoff -df=$(DEVICE_FILE_FOLDER)\$(DEVICE_FILE_NAME).dvf -id ffffffffffffffffffffffffffffffff -noiop
  endif
endif

# Definition of additional commands for each platform, will be added at the end of the *.rc file
# Should be independent from used debugger
# DCLOCK [main_clock sub_clock (swon|swoff)]
# main_clock  specifies main clock per kHz
# sub_clock   specifies sub clock per kHz
# swon        switches to main clock automatically from sub clock at the time of a break
# swoff       impossible automatic change on main clock
# for details see docu of ESERV
define ESERV2_DEFAULT_COMMAND
  $(ECHO) "                                         "  >> $@; \
  $(ECHO) "/* Set target clock */                   "  >> $@; \
  $(ECHO) "target dclock $(MAIN_OSC_CLK)000 0 swoff "  >> $@; \
  $(ECHO) "                                         "  >> $@; \
  $(ECHO) "/* Disable interrupts */                 "  >> $@; \
  $(ECHO) "target reg psw=0x0020                    "  >> $@; \
  $(ECHO) "                                         "  >> $@; \
  $(ECHO) "/* Unmask reset sources */               "  >> $@; \
  $(ECHO) "target pinmask k                         "  >> $@; \
  $(ECHO) "                                         "  >> $@; \
  $(ECHO) "/* load program and go to main */        "  >> $@; \
  $(ECHO) "load $(PROJECT_NAME).$(BINARY_SUFFIX)    "  >> $@; \
  $(ECHO) "                                         "  >> $@; \
  $(ECHO) "/* Set breakpoint to main */             "  >> $@; \
  $(ECHO) "b main                                   "  >> $@; \
  $(ECHO) "                                         "  >> $@; \
  $(ECHO) "/* Reset target */                       "  >> $@; \
  $(ECHO) "target rst                               "  >> $@; \
  $(ECHO) "                                         "  >> $@; \
  $(ECHO) "/* Run application*/                     "  >> $@; \
  $(ECHO) "c                                        "  >> $@
endef

ADDITIONAL_COMMAND_F1L          = $(ESERV2_DEFAULT_COMMAND)
ADDITIONAL_COMMAND_P1X          = $(ESERV2_DEFAULT_COMMAND)
ADDITIONAL_COMMAND_D1M          = $(ESERV2_DEFAULT_COMMAND)
ADDITIONAL_COMMAND_F1H          = $(ESERV2_DEFAULT_COMMAND)

##########################################################################################################
# LINKER FILE GENERATION
##########################################################################################################

#Linker script generation moved to external file
ifeq ($(OS_USECASE),$(filter $(OS_USECASE),BRS OS))
  include Makefile.RH850.GHS.GHS.LinkerScriptDefault
endif

ifeq ($(OS_USECASE),MULTICORE_OS)
  include Makefile.RH850.GHS.GHS.LinkerScriptMultiCore
endif

ifeq ($(OS_USECASE),SAFECTX)
  include Makefile.RH850.GHS.GHS.LinkerScriptSafeCtx
endif

ifeq ($(OS_USECASE),OSGEN7)
  include Makefile.RH850.GHS.GHS.LinkerScriptOsGen7
endif

##########################################################################################################
# RULE TO DELETE PYTHON SCRIPT - Tested with Cubesuite Version 3.00.00 and higher
##########################################################################################################
RM_$(PROJECT_NAME):
	@$(RM) $(PROJECT_NAME).py; \
   $(ECHO) "Deleted $(PROJECT_NAME).py";

##########################################################################################################
# RULE TO GENERATE PYTHON SCRIPT - Tested with Cubesuite Version 3.00.00 and higher
##########################################################################################################
$(PROJECT_NAME).py:
	@$(ECHO) "Generated $(PROJECT_NAME).py"; \
    $(ECHO) "project.Create('$(subst \,/,$(CURDIR))/Debug/$(PROJECT_NAME)/$(PROJECT_NAME).mtpj', MicomType.RH850, '$(DEVICE_FILE_NAME)', ProjectKind.DebugOnly) "   > $@; \
    $(ECHO) "project.Open(r'$(subst \,/,$(CURDIR))/Debug/$(PROJECT_NAME)/$(PROJECT_NAME).mtpj')                                                                 "   >> $@; \
    $(ECHO) "project.File.Add('$(subst \,/,$(CURDIR))/$(PROJECT_NAME).$(BINARY_SUFFIX)', 'Download files')                                                      "   >> $@; \
    $(ECHO) "debugger.Disconnect()                                                                                                                              "   >> $@; \
    $(ECHO) "debugger.DebugTool.Change(DebugTool.E1Lpd)                                                                                                         "   >> $@; \
    $(ECHO) "Generated $(PROJECT_NAME).mtpj"; 

##########################################################################################################
# MAKE TARGETS
##########################################################################################################


$(PROJECT_NAME).$(LNK_SUFFIX): Makefile \
                               Makefile.Project.Part.Defines \
                               Makefile.$(PLATFORM).$(COMPILER_MANUFACTURER).$(EMULATOR).make \
                               Makefile.config \
                               $(PROJECT_NAME).rc \
                               $(PROJECT_NAME).$(BINARY_SUFFIX).bld
	@$(ECHO) "Creating $@"; \
$(LD_FILE)

$(PROJECT_NAME).rc:  Makefile \
                     Makefile.Project.Part.Defines \
                     Makefile.$(PLATFORM).$(COMPILER_MANUFACTURER).$(EMULATOR).make \
                     Makefile.config
	@$(ECHO) "Creating $@";                          \
   $(ECHO) "/* ICE setup */               "  > $@; \
   $(ECHO) "connect $(REMOTE_ESERV) " >> $@; \
   $(ADDITIONAL_COMMAND_$(DERIVATIVE_GROUP_$(DERIVATIVE)))

$(PROJECT_NAME).$(BINARY_SUFFIX).bld: Makefile \
                     Makefile.Project.Part.Defines \
                     Makefile.$(PLATFORM).$(COMPILER_MANUFACTURER).$(EMULATOR).make \
                     Makefile.config
	@$(ECHO) "Creating $@";                           \
   $(ECHO) "#!build                        "  > $@; \
   $(ECHO) "default:                       " >> $@; \
   $(ECHO) "        program                " >> $@; \
   $(ECHO) "        :compilation=assembly  " >> $@; \
   for dir in $(subst \,\\,$(SUBDIRS)); do          \
       $(ECHO) "        :sourcedirs=$$dir  " >> $@; \
   done;                                            \
   $(ECHO) "$(PROJECT_NAME).$(LNK_SUFFIX)  " >> $@; \
   $(ECHO) "        linker_file            " >> $@; \
   $(ECHO) "$(PROJECT_NAME).rc             " >> $@; \
   $(ECHO) "        custom                 " >> $@;

multi: $(PROJECT_NAME).rc
	@export DEVICE_FILE="$(DEVICE_FILE_NAME).dvf"; \
   export GHS_LMHOST=@$(LICENSE_SERVER); \
   export GHS_LMPORT=$(LICENSE_SERVER_PORT); \
   $(ECHO) "DF = $(DEVICE_FILE_NAME).dvf"; \
   $(ECHO) "$(COMPILER_BASE)" > "$(MULTI_BASE)\.redirect_tools"; \
   $(ECHO) "File '$(MULTI_BASE)\.redirect_tools' updated with compiler path '$(COMPILER_BASE)'"; \
   $(subst \,/,$(MULTI_BASE)\multi.exe $(PROJECT_NAME).$(BINARY_SUFFIX) -rc $(PROJECT_NAME).rc) &

$(OBJ_PATH)/%.$(ASM_SUFFIX):
	$(MAKEDEPEND) $< $@

##########################################################################################################
# Automatic connect script for Renesas Debug Environment CubeSuite+
# Tested with Cubesuite Version 3.00.00 and higher
##########################################################################################################
cs: RM_$(PROJECT_NAME) \
    $(PROJECT_NAME).py
	@$(ECHO) "DF = $(DEVICE_FILE_NAME).dvf"; \
   $(ECHO) "$(COMPILER_BASE)" > "$(EMU_PATH)\.redirect_tools"; \
   $(ECHO) "File '$(EMU_PATH)\.redirect_tools' updated with compiler path '$(COMPILER_BASE)'"; \
   $(subst \,/,$(EMU_PATH)\CubeSuiteW+.exe /lpall /noproj /ps TestSuit.py) &

# end of Makefile.RH850.GHS.GHS.make
