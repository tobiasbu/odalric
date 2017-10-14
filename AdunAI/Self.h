
#ifndef _OAI_SELF_H_
#define _OAI_SELF_H_

#include <BWAPI.h>
#include "Singleton.hpp"

namespace Odarich {

	class Self : public Singleton<Self>
	{
	private:

		bool hasInitialized = false;
		BWAPI::TilePosition startPostion;
		BWAPI::Player player;
		BWAPI::Race _race;

	public:

		//Throbi();
		void init();

		BWAPI::Player me();
		BWAPI::Race race();
		BWAPI::TilePosition startPosition() const;

	};

	// this is a bad hack
#define SELF Self::getInstance()

}

#endif