CC=g++
CFLAGS=-c -g -Wall -lboost_system
LDFLAGS=-lpthread -lrt -lboost_thread -lboost_system
SOURCES=agency.cc agent.cc attributes.cc problemSpace.cc workload.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=GenALife

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o
