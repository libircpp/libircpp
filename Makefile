export CPP  =g++
export LNK =$(CPP)

#export OPTS         =-O3 -DNDEBUG
export OPTS         =-O0 -ggdb -DCONS_FAST_COMPILE #-D_GLIBCXX_DEBUG

INC          =
LIB          =-lboost_system -lboost_signals -lpthread

export CFLAGS=$(OPTS) -std=c++11 -pedantic -Wall -Wextra -Wno-unused-parameter 
#-Werror -Wfatal-errors

export LFLAGS=$(OPTS)

#slow objects are library elements and spirit parsers 
SLOW_OBJS=
FAST_OBJS=irc_connection.o irc_parser.o session.o

OBJS=$(SLOW_OBJS) $(FAST_OBJS)

all: irc_test session_test

session_test: $(OBJS) session_test.o
	$(CPP) $^ $(LFLAGS) $(LIB) -o session_test

irc_test: $(OBJS) irc_connection_test.o
	$(CPP) $^ $(LFLAGS) $(LIB) -o irc_test 

#[ $(OBJS)

%.o: %.cpp 
	$(CPP) -c $(CFLAGS) $(INC) $< -o $@ 

clean:
	rm -rf $(OBJS) irc_test

qclean:
	rm -rf $(FAST_OBJS) irc_client irc_connection_test.o  session_test.o

stat:
	wc -l *.hpp *.cpp
