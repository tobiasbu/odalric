
#include "UnitManager.h"
#include "WorkersManager.h"
#include "BaseManager.h"

namespace Odalric {

	void UnitManager::add(BWAPI::Unit unit)
	{
		const BWAPI::UnitType & type = unit->getType();

		if (type.isWorker())
		{
			WorkersManager::getInstance().workersInfo.add(unit);
		}
		else if (type.isResourceDepot())
		{
			BaseManager::getInstance().add(unit);
		}
	}
	

}