#include "Difficulty.hpp"
#include <algorithm>
#include <cmath>

DifficultyManager::DifficultyManager()
    : mLevel(0),
      mScoreThreshold(60),
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
    // Incremento moderado: sqrt(nivel) * 0.40
    mEnemySpeedMultiplier = std::min(1.0f + (scoreProgress * 0.40f), 2.0f);
    
    // Spawn rate: aumenta gradualmente hasta 2.2x más rápido
    // Disminuye el intervalo moderadamente: sqrt(nivel) * 0.24
    mSpawnRateMultiplier = std::max(1.0f - (scoreProgress * 0.24f), 0.40f);
    
    // Late Game (1000+ puntos): Incrementos adicionales progresivos
    if (mLevel >= 17) {
        float lateGameProgress = std::sqrt(static_cast<float>(mLevel - 16));
        
        // Velocidad: boost adicional suave hasta 2.5x
        mEnemySpeedMultiplier += lateGameProgress * 0.15f;
        mEnemySpeedMultiplier = std::min(mEnemySpeedMultiplier, 2.5f);
        
        // Spawn: aparecen más rápido aún, hasta 0.30x
        mSpawnRateMultiplier -= lateGameProgress * 0.06f;
        mSpawnRateMultiplier = std::max(mSpawnRateMultiplier, 0.30f);
    }
    
    // Consumo de gasolina: aumenta progresivamente después del nivel 25 (~1500 puntos)
    if (mLevel >= 25) {
        mFuelConsumptionMultiplier = 1.0f + (scoreProgress - 5.0f) * 0.16f;
        mFuelConsumptionMultiplier = std::min(mFuelConsumptionMultiplier, 2.0f);
    } else {
        mFuelConsumptionMultiplier = 1.0f;
    }
}
