// Lee el teclado y devuelve -1, 0 o 1 para movimiento horizontal.
#include "Teclado.hpp"

float getHorizontalInput(bool debugMode) {
	// En modo debug, solo usar flechas, no WASD
	bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	
	if (!debugMode) {
		// Si NO estamos en debug, permitir A/D también
		left = left || sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		right = right || sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	}

	if (left && !right) return -1.f;
	if (right && !left) return 1.f;
	return 0.f;
}
