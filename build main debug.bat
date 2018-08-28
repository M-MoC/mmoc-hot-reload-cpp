g++ -g -std=c++14 -c "main.cpp" -o "main.o"
g++ "main.o" "processes.dll" -o "main.exe"
pause