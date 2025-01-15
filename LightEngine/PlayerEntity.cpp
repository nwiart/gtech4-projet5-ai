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

void PlayerEntity::OnUpdate()
{
    mStateMachine.Update(this);
}

bool PlayerEntity::HasBall() const
{
    return GetScene<SampleScene>()->GetBallHolder() == this;
}

// Gestion des collisions avec le ballon
void PlayerEntity::OnCollision(Entity* collidedWith) {
    if (dynamic_cast<BallEntity*>(collidedWith)) {
        if (!HasBall()) {
            std::cout << "Un joueur récupère la balle." << std::endl;

            GetScene<SampleScene>()->SetBallHolder(this);
        }
    }
}
