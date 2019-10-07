#
# build SLIC
#

CC=g++
IDIR=./include
SRC=./src/*.cpp
bin=./bin/slic_cli
obj=*.o

build:  $(IDIR) 
	$(CC) -c $(SRC) `libpng-config --cflags`
	$(CC) -o $(bin) $(obj) `libpng-config --ldflags`
test:
	$(bin) -i sample/3063.png -o sample/3063_slic.png -s 200
clean: $(obj)
	rm -f $(obj)
