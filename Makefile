# Makefile for gtest examples

GOOGLE_TEST_LIB = gtest
GOOGLE_TEST_INCLUDE = /usr/local/include

CC = g++
CFLAGS += -fprofile-arcs -ftest-coverage -I $(GOOGLE_TEST_INCLUDE)
CXXFLAGS += -fprofile-arcs -ftest-coverage -I $(GOOGLE_TEST_INCLUDE)
LD_FLAGS += -L /usr/local/lib -l $(GOOGLE_TEST_LIB) -l pthread
CFLAG  = -D PACKET


OBJECT1 = tests.cpp Packet_trace.c createIP.c next_packet.c printConnection.c dePopulateTable.c transportPacketDumping.c

TARGET1 = packet

all: $(TARGET1)   

$(TARGET1): $(OBJECT1)
	$(CC) $(CFLAGS) $(CFLAG) $(CXXFLAGS) $(OBJECT1) -o $(TARGET1) $(LD_FLAGS)


%.o : %.cpp %c
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $<

clean:
	rm -f $(TARGET1)  *.info *.gc* *.png *.html *.css
                    
.PHONY: all clean




