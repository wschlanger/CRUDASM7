echo "rulecom"
cd ../util/rulecom
./build.sh
cd ../../scripts

echo "generated"
cd ../generated
./build.sh
cd ../scripts

echo "makexml"
cd ../util/makexml
./build.sh
cd ../../scripts
