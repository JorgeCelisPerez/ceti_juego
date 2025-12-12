#include "Enemigo.hpp"

Enemy::Enemy(float x, float y, int lane, float speed, float laneLeft, float laneRight, sf::Vector2f baseSize, float scaleFactor, const sf::Texture& texture, bool rotateSprite, bool isTruck)
    : mSpeed(speed), mLane(lane), mLaneLeft(laneLeft), mLaneRight(laneRight), mBaseSize(baseSize), mIsTruck(isTruck)
{
    mSprite.setTexture(texture);
    
    // Calcular escala para que coincida con baseSize
    sf::FloatRect textureBounds = mSprite.getLocalBounds();
    float scaleX = (baseSize.x * scaleFactor) / textureBounds.width;
    float scaleY = (baseSize.y * scaleFactor) / textureBounds.height;
    mSprite.setScale(scaleX, scaleY);
    
    // Establecer origen al centro
    mSprite.setOrigin(textureBounds.width * 0.5f, textureBounds.height * 0.5f);
    mSprite.setPosition(x, y);
    
    // Rotar 180 grados si es necesario (carriles izquierdos)
    if (rotateSprite) {
        mSprite.setRotation(180.f);
    }
}

void Enemy::update(float dt) {
    mSprite.move(0.f, mSpeed * dt);
}

bool Enemy::isOutOfBounds(float maxY) const {
    return mSprite.getPosition().y > maxY;
}

void Enemy::updateLanePosition(float newX, float newLeft, float newRight) {
    mLaneLeft = newLeft;
    mLaneRight = newRight;
    sf::Vector2f pos = mSprite.getPosition();
    mSprite.setPosition(newX, pos.y);
}

void Enemy::updateSize(sf::Vector2f baseSize, float scaleFactor) {
    // Usar el tamaño base guardado si no se proporciona uno nuevo
    sf::Vector2f sizeToUse = mBaseSize;
    
    sf::FloatRect textureBounds = mSprite.getLocalBounds();
    float scaleX = (sizeToUse.x * scaleFactor) / textureBounds.width;
    float scaleY = (sizeToUse.y * scaleFactor) / textureBounds.height;
    mSprite.setScale(scaleX, scaleY);
}

sf::Sprite& Enemy::getSprite() {
    return mSprite;
}

const sf::Sprite& Enemy::getSprite() const {
    return mSprite;
}

int Enemy::getLane() const {
    return mLane;
}

float Enemy::getY() const {
    return mSprite.getPosition().y;
}

bool Enemy::isTruck() const {
    return mIsTruck;
}
