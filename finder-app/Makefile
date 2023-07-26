all: clean writer

default: writer

writer:
	@echo "GCC=${GCC}"
	${GCC} writer.c -o writer

clean:
	@echo "rm -f writer *.o"
	@rm -f writer *.o && echo "success!" || echo "failure!" 

