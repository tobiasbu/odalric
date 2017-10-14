
#ifndef _OAI_COMMANDER_H_
#define _OAI_COMMANDER_H_

#include <BWAPI.h>
#include "ActionContainer.h"
#include "ActionCommand.h"
#include "Singleton.hpp"
#include "UnitManager.h"

namespace Odalric {

	class Commander : public Singleton<Commander>
	{
	private:

		ActionContainer _actions;
		int _perfomedActions = 0;

		UnitManager _units;

		void trackOrderedActions();
		void performCommand(ActionCommand * action);
		bool performAction(Action * action);
		
	public:

		// initial state
		void init();
		void update();

		// callback when a action is unitialzed (append new action)
		void onActionUnitialized(Action * action);

		// callback when a action is in progress
		void onActionInProgress(Action * action);

		// callback when a action is ordered
		void onActionOrdered(Action * action);

		// callback when a action is done
		void onActionDone(Action * action);
		
		// callback when a action is updated
		void onActionUpdate(Action * action);

		int getPerfomedActions() const;
		const ActionContainer & getActionsContainer();
	};

}

#endif