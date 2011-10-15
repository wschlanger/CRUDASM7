echo "rulecom"
cd ../util/rulecom
chmod +x build.sh
./build.sh
cd ../../scripts

echo "generated"
cd ../generated
chmod +x build.sh
./build.sh
cd ../scripts

echo "makexml"
cd ../util/makexml
chmod +x build.sh
./build.sh
cd ../../scripts
