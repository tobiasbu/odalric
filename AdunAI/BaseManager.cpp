


#include "BaseManager.h"
#include "Commons.h"
#include "Math.h"
#include "Draw.h"

namespace Odalric {

	BaseManager::BaseManager()
	{

	}

	void BaseManager::add(const BWAPI::Unit & base)
	{

		if (!base)
			return;

		if (!base->getType().isResourceDepot())
			return;
		//std::pair<BWAPI::Unitset::iterator, bool> it = _bases.insert(base);
		BasesMap::iterator it = _bases.find(base);


		if (it == _bases.end())
		{
			std::pair<std::unordered_map<BWAPI::Unit, Base>::iterator, bool> it = _bases.insert(std::make_pair(base, Base(base)));

			if (it.second) {

				if (_bases.size() == 1)
				{
					Log::info("added main base");

					_mainBase = &(it.first->second);

					//_mainBase->setupResources(new ResourcesArea(it.first->first));
				}
				else
				{
					Log::info("added another base");
				}

			}


		}
	}

	

	int BaseManager::count() const
	{
		return _bases.size();
	}

	Base * BaseManager::getMainBase() const
	{
		return _mainBase;
	}

	BasesMap & BaseManager::getBases()
	{
		return _bases;
	}

	Base * BaseManager::getClosest(const BWAPI::PositionOrUnit & pos)
	{
		//float closeDistance = 0;
		int closeDistx = 100000, closeDisty = 100000;
		int distx, disty;
		Base * closeBase = nullptr;
		BWAPI::Position unitpos = pos.getPosition();

		// if position is none, return the main base
		if (pos.getPosition() == BWAPI::Positions::None)
		{
			return _mainBase;
		}
		else {

			for (BasesMap::iterator it = _bases.begin(); it != _bases.end(); ++it)
			{
				//BWAPI::Position pos = it->first->getPosition();

				// too slow: float dist = (float)it->first->getDistance(unit);

				Math::manhattan(it->first->getPosition(), unitpos, distx, disty);

				if (!closeBase || (distx < closeDistx && disty < closeDisty))
				{
					closeBase = &it->second;
					closeDistx = distx;
					closeDisty = disty;
				}
			}

			return closeBase;

		}
	}

	void BaseManager::onUnitShow(const BWAPI::Unit & unit)
	{
		if (unit->getType().isResourceDepot() && unit->getPlayer() == SELF.me())
		{
			add(unit);
		}
	}

	void BaseManager::draw()
	{
		if (_mainBase->hasBuildBounds())
		{
			//Log::info("BOUNDS %d", _mainBase->getBuildBounds().size());

			for each (auto var in _mainBase->getBuildBounds())
			{
				Draw::tiledBounds(var,BWAPI::Colors::Orange, false);
			}
		}
	}
}