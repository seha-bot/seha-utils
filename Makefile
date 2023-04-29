FLAGS := -Iinc -Ibuild/deps
LIBS := -lm

all: build/main

dep := $(addprefix build/,$(shell cat deps.list))
$(dep): deps.list
	@mkdir -p build/deps && cd build/deps && \
	GET=$(patsubst build/%,https://%,$@) && \
	echo Downloading $$GET && \
	curl -O $$GET
	@mkdir -p $$(dirname $@) && touch $@

build: | $(dep)
	@echo $(FLAGS) | tr " " "\n" > compile_flags.txt
	@#mkdir -p build

src := $(wildcard src/*.c build/deps/*.c)
obj := $(addprefix build/,$(shell basename -a $(src)))
obj := $(obj:%.c=%.o)

build/main: $(obj)
	@gcc $^ -Llib $(LIBS) -o $@

clean:
	rm -rf build compile_flags.txt

