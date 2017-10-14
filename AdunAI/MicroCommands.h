
#ifndef _OAI_MICROCOMMANDS_HPP_
#define _OAI_MICROCOMMANDS_HPP_

#include "Commons.h"


namespace Odalric {

	namespace MicroCommands
	{
		BWAPI::Unit getClosestEnemy(BWAPI::Unit & myUnit, const int & minRadius = 300);
		void attack(BWAPI::Unit & myUnit, BWAPI::Unit & enemyUnit);
		void rightClick(BWAPI::Unit unit, BWAPI::Unit target);
		void move(BWAPI::Unit unit, const BWAPI::Position & target);
	}

}

#endif