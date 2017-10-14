
#ifndef _OAI_ACTIONCONTAINER_H_
#define _OAI_ACTIONCONTAINER_H_

#include "Action.h"
#include <unordered_set>
#include <vector>

namespace Odarich {


	typedef std::unordered_set<Action*> ActionSet;

	class ActionContainer
	{
		int _actionIdentifier;
		std::vector<Action*> _done;
		ActionSet _ordered;
		ActionSet _unitialized;
		ActionSet _inProgress;

	public:

		ActionContainer();
		void add(Action * action);
		void removeUnitialized(Action * action);
		void removeInProgress(Action * action);
		void removeOrdered(Action * action);
		void done(Action * doneAction);
		void ordered(Action * orderedAction);
		void inProgress(Action * actionInProgress);
		void clear();
		const bool unitializedEmpty() const;
		const int32 unitializedSize() const;
		const bool inProgressEmpty() const;
		const int32 inProgressSize() const;

		ActionSet & getOrderedActions();
		ActionSet & getUnitializedActions();
		ActionSet & getInProgressActions();

	};

}

#endif