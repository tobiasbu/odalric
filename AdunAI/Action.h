
#ifndef _OAI_ACTION_H_
#define _OAI_ACTION_H_

#include "Types.h"
#include "FunctionTraits.h"
#include "Any.h"
#include <vector>

// base class for actions

namespace Odarich {

	

	class Action
	{
		friend class ActionContainer;

	public:


		enum ActionState
		{
			Unitialized,
			InProgress,
			Blocked,
			Ordered,
			Done,
			Failed
		};

		enum ActionType
		{
			Build,
			Command
		};

		enum PreRequisite
		{
			None,
			BuildRemainingTime,
			ActionOrdered
		};

	private:

		
		ActionType _type;
		ActionState _state = Unitialized;
		PreRequisite _prerequisiteCondition = None;

	protected:

		int32 _id;
		//void * _prerequisteCondition = nullptr;
		std::vector<any> _prerequiseteArgs;
		Action * _prerequisiteAction = nullptr;
		
	public:

		Action(const ActionType & type);
		void setState(const ActionState & state);
		void setPrerequisiteAction( Action * prerequiste);

		/*template<class Fun, class... Args>
		void setPrerequisteCondition(Fun && function, Args&&... args)
		{
			//std::tuple<Args...> store(args...);
		
			//function(std::forward<Args>(args)...);
			_prerequiseteArgs.clear();

			_prerequiseteArgs = { args... };
			_prerequisteCondition = function; // prerequisteCondition;

			//_prerequisteCondition = std::bind(function, args);
		}*/

		template<class... Args>
		void setPrerequisteCondition(const PreRequisite & prerequiste, Args&&... args)
		{
			_prerequiseteArgs.clear();
			_prerequiseteArgs = { args... };
			_prerequisiteCondition = prerequiste;
		}

		const Action::ActionState & getState() const;
		uint32 getID() const;
		const Action::ActionType & getActionType() const;
		const Action * getPrerequisteAction() const;
		PreRequisite getPrerequisiteCondition() const;
		virtual bool checkPrerequiste();
		

	};

}

#endif