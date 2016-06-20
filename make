default: build

433mhzdeamon.o: 433mhzdeamon.c
  gcc -c 433mhzdeamon.c -o 433mhzdeamon.o

build: 433mhzdeamon.o
  gcc 433mhzdeamon.o -o 433mhzdeamon

clean:
  -rm -f 433mhzdeamon.o
  -rm -f 433mhzdeamon

install:
  cp init.d/430MHz-Deamon.sh /etc/init.d/430MHz-Deamon
