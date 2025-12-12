#ifndef DIFFICULTY_HPP
#define DIFFICULTY_HPP

class DifficultyManager {
public:
    DifficultyManager();
    
    void updateFromScore(int currentScore);
    
    // Getters para multiplicadores
    int getLevel() const { return mLevel; }
    float getEnemySpeedMultiplier() const { return mEnemySpeedMultiplier; }
    float getSpawnRateMultiplier() const { return mSpawnRateMultiplier; }
    float getFuelConsumptionMultiplier() const { return mFuelConsumptionMultiplier; }
    
private:
    int mLevel;
    int mScoreThreshold;  // Cada cuántos puntos sube la dificultad
    
    float mEnemySpeedMultiplier;
    float mSpawnRateMultiplier;
    float mFuelConsumptionMultiplier;
    
    void calculateMultipliers();
};

#endif
