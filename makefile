SOURCES := $(shell find include/angelscript/include include/angelscript/source WoopWoop include/box2d src include/imgui -type f -name '*.cpp')
OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))

INCLUDE := -Iinclude/ -Isrc -IWoopWoop -Iinclude/angelscript/include -Iinclude/angelscript/source
CPP_FLAGS := -std=c++20 -fsanitize=address -fno-omit-frame-pointer
LIBRARIES := -lyaml-cpp -lsfml-graphics -lsfml-window -lsfml-system -lGL -lbox2d
LIB_DIR := -Llib/
MODE_FLAGS := 

TOTAL_FILES := $(words $(SOURCES))
COMPILED_FILES := 0

define show_progress
    $(eval COMPILED_FILES=$(shell echo $$(($(COMPILED_FILES)+1))))
    @echo "Progress: $(shell echo $$(($(COMPILED_FILES) * 100 / $(TOTAL_FILES))))% ($(COMPILED_FILES)/$(TOTAL_FILES))\r"
endef

all: build/main

# Create build directory if it doesn't exist
build:
	mkdir -p build

# Compile each .cpp file to .o
%.o: %.cpp
	$(call show_progress)
	g++ -c $< -o $@ $(INCLUDE) $(CPP_FLAGS) $(MODE_FLAGS)

# Link all .o files to create the main executable inside the build directory
build/main: $(OBJECTS) | build
	g++ $^ -o $@ $(LIB_DIR) $(LIBRARIES) $(CPP_FLAGS) $(MODE_FLAGS)
	@echo '\n'

debug: MODE_FLAGS := -DDEBUG -g
debug: build/main

clean:
	rm -f $(OBJECTS) build/main
