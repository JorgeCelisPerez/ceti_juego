#include "Enemigo.hpp"

Enemy::Enemy(float x, float y, int lane, float speed, float laneLeft, float laneRight, sf::Vector2f baseSize, float scaleFactor)
    : mSpeed(speed), mLane(lane), mLaneLeft(laneLeft), mLaneRight(laneRight)
{
    sf::Vector2f size(baseSize.x * scaleFactor, baseSize.y * scaleFactor);
    mShape.setSize(size);
    mShape.setOrigin(size.x * 0.5f, size.y * 0.5f);
    mShape.setFillColor(sf::Color(80, 80, 255));
    mShape.setPosition(x, y);
}

void Enemy::update(float dt) {
    mShape.move(0.f, mSpeed * dt);
}

bool Enemy::isOutOfBounds(float maxY) const {
    return mShape.getPosition().y > maxY;
}

void Enemy::updateLanePosition(float newX, float newLeft, float newRight) {
    mLaneLeft = newLeft;
    mLaneRight = newRight;
    sf::Vector2f pos = mShape.getPosition();
    mShape.setPosition(newX, pos.y);
}

void Enemy::updateSize(sf::Vector2f baseSize, float scaleFactor) {
    sf::Vector2f newSize(baseSize.x * scaleFactor, baseSize.y * scaleFactor);
    mShape.setSize(newSize);
    mShape.setOrigin(newSize.x * 0.5f, newSize.y * 0.5f);
}

sf::RectangleShape& Enemy::getShape() {
    return mShape;
}

const sf::RectangleShape& Enemy::getShape() const {
    return mShape;
}

int Enemy::getLane() const {
    return mLane;
}

float Enemy::getY() const {
    return mShape.getPosition().y;
}
