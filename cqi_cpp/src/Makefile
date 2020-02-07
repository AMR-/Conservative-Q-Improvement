# Variables to control Makefile operation
CC = g++
CFLAGS = -Wall -g
I = ../include

# Targets needed to bring the executable up to date
all: test

test: test.o qtree.o qtreeleaf.o qtreeinternal.o leafsplit.o box.o discrete.o
	$(CC) $(CFLAGS) -o test test.o qtree.o qtreeleaf.o qtreeinternal.o leafsplit.o box.o discrete.o

test.o: test.cpp $(I)/qtree.hpp 
	$(CC) $(CFLAGS) -c test.cpp

qtree.o: qtree.cpp $(I)/qfunc.hpp $(I)/qtreeleaf.hpp $(I)/box.hpp $(I)/discrete.hpp
	$(CC) $(CFLAGS) -c qtree.cpp

qtreeleaf.o: qtreeleaf.cpp $(I)/qtreeinternal.hpp
	$(CC) $(CFLAGS) -c qtreeleaf.cpp 

qtreeinternal.o: qtreeinternal.cpp $(I)/qtreenode.hpp $(I)/leafsplit.hpp
	$(CC) $(CFLAGS) -c qtreeinternal.cpp 

leafsplit.o: leafsplit.cpp $(I)/state.hpp $(I)/action.hpp $(I)/utils.hpp
	$(CC) $(CFLAGS) -c leafsplit.cpp 

box.o: box.cpp $(I)/space.hpp $(I)/utils.hpp
	$(CC) $(CFLAGS) -c box.cpp

discrete.o: discrete.cpp $(I)/space.hpp $(I)/action.hpp
	$(CC) $(CFLAGS) -c discrete.cpp 

clean:
	$(RM) test *.o *~
