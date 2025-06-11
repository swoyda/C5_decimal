OS=$(shell uname)
CC=gcc
CFLAGS = -std=c11 -g
pFLAGS = -std=c11 -Werror -Wextra -Wall -g

ifeq ($(OS), Darwin)
 CHECK= -lcheck -lpthread -lm 
else
 CHECK= -lcheck -lpthread -lm -lsubunit
endif
SOURCE = s21_*.c tests/*.c
SRC = $(wildcard s21_*.c)

all: main

main: s21_decimal.a test

s21_decimal.o:
	$(CC) $(pFLAGS) -c $(SRC)


s21_decimal.a: s21_decimal.o
	ar rcs s21_decimal.a s21_*.o
	ranlib s21_decimal.a

test: 
	$(CC) $(CFLAGS) tests/*.c tests/*.h s21_decimal.a $(CHECK) -o test
	./test

rebuild: clean all

start: 
	gcc s21*.c s21*.h $(pFLAGS)
	./a.out

gcov_report: s21_decimal.o s21_decimal.a
	$(CC) $(CFLAGS) --coverage $(SOURCE) s21_decimal.a -L. s21_decimal.a $(CHECK) -o ./test 
	./test
	mkdir report
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	open report/index.html
	rm -rf *.gcda *.gcno *.out *.o
	
clear: clean
	

clean:
	rm -rf test *.out
	rm -rf $(ODIR)/*.o $(ODIR)/*.gc*
	rm -rf $(ODIR)/*/*.o $(ODIR)/*/*.gc*
	rm -rf ./gcov/*.o ./gcov/*.gc*
	rm -rf ./*.gc*
	rm -rf $(BUILD_LIB)/$(PROJECT_NAME).a
	rm -rf report*
	rm -rf *.o *.a
	rm -rf *.info
	rm -rf $(TEST_NAME)

clang-replace:
	clang-format --style=Google -i *.c *.h ${LIB_SRC_DIR}/*.c

valgrind:
	valgrind -s --tool=memcheck --leak-check=full --track-origins=yes test

check:
	clang-format -style=Google -i *.c *.h tests/*.c
	clang-format -style=Google -n *.c *.h tests/*.c
