CXX = g++
CXXFLAGS = -Wall
LDFLAGS = -lSDL2

EXEC = build/snake.out
REQUIRED_OBJS = build/utils.o build/game.o build/$(ai).o

$all: build $(REQUIRED_OBJS)
	$(CXX) $(LDFLAGS) $(REQUIRED_OBJS) -o $(EXEC)
	./$(EXEC)

build/%.o: %.cpp defs.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir build

clean:
	@rm -r build

.PHONY: clean
