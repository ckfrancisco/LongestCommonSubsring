g++ -w -g main.cpp -o main

./main input/sample.txt 0 > output2/sampleGlobal.txt
./main input/sample.txt 1 > output2/sampleLocal.txt

./main input/Opsin1.txt 0 > output2/Opsin1Global.txt
./main input/Opsin1.txt 1 > output2/Opsin1Local.txt

./main input/BRCA2.txt 0 > output2/BRCA2Global.txt
./main input/BRCA2.txt 1 > output2/BRCA2Local.txt