:: Author: Andreas Wenckebach, Vector Informatik Gmbh
:: Version: 1.00.00 (2014-10-20)
::
:: Info: This script allows to extract the SignerInfo from GM provided container files (e.g. SBA ticket)
::       This may be useful, if you want to create signed container files with our dummy key, while you already configure
::       the GM production key in Fbl.
@echo off

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Configuration
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

::Path to HexView tool and external files
set HEXVIEW_EXE=..\..\..\..\Misc\Hexview\hexview.exe

set SBAT=%1

%HEXVIEW_EXE%  %1 -cr:0-27:566-821 -XB -o SignerInfo.bin

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
