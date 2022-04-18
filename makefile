TARGET = saveObjs

# Raylib stuff for compression
INCLUDES = -I include
LIBS = -L lib -lraylib

# OS Detection for Libs
ifeq ($(OS),Windows_NT)
	 LIBS += -lopengl32 -lgdi32 -lwinmm
else
	OSNAME = $(shell uname)
	ifeq (($OSNAME),Linux)
		LIBS += -lGL -lm -ldl -lrt
	endif
	ifeq ($(OSNAME),Darwin)
		LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
	endif
endif

CC = g++
CFLAGS = -Os $(INCLUDES)

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@ $(LDFLAGS)

clean:
ifeq ($(OS), Windows_NT)
	-del *.o $(TARGET).exe *.sav /q /f 
else
	-rm -r *.o $(TARGET) *.sav
endif