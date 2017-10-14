
#include "ActionContainer.h"
#include "Assert.h"

namespace Odalric {

	ActionContainer::ActionContainer()
	{
		_actionIdentifier = 0;
	}

	void ActionContainer::add(Action * action)
	{
		OAI_ASSERT(action != nullptr);

		action->_id = _actionIdentifier;

		_unitialized.insert(action);

		_actionIdentifier++;
	}

	void ActionContainer::removeUnitialized(Action * action)
	{
		OAI_ASSERT(action != nullptr);

		ActionSet::iterator it = _unitialized.find(action);

		if (it != _unitialized.end())
		{
			_unitialized.erase(it);
		}
	}

	void ActionContainer::removeInProgress(Action * action)
	{
		OAI_ASSERT(action != nullptr);

		ActionSet::iterator it = _inProgress.find(action);

		if (it != _inProgress.end())
		{
			_inProgress.erase(it);
		}
	}

	void ActionContainer::removeOrdered(Action * action)
	{
		OAI_ASSERT(action != nullptr);

		ActionSet::iterator it = _ordered.find(action);

		if (it != _ordered.end())
		{
			_ordered.erase(it);
		}
	}

	void ActionContainer::done(Action * doneAction)
	{
		OAI_ASSERT(doneAction != nullptr);

		doneAction->setState(Action::ActionState::Done);

		_done.push_back(doneAction);
	}

	void ActionContainer::ordered(Action * orderedAction)
	{
		orderedAction->setState(Action::ActionState::Ordered);
		_ordered.insert(orderedAction);
	}

	void ActionContainer::inProgress(Action * actionInProgress)
	{
		OAI_ASSERT(actionInProgress != nullptr);

		actionInProgress->setState(Action::ActionState::InProgress);

		_inProgress.insert(actionInProgress);
	}

	void ActionContainer::clear()
	{
		for each(auto action in _done)
		{
			delete action;
		}

		_done.clear();

		for each(auto action in _inProgress)
		{
			delete action;
		}

		_inProgress.clear();

		for each(auto action in _unitialized)
		{
			delete action;
		}

		_unitialized.clear();
	}

	const bool ActionContainer::unitializedEmpty() const
	{
		return _unitialized.empty();
	}

	const int32 ActionContainer::unitializedSize() const
	{
		return _unitialized.size();
	}

	const bool ActionContainer::inProgressEmpty() const
	{
		return _inProgress.empty();
	}

	const int32 ActionContainer::inProgressSize() const
	{
		return _inProgress.size();
	}

	ActionSet & ActionContainer::getOrderedActions()
	{
		return _ordered;
	}

	ActionSet & ActionContainer::getUnitializedActions()
	{
		return _unitialized;
	}

	ActionSet & ActionContainer::getInProgressActions()
	{
		return _inProgress;
	}

}