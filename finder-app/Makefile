all: clean writer

default: writer

writer:
	@echo "GCC=${GCC}"
	GCC_OUTPUT=$(${GCC} writer.c -o writer)
	@echo ${GCC_OUTPUT}

clean:
	@echo "rm -f writer *.o"
	@rm -f writer *.o
