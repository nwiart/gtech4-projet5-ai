#include "SampleScene.h"

#include "DummyEntity.h"

#include "PlayerEntity.h"
#include "BallEntity.h"

#include "Debug.h"

void SampleScene::OnInitialize()
{
	CreatePlayersAndBall();
	ResetPlayers();

	pEntitySelected = nullptr;
}

void SampleScene::OnEvent(const sf::Event& event)
{
	if (event.type != sf::Event::EventType::MouseButtonPressed)
		return;

	if (event.mouseButton.button == sf::Mouse::Button::Right)
	{
		//TrySetSelectedEntity(pEntity1, event.mouseButton.x, event.mouseButton.y);
		//TrySetSelectedEntity(pEntity2, event.mouseButton.x, event.mouseButton.y);
	}

	if (event.mouseButton.button == sf::Mouse::Button::Left)
	{
		if (pEntitySelected != nullptr) 
		{
			pEntitySelected->GoToPosition(event.mouseButton.x, event.mouseButton.y, 100.f);
		}
	}
}

void SampleScene::TrySetSelectedEntity(DummyEntity* pEntity, int x, int y)
{
	if (pEntity->IsInside(x, y) == false)
		return;

	pEntitySelected = pEntity;
}

void SampleScene::CreatePlayersAndBall()
{
	const float playerRadius = 34.0F;
	const float ballRadius = 20.0F;

	for (int i = 0; i < 5; i++) {
		PlayerEntity* p0 = CreateEntity<PlayerEntity>(playerRadius, sf::Color::Green);
		p0->SetRigidBody(true);
		pPlayersTeam1[i] = p0;

		PlayerEntity* p1 = CreateEntity<PlayerEntity>(playerRadius, sf::Color::Red);
		p1->SetRigidBody(true);
		pPlayersTeam2[i] = p1;
	}

	pBall = CreateEntity<BallEntity>(ballRadius, sf::Color(255, 200, 0));
	pBall->SetPosition(400, 200);
}

void SampleScene::ResetPlayers()
{
	const int wy = GetWindowHeight();
	const int hy = GetWindowHeight() / 2;

	// Player placement.
	const float pos[5][2] = {
		{ 80.0F,  80.0F },
		{ 200.0F, hy - 240.0F },
		{ 260.0F, hy },
		{ 200.0F, hy + 240.0F },
		{ 80.0F,  wy - 80.0F },
	};

	for (int i = 0; i < 5; i++) {
		pPlayersTeam1[i]->SetPosition(pos[i][0], pos[i][1]);
		pPlayersTeam2[i]->SetPosition(GetWindowWidth() - pos[i][0], pos[i][1]);
	}
}

void SampleScene::OnUpdate()
{
	if (pEntitySelected != nullptr)
	{
		sf::Vector2f position = pEntitySelected->GetPosition();
		Debug::DrawCircle(position.x, position.y, 10, sf::Color::Blue);
	}

	// Goal delimiters.
	int xPos = GetWindowWidth() / 10;
	Debug::DrawLine(xPos, 0, xPos, GetWindowHeight(), sf::Color::White);
	xPos = GetWindowWidth() - xPos;
	Debug::DrawLine(xPos, 0, xPos, GetWindowHeight(), sf::Color::White);

	// Score.
	Debug::DrawText(10, 10, "Score: 0 - 0", sf::Color::White);
}