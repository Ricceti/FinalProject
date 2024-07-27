#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "HealthDisplay.hpp"
#include <map>
#include <string>
#include <cmath>
#include <iostream>
#include "Game.hpp"
#include "MainMenuState.hpp"
#include "PlayState.hpp"
#include "PauseState.hpp"


const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 900;
const int GAME_WIDTH = 1280;
const int GAME_HEIGHT = 720;

bool detectarColision(const Player& player1, const Player& player2);
void handleCollision(Player& player1, Player& player2);
void handleCombat(Player& attacker, Player& defender);

// Método para inicializar los recursos y configuraciones
void initializeGame(Player& player1, Player& player2) {
    std::map<PlayerState, std::string> texturePaths1 = {
        {PlayerState::Idle, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\kano\\left\\stand\\0.png"},
    };

    std::map<PlayerState, std::string> texturePaths2 = {
        {PlayerState::Idle, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\subzero\\left\\stand-up\\2.png"},
    };

    player1 = Player(100, 100, texturePaths1);
    player2 = Player(300, 100, texturePaths2);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Game");
    window.setFramerateLimit(60);

    sf::View gameView(sf::FloatRect(0, 0, GAME_WIDTH, GAME_HEIGHT));
    gameView.setViewport(sf::FloatRect(
        (WINDOW_WIDTH - GAME_WIDTH) / 2.0f / WINDOW_WIDTH,
        (WINDOW_HEIGHT - GAME_HEIGHT) / 2.0f / WINDOW_HEIGHT,
        GAME_WIDTH / (float)WINDOW_WIDTH,
        GAME_HEIGHT / (float)WINDOW_HEIGHT
    ));

    std::map<PlayerState, std::string> player1Textures = {
        {PlayerState::Idle, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\subzero\\left\\stand-up\\2.png"},
        {PlayerState::Running, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\subzero\\left\\walking\\4.png"},
        {PlayerState::Jumping, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\subzero\\left\\jumping\\5.png"},
        {PlayerState::Crouching, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\subzero\\left\\squating\\2.png"},
        {PlayerState::PunchLight, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\subzero\\left\\high-punch\\5.png"},
        {PlayerState::PunchHeavy, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\subzero\\left\\high-punch\\2.png"},
        {PlayerState::KickLight, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\subzero\\left\\low-kick\\5.png"},
        {PlayerState::KickHeavy, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\subzero\\left\\spin-kick\\4.png"},
        {PlayerState::Dying, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\subzero\\left\\fall\\4.png"},
        {PlayerState::Dead, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\subzero\\left\\fall\\6.png"}
    };

    std::map<PlayerState, std::string> player2Textures = {
        
        {PlayerState::Idle, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\kano\\left\\stand\\0.png" },
        {PlayerState::Running, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\kano\\left\\walking\\4.png"},
        {PlayerState::Jumping, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\kano\\left\\jumping\\4.png"},
        {PlayerState::Crouching, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\kano\\left\\squating\\2.png"},
        {PlayerState::PunchLight, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\kano\\left\\high-punch\\5.png"},
        {PlayerState::PunchHeavy, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\kano\\left\\high-punch\\2.png"},
        {PlayerState::KickLight, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\kano\\left\\low-kick\\5.png"},
        {PlayerState::KickHeavy, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\kano\\left\\spin-kick\\3.png"},
        {PlayerState::Dying, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\kano\\right\\spin-kick\\4.png"},
        {PlayerState::Dead, "C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\fighters\\kano\\right\\spin-kick\\6.png"}
    };

    Player player1(200, GAME_HEIGHT - 100, player1Textures);
    Player player2(GAME_WIDTH - 200, GAME_HEIGHT - 100, player2Textures);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\arenas\\1\\arena.png")) {
        std::cout << "Error loading background texture" << std::endl;
    }
    sf::Sprite background(backgroundTexture);
    background.setScale(
        GAME_WIDTH / background.getLocalBounds().width,
        GAME_HEIGHT / background.getLocalBounds().height
    );
    // Carga la imagen de pausa
    sf::Texture pauseTexture;
    if (!pauseTexture.loadFromFile("C:\\Users\\LENOVO\\Documents\\Mortal Kombat\\resources\\images\\arenas\\win\\3.png")) {
        std::cout << "Error loading pause texture" << std::endl;
    }
    sf::Sprite pauseSprite(pauseTexture);
    pauseSprite.setPosition((WINDOW_WIDTH - pauseSprite.getGlobalBounds().width) / 2,
        (WINDOW_HEIGHT - pauseSprite.getGlobalBounds().height) / 2);

    HealthDisplay healthDisplay;
    player1.addObserver(&healthDisplay);
    player2.addObserver(&healthDisplay);

    sf::Clock clock;
    sf::Time lastHitTime;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        // Controles del jugador 1 (WASD)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            player1.moveLeft();
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            player1.moveRight();
        else
            player1.stopMoving();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            player1.jump();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            player1.crouch();
        else
            player1.standUp();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            player1.punchLight();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
            player1.punchHeavy();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
            player1.kickLight();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
            player1.kickHeavy();

        // Controles del jugador 2 (flechas)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            player2.moveLeft();
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            player2.moveRight();
        else
            player2.stopMoving();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            player2.jump();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            player2.crouch();
        else
            player2.standUp();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
            player2.punchLight();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
            player2.punchHeavy();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            player2.kickLight();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            player2.kickHeavy();

        // Simulación de daño
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            player2.takeDamage(1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
            player1.takeDamage(1);

        player1.update(deltaTime);
        player2.update(deltaTime);

        if (!player1.isDead() && !player2.isDead()) {
            handleCollision(player1, player2);
            handleCombat(player1, player2);
            handleCombat(player2, player1);
        }

        // cambio en la salud
        player2.notifyObservers(player2.getHealth(), player1.getHealth());
        player1.notifyObservers(player1.getHealth(), player2.getHealth());

        if (player1.isDead() || player2.isDead()) {
            if (player1.isDead()) {
                std::cout << "Kano 2 wins!" << std::endl;
            }
            else {
                std::cout << "Zub-zero 1 wins!" << std::endl;
            }
            sf::sleep(sf::seconds(3));
            window.close();
        }

        window.clear(sf::Color::Black);

        window.setView(gameView);
        window.draw(background);
        window.draw(player1.getSprite());
        window.draw(player2.getSprite());

        // Dibuja los cuadros de colisión para depuración
        for (const auto& box : player1.getCollisionBoxes()) {
            window.draw(box);
        }
        for (const auto& box : player2.getCollisionBoxes()) {
            window.draw(box);
        }

        window.setView(window.getDefaultView());
        healthDisplay.draw(window);

        window.display();
    }

    return 0;
}

void handleCollision(Player& player1, Player& player2) {
    if (detectarColision(player1, player2)) {
        sf::Vector2f pos1 = player1.getPosition();
        sf::Vector2f pos2 = player2.getPosition();
        float radius1 = player1.getRadius();
        float radius2 = player2.getRadius();

        float distance = std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2));
        float overlap = radius1 + radius2 - distance;

        if (overlap > 0) {
            sf::Vector2f direction = (pos2 - pos1) / distance;
            sf::Vector2f correction = direction * (overlap / 2.0f);

            player1.setPosition(pos1 - correction);
            player2.setPosition(pos2 + correction);

            player1.setVelocity(sf::Vector2f(0, player1.getVelocity().y));
            player2.setVelocity(sf::Vector2f(0, player2.getVelocity().y));
        }
    }
}

void handleCombat(Player& attacker, Player& defender) {
    static sf::Clock hitCooldown;

    if (detectarColision(attacker, defender) && hitCooldown.getElapsedTime().asSeconds() > 0.5f) {
        PlayerState attackerState = attacker.getCurrentState();
        int damage = 0;

        switch (attackerState) {
        case PlayerState::PunchLight:
            damage = 5;
            break;
        case PlayerState::PunchHeavy:
            damage = 10;
            break;
        case PlayerState::KickLight:
            damage = 7;
            break;
        case PlayerState::KickHeavy:
            damage = 15;
            break;
        default:
            break;
        }

        if (damage > 0) {
            defender.takeDamage(damage);
            hitCooldown.restart();

            sf::Vector2f knockback(50.0f, 0);
            if (attacker.getPosition().x > defender.getPosition().x) {
                knockback.x = -knockback.x;
            }
            defender.setVelocity(defender.getVelocity() + knockback);
        }
    }
}

bool detectarColision(const Player& player1, const Player& player2) {
    for (const auto& box1 : player1.getCollisionBoxes()) {
        for (const auto& box2 : player2.getCollisionBoxes()) {
            if (box1.getGlobalBounds().intersects(box2.getGlobalBounds())) {
                return true;
            }
        }
    }
    return false;
}
