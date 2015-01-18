CC=g++
CFLAGS=-g -Wall -Werror -std=c++11 -O3
EXECUTIBLE=snow
SOURCES=main.cpp lexer.cpp
# replace all appearance of .cpp with .o
OBJECTS=$(SOURCES:.cpp=.o)

all: $(EXECUTIBLE)

# $@ is automatic variable for target name
$(EXECUTIBLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -c 

clean:
	rm -rf *o snow

test: $(EXECUTIBLE)
	valgrind -q --track-origins=yes ./$(EXECUTIBLE)

debug: $(EXECUTIBLE)
	gdb ./$(EXECUTIBLE)
