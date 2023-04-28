CFLAGS=-lm

src := $(wildcard src/*.c)
obj := $(src:src/%.c=build/%.o)
dep := $(addprefix build/,$(shell cat deps.list))

all: build

build: | $(dep)
	@mkdir -p build

$(dep): deps.list
	@mkdir -p deps && cd deps && \
	GET=$(patsubst build/%,https://%,$@) && \
	echo Downloading $$GET && \
	curl -O $$GET
	@mkdir -p $$(dirname $@) && touch $@

main: $(obj)
	@gcc $^ -Llib $(CFLAGS) -o build/$@

build/%.o: src/%.c
	@gcc -c -Iinc $^ -o $@

clean:
	rm -rf build deps

