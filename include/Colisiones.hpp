#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Enemigo.hpp"
#include "Gasolina.hpp"

class ColisionManager {
public:
    // Comprobar colisiones del jugador con items de gasolina
    static void checkGasolinaCollisions(
        const sf::Sprite& player,
        std::vector<Gasolina>& gasolinas,
        float& gasolinaActual,
        float gasolinaMax
    );

    // Comprobar colisiones del jugador con enemigos
    static void checkEnemyCollisions(
        const sf::Sprite& player,
        std::vector<Enemy>& enemigos,
        float& gasolinaActual,
        float collisionPenalty = 25.0f
    );
};
