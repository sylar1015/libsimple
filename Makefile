INCLUDE:=-Iinclude

SOURCE:=${wildcard source/*.c}

CFLAGS:= #-fpermissive

all:
	make clean
	make obj;make shared
	make clean

obj:
	mkdir -p obj
	gcc -c -fPIC ${INCLUDE} ${CFLAGS} ${SOURCE}
	mv *.o obj/

shared:
	mkdir -p lib
	gcc --shared obj/*.o -o lib/libsp.so

clean:
	rm -rf obj
	rm -rf *.o
