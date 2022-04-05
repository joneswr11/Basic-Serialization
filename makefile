TARGET = saveObjs

CC = g++
CFLAGS = -Os

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean:
ifeq ($(OS), Windows_NT)
	-del *.o $(TARGET).exe *.sav /q /f 
else
	-rm -r *.o $(TARGET) *.sav
endif