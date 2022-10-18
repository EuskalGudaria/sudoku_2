OUTPUT= sudoku
.PHONY: all clean

all: 
	@cd src && $(MAKE)
	@cp -f src/$(OUTPUT) ./

clean: 
	@cd src && $(MAKE) clean
	@cd ./
	@rm -f ${OUTPUT}
	@rm -f test/${OUTPUT}

test: all
	@cp -f ${OUTPUT} test/$(OUTPUT)
	cd test && ./test.sh

help:
	@echo "all: 	build and execute the whole project"
	@echo "clean: 	delete all object files"
	@echo "help: Display the main targets of this Makefile with a short description"
