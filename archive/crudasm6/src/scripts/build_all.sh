# To use this, please make sure you CD to the "scripts" directory then type
#   ./build_all.sh
# Also, make sure you chmod +x all *.sh files before attempting to run this.

echo "Testing configuration..."
cd ..
cd tests
./common_test.sh
cd ../scripts

echo "Building rulecom..."
cd ..
cd util/rulecom
./build.sh
cd ../../scripts

echo "Building makexml..."
cd ..
cd util/makexml
./mkgram.sh
./build.sh
cd ../../scripts

echo "Building mkdecoder..."
cd ..
cd util/mkdecoder
./build.sh
cd ../../scripts

echo "Building mkdisasm..."
cd ..
cd util/mkdisasm
./build.sh
cd ../../scripts

echo "Building mkemu..."
cd ..
cd util/mkemu
./build.sh
cd ../../scripts

echo "Building core..."
cd ..
cd core
./build.sh
cd ../scripts

echo "Testing disassembler..."
cd ..
cd tests
./simple_disasm_test.sh
cd ../scripts

echo "Building crudbin..."
cd ..
cd programs/crudbin
gcc -O2 -o ../../build/crudbin crudbin.c ../../build/decoder_internal.o ../../build/ixdecoder.o ../../build/ixdisasm.o
cd ../../scripts

