# --- Configuración del Proyecto ---
APP_NAME := TrafficRacer
SRC_DIR := src
BIN_DIR := bin
INCLUDE_DIR := include

# --- Configuración del Compilador ---
CXX := g++
CXXFLAGS := -I$(INCLUDE_DIR) -std=c++17 -Wall

# Librerías (Estas son las que tienes instaladas)
LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d

# --- Automagia: Encontrar archivos ---
# Esto busca TODOS los archivos .cpp (main.cpp Y Game.cpp) y los junta
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)

# Nombre final del ejecutable
EXECUTABLE := $(BIN_DIR)/$(APP_NAME).exe

# --- Reglas ---

all: $(EXECUTABLE)

# Esta regla crea la carpeta bin si no existe y compila todo junto
$(EXECUTABLE): $(SRC_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(SRC_FILES) -o $@ $(CXXFLAGS) $(LIBS)

# Regla para ejecutar
run: all
	./$(EXECUTABLE)

clean:
	rm -f $(BIN_DIR)/*.exe

.PHONY: all run clean