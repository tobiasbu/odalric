
#include "Self.h"
#include "Log.h"

namespace Odalric 
{
	/*Throbi::Throbi()
	{
		init();
	}*/

	void Self::init()
	{
		if (!hasInitialized) {
			
			player = BWAPI::Broodwar->self();
			_race = BWAPI::Broodwar->self()->getRace();
			startPostion = BWAPI::Broodwar->self()->getStartLocation();
			hasInitialized = true;
		}
	}

	BWAPI::Player Self::me()
	{
		return player;
	}

	BWAPI::Race Self::race()
	{
		return _race;
	}

	BWAPI::TilePosition Self::startPosition() const
	{
		return startPostion;
	}
}