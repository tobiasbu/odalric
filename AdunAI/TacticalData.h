
#ifndef _OAI_TACTICALDATA_H_
#define _OAI_TACTICALDATA_H_

#include <BWAPI.h>
#include <unordered_set>
#include "Singleton.hpp"
#include "Types.h"

namespace Odalric {

	struct EnemyInfo
	{
		BWAPI::Player player;
		BWAPI::TilePosition baseLocation;

		EnemyInfo(BWAPI::Player enemy);
	};

}

namespace std
{

	template<>
	struct hash<Odalric::EnemyInfo>
	{
		BWAPI::Player operator()(Odalric::EnemyInfo const & x) const
		{
			return x.player;

		}


	};

	//template <class U>
	template<>
	struct equal_to<Odalric::EnemyInfo> : public unary_function<Odalric::EnemyInfo, bool>
	{
		bool operator()(const Odalric::EnemyInfo& x, const Odalric::EnemyInfo& y) const
		{
			return  (x.player == y.player);
		}
	};
}

namespace Odalric {

	typedef std::vector<EnemyInfo> EnemyList;

	class TacticalData : public Singleton<TacticalData>
	{
	private:

		std::vector<BWAPI::TilePosition> _enemyBaseLocations;
		EnemyList _enemies;
		
		BWAPI::TilePosition getPossibleDifferenceBetweenBases(const int & locationsSize, const BWAPI::TilePosition & myStartLocation, BWAPI::TilePosition & difference);

		void baseFound(BWAPI::Unit unit);
		

	public:

		TacticalData();
		void init();
		
		uint32 enemiesCount() const;
		EnemyList & getEnemies();
		EnemyInfo * getEnemy();
		const std::vector<BWAPI::TilePosition> & getEnemiesPossibleLocations() const;
		const BWAPI::TilePosition & getEnemyMainBaseLocation() const;

		void onUnitShow(BWAPI::Unit unit);
	};

}




#endif