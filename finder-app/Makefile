all: clean writer

default: writer

writer:
	@echo "GCC=${GCC}"
	${GCC} writer.c -o writer && echo "successfull compile!" || echo "failed compile!" 

clean:
	@echo "rm -f writer *.o"
	@rm -f writer *.o && echo "successfull remove!" || echo "failed remove!" 

