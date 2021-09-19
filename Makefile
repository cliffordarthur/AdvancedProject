cc = g++
build_dir = ./build 
src = $(shell find ./src/ -name "*.cpp")
deps = $(shell find ./include/ -name "*.h")

.PHONY: run clean
run: #TODO:

clean: 
	rm -rf $(build_dir)