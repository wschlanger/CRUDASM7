rm ../../../build/makexml*
g++ -o ../../../build/makexml makexml.cpp
cd ../../generated
../../build/makexml < ../scripts/in_intel.txt
cd ../util/makexml
