FLAGS := -Iinc -Ibuild/deps
LIBS := -lm

src := $(wildcard src/*.c)
obj := $(src:src/%.c=build/%.o)
dep := $(addprefix build/,$(shell cat deps.list))

all: build build/main

$(dep): deps.list
	@mkdir -p build/deps && cd build/deps && \
	GET=$(patsubst build/%,https://%,$@) && \
	echo Downloading $$GET && \
	curl -O $$GET
	@mkdir -p $$(dirname $@) && touch $@

build: | $(dep)
	@echo $(FLAGS) | tr " " "\n" > compile_flags.txt
	@mkdir -p build

build/%.o: src/%.c
	@gcc -c $(FLAGS) $^ -o $@

build/main: $(obj)
	@gcc $^ -Llib $(LIBS) -o $@

clean:
	rm -rf build compile_flags.txt

