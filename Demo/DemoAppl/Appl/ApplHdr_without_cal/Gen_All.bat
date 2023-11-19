:: Author: Andreas Wenckebach, Vector Informatik Gmbh
:: Version: 1.01.00 (2014-02-18)
:: Version: 1.02.00 (2014-06-06) Create PLAIN_FOLDER if not existant, create info files.
:: Version: 1.03.00 (2015-02-09) Add compressed ARLE container generation, allow configuring optional example containers
:: Version: 1.04.00 (2015-02-11) Use dedicated signed header alignment.
:: Version: 1.05.00 (2016-04-13) Evaluate Hexview err-files
:: Version: 1.06.00 (2016-11-02) Added LZMA decompression ability
@echo off

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Configuration
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

::Path to HexView tool and external files
set HEXVIEW_EXE=..\..\..\..\Misc\Hexview\hexview.exe
set LZMA_COMPRESSION_UTILITY="..\..\..\..\Misc\Cmpr_Lzma\COMPRESS_LZMA_Util.exe"
set XML_MOD_DEF=ModGenBase.xml
set PRIVATE_KEY_SOURCE=..\..\..\..\Misc\DemoKey_2048\rsakeys_2048.txt
set CAL_FOLDER=DemoCal_withoutHeader

:: file format and file name extensions for generated files (added to module name)
set OUTFORMAT=gbf
set LZMAFORMAT=lzma
set PEXT=_plain
set PLAIN_FOLDER=.\GeneratedAndToSignedByGm
set SEXT=_sign
set ARLECOMPEXT=_arlecomp
set LZMACOMPEXT=_lzmacomp

:: Configure to be gnerated example containers (signed / ARLE COMPRESSED)
set GEN_SIGNED_CONTAINERS=YES
set GEN_ARLE_COMPRESSED_CONTAINERS=NO
set GEN_LZMA_COMPRESSED_CONTAINERS=YES

:: LZMA configuration
:: Valid values OPTLEVEL 0-9; DICTIZE in byte
set OPTLEVEL=9
set DICTSIZE=4096
set LC=0
set LP=0
set PB=0

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Hexview configuration parameters (GENERAL, PLAIN + SIGNED HEADER) (check hexview tool documentation):
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

::   GENERAL:
::   - Call Hexview silently ("-s") or not (leave empty)
set SILENT=-s
::   PLAIN HEADER (to be programmed data):
::   - Fill all ( /FA )
::   - alignment programmed data (address /Ad<val> and length /Al)
::   - checksum param ("Cs5" for B. endian / "Cs6" for l. endian ECU)
set FILL=/FA
set ALIGN=/Ad:16 /Al /Af:0xCA
set CS=/Cs6
::   SIGNED HEADER:
::   - alignment Ram hold data (address /Ad<val> and length /Al); should be always 4
set ALIGN_SIGNED_HDR=/Ad:4 /Al /Af:0x00

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Check input file existence and create info files
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
IF NOT EXIST %PRIVATE_KEY_SOURCE% call :Error %PRIVATE_KEY_SOURCE%
IF NOT EXIST %HEXVIEW_EXE% call :Error %HEXVIEW_EXE%
IF /I "%GEN_LZMA_COMPRESSED_CONTAINERS%" EQU "YES" (
IF NOT EXIST %LZMA_COMPRESSION_UTILITY% call :Error %LZMA_COMPRESSION_UTILITY%
)
IF NOT EXIST %XML_MOD_DEF% call :Error %XML_MOD_DEF%
IF NOT EXIST %CAL_FOLDER% mkdir %CAL_FOLDER%
IF NOT EXIST %PLAIN_FOLDER% mkdir %PLAIN_FOLDER%
IF EXIST *.err DEL *.err
cd. > ".\Files with _sign extension are Demo Download containers"
cd. > ".\%PLAIN_FOLDER%\Files with _plain extension are to be send to GM for signing"

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Generation calls for different Standard download modules.
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Required Function Parameters for GenAppHeaders (Application files), GenCalHeaders (calibration files):
:: - <module name> : "Module Name" - Needs to match configured module name (XML_MOD_DEF file)
:: - <file name>   : "File name" - File name of raw input file
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: call :<func name>   "<module name>" "<file name>"

:: For all cal files in CAL_FOLDER call function GenCalHeaders (Assumption: module name == file name without extension )
for /f %%f in ('dir /b .\%CAL_FOLDER%') do (
   call :GenCalHeaders "%%~nf" "%CAL_FOLDER%\%%f"
)
call :GenAppHeaders "DemoAppl"      "..\DemoAppl.hex"

:: Move formats not required for testdownload, but only for later signing by GM to defined folder
@echo on
move *%PEXT%.%OUTFORMAT% %PLAIN_FOLDER%
@echo off

goto :EOF

:Error
echo "Input file %~1 not found."
goto :end
goto :EOF

