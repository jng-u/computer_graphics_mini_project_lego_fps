CFLAGS = -std=c++17 -O2
LDFLAGS = -lglut -lGL -lGLEW -lGLU -lOpenGL
INCLUDES = 

TARGET = output
OBJECTS =  main.o bmpfuncs.o Button.o GameManager.o Object.o Player.o Block.o 

BUILDID=$(shell date +%Y%m%d-%H:%M:%S)

.PHONY: all test clean

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@ $(INCLUDES)

all: $(TARGET) test

$(TARGET): $(OBJECTS)
	g++ $(CFLAGS) -o $(TARGET) $(OBJECTS) $(INCLUDES) $(LDFLAGS)

test: 
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)
