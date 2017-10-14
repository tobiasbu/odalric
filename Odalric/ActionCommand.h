
#ifndef _OAI_ACTIONCOMMAND_H_
#define _OAI_ACTIONCOMMAND_H_


#include "Action.h"

namespace Odalric {



	class ActionCommand : public Action
	{
		friend class ActionContainer;




	public:

		enum class ActionCommandType
		{
			SendScout
		};

	private:

		ActionCommandType _commandType;

	public:

		//ActionAttributes attributes;

		ActionCommand(const ActionCommandType & command);
	
		ActionCommandType getCommand() const;

		bool checkPrerequiste();


	};

}

#endif