#include "StateAction.h"
#include "Player.h"
#include "Ball.h"
#include <cmath>

float calculateDistance(float x1, float y1, float x2, float y2) {
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}


void PossessionState::start(Player& player, Ball& ball) {
    std::cout << "Le joueur prend possession de la balle." << std::endl;
}

void PossessionState::update(Player& player, Ball& ball, float deltaTime) {
    float goalX = 100.0f;
    float goalY = player.y;

    float dx = goalX - player.x;
    float dy = goalY - player.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 1.0f) {
        player.x += dx / distance * player.speed * deltaTime;
        player.y += dy / distance * player.speed * deltaTime;
    }

    avoidDefenders(player, deltaTime);

    for (Player* teammate : player.teammates) {
        if (!teammate->isMarked && calculateDistance(player.x, player.y, teammate->x, teammate->y) < 10.0f) {
            std::cout << "Passe à un coéquipier." << std::endl;
            ball.x = teammate->x;
            ball.y = teammate->y;
            teammate->hasBall = true;
            player.hasBall = false;
            break;
        }
    }
}

void PossessionState::avoidDefenders(Player& player, float deltaTime) {
    for (Player* opponent : player.opponents) {
        float oppDistance = calculateDistance(player.x, player.y, opponent->x, opponent->y);
        if (oppDistance < 5.0f) {
            std::cout << "Le porteur de balle évite un défenseur." << std::endl;
            player.x -= (opponent->x - player.x) / oppDistance * player.speed * deltaTime * 0.5f;
            player.y -= (opponent->y - player.y) / oppDistance * player.speed * deltaTime * 0.5f;
        }
    }
}

void TeammateState::start(Player& player, Ball& ball) {
    std::cout << "Le coéquipier soutient le porteur de balle." << std::endl;
}

void TeammateState::update(Player& player, Ball& ball, float deltaTime) {
    Player* ballHolder = nullptr;
    for (Player* teammate : player.teammates) {
        if (teammate->hasBall) {
            ballHolder = teammate;
            break;
        }
    }

    if (ballHolder) {
        findOpenSpace(player, ball, deltaTime);
    }
}

void TeammateState::findOpenSpace(Player& player, Ball& ball, float deltaTime) {
    float dx = ball.x - player.x;
    float dy = ball.y - player.y;
    float distance = calculateDistance(player.x, player.y, ball.x, ball.y);

    if (distance < 8.0f) {
        player.x += dy / distance * player.speed * deltaTime * 0.5f;
        player.y -= dx / distance * player.speed * deltaTime * 0.5f;
        std::cout << "Le coéquipier cherche à se démarquer." << std::endl;
    }
}

void OpponentState::start(Player& player, Ball& ball) {
    std::cout << "L'adversaire se dirige vers le porteur de balle." << std::endl;
}

void OpponentState::update(Player& player, Ball& ball, float deltaTime) {
    Player* ballHolder = nullptr;
    for (Player* opponent : player.opponents) {
        if (opponent->hasBall) {
            ballHolder = opponent;
            break;
        }
    }

    if (ballHolder) {
        positionStrategically(player, ball, deltaTime);
        markPlayers(player, deltaTime);
    }
}

void OpponentState::positionStrategically(Player& player, Ball& ball, float deltaTime) {
    Player* ballHolder = nullptr;
    for (Player* opponent : player.opponents) {
        if (opponent->hasBall) {
            ballHolder = opponent;
            break;
        }
    }

    if (ballHolder) {
        float dx = ballHolder->x - player.x;
        float dy = ballHolder->y - player.y;
        float distance = calculateDistance(player.x, player.y, ballHolder->x, ballHolder->y);

        if (distance > 1.0f) {
            player.x += dx / distance * player.speed * deltaTime * 0.5f;
            player.y += dy / distance * player.speed * deltaTime * 0.5f;
            std::cout << "L'adversaire bloque une ligne de passe." << std::endl;
        }
    }
}

void OpponentState::markPlayers(Player& player, float deltaTime) {
    for (Player* teammate : player.opponents) {
        float markDistance = calculateDistance(player.x, player.y, teammate->x, teammate->y);
        if (markDistance < 10.0f) {
            teammate->isMarked = true;
            std::cout << "L'adversaire marque un joueur." << std::endl;
        }
    }
}