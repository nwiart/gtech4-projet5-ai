#include "SampleScene.h"
#include "PlayerEntity.h"
#include "BallEntity.h"
#include "Debug.h"
#include <random> // Pour le choix al�atoire

void SampleScene::OnInitialize()
{
    CreatePlayersAndBall();
    ResetPlayers();
    pEntitySelected = nullptr;
}

void SampleScene::OnEvent(const sf::Event &event)
{
    if (event.type != sf::Event::EventType::MouseButtonPressed)
        return;

    if (event.mouseButton.button == sf::Mouse::Button::Left)
    {
        if (pEntitySelected != nullptr)
        {
            pEntitySelected->GoToPosition(event.mouseButton.x, event.mouseButton.y, 100.f);
        }
    }
}

void SampleScene::CreatePlayersAndBall()
{
    const float playerRadius = 34.0F;
    const float ballRadius = 20.0F;

    // Cr�ation des joueurs
    for (int i = 0; i < 5; i++)
    {
        PlayerEntity *p0 = CreateEntity<PlayerEntity>(playerRadius, sf::Color::Green);
        p0->SetRigidBody(true);
        p0->SetId(i);
        p0->SetTeam(1);
        p0->SetSpeed(100.f);
        pPlayersTeam1[i] = p0;

        PlayerEntity *p1 = CreateEntity<PlayerEntity>(playerRadius, sf::Color::Red);
        p1->SetRigidBody(true);
        p1->SetId(i + 5);
        p1->SetTeam(2);
        p0->SetSpeed(100.f);
        pPlayersTeam2[i] = p1;

        // D�bogage : confirmation des joueurs
        std::cout << "Joueur " << p0->GetId() << " de l'�quipe 1 cr��." << std::endl;
        std::cout << "Joueur " << p1->GetId() << " de l'�quipe 2 cr��." << std::endl;
    }

    // D�finition des co�quipiers et adversaires
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

    // Cr�ation du ballon
    pBall = CreateEntity<BallEntity>(ballRadius, sf::Color(255, 200, 0));

    // Choix al�atoire pour attribuer la balle
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 4);

    int ballHolderIndex = dist(gen); // Index al�atoire entre 0 et 4
    pPlayersTeam1[ballHolderIndex]->SetHasBall(true);
    pPlayersTeam1[ballHolderIndex]->SetCurrentState(new PossessionState());
    pBall->SetPosition(pPlayersTeam1[ballHolderIndex]->GetPosition().x, pPlayersTeam1[ballHolderIndex]->GetPosition().y);

    std::cout << "Initialisation termin�e : Joueur " << ballHolderIndex
              << " de l'�quipe 1 commence avec la balle." << std::endl;

    // Les autres joueurs de l'�quipe 1 soutiennent le porteur de balle
    for (int i = 0; i < 5; i++)
    {
        if (i != ballHolderIndex)
        {
            pPlayersTeam1[i]->SetCurrentState(new TeammateState());
        }
    }

    // Tous les joueurs de l'�quipe 2 tentent d'intercepter
    for (int i = 0; i < 5; i++)
    {
        pPlayersTeam2[i]->SetCurrentState(new OpponentState());
    }
}

void SampleScene::ResetPlayers()
{
    const int wy = GetWindowHeight();
    const int hy = GetWindowHeight() / 2;

    // Positionnement des joueurs
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
}

