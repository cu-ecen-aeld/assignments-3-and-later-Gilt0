
all: clean aesdsocket

default: aesdsocket

aesdsocket:
	@echo "GCC=${GCC}"
	${GCC} aesdsocket.c -o aesdsocket &> /tmp/aesdsocket.make.out
	@cat /tmp/aesdsocket.make.out

clean:
	@echo "rm -f aesdsocket *.o"
	@rm -f aesdsocket *.o