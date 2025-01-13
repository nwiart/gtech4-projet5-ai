#include "StateAction.h"
#include "PlayerEntity.h"
#include "BallEntity.h"
#include <cmath>

float calculateDistance(float x1, float y1, float x2, float y2) {
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void PossessionState::start(PlayerEntity& player, BallEntity& ball) {
    std::cout << "Le joueur prend possession de la balle." << std::endl;
}

void PossessionState::update(PlayerEntity& player, BallEntity& ball, float deltaTime) {
    sf::Vector2f playerPos = player.GetPosition();
    sf::Vector2f goalPos = { 100.0f, playerPos.y };

    sf::Vector2f direction = goalPos - playerPos;
    float distance = calculateDistance(playerPos.x, playerPos.y, goalPos.x, goalPos.y);

    if (distance > 1.0f) {
        direction /= distance;
        player.SetDirection(direction.x, direction.y, player.GetSpeed());
    }

    avoidDefenders(player, deltaTime);

    for (PlayerEntity* teammate : player.GetTeammates()) {
        sf::Vector2f teammatePos = teammate->GetPosition();
        if (!teammate->IsMarked() && calculateDistance(playerPos.x, playerPos.y, teammatePos.x, teammatePos.y) < 10.0f) {
            std::cout << "Passe à un coéquipier." << std::endl;
            ball.SetPosition(teammatePos.x, teammatePos.y);
            teammate->SetHasBall(true);
            player.SetHasBall(false);
            break;
        }
    }
}

void PossessionState::avoidDefenders(PlayerEntity& player, float deltaTime) {
    sf::Vector2f playerPos = player.GetPosition();
    for (PlayerEntity* opponent : player.GetOpponents()) {
        sf::Vector2f opponentPos = opponent->GetPosition();
        float distance = calculateDistance(playerPos.x, playerPos.y, opponentPos.x, opponentPos.y);
        if (distance < 5.0f) {
            std::cout << "Le porteur de balle évite un défenseur." << std::endl;
            sf::Vector2f avoidDir = playerPos - opponentPos;
            avoidDir /= distance;
            player.SetDirection(avoidDir.x, avoidDir.y, player.GetSpeed() * 0.5f);
        }
    }
}

void TeammateState::start(PlayerEntity& player, BallEntity& ball) {
    std::cout << "Le coéquipier soutient le porteur de balle." << std::endl;
}

void TeammateState::update(PlayerEntity& player, BallEntity& ball, float deltaTime) {
    PlayerEntity* ballHolder = nullptr;
    for (PlayerEntity* teammate : player.GetTeammates()) {
        if (teammate->HasBall()) {
            ballHolder = teammate;
            break;
        }
    }

    if (ballHolder) {
        findOpenSpace(player, ball, deltaTime);
    }
}

void TeammateState::findOpenSpace(PlayerEntity& player, BallEntity& ball, float deltaTime) {
    sf::Vector2f playerPos = player.GetPosition();
    sf::Vector2f ballPos = ball.GetPosition();
    sf::Vector2f direction = ballPos - playerPos;

    float distance = calculateDistance(playerPos.x, playerPos.y, ballPos.x, ballPos.y);

    if (distance < 8.0f) {
        direction /= distance; // Normalize
        player.SetDirection(-direction.y, direction.x, player.GetSpeed() * 0.5f); // Move sideways
        std::cout << "Le coéquipier cherche à se démarquer." << std::endl;
    }
}

void OpponentState::start(PlayerEntity& player, BallEntity& ball) {
    std::cout << "L'adversaire se dirige vers le porteur de balle." << std::endl;
}

void OpponentState::update(PlayerEntity& player, BallEntity& ball, float deltaTime) {
    PlayerEntity* ballHolder = nullptr;
    for (PlayerEntity* opponent : player.GetOpponents()) {
        if (opponent->HasBall()) {
            ballHolder = opponent;
            break;
        }
    }

    if (ballHolder) {
        positionStrategically(player, ball, deltaTime);
        markPlayers(player, deltaTime);
    }
}

void OpponentState::positionStrategically(PlayerEntity& player, BallEntity& ball, float deltaTime) {
    PlayerEntity* ballHolder = nullptr;
    for (PlayerEntity* opponent : player.GetOpponents()) {
        if (opponent->HasBall()) {
            ballHolder = opponent;
            break;
        }
    }

    if (ballHolder) {
        sf::Vector2f playerPos = player.GetPosition();
        sf::Vector2f ballHolderPos = ballHolder->GetPosition();
        sf::Vector2f direction = ballHolderPos - playerPos;

        float distance = calculateDistance(playerPos.x, playerPos.y, ballHolderPos.x, ballHolderPos.y);

        if (distance > 1.0f) {
            direction /= distance; // Normalize
            player.SetDirection(direction.x, direction.y, player.GetSpeed() * 0.5f);
            std::cout << "L'adversaire bloque une ligne de passe." << std::endl;
        }
    }
}

void OpponentState::markPlayers(PlayerEntity& player, float deltaTime) {
    for (PlayerEntity* teammate : player.GetOpponents()) {
        sf::Vector2f playerPos = player.GetPosition();
        sf::Vector2f teammatePos = teammate->GetPosition();
        float distance = calculateDistance(playerPos.x, playerPos.y, teammatePos.x, teammatePos.y);
        if (distance < 10.0f) {
            teammate->SetIsMarked(true);
            std::cout << "L'adversaire marque un joueur." << std::endl;
        }
    }
}
