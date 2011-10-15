@echo off
g++ -c -Wall read_script.cpp
g++ -c -Wall x86s_common.cpp
g++ -c -Wall write_script.cpp
g++ -c -Wall write_semantics.cpp
g++ -Wall -o make_out_files read_script.o x86s_common.o write_script.o write_semantics.o
make_out_files
g++ -o make_decoder make_decoder.cpp x86s_common.o
make_decoder
g++ -c test_out.cpp
g++ -c test_semantics.cpp

rem g++ bug.
rem g++ -c -Wall -o decode decode.cpp x86s_common.cpp 

rem decode.cpp is a main program right now.
rem g++ -Wall -o decode decode.cpp x86s_common.o

g++ -c -Wall decode.cpp
