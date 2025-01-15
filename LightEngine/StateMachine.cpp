#include "StateMachine.h"

#include "StateAction.h"
#include "PlayerEntity.h"
#include "BallEntity.h"


StateMachine::StateMachine()
	: mCurrentState(-1), mStarted(false)
{
}

int StateMachine::AddState(StateAction* state)
{
	int id = mStates.size();
	mStates.push_back(State{state});
	return id;
}

void StateMachine::AddTransition(int fromID, int toID, StateTransitionFunc func)
{
	mStates[fromID].mTransitions.push_back(Transition{toID, func});
}


void StateMachine::Start(PlayerEntity* player, int initialStateID)
{
	if (mStarted) {
		std::cout << "State machine already started.\n";
		return;
	}

	mStarted = true;
	mCurrentState = initialStateID;

	BallEntity dummy;
	GetCurrentState()->start(*player, dummy);
}

void StateMachine::Update(PlayerEntity* player)
{
	if (!mStarted) return;

	float deltaTime = player->GetDeltaTime();
	BallEntity dummy;
	GetCurrentState()->update(*player, dummy, deltaTime);

	for (const Transition& t : mStates[mCurrentState].mTransitions) {
		if (t.mCondition(player)) {
			mCurrentState = t.mTargetStateID;
			mStates[mCurrentState].mStatePtr->start(*player, dummy);
			break;
		}
	}
}
