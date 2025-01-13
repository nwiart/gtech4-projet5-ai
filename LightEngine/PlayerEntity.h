#pragma once

#include "Entity.h"

class PlayerEntity : public Entity
{
    friend class SampleScene;

public:
    bool IsMarked() const { return mIsMarked; };
    void SetIsMarked(bool isMarked) { mIsMarked = isMarked; };

    bool HasBall() const { return mHasBall; };
    void SetHasBall(bool hasBall) { mHasBall = hasBall; };

    const std::vector<PlayerEntity*>& GetTeammates() const { return mTeammates; };
    void SetTeammates(const std::vector<PlayerEntity*>& teammates) { mTeammates = teammates; };
    const std::vector<PlayerEntity*>& GetOpponents() const { return mOpponents; };
    void SetOpponents(const std::vector<PlayerEntity*>& opponents) { mOpponents = opponents; };
private:
    bool mIsMarked = false;
    bool mHasBall = false;
    std::vector<PlayerEntity*> mTeammates;
    std::vector<PlayerEntity*> mOpponents;
};
