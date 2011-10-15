echo "Generating intel.xml..."
gcc -E ../../scripts/in_intel.c | ../../build/makexml >../../generated/intel.xml
