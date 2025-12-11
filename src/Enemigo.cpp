#include "Enemigo.hpp"

Enemy::Enemy(float x, float y, int lane, float speed, float laneLeft, float laneRight)
    : mSpeed(speed), mLane(lane), mLaneLeft(laneLeft), mLaneRight(laneRight)
{
    mShape.setSize(sf::Vector2f(80.f, 140.f));
    sf::Vector2f size = mShape.getSize();
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
