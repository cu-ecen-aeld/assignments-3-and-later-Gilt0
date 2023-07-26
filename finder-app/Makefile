all: clean writer

default: writer

writer:
	@echo "GCC=${GCC}"
	${GCC} writer.c -o writer &> /tmp/writer.make.out
	@echo ${GCC_OUTPUT}

clean:
	@echo "rm -f writer *.o"
	@rm -f writer *.o
