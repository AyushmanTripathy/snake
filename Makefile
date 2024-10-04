CXX = g++
CXXFLAGS = -Wall
LDFLAGS = -lSDL2

EXEC = build/snake.out
REQUIRED_OBJS = build/utils.o build/game.o build/$(algo).o

$all: build $(REQUIRED_OBJS)
	$(CXX) $(REQUIRED_OBJS) $(LDFLAGS) -o $(EXEC)
	./$(EXEC)

build/%.o: src/%.cpp src/defs.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir build

clean:
	@rm -r build

.PHONY: clean
