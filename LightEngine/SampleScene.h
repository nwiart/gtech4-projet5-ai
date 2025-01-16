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

	PlayerEntity* pBallHolder;

private:
	void TrySetSelectedEntity(DummyEntity* pEntity, int x, int y);

	void CreatePlayersAndBall();
	void ResetPlayers();
	void ResetGame(int teamWithBall);

	void CheckForGoal();

public:
	SampleScene();

	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
	BallEntity* GetBall() const { return pBall; }
	PlayerEntity* GetBallHolder() const { return pBallHolder; }
	void SetBallHolder(PlayerEntity* p);

	float GetBallInterceptTime() const { return mBallInterceptTime; }

	void DrawDistanceLines();

private:
	int scoreTeam1 = 0;
	int scoreTeam2 = 0;

	float mBallInterceptTime;
};