:Error2
echo Hexview reported Error:
type %~1
goto :end
goto :EOF


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Functions to be used to generate files - !! Usually not required to be edited !!
::
:: In case of problems:
:: - check generated error files
:: - check hexview.exe parameters used when calling the below functions in batch and compare to Hexview manual for plausibility
:: - check GENy file module configuration (configuration base for XML_MOD_DEF) and XML_MOD_DEF output for plausibility
:: - Please contact our support in case of questions
::
:: Required Function Parameters are same as for above described for GenAppHeaders, GenCalHeaders.
:: ( GenSignAppFunc and GenSignCalFunc require only the "Module Name" parameter (%~1) )
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:GenAppHeaders
IF NOT EXIST %~2 (
 call :Error %~2
 echo "ERROR: Application file cannot be generated."
 goto :EOF
)
call :GenPlainAppFunc %~1 %~2
IF /I "%GEN_SIGNED_CONTAINERS%" EQU "YES" (
call :GenSignAppFunc %~1
)
IF /I "%GEN_ARLE_COMPRESSED_CONTAINERS%" EQU "YES" (
call :GenArleCompressedModuleFunc %~1
)
IF /I "%GEN_LZMA_COMPRESSED_CONTAINERS%" EQU "YES" (
call :GenLzmaCompressedModuleFunc %~1
)
goto :EOF

:GenCalHeaders
IF NOT EXIST %~2 (
 call :Error %~2
 echo "Continue without considering cal file %~1 for header generation."
 goto :EOF
)
call :GenPlainCalFunc %~1 %~2
IF /I "%GEN_SIGNED_CONTAINERS%" EQU "YES" (
call :GenSignCalFunc %~1
)
IF /I "%GEN_ARLE_COMPRESSED_CONTAINERS%" EQU "YES" (
call :GenArleCompressedModuleFunc %~1
)
IF /I "%GEN_LZMA_COMPRESSED_CONTAINERS%" EQU "YES" (
call :GenLzmaCompressedModuleFunc %~1
)
goto :EOF

:GenPlainAppFunc
@echo on
%HEXVIEW_EXE% %~2 %SILENT% -xgc_app_plain=%~1 -xml=%XML_MOD_DEF% %FILL% %ALIGN% %CS% /E:%~1%PEXT%.err -o %~1%PEXT%.%OUTFORMAT%
@echo off
IF EXIST %~1%PEXT%.err call :Error2 %~1%PEXT%.err
goto :EOF

:GenPlainCalFunc
@echo on
%HEXVIEW_EXE% %~2 %SILENT% -xgc_cal_plain=%~1 -xml=%XML_MOD_DEF% %FILL% %ALIGN% %CS% /E:%~1%PEXT%.err -o %~1%PEXT%.%OUTFORMAT%
@echo off
IF EXIST %~1%PEXT%.err call :Error2 %~1%PEXT%.err
goto :EOF

:GenSignAppFunc
@echo on
%HEXVIEW_EXE% %~1%PEXT%.%OUTFORMAT% %SILENT% -xgc_app_sign -xml=%XML_MOD_DEF% %ALIGN_SIGNED_HDR% /cs20 /dp32:%PRIVATE_KEY_SOURCE% /E:%~1%SEXT%.err -o %~1%SEXT%.%OUTFORMAT%
@echo off
IF EXIST %~1%SEXT%.err call :Error2 %~1%SEXT%.err
goto :EOF

:GenSignCalFunc
@echo on
%HEXVIEW_EXE% %~1%PEXT%.%OUTFORMAT% %SILENT% -xgc_cal_sign -xml=%XML_MOD_DEF% %ALIGN_SIGNED_HDR% /cs20 /dp32:%PRIVATE_KEY_SOURCE% /E:%~1%SEXT%.err -o %~1%SEXT%.%OUTFORMAT%
@echo off
IF EXIST %~1%SEXT%.err call :Error2 %~1%SEXT%.err
goto :EOF

:GenArleCompressedModuleFunc
@echo on
%HEXVIEW_EXE% %~1%SEXT%.%OUTFORMAT% %SILENT% -dp36 -xgc_sign_cmpr:0401 /E:%~1%ARLECOMPEXT%.err -o %~1%ARLECOMPEXT%.%OUTFORMAT%
@echo off
IF EXIST %~1%PEXT%.err call :Error2 %~1%PEXT%.err
goto :EOF

:GenLzmaCompressedModuleFunc
@echo on
%LZMA_COMPRESSION_UTILITY% -O%OPTLEVEL% -lc=%LC% -lp=%LP% -pb=%PB% -ws=%DICTSIZE% %~1%SEXT%.%OUTFORMAT% %~1%LZMACOMPEXT%.%LZMAFORMAT%
%HEXVIEW_EXE% %~1%LZMACOMPEXT%.%LZMAFORMAT% %SILENT% -xgc_sign_cmpr:0402 /E:%~1%LZMACOMPEXT%.err -o %~1%LZMACOMPEXT%.%OUTFORMAT%
IF EXIST %~1%LZMACOMPEXT%.%LZMAFORMAT% DEL %~1%LZMACOMPEXT%.%LZMAFORMAT%
@echo off
IF EXIST %~1%PEXT%.err call :Error2 %~1%PEXT%.err
goto :EOF

:end
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
