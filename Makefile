INCLUDE:=-Iinclude \
		-I/usr/include/libxml2

SOURCE:=${wildcard source/*.c}

CFLAGS:= #-fpermissive

LDFLAGS:=-L/usr/local/lib/ -levent -lxml2

all:
	make clean
	make obj;make shared
	make clean

obj:
	mkdir -p obj
	gcc -c -fPIC ${INCLUDE} ${CFLAGS} ${SOURCE} ${LDFLAGS}
	mv *.o obj/

shared:
	make obj
	mkdir -p lib
	gcc --shared obj/*.o -o lib/libsp.so

libevent:
	cd 3party/libevent && ./configure && make && make install

all:
	make test
	make shared

test:
	gcc ${INCLUDE} ${SOURCE} ${CFLAGS} ${LDFLAGS} tests/test.c -o tests/a.out

clean:
	rm -rf obj
	rm -rf *.o
