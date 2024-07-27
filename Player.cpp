#include "Player.hpp"
#include "Observer.hpp"
#include <iostream>

// constructor del player
Player::Player(float x, float y, const std::map<PlayerState, std::string>& texturePaths)
    : position(x, y), velocity(0, 0), health(100), currentState(PlayerState::Idle),
    isOnGround(true), jumpVelocity(-500), gravity(1000), attackDuration(0.3f), attackTimer(0), dead(false), deathAnimationTimer(0), maxHealth(100) {
    for (const auto& pair : texturePaths) {
        sf::Texture tex;
        if (!tex.loadFromFile(pair.second)) {
            std::cout << "Error loading texture " << pair.second << std::endl;
        }
        else {
            textures[pair.first] = tex;
        }
    }
    if (!textures.empty()) {
        // configurar el sprite para que se muestre completamente
        sprite.setTexture(textures[PlayerState::Idle]);
        sprite.setTextureRect(sf::IntRect(0, 0, textures[PlayerState::Idle].getSize().x, textures[PlayerState::Idle].getSize().y));
    }
    sprite.setPosition(position);

    // se verifica el tama�o y la posici�n del sprite
    sf::FloatRect bounds = sprite.getGlobalBounds();
    std::cout << "Sprite width: " << bounds.width << ", height: " << bounds.height << std::endl;
    std::cout << "Sprite position x: " << sprite.getPosition().x << ", y: " << sprite.getPosition().y << std::endl;

    // se inicializa los c�rculos de colisi�n
    collisionBox.setSize(sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height));
    collisionBox.setPosition(position);
    collisionBox.setFillColor(sf::Color::Transparent); // para que no se vean los c�rculos, solo para la l�gica de colisi�n

    // hitboxes para da�o en la pelea
    hitboxes.push_back(sprite.getGlobalBounds());
    sprite.setPosition(x, y);
}

void Player::moveLeft() {
    velocity.x = -200;

    if (sprite.getScale().x > 0) {
        sprite.setScale(-1.f, 1.f);
        sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getOrigin().y);
    }

    if (currentState != PlayerState::Jumping && currentState != PlayerState::Falling) {
        changeState(PlayerState::Running);
    }
}

void Player::moveRight() {
    velocity.x = 200;

    if (sprite.getScale().x < 0) {
        sprite.setScale(1.f, 1.f);
        sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getOrigin().y);
    }

    if (currentState != PlayerState::Jumping && currentState != PlayerState::Falling) {
        changeState(PlayerState::Running);
    }
}


void Player::stopMoving() {
    // para detener el movimiento horizontal
    velocity.x = 0;

    // cambia el estado solo si no estamos en salto o ca�da
    if (currentState != PlayerState::Jumping && currentState != PlayerState::Falling) {
        changeState(PlayerState::Idle);
    }
}

// saltar
void Player::jump() {
    if (isOnGround) {
        velocity.y = jumpVelocity;
        isOnGround = false;
        changeState(PlayerState::Jumping);
    }
}

// agacharse
void Player::crouch() {
    if (isOnGround && currentState != PlayerState::Crouching) {
        changeState(PlayerState::Crouching);
    }
}

// levantarse
void Player::standUp() {
    if (currentState == PlayerState::Crouching) {
        changeState(PlayerState::Idle);
    }
}

// pu�ete tranqui
void Player::punchLight() {
    if (currentState != PlayerState::PunchLight) {
        changeState(PlayerState::PunchLight);
        attackTimer = attackDuration;
    }
}

// pu�ete diab�lico
void Player::punchHeavy() {
    if (currentState != PlayerState::PunchHeavy) {
        changeState(PlayerState::PunchHeavy);
        attackTimer = attackDuration;
    }
}

// patada tranqui
void Player::kickLight() {
    if (currentState != PlayerState::KickLight) {
        changeState(PlayerState::KickLight);
        attackTimer = attackDuration;
    }
}

// patada nuclear
void Player::kickHeavy() {
    if (currentState != PlayerState::KickHeavy) {
        changeState(PlayerState::KickHeavy);
        attackTimer = attackDuration;
    }
}

// tomar da�o
void Player::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        die();
    }
    notifyObservers(health, 0); 
}

// actualizar el estado del jugador
void Player::update(float deltaTime) {
    if (dead) {
        updateDeathAnimation(deltaTime);
        return;
    }

    if (!isOnGround) {
        velocity.y += gravity * deltaTime;
    }

    position += velocity * deltaTime;

    // chequeo del suelo
    if (position.y > 500) {  // Ajusta este valor seg�n el nivel del suelo
        position.y = 500;
        velocity.y = 0;
        isOnGround = true;
        if (currentState == PlayerState::Jumping || currentState == PlayerState::Falling) {
            changeState(PlayerState::Idle);
        }
    }

    if (velocity.y > 0 && currentState == PlayerState::Jumping) {
        changeState(PlayerState::Falling);
    }

    if (attackTimer > 0) {
        attackTimer -= deltaTime;
        if (attackTimer <= 0) {
            changeState(PlayerState::Idle);
        }
    }

    sprite.setPosition(position);
    collisionBox.setPosition(position); 
    updateCollisionBoxes();
}

