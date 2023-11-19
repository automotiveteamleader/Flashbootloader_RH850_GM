@echo off
rem **********************************************************************************************************************
rem    FILE DESCRIPTION
rem  --------------------------------------------------------------------------------------------------------------------
rem    \file
rem    \brief        Prepare updater script
rem  --------------------------------------------------------------------------------------------------------------------
rem  COPYRIGHT
rem  --------------------------------------------------------------------------------------------------------------------
rem    \par Copyright
rem    \verbatim
rem    Copyright (c) 2016 by Vector Informatik GmbH.                                                 All rights reserved.
rem 
rem                This software is copyright protected and proprietary to Vector Informatik GmbH.
rem                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
rem                All other rights remain with Vector Informatik GmbH.
rem    \endverbatim
rem
rem **********************************************************************************************************************

rem **********************************************************************************************************************
rem  AUTHOR IDENTITY
rem  --------------------------------------------------------------------------------------------------------------------
rem  Name                          Initials      Company
rem  --------------------------------------------------------------------------------------------------------------------
rem  Marco Riedl                   Rie           Vector Informatik GmbH
rem  --------------------------------------------------------------------------------------------------------------------
rem  REVISION HISTORY
rem  --------------------------------------------------------------------------------------------------------------------
rem  Version   Date        Author  Change Id          Description
rem  --------------------------------------------------------------------------------------------------------------------
rem  01.00.00  2016-08-18  Rie     -                  First implementation
rem **********************************************************************************************************************
if "%1" == "" goto end

rem *****************************************************************************
rem * Prepare FBL update
rem *****************************************************************************
echo Prepare the FBL update

set local_dir=%~d0%~p0

rem *** Configuration section for files and directories *************************

rem Please configure the following variables
set root_dir=%local_dir%..\..\..

rem Hexview executable
set hexview_exe=%root_dir%\Misc\Hexview\hexview.exe

rem Location of new fbl hex file
set NEW_FBL_DIR=%local_dir%

rem *** End of configuration section ********************************************

echo Cut new FBL into Application and Calibration
%hexview_exe% %NEW_FBL_DIR%\%~n1%~x1 /S /CR:0x00000000-0x00037FFF /XI -o %NEW_FBL_DIR%\%~n1_cal%~x1
%hexview_exe% %NEW_FBL_DIR%\%~n1%~x1 /S /CR:0x00038000-0x000FFFFF /XI -o %NEW_FBL_DIR%\%~n1%~x1

copy %NEW_FBL_DIR%\%~n1_cal%~x1 ApplHdr_Updater\DemoCal_withoutHeader

cd ApplHdr_Updater
call Gen_All.bat
cd ..

Echo New Download Image successfully prepared
:end

