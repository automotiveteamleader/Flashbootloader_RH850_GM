@echo off
rem **********************************************************************************************************************
rem    FILE DESCRIPTION
rem  --------------------------------------------------------------------------------------------------------------------
rem    \file
rem    \brief        Hardware specific prepare updater script
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

rem Note: %local_dir% is set by calling instance
rem Note: %NEW_FBL_DIR% is set by calling instance
rem Note: %root_dir% is set by calling instance
rem Note: %hexview_exe% is set by calling instance

echo Cut everything above FBL region
%hexview_exe% %NEW_FBL_DIR%\%~n1%~x1 /S /CR:0x00020000-0x00FFFFFF /XI -o %NEW_FBL_DIR%\%~n1%~x1

echo Align data to flash write segment size
%hexview_exe% %NEW_FBL_DIR%\%~n1%~x1 /S /AD:0x100 /AL:0x100 /AF:0xCA /XI -o %NEW_FBL_DIR%\%~n1%~x1

rem echo Fill Bootloader region if necessary
rem %hexview_exe% %1 /S /FA /XI -o %NEW_FBL_DIR%\%~n1%~x1

:end
echo.
exit /b
