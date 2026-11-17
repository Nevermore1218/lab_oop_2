CXX = g++
CPPFLAGS = -Ithird_party
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2 -finput-charset=UTF-8 -fexec-charset=UTF-8
LDLIBS = -lwinhttp -lshell32

sources := $(wildcard src/*.cpp)
objects := $(patsubst src/%.cpp,build/%.o,$(sources))

wiki-search.exe: $(objects)
	$(CXX) -static $(LDFLAGS) $^ $(LDLIBS) -o $@

build/%.o: src/%.cpp | build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

build:
	mkdir build

-include $(objects:.o=.d)
