CC = gcc
CFLAGC =  -Wall -Werror -Wextra  -std=c11 
SOURCES = calc/smartcalc.c
OBJECTS = calc/smartcalc.o


all: build static tests

build:
	$(CC) $(CFLAGC) -c $(SOURCES)
	mv smartcalc.o ./calc/

static: build
	ar rc s21_smartcalc.a $(OBJECTS)
	ranlib s21_smartcalc.a

tests: static
	$(CC) $(CFLAGC) -lcheck unit_test.c s21_smartcalc.a -o test
	./test

gcov_report:
	gcc -lcheck --coverage unit_test.c $(SOURCES) -o gcov_report
	./gcov_report
	lcov -t "./gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	open ./report/index.html

install:
	make clean
	mkdir build
	cd calc && qmake && make && make clean && rm Makefile && cd ../ && mv calc/calc.app build

uninstall:
	rm -rf build*

dvi:
	open doc.html

dist:
	rm -rf Archive_smartcalc_v1.0/
	mkdir Archive_smartcalc_v1.0/
	mkdir Archive_smartcalc_v1.0/src
	mv ./build/calc.app Archive_smartcalc_v1.0/src/
	tar cvzf Archive_smartcalc_v1.0.tgz Archive_smartcalc_v1.0/
	rm -rf Archive_smartcalc_v1.0/

clean:
	rm -rf *.{a,o,dSYM,out}
	rm -rf $(EXECUTABLE)
	rm -rf gcov_report
	rm -rf report
	rm -rf CPPLINT.cfg
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info
	rm -rf test
	rm -rf Archive_smartcalc_v1.0 
	rm -rf *tgz
	rm -rf build
	rm -rf calc/qmake_qmake_qm_files.qrc
	rm -rf calc/.qmake.stash
	rm -rf calc/*.o

rebuild: clean all
