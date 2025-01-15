#include "PlayerEntity.h"
#include "SampleScene.h"
#include "BallEntity.h"

#include "StateAction.h"

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
            std::cout << "Le joueur " << GetId() << " n'est plus invincible." << std::endl;
        }
    }

    if (mSpeedBoostTime > 0.0f) {
        mSpeedBoostTime -= GetScene<SampleScene>()->GetDeltaTime();
        if (mSpeedBoostTime <= 0.0f) {
            mSpeedBoost = 1.0f;
            std::cout << "Le boost de vitesse du joueur " << GetId() << " est terminé." << std::endl;
        }
    }
}

bool PlayerEntity::HasBall() const
{
    return GetScene<SampleScene>()->GetBallHolder() == this;
}

void PlayerEntity::OnCollision(Entity* collidedWith) {
    if (mIsInvincible) {
        std::cout << "Collision ignoree car le joueur " << GetId() << " est invincible." << std::endl;
        return;
    }

    PlayerEntity* otherPlayer = dynamic_cast<PlayerEntity*>(collidedWith);
    if (otherPlayer) {
        SampleScene* scene = GetScene<SampleScene>();
        if (scene->GetBallHolder() == otherPlayer && otherPlayer->GetTeam() != GetTeam() && !otherPlayer->mIsInvincible) {
            std::cout << "Le joueur " << GetId() << " a pris la balle au joueur " << otherPlayer->GetId() << "." << std::endl;

            scene->SetBallHolder(this);

            SetInvincibility(2.0f);
            ApplySpeedBoost(1.5f, 3.0f);
        }
    }
}


void PlayerEntity::SetInvincibility(float duration) {
    mIsInvincible = true;
    mInvincibilityTime = duration;
    std::cout << "Le joueur " << GetId() << " est invincible pour " << duration << " secondes." << std::endl;
}

void PlayerEntity::ApplySpeedBoost(float boostMultiplier, float duration) {
    mSpeedBoost = boostMultiplier;
    mSpeedBoostTime = duration;
    std::cout << "Le joueur " << GetId() << " a un boost de vitesse (" << boostMultiplier
        << "x) pour " << duration << " secondes." << std::endl;
}


void PlayerEntity::GoToPosition(float x, float y, float baseSpeed) {
    float boostedSpeed = baseSpeed * mSpeedBoost;
    SetDirection((x - GetPosition().x), (y - GetPosition().y), boostedSpeed);
}

