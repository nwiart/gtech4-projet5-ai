#include "StateAction.h"
#include "PlayerEntity.h"
#include "BallEntity.h"
#include <cmath>
#include "SampleScene.h"

namespace
{
    constexpr float PASS_DISTANCE = 10.0f;
    constexpr float INTERCEPT_DISTANCE = 5.0f;

    float calculateDistance(float x1, float y1, float x2, float y2)
    {
        return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }

    sf::Vector2f calculateDirection(const sf::Vector2f &from, const sf::Vector2f &to)
    {
        sf::Vector2f direction = to - from;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        return (length > 0.0f) ? direction / length : sf::Vector2f(0.0f, 0.0f);
    }
}

// ----------------------------------------------------
// PossessionState (Joueur avec la balle)
// ----------------------------------------------------

void PossessionState::start(PlayerEntity &player, BallEntity &)
{
    std::cout << "Le joueur " << player.GetId() << " a pris possession de la balle." << std::endl;
}

void PossessionState::update(PlayerEntity &player, BallEntity &ball, float deltaTime)
{
    sf::Vector2f playerPos = player.GetPosition();
    float goalX = (player.GetTeam() == 1) ? player.GetScene()->GetWindowWidth() - 50.0f : 50.0f;
    sf::Vector2f goalPos = {goalX, playerPos.y};

    // Avancer vers les buts adverses
    sf::Vector2f direction = calculateDirection(playerPos, goalPos);
    player.SetDirection(direction.x, direction.y, 200.f);

    // Tenter une passe si un adversaire est proche
    for (PlayerEntity *opponent : player.GetOpponents())
    {
        float distance = calculateDistance(playerPos.x, playerPos.y, opponent->GetPosition().x, opponent->GetPosition().y);
        if (distance < PASS_DISTANCE)
        {
            //passToTeammate(player, ball);
            break;
        }
    }
}

//void PossessionState::passToTeammate(PlayerEntity &player, BallEntity &ball)
//{
//    for (PlayerEntity *teammate : player.GetTeammates())
//    {
//        if (!teammate->IsMarked())
//        {
//            float distance = calculateDistance(player.GetPosition().x, player.GetPosition().y,
//                                               teammate->GetPosition().x, teammate->GetPosition().y);
//
//            if (distance < PASS_DISTANCE)
//            {
//                sf::Vector2f passDirection = calculateDirection(player.GetPosition(), teammate->GetPosition());
//                ball.SetDirection(passDirection.x, passDirection.y, 300.0f);
//                teammate->SetHasBall(true);
//                player.SetHasBall(false);
//                std::cout << "Passe effectu�e par le joueur " << player.GetId()
//                          << " au joueur " << teammate->GetId() << "." << std::endl;
//                return;
//            }
//        }
//    }
//}

// ----------------------------------------------------
// TeammateState (Co�quipier sans la balle)
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
