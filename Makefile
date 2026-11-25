CXX = clang++
CPPFLAGS = -Ithird_party
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2
LDLIBS = -lcurl
target = wiki-search

sources := $(wildcard src/*.cpp)
objects := $(patsubst src/%.cpp,build/%.o,$(sources))

$(target): $(objects)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

build/%.o: src/%.cpp | build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

build:
	mkdir build

-include $(objects:.o=.d)
