
#include "ActionCommand.h"
#include "ActionBuild.h"

namespace Odalric {

	ActionCommand::ActionCommand(const ActionCommand::ActionCommandType & command)
		:
		Action(Action::ActionType::Command)
	{
		_commandType = command;
	}

	ActionCommand::ActionCommandType ActionCommand::getCommand() const
	{
		return _commandType;
	}

	// FUTURE IMPLEMENTATION
	bool ActionCommand::checkPrerequiste()
	{
		if (_prerequisiteAction != nullptr && getPrerequisiteCondition() != Action::PreRequisite::None)
		{
			if (_prerequiseteArgs.size() > 0)
			{
				if (Action::PreRequisite::BuildRemainingTime)
				{
					//static_cast<ActionBuild*>(_prerequisiteAction)->get
				}
			}
			else
				return true;
		}
		else
			return true;

	}
}