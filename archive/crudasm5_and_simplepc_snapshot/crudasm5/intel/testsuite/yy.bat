@echo off
nasm yy.asm
vs_2010\testsuite\testsuite.exe ..\makexml\intel.xml yy >yy.txt
