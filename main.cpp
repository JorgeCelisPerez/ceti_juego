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
        // --- A. PROCESAR EVENTOS (INPUT) ---
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Si le dan a la X de la ventana, cerrar
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- B. LÓGICA / MOVIMIENTO (UPDATE) ---
        // Aquí preguntamos al teclado directamente
        
        // Mover a la Izquierda
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            coche.move(-velocidad, 0.0f); // Restamos a X
        }
        
        // Mover a la Derecha
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            coche.move(velocidad, 0.0f); // Sumamos a X
        }
        
        // Mover Arriba (Opcional, por si quieres esquivar avanzando)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            coche.move(0.0f, -velocidad); // Restamos a Y para subir
        }

        // Mover Abajo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            coche.move(0.0f, velocidad); // Sumamos a Y para bajar
        }

        // --- C. DIBUJAR (RENDER) ---
        window.clear(sf::Color::Black); // Limpiamos con fondo negro (asfalto)
        window.draw(coche);             // Dibujamos el coche
        window.display();               // Mostramos en pantalla
    }

    return 0;
}