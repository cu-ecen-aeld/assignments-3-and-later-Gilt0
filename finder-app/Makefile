all: writer

default: writer

writer:
	@echo "rm -f writer *.o"
	@rm -f writer *.o && echo "successfull remove!" || echo "failed remove!" 
	@echo "GCC=${GCC}"
	${GCC} writer.c -o writer && echo "successfull compile!" || echo "failed compile!" 

clean:
	@echo "rm -f writer *.o"
	@rm -f writer *.o && echo "successfull remove!" || echo "failed remove!" 

