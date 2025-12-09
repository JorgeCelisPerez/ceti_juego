#include <SFML/Graphics.hpp>

int main()
{
    // 1. CONFIGURACIÓN (SETUP)
    // Creamos una ventana más grande (800 ancho x 600 alto)
    sf::RenderWindow window(sf::VideoMode(800, 600), "Traffic Racer - Version 0.1");
    
    // -- EL JUGADOR (TU COCHE) --
    // Creamos un rectángulo de 50px de ancho por 100px de alto
    sf::RectangleShape coche(sf::Vector2f(50.0f, 100.0f));
    
    // Le ponemos color rojo
    coche.setFillColor(sf::Color::Red);
    
    // Lo ponemos en la posición inicial (Centro abajo)
    // X = 375 (aprox la mitad), Y = 450 (parte baja)
    coche.setPosition(375.0f, 450.0f);

    // Variable de velocidad (cuántos pixeles se mueve por frame)
    float velocidad = 0.5f;

   // 2. BUCLE DEL JUEGO (GAME LOOP)
    while (window.isOpen())
    {
        // --- A. PROCESAR EVENTOS ---
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- B. MOVIMIENTO (INPUT) ---
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            coche.move(-velocidad, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            coche.move(velocidad, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            coche.move(0.0f, -velocidad);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            coche.move(0.0f, velocidad);
        }

        // --- C. LÍMITES DE PANTALLA (NUEVO CÓDIGO) ---
        // Obtenemos la posición actual del coche (x, y)
        sf::Vector2f pos = coche.getPosition();

        // 1. Límite Izquierdo (X < 0)
        if (pos.x < 0) {
            coche.setPosition(0, pos.y);
        }

        // 2. Límite Derecho (X > 800 - 50 ancho del coche)
        // 800 (pantalla) - 50 (coche) = 750
        if (pos.x > 750) {
            coche.setPosition(750, pos.y);
        }

        // 3. Límite Superior (Y < 0)
        if (pos.y < 0) {
            coche.setPosition(pos.x, 0);
        }

        // 4. Límite Inferior (Y > 600 - 100 alto del coche)
        // 600 (pantalla) - 100 (coche) = 500
        if (pos.y > 500) {
            coche.setPosition(pos.x, 500);
        }

        // --- D. DIBUJAR ---
        window.clear(sf::Color::Black);
        window.draw(coche);
        window.display();
    }

    return 0;
}