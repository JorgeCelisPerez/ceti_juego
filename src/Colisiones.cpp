#include "Colisiones.hpp"

void ColisionManager::checkGasolinaCollisions(
    const sf::Sprite& player,
    std::vector<Gasolina>& gasolinas,
    float& gasolinaActual,
    float gasolinaMax,
    SoundManager& soundManager
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
            
            // Reproducir sonido de item recogido
            soundManager.playItemSound();
            
            // Eliminar el item recolectado
            it = gasolinas.erase(it);
        } else {
            ++it;
        }
    }
}

void ColisionManager::checkEnemyCollisions(
    const sf::Sprite& player,
    std::vector<Enemy>& enemigos,
    float& gasolinaActual,
    float collisionPenalty,
    SoundManager& soundManager
) {
    sf::FloatRect playerBounds = player.getGlobalBounds();
    
    // Hitbox del jugador al 100% (tamaño exacto del sprite)
    // Sin expansión

    for (auto it = enemigos.begin(); it != enemigos.end(); ) {
        sf::FloatRect enemyBounds = it->getSprite().getGlobalBounds();
        
        // Hitbox del enemigo al 100% (tamaño exacto del sprite)
        // Sin expansión
        
        if (playerBounds.intersects(enemyBounds)) {
            // Aplicar penalización de gasolina
            gasolinaActual -= collisionPenalty;
            if (gasolinaActual < 0.0f) {
                gasolinaActual = 0.0f;
            }
            
            // Reproducir sonido de crash
            soundManager.playCrashSound();
            
            // Eliminar el enemigo colisionado para evitar penalizaciones múltiples
            it = enemigos.erase(it);
        } else {
            ++it;
        }
    }
}
