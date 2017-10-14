
#ifndef _OAI_BUILDINGINFO_H_
#define _OAI_BUILDINGINFO_H_

#include "Building.h"
#include <unordered_map>

namespace Odarich {

	typedef std::vector<Building> BuildingsList;

	class BuildingInfo
	{
	private:

		BuildingsList _buildings;
		//std::unordered_map<BWAPI::Unit

	public:
		BuildingInfo();

		void add(const Building & building);
		void remove(const Building & building);
		bool isAlreadyBuilt(const BWAPI::UnitType & type) const;
		bool isUnderConstruction(const BWAPI::UnitType & type) const;
		BuildingsList & getBuildings();

	};

}

#endif