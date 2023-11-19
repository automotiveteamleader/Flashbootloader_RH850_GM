:: Authors: Jason Learst (jle), Vector CANtech Inc.
::          Andreas Wenckebach (awh), Vector Informatik GmbH
:: Versions: jle     1.00.00 (2015-10-03), Creation
::           jle,awh 1.01.00 (2015-11-03), Improve usability
::
:: Info: This script generates a new DummySignerInfo with updated Subject Name, Certificate ID, Key-NBID, etc.
::       This may be useful if you want to update these parameters in the bootloader without having received an
::       SBA Ticket from GM.
@echo off

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Configuration
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

::Set new SignerInfo parameters
set SUBJECT_NAME=Engine
set CERT_ID=0xAAAAAAAAAAAAAAAA
set KEY_NBID=0x0000

::Set output file name
set OUTPUT_FILE=%SUBJECT_NAME%_SignerInfo.hex

::Path to Hexview tool and external files
set HEXVIEW_EXE=..\Hexview\hexview.exe
set PRIVATE_KEY_SOURCE=..\DemoKey_2048\rsakeys_2048.txt
set SILENT=/s
set SIGNER_INFO=SignerInfoDummyKey.hex

::Create the new SignerInfo hex
echo|set /p dummyName="%SUBJECT_NAME%" > subject_name.bin
%HEXVIEW_EXE% %SIGNER_INFO% %SILENT% /CR:0x11A,256:0x0,16:0x10,10 /XI -o %OUTPUT_FILE%
%HEXVIEW_EXE% %OUTPUT_FILE% %SILENT% /FR:0x0,16 /FP:0x00 /XI -o %OUTPUT_FILE%
%HEXVIEW_EXE% %OUTPUT_FILE% %SILENT% /MO:subject_name.bin;0x0,16 /XI -o %OUTPUT_FILE%
%HEXVIEW_EXE% %OUTPUT_FILE% %SILENT% /FR:0x10,8 /FP:%CERT_ID% /XI -o %OUTPUT_FILE%
%HEXVIEW_EXE% %OUTPUT_FILE% %SILENT% /FR:0x18,2 /FP:%KEY_NBID% /XI -o %OUTPUT_FILE%
%HEXVIEW_EXE% %OUTPUT_FILE% %SILENT% /dp32:%PRIVATE_KEY_SOURCE%;sc.rsa /E:Sign.err
%HEXVIEW_EXE% %SILENT% /IA:sc.rsa /XI -o sc.hex
%HEXVIEW_EXE% %OUTPUT_FILE% %SILENT% /mo:sc.hex;0x11a /XI -o %OUTPUT_FILE%

::Clean up intermediate files
DEL sc.hex
DEL sc.rsa
DEL subject_name.bin

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
