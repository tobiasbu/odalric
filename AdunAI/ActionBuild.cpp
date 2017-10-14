
#include "ActionBuild.h"

namespace Odalric {




	ActionBuild::ActionBuild(const MetaType & metaType)
		:
		Action(ActionType::Build),
		_meta(metaType)
		
	{
		_amount = 1;
	}

	ActionBuild::ActionBuild(const MetaType & metaType, const int32 & id)
		:
		Action(ActionType::Build),
		_meta(metaType)	
	{
		_id = id;
	}

	void ActionBuild::setAmount(const int & amount)
	{
		_amount = amount;

		if (_amount <= 0)
			_amount = 1;

		
	}

	void ActionBuild::setBuildCondition(const void * buildCondition)
	{
		_buildCondition = buildCondition;
	}

	const void * ActionBuild::getBuildCondition() const
	{
		return _buildCondition;

	}

	const MetaType & ActionBuild::getMetaType() const
	{
		return _meta;
	}

	int ActionBuild::getAmount() const
	{
		return _amount;
	}

	int ActionBuild::getRemainingTime() const
	{
		return BWAPI::Broodwar->getFrameCount() - (_startTime + _meta.buildTime());
	}



	/*void Action::setState(const ActionState & state)
	{
		_state = state;

		if (state == InProgress)
		{
			_startTime = BWAPI::Broodwar->getFrameCount();
		}
	}*/

	/*void Action::setAttributes(ActionAttributes * attributes)
	{
		_attributes = attributes;
	}*/



	/*void Action::setStartTime(const int & startTime)
	{
		_startTime = startTime;
	}*/

	




	/*ActionAttributes * Action::getAttributes() const
	{
		return _attributes;
	}*/



}