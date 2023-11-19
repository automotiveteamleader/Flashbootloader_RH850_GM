@echo off
rem *****************************************************************************
rem * MAKESUPPORT_DIR has to be set to MakeSupport root directory
rem * There MUSTN'T be any blanks between the '=' sign and the path string
rem * Example: 
rem *     set MAKESUPPORT_DIR=..\..\MakeSupport
rem *****************************************************************************

set MAKESUPPORT_DIR=..\..\..\MakeSupport

rem *****************************************************************************
rem * DO NOT EDIT ANYTHING BELOW THIS
rem *****************************************************************************

if "%MAKESUPPORT_DIR% "==" "   goto ErrorNotSet
if not exist %MAKESUPPORT_DIR% goto ErrorWrongPath

set PATH_OLD=%PATH%
set PATH=%MAKESUPPORT_DIR%\cmd;%PATH%
set CYGWIN=nodosfilewarning

rem *****************************************************************************
rem * Run make and store its return value afterwards.
rem * The return value is passed to the caller of m.bat at the end of the file.
rem *****************************************************************************
%MAKESUPPORT_DIR%\cmd\make %1 %2 %3 %4 %5 %6 %7 %8 %9
SET GNU_MAKE_RETURN_CODE=%ERRORLEVEL%

rem stop execution on error
IF ERRORLEVEL 2 (
  color 0C
  echo ERROR on Make/Build/Compile
  pause
  goto End
)
IF ERRORLEVEL 1 (
  color 0C
  echo Error on ???
  pause
  goto End
)

if exist DemoUpdater.hex FblUpd_Prepare_DL.bat DemoUpdater.hex

set PATH=%PATH_OLD%
set PATH_OLD=

goto End

:ErrorNotSet
echo ************************************************************************
echo BRS warning: MAKESUPPORT_DIR has to be set to MakeSupport\cmd directory!
echo              Please correct setting in this batch file and try again!
echo ************************************************************************
goto End

:ErrorWrongPath
echo ************************************************************************
echo BRS warning: %MAKESUPPORT_DIR% does not exist
echo              Please correct setting in this batch file and try again!
echo ************************************************************************
goto End

:End
set MAKESUPPORT_DIR=
exit /b %GNU_MAKE_RETURN_CODE%
