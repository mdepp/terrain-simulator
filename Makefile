CXXFLAGS = -Wall -std=c++1y
LDLIBS = -lSDL2main -lSDL2
CXX = clang++
OUTPUT_FILE = terrain-simulator


all: terrain-simulator

terrain-simulator: main.o perlin.o
	$(CXX) $(LDFLAGS) -o $(OUTPUT_FILE) main.o perlin.o $(LDLIBS)
main.o: main.cpp perlin.h
perlin.o: perlin.cpp perlin.h


clean:
	rm -f *.o $(OUTPUT_FILE)

.c.o:
	$(CXX) $(CXXFLAGS) -c $<