#The Makefile for compiling the shadow detection program

CC = g++
IFLAGS = -I /usr/local/include/opencv2 
LDFLAGS = -lm -lopencv_core -lopencv_highgui -lopencv_video -lopencv_imgproc
CFLAGS = -c -Wall
OBJECTS = shadowDetector.o

all: shadowDetector.o main.cpp
	$(CC) $(IFLAGS) $(LDFLAGS) main.cpp shadowDetector.o -o shadowDetector

shadowDetector.o: shadowDetector.cpp shadowDetector.h
	$(CC) $(IFLAGS) $(CFLAGS) -c shadowDetector.cpp

clean:
	rm -rf *.o *~ shadowDetector