// muerte del jugador
void Player::die() {
    dead = true;
    deathAnimationTimer = 0;  // Reiniciar el temporizador de la animaci�n de muerte
    changeState(PlayerState::Dying);  // Cambiar a un estado relacionado con la muerte
    std::cout << "Player has died." << std::endl;
}

// animaci�n de muerte
void Player::updateDeathAnimation(float deltaTime) {
    deathAnimationTimer += deltaTime;
    if (deathAnimationTimer >= 2.0f) {
        changeState(PlayerState::Dead);
    }
}

// a�adimos el observador
void Player::addObserver(Observer* observer) {
    observers.push_back(observer);
}

// Obtener el estado actual del jugador
PlayerState Player::getCurrentState() const {
    return currentState;
}

// Notificar a los observadores
void Player::notifyObservers(int player1Health, int player2Health) {
    for (Observer* observer : observers) {
        observer->update(player1Health, player2Health);
    }
}

// Verificar si el jugador est� muerto
bool Player::isDead() const {
    return dead;
}

// Obtener el sprite del jugador
sf::Sprite& Player::getSprite() {
    return sprite;
}

// Obtener la posici�n del jugador
sf::Vector2f Player::getPosition() const {
    return position;
}

// Establecer la posici�n del jugador
void Player::setPosition(sf::Vector2f pos) {
    position = pos;
    sprite.setPosition(position);
}

// Obtener la velocidad del jugador
sf::Vector2f Player::getVelocity() const {
    return velocity;
}

// Establecer la velocidad del jugador
void Player::setVelocity(sf::Vector2f vel) {
    velocity = vel;
}

// Obtener el radio del jugador
float Player::getRadius() const {
    return sprite.getGlobalBounds().width / 2;
}

// Obtener la salud del jugador
int Player::getHealth() const {
    return health;
}

// Cambiar el estado del jugador y actualizar la textura
void Player::changeState(PlayerState newState) {
    if (textures.find(newState) != textures.end()) {
        currentState = newState;
        sprite.setTexture(textures[newState]);
        sprite.setTextureRect(sf::IntRect(0, 0, textures[newState].getSize().x, textures[newState].getSize().y));

        // Verificar el tama�o y la posici�n del sprite
        sf::FloatRect bounds = sprite.getGlobalBounds();
        std::cout << "Sprite width: " << bounds.width << ", height: " << bounds.height << std::endl;
        std::cout << "Sprite position x: " << sprite.getPosition().x << ", y: " << sprite.getPosition().y << std::endl;
    }
}

// Obtener los l�mites de colisi�n del jugador
sf::FloatRect Player::getCollisionBounds() const {
    return collisionBox.getGlobalBounds();
}

void Player::updateCollisionBoxes() {
    collisionBoxes.clear();

    sf::RectangleShape newCollisionBox;

    // Aumenta solo el ancho de la caja de colisi�n, manteniendo la altura original
    float width = sprite.getGlobalBounds().width;
    float height = sprite.getGlobalBounds().height;

    // Ajusta el ancho al 120% del ancho original
    newCollisionBox.setSize(sf::Vector2f(width * 1.2f, height)); // 20% m�s ancho

    // Ajustar la posici�n de la caja de colisi�n para coincidir con el sprite
    newCollisionBox.setPosition(sprite.getPosition().x - (width * 0.2f), sprite.getPosition().y); // Ajustar la posici�n para que el centro del nuevo tama�o coincida con el centro del sprite
    newCollisionBox.setFillColor(sf::Color::Transparent); // No visible

    collisionBoxes.push_back(newCollisionBox);
}

// Obtener las hitboxes del jugador
const std::vector<sf::FloatRect>& Player::getHitboxes() const {
    static std::vector<sf::FloatRect> hitboxRects;
    hitboxRects.clear();
    for (const auto& box : collisionBoxes) {
        hitboxRects.push_back(box.getGlobalBounds());
    }
    return hitboxRects;
}

// Obtener las cajas de colisi�n del jugador
const std::vector<sf::RectangleShape>& Player::getCollisionBoxes() const {
    return collisionBoxes;
}

void Player::updateSprite() {
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    collisionBox.setSize(sf::Vector2f(spriteBounds.width, spriteBounds.height));
    collisionBox.setPosition(spriteBounds.left, spriteBounds.top);
}
bool Player::hasWon() const {
    return health <= 0; // O cualquier otra l�gica para determinar si el jugador ha ganado
}