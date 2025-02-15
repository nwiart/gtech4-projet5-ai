#include "StateAction.h"
#include "PlayerEntity.h"
#include "BallEntity.h"
#include <cmath>
#include "SampleScene.h"

namespace
{
    constexpr float PASS_DISTANCE = 150.0f;
    constexpr float INTERCEPT_DISTANCE = 5.0f;

    float calculateDistance(float x1, float y1, float x2, float y2)
    {
        return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }

    float length(const sf::Vector2f& v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    sf::Vector2f calculateDirection(const sf::Vector2f &from, const sf::Vector2f &to)
    {
        sf::Vector2f direction = to - from;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        return (length > 0.0f) ? direction / length : sf::Vector2f(0.0f, 0.0f);
    }

    float pointLineDistance(const sf::Vector2f& point, const sf::Vector2f& lineStart, const sf::Vector2f& lineEnd)
    {
        sf::Vector2f lineDir = lineEnd - lineStart;
        float lineLength = std::sqrt(lineDir.x * lineDir.x + lineDir.y * lineDir.y);
        lineDir /= lineLength;

        sf::Vector2f pointToStart = point - lineStart;
        float projection = pointToStart.x * lineDir.x + pointToStart.y * lineDir.y;

        if (projection < 0.0f)
            return std::sqrt(pointToStart.x * pointToStart.x + pointToStart.y * pointToStart.y);

        if (projection > lineLength)
            return std::sqrt((point - lineEnd).x * (point - lineEnd).x + (point - lineEnd).y * (point - lineEnd).y);

        sf::Vector2f closestPoint = lineStart + projection * lineDir;
        return std::sqrt((point - closestPoint).x * (point - closestPoint).x + (point - closestPoint).y * (point - closestPoint).y);
    }
}

// ----------------------------------------------------
// PossessionState (Joueur avec la balle)
// ----------------------------------------------------

void PossessionState::start(PlayerEntity &player, BallEntity &)
{
    std::cout << "Le joueur " << player.GetId() << " a pris possession de la balle." << std::endl;
}

void PossessionState::update(PlayerEntity& player, BallEntity&, float deltaTime)
{
    sf::Vector2f playerPos = player.GetPosition();
    float goalX = (player.GetTeam() == 1) ? player.GetScene()->GetWindowWidth() - 50.0f : 50.0f;
    sf::Vector2f goalPos = { goalX, playerPos.y };

    //sf::Vector2f direction = calculateDirection(playerPos, goalPos);
    //player.SetDirection(direction.x, direction.y, 200.0f);

    constexpr float THREAT_DISTANCE = 200.0f;

    BallEntity* ball = player.GetScene<SampleScene>()->GetBall();

    // Player direction calculation.
    sf::Vector2f direction((player.GetTeam() == 1) ? 1.0F : -1.0F, 0.0F);

    PlayerEntity* closestOpponent = GetClosestOpponentInFront(&player);
    if (closestOpponent) {
        sf::Vector2f diff = closestOpponent->GetPosition() - player.GetPosition();
        float dist = length(diff);

        if (dist < THREAT_DISTANCE && !player.IsInvincible())
        {
            passToTeammate(player, *ball);
        }
        else {
            float dirSign = closestOpponent->GetDirection().y > 0.0F ? 1.0F : -1.0F;
            float angle = atan2(diff.y, abs(diff.x)) * 20.0F / (dist);

            direction.y = -angle * 8.0F;
        }
    }

    player.SetDirection(direction.x, direction.y, 200.0f);
}

void PossessionState::passToTeammate(PlayerEntity& player, BallEntity& ball)
{
    for (PlayerEntity* teammate : player.GetTeammates())
    {
        if (!teammate->IsMarked())
        {
            sf::Vector2f playerPos = player.GetPosition();
            sf::Vector2f teammatePos = teammate->GetPosition();

            float distance = calculateDistance(playerPos.x, playerPos.y, teammatePos.x, teammatePos.y);
            if (distance < PASS_DISTANCE)
            {
                bool intercepted = false;

                for (PlayerEntity* opponent : player.GetOpponents())
                {
                    sf::Vector2f opponentPos = opponent->GetPosition();
                    float distanceToPass = pointLineDistance(opponentPos, playerPos, teammatePos);

                    if (distanceToPass < INTERCEPT_DISTANCE)
                    {
                        intercepted = true;
                        break;
                    }
                }

                float riskFactor = player.GetRiskTaking();

                if (intercepted && static_cast<float>(std::rand()) / RAND_MAX > riskFactor)
                {
                    std::cout << "Passe annulee : trop risque." << std::endl;
                    continue;
                }

                ball.ThrowAt(&player, teammatePos, 300.0f);
                player.GetScene<SampleScene>()->SetBallHolder(0);
                std::cout << "Passe effectuee vers le coequipier !" << std::endl;
                return;
            }
        }
    }
}

PlayerEntity* PossessionState::GetClosestOpponentInFront(PlayerEntity* player) const
{
    float dist = INFINITY;
    PlayerEntity* closest = 0;

    for (PlayerEntity* opponent : player->GetOpponents()) {
        sf::Vector2f diff = opponent->GetPosition() - player->GetDirection();
        float xDir = player->GetTeam() == 1 ? 1.0F : -1.0F;
        float a = length(diff);

        float angle = abs(atan2(diff.y, diff.x * xDir));
        if (angle >= 2.0F) continue;

        if (a < dist) {
            dist = a;
            closest = opponent;
        }
    }

    return closest;
}

// ----------------------------------------------------
// TeammateState (Coequipier sans la balle)
// ----------------------------------------------------

void TeammateState::start(PlayerEntity &player, BallEntity &)
{
    std::cout << "Le joueur " << player.GetId() << " se positionne pour soutenir." << std::endl;
}

void TeammateState::update(PlayerEntity &player, BallEntity &ball, float deltaTime)
{
    PlayerEntity *ballHolder = player.GetScene<SampleScene>()->GetBallHolder();
    if (!ballHolder)
        return;

    if (ballHolder)
    {
        const float distanceBehind = 70.0F;
        float goalDirection = player.GetTeam() == 1 ? 1.0F : -1.0F;
        // findOpenSpace(player, ball, deltaTime);
        float xBehind = ballHolder->GetPosition().x - distanceBehind * goalDirection;
        if ((xBehind - player.GetPosition().x) * goalDirection > 0.0F)
        {
            player.SetDirection(goalDirection, 0.0F, 200.0F);
        }
        else
        {
            player.SetSpeed(0.0F);
        }
    }
}

// ----------------------------------------------------
// OpponentState (Adversaire sans la balle)
// ----------------------------------------------------

void OpponentState::start(PlayerEntity &player, BallEntity &)
{
    std::cout << "Le joueur " << player.GetId() << " se dirige vers le porteur de balle." << std::endl;
}

void OpponentState::update(PlayerEntity &player, BallEntity &ball, float deltaTime)
{
    PlayerEntity *ballHolder = player.GetScene<SampleScene>()->GetBallHolder();
    if (!ballHolder)
        return;

    sf::Vector2f direction = calculateDirection(player.GetPosition(), ballHolder->GetPosition());
    player.SetDirection(direction.x, direction.y, 200.f);

}
