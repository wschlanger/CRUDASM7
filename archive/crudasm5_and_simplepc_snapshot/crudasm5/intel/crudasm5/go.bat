@echo off
if exist intel.xml del intel.xml
copy ..\makexml\intel.xml >nul
vs_2010\crudasm5\crudasm5 %1
