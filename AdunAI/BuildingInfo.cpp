#include "BuildingInfo.h"

namespace Odarich {

	BuildingInfo::BuildingInfo()
	{
	}

	void BuildingInfo::add(const Building & building)
	{
		_buildings.push_back(building);
	}

	void BuildingInfo::remove(const Building & building)
	{
		auto & b = std::find(_buildings.begin(), _buildings.end(), building);

		if (b != _buildings.end())
		{
			_buildings.erase(b);
		}
	}

	bool BuildingInfo::isAlreadyBuilt(const BWAPI::UnitType & type) const
	{
		for (auto & b : _buildings)
		{
			if (b.getType() == type && b.isDone())
			{
				return true;
			}
		}

		return false;
	}

	bool BuildingInfo::isUnderConstruction(const BWAPI::UnitType & type) const
	{
		for (auto & b : _buildings)
		{
			if (b.getType() == type && b.isUnderConstruction())
			{
				return true;
			}
		}

		return false;
	}

	BuildingsList & BuildingInfo::getBuildings()
	{
		return _buildings;
	}

}