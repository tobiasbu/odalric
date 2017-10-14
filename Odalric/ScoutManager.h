
#ifndef _OAI_SCOUTMANAGER_H_
#define _OAI_SCOUTMANAGER_H_

#include <BWAPI.h>
#include <unordered_map>
#include "Singleton.hpp"




namespace Odalric {

	

	struct ScoutPath
	{
		std::vector<BWAPI::Position> waypoints;
		ScoutPath()
		{

		}
	};

	struct ScoutUnit
	{
		BWAPI::Unit unit;
		int scoutPathIndex;
		BWAPI::Position toExplore;
		ScoutUnit(BWAPI::Unit unit, const int & pathIndex);
		void update();

	};

	typedef std::unordered_map<BWAPI::Unit, ScoutUnit> ScoutMap;
	typedef std::vector<ScoutPath> ScoutPaths;

	class ScoutManager : public Singleton<ScoutManager>
	{
	public:

		enum ScoutState
		{
			FindEnemyBase,
			StealGas,
			Explore
		};

	private:

		
		ScoutMap _scouts;
		ScoutPaths _paths;

		void remove(ScoutMap::iterator & it);
		void getNextLocationToExplore(ScoutUnit & unit);

	public:

		ScoutManager();
		
		void add(BWAPI::Unit unit);
		void release(BWAPI::Unit unit);
		
		void update();
	};

}

#endif