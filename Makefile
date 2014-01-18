export CPP =g++
export LNK =$(CPP)

#export OPTS         =-O3 
export OPTS         ?=-O0 -ggdb 

INC          =-I ./include/irc
LIB          =-lboost_system -lboost_signals -lpthread

export CFLAGS=$(OPTS) -std=c++11 -pedantic -Wall -Wextra -Wno-unused-parameter -DCONS_FAST_COMPILE  -DBOOST_RESULT_OF_USE_DECLTYPE
#-DNDEBUG  
#-Wfatal-errors
#-D_GLIBCXX_DEBUG
#-Werror

export LFLAGS=$(OPTS)

#slow objects are library elements and spirit parsers 
SLOW_OBJS=src/parse_message.o
FAST_OBJS=src/connection.o src/session.o src/channel.o src/user.o src/prefix.o src/command.o src/modes.o

OBJS=$(SLOW_OBJS) $(FAST_OBJS) 

all: src/irc.o

#irc_test session_test 

src/irc.o: $(OBJS)
	ld -r $^ $(LFLAGS) -o src/irc.o 

#[ $(OBJS)

src/%.o: src/%.cpp 
	$(CPP) -c $(CFLAGS) $(INC) $< -o $@ 

tests/%.o:
	cd tests && $(MAKE)

clean:
	rm -rf $(OBJS) tests/irc_test tests/irc_connection_test.o  tests/session_test.o

qclean:
	rm -rf $(FAST_OBJS) irc_client tests/irc_connection_test.o  tests/session_test.o

stat:
	wc -l include/*.hpp src/*.cpp
