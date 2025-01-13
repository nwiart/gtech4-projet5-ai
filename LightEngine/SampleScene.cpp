#include "SampleScene.h"

#include "DummyEntity.h"

#include "PlayerEntity.h"
#include "BallEntity.h"

#include "Debug.h"

void SampleScene::OnInitialize()
{
	const float playerRadius = 34.0F;
	const float ballRadius = 20.0F;

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
		PlayerEntity* p0 = CreateEntity<PlayerEntity>(playerRadius, sf::Color::Green);
		p0->SetPosition(pos[i][0], pos[i][1]);
		p0->SetRigidBody(true);

		PlayerEntity* p1 = CreateEntity<PlayerEntity>(playerRadius, sf::Color::Red);
		p1->SetPosition(GetWindowWidth() - pos[i][0], pos[i][1]);
		p1->SetRigidBody(true);
	}

	BallEntity* ball = CreateEntity<BallEntity>(ballRadius, sf::Color(255, 200, 0));
	ball->SetPosition(400, 200);

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

void SampleScene::OnUpdate()
{
	if(pEntitySelected != nullptr)
	{
		sf::Vector2f position = pEntitySelected->GetPosition();
		Debug::DrawCircle(position.x, position.y, 10, sf::Color::Blue);
	}
}