@echo off
vs_2010\testsuite\testsuite ..\makexml\intel.xml
ndisasm -p amd -b 16 test16.bin >test16.txt
ndisasm -p amd -b 32 test32.bin >test32.txt
ndisasm -p amd -b 64 test64.bin >test64.txt
vs_2010\testsuite\testsuite ..\makexml\intel.xml --pass2
nasm testxx.asm
