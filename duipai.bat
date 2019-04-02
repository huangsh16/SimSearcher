@echo off
:loop
  python a.py
  main.exe dict.txt query.txt 0.txt
  main1.exe dict.txt query.txt 1.txt
  fc 0.txt 1.txt
if not errorlevel 1 goto loop
pause
goto loop