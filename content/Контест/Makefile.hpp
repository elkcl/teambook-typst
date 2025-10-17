.PHONY: all clean

BUILD_DIR = build
CPP_SRCS = $(wildcard *.cpp)
CPP_PROGS = $(patsubst %.cpp,$(BUILD_DIR)/%,$(CPP_SRCS))

all: $(CPP_PROGS)

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/%: %.cpp | $(BUILD_DIR)
	g++ -Wall
		-Wextra
		-pedantic
		-Wfloat-equal
		-Wconversion
		-Wlogical-op
		-Wshift-overflow=2
		-fsanitize=
			address,
			undefined,
			signed-integer-overflow,
			pointer-compare,
			pointer-subtract,
			leak,
			bounds,
			pointer-overflow
		-fno-sanitize-recover
		-D_GLIBCXX_DEBUG
		-D_GLIBCXX_DEBUG_PEDANTIC
		-DONPC 
		-Og -g --std=gnu++20 -o $@ $<
