@echo off
g++ -m32 -o boot3 simple_pc.cpp out_metadata.cpp ../x86s/x86s_common.cpp ../x86s/decode.cpp ../crudasm/asmwriter.cpp
