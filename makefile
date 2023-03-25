FILE = $(shell pwd)

PATH_CC=$(FILE)/compileur/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin

PATH_TPC=$(FILE)/target-pc
PATH_TPI=$(FILE)/target-pi

PC_CFLAGS=-I$(PATH_TPC)/include/ncurses -I$(PATH_TPC)/include
PC_LDFLAGS=-L$(PATH_TPC)/lib

PI_CFLAGS=-I$(PATH_TPI)/include/ncurses -I$(PATH_TPI)/include
PI_LDFLAGS=-L$(PATH_TPI)/lib

CCC=$(PATH_CC)/arm-linux-gnueabihf-gcc
CXX=$(PATH_CC)/arm-linux-gnueabihf-g++
AR=$(PATH_CC)/arm-linux-gnueabihf-gcc-ar

all : install_pi

crea_lib : 
	cd compileur && unzip tools-master.zip
	cd lib && unzip bcm2835.zip
	cd lib && tar -zxvf ncurses-6.4.tar.gz
	
	mkdir target-pc target-pi
	cd lib/ncurses-6.4/ && ./configure --prefix=$(PATH_TPC) --with-shared
	cd lib/ncurses-6.4/ && make -j9
	cd lib/ncurses-6.4/ && make install
	
	
	cd lib/bcm2835-1.71/ && ./configure -host=arm -prefix=$(PATH_TPI) CC=$(CCC) ar=$(AR)
	cd lib/bcm2835-1.71/ && make -j9
	cd lib/bcm2835-1.71/ && make install
	
	#cd lib/ncurses-6.4/ && ./configure --prefix=$(PATH_TPI) --host --with-shared CC=$(CCC) CXX=$(CXX)
	

	# export CC=$(PATH_CC)/arm-linux-gnueabihf-gcc
	export CC=$(PATH_CC)/arm-linux-gnueabihf-gcc && export CXX=$(PATH_CC)/arm-linux-gnueabihf-g++ && cd lib/ncurses-6.4/ && ./configure --prefix=$(PATH_TPI) --with-shared --host=x86_64-build_unknown-linux-gnu --target=arm-linux-gnueabihf --disable-stripping
	#export CC=$(PATH_CC)/arm-linux-gnueabihf-gcc && export CXX=$(PATH_CC)/arm-linux-gnueabihf-g++ && cd lib/ncurses-6.4/ && make -j9
	#export CC=$(PATH_CC)/arm-linux-gnueabihf-gcc && export CXX=$(PATH_CC)/arm-linux-gnueabihf-g++ && cd lib/ncurses-6.4/ && make install
	cd lib/ncurses-6.4/ && make -j9
	cd lib/ncurses-6.4/ && make install

screen : screen.c bouton.c matrice.c
	$(CCC) -std=c11 $(PI_CFLAGS) $(PI_LDFLAGS) $^ -o $@ -lncurses -lbcm2835
	
matrice : matrice.c matrice.h
	$(CCC) -std=c11 $(PI_CFLAGS) $(PI_LDFLAGS) $^ -o $@ -lbcm2835

rfid : rc522/value.c rc522/config.c rc522/rfid.c rc522/rc522.c rc522/main.c rfid.c
	$(CCC) $(PI_CFLAGS) $(PI_LDFLAGS) $^ -o $@ -lbcm2835

bouton : bouton.c
	$(CCC) -std=c11 $(PI_CFLAGS) $(PI_LDFLAGS) $^ -o $@ -lbcm2835 -pthread

install: install_prog
install_pi : screen
	cp screen $(PATH_TPI)/bin
	tar -zcvf Projet.tar.gz target-pi
	scp  Projet.tar.gz pi@192.168.1.181:/home/pi/Desktop/Projet/use

clean: 
	rm -rf screen rfid matrice bouton Projet.tar.gz
