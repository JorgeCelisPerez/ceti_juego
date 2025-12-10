#include <SFML/Graphics.hpp>
#include <cstdlib> // Es para números aleatorios (rand)
#include <ctime>   // Es para inicializar lo aleatoria
#include <iostream>

using namespace sf;
using namespace std;

int main()
{
                        // 1. CONFIGURACIÓN (SETUP)
    //Es para la ventana
    RenderWindow window(VideoMode(800, 600), "Traffic Racer - Version 0.1");
    //Limitamos a 60 FPS para que no vaya muy rápido
    window.setFramerateLimit(60);
    
                //(TU COCHE)
    RectangleShape coche(Vector2f(50.0f, 100.0f));
    coche.setFillColor(Color::Red);
         // Posición inicial. Que lo la mitad de abajo y centrado es X= 375, Y= 450
    coche.setPosition(375.0f, 450.0f);
    float velocidad = 4.0f; //Estaba en 0.5

        //EL ENEMIGO (AZUL)
    RectangleShape enemigo(Vector2f(50.0f, 100.0f));  //Cambiar los valores de para cambiar el tamaño
    enemigo.setFillColor(sf::Color::Blue);
    enemigo.setPosition(100.0f, -150.0f);  //Arriba de la pantalla
    float velocidadEnemigo = 3.0f;

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

 // |||||||||||||||||||||||||||| Movimiento del jugador (INPUT) ||||||||||||

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
 // |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

 // ****************************** LÍMITES DE PANTALLA *******************************
        Vector2f pos = coche.getPosition(); //Es la posicion del carro (x, y)
                //Límite Izquierdo (X < 0)
        if (pos.x < 0) {
            coche.setPosition(0, pos.y); 
        }
                //Límite Derecho (X > 800 - 50 ancho del coche)
        if (pos.x > 750) {
            coche.setPosition(750, pos.y); // 800 (pantalla) - 50 (coche) = 750
        }
                // 3. Límite Superior (Y < 0)
        if (pos.y < 0) {
            coche.setPosition(pos.x, 0);
        }
                // 4. Límite Inferior (Y > 600 - 100 alto del coche)
        if (pos.y > 500) {
            coche.setPosition(pos.x, 500); // 600 (pantalla) - 100 (coche) = 500
        }
// ***************************************************************************************

// ||||||||||||||||||||||||||||| Enemigo ||||||||||||||||||||||||||||||||||||||||||||||||||
        // 1. Mover hacia abajo
        enemigo.move(0.0f, velocidadEnemigo);
        // 2. ¿Salió de la pantalla por abajo? (Reset)
        if (enemigo.getPosition().y > 600) {
            // Regresa arriba fuera de pantalla
            float xAleatoria = rand() % 750; // Número entre 0 y 750
            enemigo.setPosition(xAleatoria, -150.0f);
            
            // Opcional: Cada vez que reaparece, cambia de velocidad ligeramente
            velocidadEnemigo = 4.0f + (rand() % 5); 
        }
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

// *************************** DETECCIÓN DE COLISIONES (GAME OVER) *************************
            // Se hace una "cajas invisibles" del coche y del enemigo
        FloatRect cajaJugador = coche.getGlobalBounds();
        FloatRect cajaEnemigo = enemigo.getGlobalBounds();

        if (cajaJugador.intersects(cajaEnemigo)) 
        {
            window.close();
        }
// *********************************************************************************************
        // --- D. DIBUJAR ---
        window.clear(Color::Black);
        window.draw(coche);
        window.draw(enemigo);
        window.display();
    }

    return 0;
}