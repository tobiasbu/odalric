
#ifndef _OAI_BUILDINGMANAGER_H_
#define _OAI_BUILDINGMANAGER_H_

#include "Resources.h"
#include "BuildingInfo.h"
#include "BuildingPlan.h"
#include "Singleton.hpp"


namespace Odarich {

	// manages buildings:
	// check for availble buildings, perform building action
	class BuildingManager : public Singleton<BuildingManager>
	{
	private:

		BuildingPlan _plan;

		// done buildings
		BuildingInfo _buildingInfo;
		
		// future buildings
		BuildingsList _futureBuildings;

		void validateConstructions();
		void assignWorkersToBuild();
		void constructFutureBuildings();
		void verifyStartedConstructions();
		void verifyDoneBuildings();
		BWAPI::TilePosition getBuildingPlace(const Building & building);

	public:
		
		

		BuildingManager();



		void addFuture(ActionBuild * action, const BWAPI::TilePosition & location);
		void addBuildingTask(BWAPI::UnitType type, BWAPI::TilePosition desiredLocation, bool isGasSteal);

		void onUnitMorph(BWAPI::Unit unit);
		void onUnitDestroy(BWAPI::Unit unit);
		
		void update();
		
	};

}

#endif