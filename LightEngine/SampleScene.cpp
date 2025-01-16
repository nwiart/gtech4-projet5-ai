#include "SampleScene.h"
#include "PlayerEntity.h"
#include "BallEntity.h"
#include "Debug.h"
#include <random>
#include <iostream>

constexpr float INTERCEPT_DISTANCE = 70.0f;

float calculateDistance(float x1, float y1, float x2, float y2)
{
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void SampleScene::OnInitialize()
{
    CreatePlayersAndBall();
    ResetGame(1);
    pEntitySelected = nullptr;
}

void SampleScene::OnEvent(const sf::Event &event)
{
    if (event.type != sf::Event::EventType::MouseButtonPressed)
        return;

    // Drop ball on right click.
    if (event.mouseButton.button == sf::Mouse::Button::Right)
    {
        /*float angle = rand() / (float)RAND_MAX * 2.0F * 3.14F;
        float vx = cos(angle), vy = sin(angle);
        pBall->SetDirection(vx, vy, 200.0F);

        sf::Vector2f pos = pBall->GetPosition();
        pBall->SetPosition(pos.x + vx * 60.0F, pos.y + vy * 60.0F);

        if (pBallHolder)
        {
            pBallHolder = 0;
        }*/

        if (pEntitySelected != nullptr)
        {
            pEntitySelected->SetPosition(event.mouseButton.x, event.mouseButton.y);
        }
    }
    // Select player.
    else if (event.mouseButton.button == sf::Mouse::Button::Left)
    {
        pEntitySelected = 0;
        for (int i = 0; i < 5; i++) {
            if (pPlayersTeam1[i]->IsInside(event.mouseButton.x, event.mouseButton.y)) {
                pEntitySelected = pPlayersTeam1[i];
                break;
            }
            if (pPlayersTeam2[i]->IsInside(event.mouseButton.x, event.mouseButton.y)) {
                pEntitySelected = pPlayersTeam2[i];
                break;
            }
        }
    }
}

void SampleScene::CreatePlayersAndBall()
{
    const float playerRadius = 34.0F;
    const float ballRadius = 20.0F;

    for (int i = 0; i < 5; i++)
    {
        PlayerEntity *p0 = CreateEntity<PlayerEntity>(playerRadius, sf::Color::Green);
        p0->SetRigidBody(true);
        p0->SetId(i);
        p0->SetTeam(1);
        pPlayersTeam1[i] = p0;

        PlayerEntity *p1 = CreateEntity<PlayerEntity>(playerRadius, sf::Color::Red);
        p1->SetRigidBody(true);
        p1->SetId(i + 5);
        p1->SetTeam(2);
        pPlayersTeam2[i] = p1;
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            pPlayersTeam1[i]->mOpponents.push_back(pPlayersTeam2[j]);
            pPlayersTeam2[i]->mOpponents.push_back(pPlayersTeam1[j]);

            if (i != j)
            {
                pPlayersTeam1[i]->mTeammates.push_back(pPlayersTeam1[j]);
                pPlayersTeam2[i]->mTeammates.push_back(pPlayersTeam2[j]);
            }
        }
    }

    pBall = CreateEntity<BallEntity>(ballRadius, sf::Color(255, 200, 0));
}

void SampleScene::ResetPlayers()
{
    const int wy = GetWindowHeight();
    const int hy = GetWindowHeight() / 2;

    const float pos[5][2] = {
        {80.0F, 80.0F},
        {200.0F, hy - 240.0F},
        {260.0F, hy},
        {200.0F, hy + 240.0F},
        {80.0F, wy - 80.0F},
    };

    for (int i = 0; i < 5; i++)
    {
        pPlayersTeam1[i]->SetPosition(pos[i][0], pos[i][1]);
        pPlayersTeam2[i]->SetPosition(GetWindowWidth() - pos[i][0], pos[i][1]);
    }

    pBallHolder = 0;
}

void SampleScene::OnUpdate()
{
    if (pBallHolder) {
        pBall->SetPosition(pBallHolder->GetPosition().x, pBallHolder->GetPosition().y);
        }

    CheckForGoal();
}

void SampleScene::SetBallHolder(PlayerEntity* p)
{
    pBallHolder = p;
    if (p) {
        p->SetInvincibility(2.0f);
        p->ApplySpeedBoost(1.5f, 3.0f);
        }
}


void SampleScene::CheckForGoal()
{
    int goalLineTeam1 = GetWindowWidth() / 10;
    Debug::DrawLine(goalLineTeam1, 0, goalLineTeam1, GetWindowHeight(), sf::Color::White);
    int goalLineTeam2 = GetWindowWidth() - goalLineTeam1;
    Debug::DrawLine(goalLineTeam2, 0, goalLineTeam2, GetWindowHeight(), sf::Color::White);
    sf::Vector2f ballPos = pBall->GetPosition();

    if (pEntitySelected) {
        sf::Vector2f v = pEntitySelected->GetPosition();
        Debug::DrawCircle(v.x, v.y, 10.0F, sf::Color::Blue);
    }

    if (ballPos.x <= goalLineTeam1 && pBallHolder && pBallHolder->GetTeam() == 2)
    {
        scoreTeam2++;
        ResetGame(1);
    }
    else if (ballPos.x >= goalLineTeam2 && pBallHolder && pBallHolder->GetTeam() == 1)
    {
        scoreTeam1++;
        ResetGame(2);
    }

    Debug::DrawText(10, 10, "Equipe 1: " + std::to_string(scoreTeam1) + " | Equipe 2: " + std::to_string(scoreTeam2), sf::Color::White);
}

void SampleScene::ResetGame(int teamWithBall)
{
    ResetPlayers();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 4);

    if (teamWithBall == 1)
    {
        for (int i = 0; i < 5; ++i)
        {
            pPlayersTeam2[i]->GetStateMachine().Start(pPlayersTeam2[i], 1);
        }

        int ballHolderIndex = dist(gen);
        for (int i = 0; i < 5; ++i)
        {
            if (i == ballHolderIndex)
            {
                pPlayersTeam1[i]->GetStateMachine().Start(pPlayersTeam1[i], 2);
                pBallHolder = pPlayersTeam1[i];
                pBall->SetPosition(pPlayersTeam1[i]->GetPosition().x, pPlayersTeam1[i]->GetPosition().y);
            }
            else
            {
                pPlayersTeam1[i]->GetStateMachine().Start(pPlayersTeam1[i], 0);
            }
        }
    }
    else if (teamWithBall == 2)
    {
        for (int i = 0; i < 5; ++i)
        {
            pPlayersTeam1[i]->GetStateMachine().Start(pPlayersTeam2[i], 1);
        }

        int ballHolderIndex = dist(gen);
        for (int i = 0; i < 5; ++i)
        {
            if (i == ballHolderIndex)
            {
                pPlayersTeam2[i]->GetStateMachine().Start(pPlayersTeam2[i], 2);
                pBallHolder = pPlayersTeam2[i];
                pBall->SetPosition(pPlayersTeam2[i]->GetPosition().x, pPlayersTeam2[i]->GetPosition().y);
            }
            else
            {
                pPlayersTeam2[i]->GetStateMachine().Start(pPlayersTeam2[i], 0);
            }
        }
    }
    sf::Vector2f ballPos = pBall->GetPosition();
}
