INC          =-I ./include/irc
LIB          =-lboost_system -lboost_signals -lpthread

INCFILE=$(realpath make.inc)
include $(INCFILE)

#slow objects are library elements and spirit parsers 
SLOW_OBJS=src/parse_message.o
FAST_OBJS=src/connection.o src/session.o src/channel.o src/user.o src/prefix.o src/command.o src/modes.o src/exception.o src/version.o src/persistant_connection.o src/simple_connection.o src/ctcp.o


OBJS=$(SLOW_OBJS) $(FAST_OBJS) 

all: src/irc.o

src/irc.o: $(OBJS)
	ld -r $^ $(LINK_FLAGS) -o src/irc.o 


src/%.o: src/%.cpp 
	$(CPP) -c $(COMPILE_FLAGS) $(INC) $< -o $@ 

tests/%.o:
	cd tests && $(MAKE)

clean:
	rm -rf $(OBJS) tests/irc_test tests/irc_connection_test.o  tests/session_test.o

qclean:
	rm -rf $(FAST_OBJS) irc_client tests/irc_connection_test.o  tests/session_test.o

stat:
	wc -l include/*.hpp src/*.cpp
