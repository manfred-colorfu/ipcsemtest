OUT_FILES= change change32 complexdec complexdec32 createary createary32 getall removeary ncnt

CFLAGS = -Wall -g -O2 -pthread
LFLAGS = -static
#LFLAGS =

CC=gcc
CPP=g++

%:	%.cpp
	$(CPP) $(CFLAGS) -o $@ $< $(LFLAGS)

%:	%.c
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

all: $(OUT_FILES)

complexdec32: complexdec.cpp
	$(CPP) $(CFLAGS) -m32 -o $@ $< $(LFLAGS)


change32: change.cpp
	$(CPP) $(CFLAGS) -m32 -o $@ $< $(LFLAGS)

createary32: createary.cpp
	$(CPP) $(CFLAGS) -m32 -o $@ $< $(LFLAGS)

clean:
	rm -f $(OUT_FILES)
	rm -f *~

