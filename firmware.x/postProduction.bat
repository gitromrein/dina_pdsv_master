set IPECMD="C:\Program Files\Microchip\MPLABX\v6.00\mplab_platform\mplab_ipe\ipecmd.exe"
set HEX=dist\default\production\pdsv-v1.10.production.hex

rem For More Information C:/Program%20Files%20(x86)/Microchip/MPLABX/v5.25/docs/Readme%20for%20IPECMD.htm
rem ICD3
rem %IPECMD% -P33CK256MP505 -OB -OAS2 -5 -TPICD4 -M -F%HEX% -K -W3.3
%IPECMD% -P33CK256MP505 -OB -5 -TPICD3 -M -F%HEX% -K -W3.3
rem ICD4
