#include "StateAction.h"
#include "PlayerEntity.h"
#include "BallEntity.h"
#include <cmath>

namespace {
    constexpr float MARK_DISTANCE = 10.0f;
    constexpr float AVOID_DISTANCE = 5.0f;
    constexpr float PASS_DISTANCE = 10.0f;

    float calculateDistance(float x1, float y1, float x2, float y2) {
        return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }

    sf::Vector2f calculateDirection(const sf::Vector2f& from, const sf::Vector2f& to) {
        sf::Vector2f direction = to - from;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        return (length > 0.0f) ? direction / length : sf::Vector2f(0.0f, 0.0f);
    }
}

//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//

void PossessionState::start(PlayerEntity& player, BallEntity&) {
    std::cout << "Le joueur prend possession de la balle." << std::endl;
}

void PossessionState::update(PlayerEntity& player, BallEntity& ball, float deltaTime) {
    // Position des buts adverses (par défaut, à droite pour l'équipe 1)
    sf::Vector2f playerPos = player.GetPosition();
    float goalX = (player.GetTeam() == 1) ? player.GetScene()->GetWindowWidth() - 50.0f : 50.0f;
    sf::Vector2f goalPos = { goalX, playerPos.y };

    // Calcul de la direction vers les buts
    sf::Vector2f direction = calculateDirection(playerPos, goalPos);

    // Déplace le joueur vers les buts
    player.SetDirection(direction.x, direction.y, player.GetSpeed());
    player.SetPosition(
        playerPos.x + direction.x * player.GetSpeed() * deltaTime,
        playerPos.y + direction.y * player.GetSpeed() * deltaTime
    );

    // Synchronise la position de la balle avec le joueur
    ball.SetPosition(player.GetPosition().x, player.GetPosition().y);

    std::cout << "Le joueur avec l'ID " << player.GetId() << " avance vers les buts adverses." << std::endl;
}

void PossessionState::avoidDefenders(PlayerEntity& player, float deltaTime) {
    sf::Vector2f playerPos = player.GetPosition();
    for (PlayerEntity* opponent : player.GetOpponents()) {
        float distance = calculateDistance(playerPos.x, playerPos.y, opponent->GetPosition().x, opponent->GetPosition().y);
        if (distance < AVOID_DISTANCE) {
            sf::Vector2f avoidDir = calculateDirection(opponent->GetPosition(), playerPos);
            player.SetDirection(avoidDir.x, avoidDir.y, player.GetSpeed() * 0.5f);
        }
    }
}

//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//

void TeammateState::start(PlayerEntity& player, BallEntity&) {
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
    sf::Vector2f direction = calculateDirection(player.GetPosition(), ball.GetPosition());
    player.SetDirection(-direction.y, direction.x, player.GetSpeed() * 0.5f);
}

// --------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//

void OpponentState::start(PlayerEntity& player, BallEntity&) {
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

void OpponentState::positionStrategically(PlayerEntity& player, BallEntity&, float deltaTime) {
    PlayerEntity* ballHolder = nullptr;
    for (PlayerEntity* opponent : player.GetOpponents()) {
        if (opponent->HasBall()) {
            ballHolder = opponent;
            break;
        }
    }

    if (ballHolder) {
        sf::Vector2f direction = calculateDirection(player.GetPosition(), ballHolder->GetPosition());
        player.SetDirection(direction.x, direction.y, player.GetSpeed() * 0.5f);
    }
}

void OpponentState::markPlayers(PlayerEntity& player, float deltaTime) {
    for (PlayerEntity* teammate : player.GetOpponents()) {
        float distance = calculateDistance(player.GetPosition().x, player.GetPosition().y,
            teammate->GetPosition().x, teammate->GetPosition().y);
        if (distance < MARK_DISTANCE) {
            teammate->SetIsMarked(true);
        }
    }
}
