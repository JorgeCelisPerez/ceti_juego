#include <SFML/Graphics.hpp>
#include <cstdlib> // Es para números aleatorios (rand)
#include <ctime>   // Es para inicializar lo aleatoria

using namespace sf;

int main()
{
                        // 1. CONFIGURACIÓN (SETUP)
    //Es para la ventana
    RenderWindow window(VideoMode(800, 600), "Traffic Racer - Version 0.1");
    window.setFramerateLimit(800); //Limitamos a 60 FPS para que no vaya muy rápido
    
                //(TU COCHE)
    RectangleShape coche(Vector2f(50.0f, 100.0f));
    coche.setFillColor(Color::Red);
         // Posición inicial. Que lo la mitad de abajo y centrado es X= 375, Y= 450
    coche.setPosition(375.0f, 450.0f);
        // Variable de velocidad (cuántos pixeles se mueve por frame)
    float velocidad = 0.5f;

   // 2. BUCLE DEL JUEGO (GAME LOOP)
    while (window.isOpen())
    {
        // --- A. PROCESAR EVENTOS ---
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        // --- B. MOVIMIENTO (INPUT) ---
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            coche.move(-velocidad, 0.0f);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            coche.move(velocidad, 0.0f);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            coche.move(0.0f, -velocidad);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            coche.move(0.0f, velocidad);
        }

        // --- C. LÍMITES DE PANTALLA (NUEVO CÓDIGO) ---
        // Obtenemos la posición actual del coche (x, y)
        Vector2f pos = coche.getPosition();

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
        window.clear(Color::Black);
        window.draw(coche);
        window.display();
    }

    return 0;
}