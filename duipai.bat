@echo off
:loop
  python a.py
  main.exe dict.in query.in a0.txt
  main1.exe dict.in query.in a1.txt
  echo "END"
  fc a0.txt a1.txt
if not errorlevel 1 goto loop
pause
goto loop