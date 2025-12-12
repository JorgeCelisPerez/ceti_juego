#include "Difficulty.hpp"
#include <algorithm>
#include <cmath>

DifficultyManager::DifficultyManager()
    : mLevel(0),
      mScoreThreshold(80),
      mEnemySpeedMultiplier(1.0f),
      mSpawnRateMultiplier(1.0f),
      mFuelConsumptionMultiplier(1.0f)
{
}

void DifficultyManager::updateFromScore(int currentScore) {
    // Calcular nivel basado en el score (cada 100 puntos = 1 nivel)
    int newLevel = currentScore / mScoreThreshold;
    
    if (newLevel != mLevel) {
        mLevel = newLevel;
    }
    
    // Calcular multiplicadores de forma CONTINUA basados en el score total
    // Similar al T-Rex de Google: gradual y apenas perceptible al principio
    calculateMultipliers();
}

void DifficultyManager::calculateMultipliers() {
    // Fórmula gradual: usa raíz cuadrada para que sea más lento al principio
    // y más notable después
    float scoreProgress = std::sqrt(static_cast<float>(mLevel));
    
    // Velocidad de enemigos: aumenta gradualmente hasta 2.0x
    // Incremento moderado: sqrt(nivel) * 0.32
    mEnemySpeedMultiplier = std::min(1.0f + (scoreProgress * 0.32f), 2.0f);
    
    // Spawn rate: aumenta gradualmente hasta 2.2x más rápido
    // Disminuye el intervalo moderadamente: sqrt(nivel) * 0.19
    mSpawnRateMultiplier = std::max(1.0f - (scoreProgress * 0.19f), 0.45f);
    
    // Consumo de gasolina: NO se modifica
    mFuelConsumptionMultiplier = 1.0f;
}
