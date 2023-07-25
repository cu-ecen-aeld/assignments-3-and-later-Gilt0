
COMPILER=${CROSS_COMPILE}gcc

all: clean aesdsocket

default: aesdsocket

aesdsocket:
	@${COMPILER} -g aesdsocket.c -o aesdsocket

clean:
	@rm -f aesdsocket *.o