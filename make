gcc -w -g main.c -o main.out

./main.out input/sample.txt 0 > output/sampleGlobal.txt
./main.out input/sample.txt 1 > output/sampleLocal.txt

./main.out input/Opsin1.txt 0 > output/Opsin1Global.txt
./main.out input/Opsin1.txt 1 > output/Opsin1Local.txt

./main.out input/BRCA2.txt 0 > output/BRCA2Global.txt
./main.out input/BRCA2.txt 1 > output/BRCA2Local.txt