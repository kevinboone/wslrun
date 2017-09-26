CC      := gcc
CFLAGS  := -Wall
LIBS    := 
PREFIX  := /usr

TARGET	:= wslrun
SOURCES := $(shell find src/ -type f -name *.c)
OBJECTS := $(patsubst src/%,build/%,$(SOURCES:.c=.o))
DEPS	:= $(OBJECTS:.o=.deps)

$(TARGET): $(OBJECTS)
	$(CC) -s $^ -o $(TARGET) $(LIBS)

build/%.o: src/%.c
	mkdir -p build/
	$(CC) $(CFLAGS) -MD -MF $(@:.o=.deps) -c -o $@ $<

clean:
	rm -r build/ $(TARGET)

install: $(TARGET)
	cp -p $(TARGET) ${DESTDIR}${PREFIX}/bin/

install-examples: $(TARGET)
	cp -pr examples/* ${DESTDIR}${PREFIX}/bin/

-include $(DEPS)

.PHONY: clean
