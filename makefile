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

all : screen_pc screen_pi

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

screen_pi : screen.c
	$(CCC) $(PI_CFLAGS) $(PI_LDFLAGS) $^ -o $@ -lncurses
	
matrix_test : matrix_test.c
	$(CCC) -std=c11 $(PI_CFLAGS) $(PI_LDFLAGS) $^ -o $@ -lbcm2835

install: install_prog
install_pi : screen_pi
	cp screen_pi $(PATH_TPI)/bin
	cp matrix_test $(PATH_TPI)/bin
	tar -zcvf Projet.tar.gz target-pi
	scp  Projet.tar.gz pi@172.24.1.1:/home/pi

install_prog: screen_pi
	scp  screen_pi pi@172.24.1.1:/home/pi/screen/lib/target-pi/bin

clean: 
	rm -rf screen_pc screen_pi
