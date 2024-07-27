#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <string>
#include "Observer.hpp"

enum class PlayerState {
    Idle, Running, Jumping, Falling, Crouching, PunchLight, PunchHeavy, KickLight, KickHeavy, Dying, Dead
};

class Player {
public:
    Player(float x, float y, const std::map<PlayerState, std::string>& texturePaths);

    void moveLeft();
    void moveRight();
    void stopMoving();
    void jump();
    void crouch();
    void standUp();
    void punchLight();
    void punchHeavy();
    void kickLight();
    void kickHeavy();
    void takeDamage(int amount);
    void update(float deltaTime);
    void die();
    void updateDeathAnimation(float deltaTime);
    void addObserver(Observer* observer);
    PlayerState getCurrentState() const;
    void notifyObservers(int player1Health, int player2Health);
    bool isDead() const;
    sf::Sprite& getSprite();
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getVelocity() const;
    void setVelocity(sf::Vector2f vel);
    float getRadius() const;
    int getHealth() const;
    sf::FloatRect getCollisionBounds() const;
    const std::vector<sf::FloatRect>& getHitboxes() const;
    void updateCollisionBoxes();
    const std::vector<sf::RectangleShape>& getCollisionBoxes() const;
    bool hasWon() const;



private:
    void addCollisionBox(const sf::RectangleShape& box);
    void changeState(PlayerState newState);
    void updateSprite();

    sf::Vector2f position;
    sf::Vector2f velocity;
    int health;
    int maxHealth; // Añadido para manejar la salud máxima
    PlayerState currentState;
    bool isOnGround;
    float jumpVelocity;
    float gravity;
    float attackDuration;
    float attackTimer;
    bool dead;
    float deathAnimationTimer;
    sf::Sprite sprite;
    std::map<PlayerState, sf::Texture> textures;
    sf::RectangleShape collisionBox; // Se puede eliminar si no se usa
    std::vector<Observer*> observers;
    std::vector<sf::RectangleShape> collisionBoxes;
    std::vector<sf::FloatRect> hitboxes;

    
};

#endif // PLAYER_HPP
