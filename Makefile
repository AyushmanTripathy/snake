CXX = g++
CXXFLAGS = -Wall
LDFLAGS = -lSDL2

EXEC = build/snake.out
REQUIRED_OBJS = build/utils.o build/game.o build/$(ai).o

all: $(EXEC)

build/%.o: %.cpp defs.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir build

$(EXEC): build $(REQUIRED_OBJS)
	$(CXX) $(LDFLAGS) $(REQUIRED_OBJS) -o $(EXEC)

clean:
	@rm -r build

run:
	./$(EXEC)

.PHONY: all clean
