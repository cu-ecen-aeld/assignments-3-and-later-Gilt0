all: clean writer

default: writer

writer:
	@echo "GCC=${GCC}"
	@${GCC} writer.c -o writer

clean:
	@rm -f writer *.o
