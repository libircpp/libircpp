export CPP =g++
export LNK =$(CPP)

#export OPTS         =-O3 
export OPTS         ?=-O0 -ggdb 

INC          =
LIB          =-lboost_system -lboost_signals -lpthread

export CFLAGS=$(OPTS) -std=c++11 -pedantic -Wall -Wextra -Wno-unused-parameter -DCONS_FAST_COMPILE  -DBOOST_RESULT_OF_USE_DECLTYPE
#-DNDEBUG  
#-Wfatal-errors
#-D_GLIBCXX_DEBUG
#-Werror

export LFLAGS=$(OPTS)

#slow objects are library elements and spirit parsers 
SLOW_OBJS=parse_message.o
FAST_OBJS=connection.o session.o channel.o user.o prefix.o command.o modes.o

OBJS=$(SLOW_OBJS) $(FAST_OBJS) 

all: irc.o

#irc_test session_test 

irc.o: $(OBJS)
	ld -r $^ $(LFLAGS) -o irc.o 

session_test: $(OBJS) session_test.o
	$(CPP) $^ $(LFLAGS) $(LIB) -o session_test

irc_test: $(OBJS) irc_connection_test.o
	$(CPP) $^ $(LFLAGS) $(LIB) -o irc_test 

#[ $(OBJS)

%.o: %.cpp 
	$(CPP) -c $(CFLAGS) $(INC) $< -o $@ 

clean:
	rm -rf $(OBJS) irc_test irc_connection_test.o  session_test.o

qclean:
	rm -rf $(FAST_OBJS) irc_client irc_connection_test.o  session_test.o

stat:
	wc -l *.hpp *.cpp
