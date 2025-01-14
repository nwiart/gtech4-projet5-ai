#include "PlayerEntity.h"
#include "SampleScene.h"
#include "BallEntity.h"

void PlayerEntity::SetCurrentState(StateAction* newState) {
    if (mCurrentState) {
        delete mCurrentState;
    }
    mCurrentState = newState;

    if (mCurrentState) {
        SampleScene* scene = dynamic_cast<SampleScene*>(GetScene());
        if (scene && scene->GetBall()) {
            mCurrentState->start(*this, *scene->GetBall());
        }
        else {
            std::cerr << "Erreur : Impossible d'acc�der � la sc�ne ou au ballon." << std::endl;
        }
    }
}

// Gestion des collisions avec le ballon
void PlayerEntity::OnCollision(Entity* collidedWith) {
    if (dynamic_cast<BallEntity*>(collidedWith)) {
        if (!HasBall()) {
            std::cout << "Un joueur r�cup�re la balle." << std::endl;
            SetHasBall(true);
            SetCurrentState(new PossessionState());

            GetScene<SampleScene>()->SetBallHolder(this);
        }
    }
}
