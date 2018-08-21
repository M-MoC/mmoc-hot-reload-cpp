g++ -DmmocbuildPROCESSES_HPP -c "processes.cpp" -o "processes.o"
g++ -shared "processes.o" -o "processes.dll"
pause