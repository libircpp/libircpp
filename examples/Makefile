export CPP =g++
export LNK =$(CPP)

export OPTS         ?=-O0 -ggdb 

INC          =-I ./../include/irc
LIB          =../src/irc.o -lboost_system -lpthread 

export CFLAGS=$(OPTS) -std=c++11 -pedantic -Wall -Wextra -Wno-unused-parameter -DCONS_FAST_COMPILE  -DBOOST_RESULT_OF_USE_DECLTYPE
export LFLAGS=$(OPTS)

PROGRAMS=basic_session

all: $(PROGRAMS) 

%.o: %.cpp
	$(CPP) -c $^ $(CFLAGS) $(INC) -o $@ 

basic_session: basic_session.o
	$(CPP) $^ $(LFLAGS) $(LIB) -o $@

clean:
	rm -rf *.o basic_session
