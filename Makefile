CXX ?= clang++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2

# Detect SFML via pkg-config or Homebrew
PKG_CONFIG_SFML := $(shell pkg-config --cflags --libs sfml-graphics sfml-window sfml-system 2>/dev/null)

ifneq ($(PKG_CONFIG_SFML),)
    SFML_FLAGS := $(PKG_CONFIG_SFML)
else
    SFML_PREFIX := $(shell brew --prefix sfml@2 2>/dev/null || brew --prefix sfml 2>/dev/null)
    ifneq ($(SFML_PREFIX),)
        SFML_FLAGS := -I$(SFML_PREFIX)/include -L$(SFML_PREFIX)/lib -lsfml-graphics -lsfml-window -lsfml-system
    else
        SFML_FLAGS := -lsfml-graphics -lsfml-window -lsfml-system
    endif
endif

TARGET = raycaster
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(SFML_FLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

