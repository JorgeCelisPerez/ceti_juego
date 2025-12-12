#pragma once
#include <SFML/Window/Keyboard.hpp>

// Devuelve -1, 0 o 1 según el input horizontal (flechas o A/D)
float getHorizontalInput(bool debugMode = false);
