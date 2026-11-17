CXX = g++
CPPFLAGS = -Ithird_party
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2
ifeq ($(OS),Windows_NT)
CXXFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8
LDLIBS = -lwinhttp -lshell32
LDFLAGS += -static
target = wiki-search.exe
else
CXX = clang++
LDLIBS = -lcurl
target = wiki-search
endif

sources := $(wildcard src/*.cpp)
objects := $(patsubst src/%.cpp,build/%.o,$(sources))

$(target): $(objects)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

build/%.o: src/%.cpp | build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

build:
	mkdir build

-include $(objects:.o=.d)
