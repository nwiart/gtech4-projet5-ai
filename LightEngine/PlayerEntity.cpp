#include "PlayerEntity.h"
#include "SampleScene.h"
#include "BallEntity.h"
#include "StateAction.h"

static float calculateDistance(float x1, float y1, float x2, float y2)
{
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

PlayerEntity::PlayerEntity()
{
    int teammateState = mStateMachine.AddState(new TeammateState());
    int opponentState = mStateMachine.AddState(new OpponentState());
    int possessState  = mStateMachine.AddState(new PossessionState());

    // Teammate to Opponent : if the ball belongs to the opposite team.
    mStateMachine.AddTransition(teammateState, opponentState, [](PlayerEntity* player) -> bool {
        PlayerEntity* ballHolder = player->GetScene<SampleScene>()->GetBallHolder();
        return ballHolder->GetTeam() != player->GetTeam();
    });

    // Opponent to Teammate : if the ball belongs to our team.
    mStateMachine.AddTransition(opponentState, teammateState, [](PlayerEntity* player) -> bool {
        PlayerEntity* ballHolder = player->GetScene<SampleScene>()->GetBallHolder();
        return ballHolder->GetTeam() == player->GetTeam();
    });

    // Teammate to Possess : I have the ball.
    mStateMachine.AddTransition(teammateState, possessState, [](PlayerEntity* player) -> bool {
        PlayerEntity* ballHolder = player->GetScene<SampleScene>()->GetBallHolder();
        return ballHolder == player;
    });

    // Opponent to Possess : I have the ball.
    mStateMachine.AddTransition(opponentState, possessState, [](PlayerEntity* player) -> bool {
        PlayerEntity* ballHolder = player->GetScene<SampleScene>()->GetBallHolder();
        return ballHolder == player;
    });

    // Possess to Teammate : if the ball belongs to our team.
    mStateMachine.AddTransition(possessState, teammateState, [](PlayerEntity* player) -> bool {
        PlayerEntity* ballHolder = player->GetScene<SampleScene>()->GetBallHolder();
        return ballHolder->GetTeam() == player->GetTeam() && ballHolder != player;
    });

    // Possess to Opponent : if the ball belongs to the opposite team.
    mStateMachine.AddTransition(possessState, opponentState, [](PlayerEntity* player) -> bool {
        PlayerEntity* ballHolder = player->GetScene<SampleScene>()->GetBallHolder();
        return ballHolder->GetTeam() != player->GetTeam();
    });
}

void PlayerEntity::OnUpdate() {
    mStateMachine.Update(this);

    if (mIsInvincible) {
        mInvincibilityTime -= GetScene<SampleScene>()->GetDeltaTime();
        if (mInvincibilityTime <= 0.0f) {
            mIsInvincible = false;
            this->SetRigidBody(true);
        }
    }

    if (mSpeedBoostTime > 0.0f) {
        mSpeedBoostTime -= GetScene<SampleScene>()->GetDeltaTime();
        if (mSpeedBoostTime <= 0.0f) {
            mSpeedBoost = 1.0f;
        }
    }
}

bool PlayerEntity::HasBall() const
{
    return GetScene<SampleScene>()->GetBallHolder() == this;
}

void PlayerEntity::OnCollision(Entity* collidedWith) {
    if (mIsInvincible) {
        return;
    }

    PlayerEntity* otherPlayer = dynamic_cast<PlayerEntity*>(collidedWith);
    if (otherPlayer) {
        SampleScene* scene = GetScene<SampleScene>();
        if (scene->GetBallHolder() == otherPlayer && otherPlayer->GetTeam() != GetTeam() && !otherPlayer->mIsInvincible) {

            scene->SetBallHolder(this);
        }
    }
}

void PlayerEntity::SetInvincibility(float duration) {
    mIsInvincible = true;
    mInvincibilityTime = duration;
    this->SetRigidBody(false);
}

void PlayerEntity::ApplySpeedBoost(float boostMultiplier, float duration) {
    mSpeedBoost = boostMultiplier;
    mSpeedBoostTime = duration;
}

void PlayerEntity::SetDirection(float x, float y, float speed) {
    mDirection.x = x;
    mDirection.y = y;

    mSpeed = speed * mSpeedBoost;
    
    float length = std::sqrt(mDirection.x * mDirection.x + mDirection.y * mDirection.y);
    if (length > 0.0f) {
        mDirection /= length;
    }
}

bool PlayerEntity::IsMarked() const
{
    for (PlayerEntity* opponent : GetOpponents())
    {
        float distance = calculateDistance(GetPosition().x, GetPosition().y,
            opponent->GetPosition().x, opponent->GetPosition().y);

        if (distance < 10.0f)
        {
            return true;
        }
    }
    return false;
}

