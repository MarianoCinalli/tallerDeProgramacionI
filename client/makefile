# Variables
 SOURCES := $(shell find src -name '*.cpp')
 TEMP := $(subst src/,obj/,$(SOURCES))
 OBJECTS := $(subst .cpp,.o,$(TEMP))
 COMPILER := g++
 LIBRARIES := -lSDL2 -lSDL2_image -lyaml-cpp -pthread
 LINKING_FLAGS := $(LIBRARIES) -Wall -I headers/ -o
 COMPILING_FLAGS := -g -std=c++11 -Wall -I headers/ -c -o

# Target
# Deleted old files. Create directories. Compile and link.
 all: directories main

 fast: main run
# Compile and link.
 main: $(OBJECTS)
	$(COMPILER) $(LINKING_FLAGS) $@ $^ $(LIBRARIES)

# Create directories for compiled files.
 directories:
	mkdir -p ./obj
	find src -type d | sed -e 's/src/obj/g' | xargs mkdir -p

 run:
	./main

# Pattern
 obj/%.o: src/%.cpp
	$(COMPILER) $(COMPILING_FLAGS) $@ $<

# Clean object files and binary.
# Forced to ignore non-existent files.
 clean:
	rm -fr ./obj
	rm -f main

# Install dependencies:
 dependencies:
	apt-get update
	apt-get install libsdl2-dev libsdl2-image-dev libyaml-cpp-dev libboost-all-dev

.PHONY: all clean main fast
