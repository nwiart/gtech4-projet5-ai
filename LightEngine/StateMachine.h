#pragma once

#include <vector>

class PlayerEntity;
class StateAction;

typedef bool (*StateTransitionFunc)(PlayerEntity*);

class StateMachine
{
public:

	struct Transition
	{
		int mTargetStateID;
		StateTransitionFunc mCondition;
	};

	struct State
	{
		StateAction* mStatePtr;
		std::vector<Transition> mTransitions;
	};

	StateMachine();

	int AddState(StateAction* state);

	void AddTransition(int fromID, int toID, StateTransitionFunc func);


	void Start(PlayerEntity* player, int initialStateID);

	void Update(PlayerEntity* player);

	inline StateAction* GetCurrentState() const { return mStates[mCurrentState].mStatePtr; }


private:

	std::vector<State> mStates;

	int mCurrentState;
	bool mStarted;
};
