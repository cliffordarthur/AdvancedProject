cc = g++
build_dir = ./build
src = $(shell find ./src/ -name "*.cpp")
deps = $(shell find ./include/ -name "*.h")
prom = $(build_dir)/$(notdir $(CURDIR))
obj = $(src:./src/%.cpp=$(build_dir)/%.o)

$(prom): $(obj) $(build_dir)
	@$(cc) -o $(prom) $(obj) -lcurses

.PHONY: run clean
run: $(obj)
	@$(cc) -o $(prom) $(obj) -lcurses && $(prom)

$(build_dir)/%.o: ./src/%.cpp $(deps)
	@mkdir -p $(dir $@)
	@$(cc) -c $< -o $@

clean: 
	rm -rf $(build_dir)