cc = g++
build_dir = ./bin
src = $(shell find ./src/ -name "*.cpp")
deps = $(shell find ./include/ -name "*.h")
prom = $(build_dir)/$(notdir $(CURDIR))
obj = $(src:./src/%.cpp=$(build_dir)/%.o)

CURSES = -lcurses
CXXFLAGS = $(shell wx-config --cxxflags)
LIBS = $(shell wx-config --libs)

$(prom): $(obj) $(build_dir)
	$(cc) -o $(prom) $(obj) $(CXXFLAGS)
# $(cc) -o $(prom) $(obj) $(CURSES)

.PHONY: run clean count
run: $(obj)
	$(cc) $(CXXFLAGS) -o $(prom) $(obj) $(LIBS) && $(prom)
# $(cc) -o $(prom) $(obj) $(CURSES) && $(prom)

$(build_dir)/%.o: ./src/%.cpp $(deps)
	@mkdir -p $(dir $@)
	@$(cc) $(CXXFLAGS) -c $< -o $@
# @$(cc) -c $< -o $@

clean: 
	rm -rf $(build_dir)
	@rm -rf ./.vscode

count:
	@echo $(shell find ./ -name "*.h" -or -name "*.cpp" |xargs cat|grep -v ^$$|wc -l)