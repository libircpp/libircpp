export CPP =g++
export LNK =$(CPP)

export OPTS         ?=-O0 -ggdb

INC          =-I ./../include/irc
LIB          =./../src/irc.o -lboost_system -lpthread -lboost_unit_test_framework 

export CFLAGS=$(OPTS) -std=c++11 -pedantic -Wall -Wextra -Wno-unused-parameter -DCONS_FAST_COMPILE  -DBOOST_RESULT_OF_USE_DECLTYPE
export LFLAGS=$(OPTS)

PROGRAMS=parser_tests ctcp_parser_test simple_connection_test persistant_connection_test coloured_string_test 
#irc_connection_test session_test crtp_channel_test 

all: $(PROGRAMS) 

%.o: %.cpp
	$(CPP) -c $^ $(CFLAGS) $(INC) -o $@

%_test: %_test.o
	$(CPP) $^ $(LFLAGS) $(LIB) -o $@

%_tests: %_tests.o
	$(CPP) $^ $(LFLAGS) $(LIB) -o $@

clean:
	rm -rf *.o *_tests *_test
