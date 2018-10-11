#CROSS_COMPILE=	/opt/swi/y17-ext/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-
CC= gcc

all: server client

server: server.o util.o cobs.o
	${CROSS_COMPILE}${CC} server.c util.c cobs.c -o server -lpthread -DDEBUG

client: client.o
	${CC} client.c util.c cobs.c -o client

clean :
	-rm -f *.o server client
