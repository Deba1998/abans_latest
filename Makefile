
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude


bin/abans:
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -O2 -pthread src/*.cpp -o bin/abans

debug: CXXFLAGS += -g
debug: bin/app-debug

bin/app-debug:
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) src/*.cpp -o -pthread bin/app-debug

clean:
	rm -rf bin/*

.PHONY: clean debug

