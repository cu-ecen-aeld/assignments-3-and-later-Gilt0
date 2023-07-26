
all: clean aesdsocket

default: aesdsocket

aesdsocket:
	@echo "GCC=${GCC}"
	GCC_OUTPUT=$(${GCC} aesdsocket.c -o aesdsocket)
	@echo ${GCC_OUTPUT}

clean:
	@echo "rm -f aesdsocket *.o"
	@rm -f aesdsocket *.o