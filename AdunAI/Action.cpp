
#include "Action.h"
#include "FunctionTraits.h"

namespace Odarich {

	Action::Action(const Action::ActionType & type)
		:
		_type(type),
		_id(0),
		_state(ActionState::Unitialized),
		_prerequisiteAction(nullptr),
		_prerequisiteCondition(Action::PreRequisite::None)
	{

	}

	void Action::setState(const ActionState & state)
	{
		_state = state;
	}

	void Action::setPrerequisiteAction(Action * prerequiste)
	{
		_prerequisiteAction = prerequiste;
	}

	const Action::ActionState & Action::getState() const
	{
		return _state;
	}

	uint32 Action::getID() const
	{
		return _id;
	}
	const Action::ActionType & Action::getActionType() const
	{
		return _type;
	}
	
	const Action * Action::getPrerequisteAction() const
	{
		return _prerequisiteAction;
	}

	Action::PreRequisite Action::getPrerequisiteCondition() const
	{
		return _prerequisiteCondition;
	}
	
	bool Action::checkPrerequiste()
	{
		
		if (_prerequisiteAction != nullptr && _prerequisiteCondition != None)
		{
			if (_prerequisiteCondition == PreRequisite::ActionOrdered)
			{
				return _prerequisiteAction->getState() == ActionState::Ordered;

				//Method<bool, int>m
				//int t = 0;
				//bool t = (bool)_prerequisteCondition;

				//(Method<bool, int>::type)&&(_prerequisteCondition)(0); //(rand() % 100); //   (0)();// _prerequiseteArgs[0].get_int());

				//typedef bool(*oai_condition)(...);

				//typedef bool(*oai_condition_int)(int);

				//((oai_condition_int)_prerequisteCondition)(rand() % 100);
			}
			
		}
		else
			return true;


	}
}