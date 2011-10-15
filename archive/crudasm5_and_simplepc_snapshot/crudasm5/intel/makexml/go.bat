@echo off
gcc -E in_intel.c | vs_2010\mkintelxml\debug\mkintelxml.exe >intel.xml
rem type intel.xml
