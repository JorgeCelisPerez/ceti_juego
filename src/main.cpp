#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window{ sf::VideoMode{800, 600}, "Ventana Negra - SFML 3.0.2" };

    while (window.isOpen()) {
        // No usamos pollEvent(), así no aparece std::optional
        // La ventana solo puede cerrarse con ALT+F4 o el botón X (SO se encarga)
        
        window.clear(sf::Color::Black);
        window.display();
    }
    return 0;
}