@echo off  
set "riqi=Counter_%date:~0,4%%date:~5,2%%date:~8,2%.bin"  
copy /y  Counter.bin %riqi%
#pause  