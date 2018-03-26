INCLUDE:=-Iinclude \
		-I/usr/include/libxml2 \
		-I/usr/local/include/event2 \
		-I/usr/local/include/zlog \
        -I/usr/local/include/cjson

SOURCE:=${wildcard source/*.c}

CFLAGS:= #-fpermissive

LDFLAGS:=-L/usr/local/lib/ -levent -lxml2 -lzlog -lpthread -lcjson -lcurl

all:
	make clean
	make obj;make shared
	make clean

obj:
	mkdir -p objs
	gcc -c -fPIC ${INCLUDE} ${CFLAGS} ${SOURCE} ${LDFLAGS}
	mv *.o objs/

shared:
	make obj
	mkdir -p lib
	gcc --shared objs/*.o -o lib/libsp.so

libevent:
	cd 3party/libevent && ./autogen.sh && ./configure && make && make install

zlog:
	cd 3party/zlog && make && mkdir -p /usr/local/include/zlog && cp -rf src/*.h /usr/local/include/zlog/ && cp -rf src/libzlog.so* /usr/local/lib/

cjson:
	cd 3party/cjson && make && cp -rf libcjson.so* /usr/local/lib/ && cp cJSON.h /usr/local/include/cjson/

all:
	make test
	make shared

test:
	gcc ${INCLUDE} ${SOURCE} ${CFLAGS} ${LDFLAGS} tests/test.c -o tests/a.out

clean:
	rm -rf objs
	rm -rf *.o
