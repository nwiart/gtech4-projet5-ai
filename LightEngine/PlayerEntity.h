#pragma once

#include "Entity.h"
#include "StateMachine.h"

#include <vector>

class SampleScene;

class PlayerEntity : public Entity {
    friend class SampleScene;

public:
    PlayerEntity();

    virtual void OnUpdate() override;

    bool IsMarked() const { return mIsMarked; }
    void SetIsMarked(bool isMarked) { mIsMarked = isMarked; }

    bool HasBall() const { return mHasBall; }
    void SetHasBall(bool hasBall) { mHasBall = hasBall; }

    const std::vector<PlayerEntity*>& GetTeammates() const { return mTeammates; }
    void SetTeammates(const std::vector<PlayerEntity*>& teammates) { mTeammates = teammates; }
    const std::vector<PlayerEntity*>& GetOpponents() const { return mOpponents; }
    void SetOpponents(const std::vector<PlayerEntity*>& opponents) { mOpponents = opponents; }

    StateAction* GetCurrentState() const { return mCurrentState; }
    void SetCurrentState(StateAction* newState);
    StateMachine& GetStateMachine() { return mStateMachine; }

    void OnCollision(Entity* collidedWith);

    void SetId(int id) { mId = id; }
    int GetId() const { return mId; }
    void SetTeam(int team) { mTeam = team; }
    int GetTeam() const { return mTeam; }

private:
    int mId;
    int mTeam;

    bool mIsMarked = false;
    bool mHasBall = false;
    std::vector<PlayerEntity*> mTeammates;
    std::vector<PlayerEntity*> mOpponents;
    StateAction* mCurrentState = nullptr;
    StateMachine mStateMachine;
};
