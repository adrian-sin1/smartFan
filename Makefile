CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Werror -g -O0

SRC   = src/FanController.cpp tests/test_fan.cpp
HDR   = src/FanController.h
BUILD = build

.PHONY: test test-gcc test-clang test-all debug clean

test: $(BUILD)/test_fan
	./$(BUILD)/test_fan

$(BUILD)/test_fan: $(SRC) $(HDR)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $(SRC) -o $@

test-gcc:
	$(MAKE) test CXX=g++ BUILD=build/gcc

test-clang:
	$(MAKE) test CXX=clang++ BUILD=build/clang

test-all: test-gcc test-clang

debug: $(BUILD)/test_fan
	gdb ./$(BUILD)/test_fan

clean:
	rm -rf build
