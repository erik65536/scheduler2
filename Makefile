CC=g++
#CFLAGS=-c -Wall -std=c++11 -O3
#LDFLAGS=-O3 -pthread
#google-perftools
CFLAGS=-c -Wall -std=c++11 -O3
LDFLAGS=-O3 -pthread -lprofiler
#debug
#CFLAGS=-c -Wall -std=c++11 -g -fno-inline
#LDFLAGS=-pthread -g -fno-inline
SOURCES=src/main.cpp src/safe_file_handle.cpp src/process_input.cpp src/input_parser.cpp src/process.cpp
INC=
OBJECTS=$(SOURCES:.cpp=.o)
	EXECUTABLE=scheduler

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	  $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	  $(CC) $(CFLAGS) $(INC) $< -o $@

clean:
	  rm -f $(OBJECTS) $(EXECUTABLE)

