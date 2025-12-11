#include "Gasolina.hpp"

Gasolina::Gasolina(float x, float y, int lane, float speed, sf::Texture& texture, float scaleFactor)
    : mSpeed(speed)
    , mLane(lane)
    , mRecargaAmount(35.0f)
    , mBaseScale(0.04f)
{
    mSprite.setTexture(texture);
    
    // Calcular el origen en el centro del sprite
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
    
    // Aplicar escala
    float finalScale = mBaseScale * scaleFactor;
    mSprite.setScale(finalScale, finalScale);
    
    // Posición inicial
    mSprite.setPosition(x, y);
}

void Gasolina::update(float dt) {
    // Mover hacia abajo
    mSprite.move(0.f, mSpeed * dt);
}

bool Gasolina::isOutOfBounds(float maxY) const {
    return mSprite.getPosition().y > maxY + 100.f;
}

void Gasolina::updateLanePosition(float newX) {
    sf::Vector2f pos = mSprite.getPosition();
    mSprite.setPosition(newX, pos.y);
}

void Gasolina::updateScale(float scaleFactor) {
    float finalScale = mBaseScale * scaleFactor;
    mSprite.setScale(finalScale, finalScale);
}

sf::Sprite& Gasolina::getSprite() {
    return mSprite;
}

const sf::Sprite& Gasolina::getSprite() const {
    return mSprite;
}

int Gasolina::getLane() const {
    return mLane;
}

float Gasolina::getY() const {
    return mSprite.getPosition().y;
}

float Gasolina::getRecargaAmount() const {
    return mRecargaAmount;
}
