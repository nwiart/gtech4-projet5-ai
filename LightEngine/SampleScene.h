#pragma once

#include "Scene.h"

class PlayerEntity;
class BallEntity;

class DummyEntity;

class SampleScene : public Scene
{
	Entity* pEntitySelected;

	PlayerEntity* pPlayersTeam1[5];
	PlayerEntity* pPlayersTeam2[5];
	BallEntity* pBall;

private:
	void TrySetSelectedEntity(DummyEntity* pEntity, int x, int y);

	void CreatePlayersAndBall();
	void ResetPlayers();
	void ResetGame(int teamWithBall);

public:
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
	BallEntity* GetBall() const { return pBall; }
};


