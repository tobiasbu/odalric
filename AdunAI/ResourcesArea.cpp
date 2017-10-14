
#include "ResourcesArea.h"
#include "Log.h"
#include "Math.h"
#include "Utils.h"
#include "Base.h"

namespace Odalric {

	ResourcesArea::ResourcesArea()
		:
		_base(nullptr)
	{
		//_bounds.set(Vector2i(target.getPosition().x, target.getPosition().y), Vector2i(target.getPosition().x+1, target.getPosition().y+1));
	}

	ResourcesArea::ResourcesArea(Base * base)
	{
		_base = base;
		_bounds = base->get();
	}

	ResourcesArea::ResourcesArea(const BWAPI::PositionOrUnit & target)
	{
		this->target = target;

		if (target.isUnit()) {
			BWAPI::Unit unit = target.getUnit();

			

			if (!unit)
				_bounds.set(Vector2i(target.getPosition().x, target.getPosition().y), Vector2i(target.getPosition().x + 1, target.getPosition().y + 1));
			else {

				_bounds = unit;
			}
		}
		else {
			_bounds.set(Vector2i(target.getPosition().x, target.getPosition().y), Vector2i(target.getPosition().x + 1, target.getPosition().y + 1));
		}
	}


	void ResourcesArea::setBase(Base * base)
	{
		_base = base;
		_bounds.extend(base->get());
	}




	void ResourcesArea::add(BWAPI::Unit resource)
	{
		if (!resource)
			return;

		if (resource->getType() == BWAPI::UnitTypes::Resource_Mineral_Field)
			addMineral(resource);
		else if (resource->getType() == BWAPI::UnitTypes::Resource_Vespene_Geyser)
			addGas(resource);
	}

	void ResourcesArea::addSet(const BWAPI::Unitset & set)
	{
		for each (const auto & var in set)
		{
			if (!var)
				continue;

			if (var->getType() == BWAPI::UnitTypes::Resource_Mineral_Field)
				addMineral(var);
			else if (var->getType() == BWAPI::UnitTypes::Resource_Vespene_Geyser)
				addGas(var);
		}


	}

	void ResourcesArea::addMineral(BWAPI::Unit mineral)
	{
		if (!mineral)
			return;

		_minerals.insert(mineral);
		Bounds b(mineral->getType(), mineral->getTilePosition());
		b[0] *= 32;
		b[1] *= 32;
		_bounds = Bounds::merge(_bounds, b);
	}

	void ResourcesArea::addGas(BWAPI::Unit gas)
	{
		if (!gas)
			return;

		_gas.insert(gas);
		Bounds b(gas->getType(), gas->getTilePosition());
		b[0] *= 32;
		b[1] *= 32;
		_bounds = Bounds::merge(_bounds, gas);//extend(gas);
	}



	void ResourcesArea::checkResources()
	{
		//std::vector<BWAPI::Unit> remove;

		for each (auto u in _minerals)
		{
			if (!u || !u->exists())
				_minerals.erase(u);
		}
	}

	std::pair<bool, bool> ResourcesArea::hasFoundResources() const
	{
		return std::make_pair(_foundMinerals, _foundGas);
	}

	const Base * ResourcesArea::getBase() const
	{
		return _base;
	}

	BWAPI::TilePosition ResourcesArea::getBaseLocation() const
	{
		return _depotPosition;
	}

	Bounds ResourcesArea::getBounds() const
	{
		return _bounds;
	}

	const BWAPI::Unitset & ResourcesArea::getMinerals()
	{
		return _minerals;
	}

	const BWAPI::Unitset & ResourcesArea::getGas()
	{
		return _gas;
	}

}

/*void ResourcesArea::findResources()
{

	int radius = 350;
	//int gasRadius = 350;

	for (auto & unit : BWAPI::Broodwar->neutral()->getUnits())
	{
		//Log::info("res find unit: %s", unit->getType().getName().c_str());

		// too slow
		//unit->getDistance(target) <= radius

		// instead use simple manhattan distance 
		int dx = 0, dy = 0;

		Math::manhattan(
			unit->getPosition().x, unit->getPosition().y,
			target.getPosition().x, target.getPosition().y,
			dx, dy
		);

		if (unit->getType() == BWAPI::UnitTypes::Resource_Mineral_Field)
		{

			if (dx <= radius && dy <= radius)
			{
				_minerals.insert(unit);
			}
		}
		else if (unit->getType() == BWAPI::UnitTypes::Resource_Vespene_Geyser)
		{
			if (dx <= radius && dy <= radius)  // unit->getDistance(target) < gasRadius
			{
				_gas.insert(unit);


			}
		}
		else
			continue;
	}

	if (_minerals.size() > 0)
		_foundMinerals = true;
	else
		_foundMinerals = false;

	if (_gas.size() > 0)
		_foundGas = true;
	else
		_foundGas = false;

	Log::info("target: %d", target.getPosition().x);
	Log::info("minerals found: %d", _minerals.size());
	Log::info("gas found: %d", _gas.size());
}*/