@echo off

echo ########################################
echo #      Scarface The World Is Yours     #
echo #                Mod                   #
echo ########################################

..\premake5.exe --file="premake-rules/main.lua" vs2019
if %0 == "%~0" pause
