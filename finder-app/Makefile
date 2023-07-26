all: clean writer

default: writer

writer:
	@echo "GCC=${GCC}"
	${GCC} writer.c -o writer &> /tmp/writer.make.out
	@cat /tmp/writer.make.out

clean:
	@echo "rm -f writer *.o"
	@rm -f writer *.o
