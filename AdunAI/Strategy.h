
#ifndef _OAI_STRATEGY_H_
#define _OAI_STRATEGY_H_


#include "ActionBuild.h"
#include "ActionCommand.h"
#include "CountContainer.h"
#include "Singleton.hpp"

#include <BWAPI.h>
#include <vector>

namespace Odarich {


	class Strategy : public Singleton<Strategy>
	{
	private:

		enum FutureActionType
		{
			MineralWorker,
			GasWorker,
			Refinary
		};

		int32 _nextSearch = 0;
		int32 _actionsSent = 0;
		CountMap<int> _counter;
		std::vector<std::pair<Action*,int>> _futureActions;
		void countUnits();
		void populateCountMap();
		void pontuateActions();
		void appendFutureAction(Action * action, const FutureActionType & type);
		void sendToCommander(Action * action);
		void checkAppendedActions();
		void basicGoals();
		void terranGoals();

	public:

		void init();
		void update();

		void searchNextBuildActions();
		static ActionBuild * makeBuildAction(const MetaType & metaType, const void * buildCondition = nullptr);
		static Action * makeCommandAction(const ActionCommand::ActionCommandType & commandType);
	};

}

#endif