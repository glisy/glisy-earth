TARGET ?= program
OS ?= $(shell uname)

GLSLIFY ?= node_modules/.bin/glslify
GLSL ?= $(wildcard glsl/*.glsl)
GLSL_COMPILELD ?= $(GLSL:.glsl=.glsl.compiled)

SRC += $(wildcard deps/*/*.c)
SRC += $(wildcard src/*.c)

CFLAGS += -I deps
CFLAGS += -l glfw3

ifeq (Darwin, $(OS))
CFLAGS += -framework Foundation
CFLAGS += -framework OpenGL
endif

all: $(GLSL_COMPILELD) $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@

$(GLSL_COMPILELD): $(GLSL)
	$(GLSLIFY) $(@:.compiled=) -o $@

$(GLSL): node_modules/

$(SRC): deps/

deps/:
	clib install

node_modules/:
	npm i glslify
	npm i glsl-earth glsl-fog glsl-atmosphere

clean:
	rm -f $(TARGET)
	rm -f $(GLSL_COMPILELD)

