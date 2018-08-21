g++ -Wall -O3 -std=c++14 -w -c "main.cpp" -o "main.o"
g++ "main.o" "processes.dll" -o "main.exe"
pause