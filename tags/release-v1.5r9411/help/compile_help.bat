@echo off

echo Building html help...

mkdir html
cd html
del *.html
..\..\tools\halibut --html ..\puzzles.but

echo done.
pause
