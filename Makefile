LIBS := -lm
DEPS := \
https://raw.githubusercontent.com/seha-bot/packages/main/nec/nec.c \
https://raw.githubusercontent.com/seha-bot/packages/main/nec/nec.h

all: build build/main

build:
	@echo -Iinc > compile_flags.txt && echo -Ibuild/deps >> compile_flags.txt
	@mkdir -p build/deps && cd build/deps && curl -s $(addprefix -O ,$(DEPS))

%.o: %.c
	@gcc -c -Iinc -Ibuild/deps $^ -o build/$(notdir $@)

build/main: src/*.c
	@make $(patsubst %.c,%.o,$(wildcard src/*.c) $(wildcard build/deps/*.c))
	@gcc build/*.o $(LIBS) -o $@

clean:
	rm -rf build compile_flags.txt