void SampleScene::OnUpdate()
{
    float deltaTime = GetDeltaTime();

    // Synchronise la position de la balle avec le porteur
    for (int i = 0; i < 5; ++i)
    {
        if (pPlayersTeam1[i]->HasBall())
        {
            pBall->SetPosition(pPlayersTeam1[i]->GetPosition().x, pPlayersTeam1[i]->GetPosition().y);
            break;
        }
        if (pPlayersTeam2[i]->HasBall())
        {
            pBall->SetPosition(pPlayersTeam2[i]->GetPosition().x, pPlayersTeam2[i]->GetPosition().y);
            break;
        }
    }

    // Mise � jour des joueurs de l'�quipe 1
    for (int i = 0; i < 5; ++i)
    {
        if (pPlayersTeam1[i]->GetCurrentState())
        {
            pPlayersTeam1[i]->GetCurrentState()->update(*pPlayersTeam1[i], *pBall, deltaTime);
        }
    }

    // Mise � jour des joueurs de l'�quipe 2
    for (int i = 0; i < 5; ++i)
    {
        if (pPlayersTeam2[i]->GetCurrentState())
        {
            pPlayersTeam2[i]->GetCurrentState()->update(*pPlayersTeam2[i], *pBall, deltaTime);
        }
    }

    // V�rification des buts
    int goalLineTeam1 = GetWindowWidth() / 10;
    Debug::DrawLine(goalLineTeam1, 0, goalLineTeam1, GetWindowHeight(), sf::Color::White);
    int goalLineTeam2 = GetWindowWidth() - goalLineTeam1;
    Debug::DrawLine(goalLineTeam2, 0, goalLineTeam2, GetWindowHeight(), sf::Color::White);
    sf::Vector2f ballPos = pBall->GetPosition();

    if (ballPos.x <= goalLineTeam1)
    {
        std::cout << "But pour l'�quipe 2 !" << std::endl;
        ResetGame(2);
    }
    else if (ballPos.x >= goalLineTeam2)
    {
        std::cout << "But pour l'�quipe 1 !" << std::endl;
        ResetGame(1);
    }
}
void SampleScene::ResetGame(int teamWithBall)
{
    // R�initialise les positions des joueurs
    ResetPlayers();

    // Choix al�atoire pour attribuer la balle
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 4);

    // �quipe qui doit recevoir la balle
    if (teamWithBall == 1)
    {
        // R�initialisation de l'�quipe 2
        for (int i = 0; i < 5; ++i)
        {
            pPlayersTeam2[i]->SetHasBall(false);
            pPlayersTeam2[i]->SetCurrentState(new OpponentState());
        }

        // Attribue la balle � un joueur al�atoire de l'�quipe 1
        int ballHolderIndex = dist(gen);
        for (int i = 0; i < 5; ++i)
        {
            if (i == ballHolderIndex)
            {
                pPlayersTeam1[i]->SetHasBall(true);
                pPlayersTeam1[i]->SetCurrentState(new PossessionState());
                pBall->SetPosition(pPlayersTeam1[i]->GetPosition().x, pPlayersTeam1[i]->GetPosition().y);
            }
            else
            {
                pPlayersTeam1[i]->SetHasBall(false);
                pPlayersTeam1[i]->SetCurrentState(new TeammateState());
            }
        }
    }
    else if (teamWithBall == 2)
    {
        // R�initialisation de l'�quipe 1
        for (int i = 0; i < 5; ++i)
        {
            pPlayersTeam1[i]->SetHasBall(false);
            pPlayersTeam1[i]->SetCurrentState(new OpponentState());
        }

        // Attribue la balle � un joueur al�atoire de l'�quipe 2
        int ballHolderIndex = dist(gen);
        for (int i = 0; i < 5; ++i)
        {
            if (i == ballHolderIndex)
            {
                pPlayersTeam2[i]->SetHasBall(true);
                pPlayersTeam2[i]->SetCurrentState(new PossessionState());
                pBall->SetPosition(pPlayersTeam2[i]->GetPosition().x, pPlayersTeam2[i]->GetPosition().y);
            }
            else
            {
                pPlayersTeam2[i]->SetHasBall(false);
                pPlayersTeam2[i]->SetCurrentState(new TeammateState());
            }
        }
    }

    // D�bogage : Afficher l'�tat des joueurs apr�s r�initialisation
    std::cout << "�tat des joueurs apr�s r�initialisation :" << std::endl;

    std::cout << "�quipe 1 :" << std::endl;
    for (int i = 0; i < 5; ++i)
    {
        std::cout << "Joueur " << pPlayersTeam1[i]->GetId()
                  << " - �tat : " << (pPlayersTeam1[i]->HasBall() ? "Possession" : "Adversaire") << std::endl;
    }

    std::cout << "�quipe 2 :" << std::endl;
    for (int i = 0; i < 5; ++i)
    {
        std::cout << "Joueur " << pPlayersTeam2[i]->GetId()
                  << " - �tat : " << (pPlayersTeam2[i]->HasBall() ? "Possession" : "Soutien") << std::endl;
    }

    sf::Vector2f ballPos = pBall->GetPosition();
    std::cout << "Position de la balle : (" << ballPos.x << ", " << ballPos.y << ")" << std::endl;
    std::cout << "Le jeu a �t� r�initialis�. L'�quipe " << teamWithBall << " commence avec la balle." << std::endl;
}
