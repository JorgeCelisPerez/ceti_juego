#include "Colisiones.hpp"

void ColisionManager::checkGasolinaCollisions(
    const sf::RectangleShape& player,
    std::vector<Gasolina>& gasolinas,
    float& gasolinaActual,
    float gasolinaMax
) {
    sf::FloatRect playerBounds = player.getGlobalBounds();
    
    for (auto it = gasolinas.begin(); it != gasolinas.end(); ) {
        sf::FloatRect gasolinaBounds = it->getSprite().getGlobalBounds();
        
        if (playerBounds.intersects(gasolinaBounds)) {
            // Recargar gasolina
            gasolinaActual += it->getRecargaAmount();
            if (gasolinaActual > gasolinaMax) {
                gasolinaActual = gasolinaMax;
            }
            
            // Eliminar el item recolectado
            it = gasolinas.erase(it);
        } else {
            ++it;
        }
    }
}

void ColisionManager::checkEnemyCollisions(
    const sf::RectangleShape& player,
    std::vector<Enemy>& enemigos,
    float& gasolinaActual,
    float collisionPenalty
) {
    sf::FloatRect playerBounds = player.getGlobalBounds();

    for (auto it = enemigos.begin(); it != enemigos.end(); ) {
        sf::FloatRect enemyBounds = it->getShape().getGlobalBounds();
        
        if (playerBounds.intersects(enemyBounds)) {
            // Aplicar penalización de gasolina
            gasolinaActual -= collisionPenalty;
            if (gasolinaActual < 0.0f) {
                gasolinaActual = 0.0f;
            }
            
            // Eliminar el enemigo colisionado para evitar penalizaciones múltiples
            it = enemigos.erase(it);
        } else {
            ++it;
        }
    }
}
