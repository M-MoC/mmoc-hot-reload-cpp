g++ -DmmocbuildPROCESSES_HPP -g -c "processes.cpp" -o "processes.o"
g++ -shared "processes.o" -o "processes.dll"
pause