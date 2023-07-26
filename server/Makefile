
all: clean aesdsocket

default: aesdsocket

aesdsocket:
	@echo "GCC=${GCC}"
	@${GCC} -g aesdsocket.c -o aesdsocket
	@echo "${ls}"


clean:
	@echo "rm -f aesdsocket *.o"
	@rm -f aesdsocket *.o