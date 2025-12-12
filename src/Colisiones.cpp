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
    
    // Reducir hitbox del jugador para colisiones más justas (70% del tamaño)
    float reductionX = playerBounds.width * 0.15f;
    float reductionY = playerBounds.height * 0.15f;
    playerBounds.left += reductionX;
    playerBounds.top += reductionY;
    playerBounds.width -= reductionX * 2.0f;
    playerBounds.height -= reductionY * 2.0f;

    for (auto it = enemigos.begin(); it != enemigos.end(); ) {
        sf::FloatRect enemyBounds = it->getShape().getGlobalBounds();
        
        // Reducir hitbox del enemigo también (70% del tamaño)
        float enemyReductionX = enemyBounds.width * 0.15f;
        float enemyReductionY = enemyBounds.height * 0.15f;
        enemyBounds.left += enemyReductionX;
        enemyBounds.top += enemyReductionY;
        enemyBounds.width -= enemyReductionX * 2.0f;
        enemyBounds.height -= enemyReductionY * 2.0f;
        
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
