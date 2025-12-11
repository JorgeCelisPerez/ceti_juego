// Lee el teclado y devuelve -1, 0 o 1 para movimiento horizontal.
#include "Teclado.hpp"

float getHorizontalInput() {
	bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);

	if (left && !right) return -1.f;
	if (right && !left) return 1.f;
	return 0.f;
}
