
all: aesdsocket

default: aesdsocket

aesdsocket:
	@echo "rm -f aesdsocket *.o"
	@rm -f aesdsocket *.o && echo "successfull remove!" || echo "failed remove!" 
	@echo "GCC=${GCC}"
	${GCC} aesdsocket.c -o aesdsocket && echo "successfull compile!" || echo "failed compile!" 

clean:
	@echo "rm -f aesdsocket *.o"
	@rm -f aesdsocket *.o && echo "successfull remove!" || echo "failed remove!" 
