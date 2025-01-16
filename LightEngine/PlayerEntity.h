#pragma once

#include "Entity.h"
#include "StateMachine.h"

#include <vector>
#include <algorithm>
#include <iostream>

class SampleScene;

class PlayerEntity : public Entity {
    friend class SampleScene;

public:
    PlayerEntity();

    virtual void OnUpdate() override;

    bool IsMarked() const;
    void SetIsMarked(bool isMarked) { mIsMarked = isMarked; }

    bool HasBall() const;

    const std::vector<PlayerEntity*>& GetTeammates() const { return mTeammates; }
    void SetTeammates(const std::vector<PlayerEntity*>& teammates) { mTeammates = teammates; }
    const std::vector<PlayerEntity*>& GetOpponents() const { return mOpponents; }
    void SetOpponents(const std::vector<PlayerEntity*>& opponents) { mOpponents = opponents; }

    StateMachine& GetStateMachine() { return mStateMachine; }

    void OnCollision(Entity* collidedWith);

    void SetId(int id) { mId = id; }
    int GetId() const { return mId; }
    void SetTeam(int team) { mTeam = team; }
    int GetTeam() const { return mTeam; }

    void SetInvincibility(float duration);
    bool IsInvincible() const { return mIsInvincible; }
    void ApplySpeedBoost(float boostMultiplier, float duration);

    void SetDirection(float x, float y, float speed);

    void SetRiskTaking(float value)
    {
        if (value < 0.0f)
        {
            riskTaking = 0.0f;
        }
        else if (value > 1.0f)
        {
            riskTaking = 1.0f;
        }
        else
        {
            riskTaking = value;
        }
    }


    float GetRiskTaking() const { return riskTaking; }

private:
    int mId;
    int mTeam;

    bool mIsMarked = false;
    std::vector<PlayerEntity*> mTeammates;
    std::vector<PlayerEntity*> mOpponents;

    StateMachine mStateMachine;

    bool mIsInvincible = false;
    float mSpeedBoost = 1.0f;
    float mInvincibilityTime = 0.0f;
    float mSpeedBoostTime = 0.0f;

    float riskTaking;
};
