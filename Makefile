FLAGS := -Iinc -Ideps
LIBS := -lm

src := $(wildcard src/*.c)
obj := $(src:src/%.c=build/%.o)
dep := $(addprefix build/,$(shell cat deps.list))

all: build main

build: compile_flags.txt | $(dep)
	@mkdir -p build

compile_flags.txt: Makefile
	@echo $(FLAGS) | tr " " "\n" > $@

$(dep): deps.list
	@mkdir -p deps && cd deps && \
	GET=$(patsubst build/%,https://%,$@) && \
	echo Downloading $$GET && \
	curl -O $$GET
	@mkdir -p $$(dirname $@) && touch $@

main: $(obj)
	@gcc $^ -Llib $(LIBS) -o build/$@

build/%.o: src/%.c
	@gcc -c $(FLAGS) $^ -o $@

clean:
	rm -rf build deps compile_flags.txt

