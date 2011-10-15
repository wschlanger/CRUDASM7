g++ -o../../build/makexml mkintelxml.cpp parser.cpp
echo "Generating intel.xml..."
gcc -E ../../scripts/in_intel.c | ../../build/makexml >../../generated/intel.xml
