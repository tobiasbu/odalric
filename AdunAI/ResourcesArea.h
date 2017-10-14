
#ifndef _OAI_RESOURCESAREA_H_
#define _OAI_RESOURCESAREA_H_

#include <BWAPI.h>
#include "Bounds.h"

namespace Odarich {

	class Base;

	class ResourcesArea
	{

		friend class TerrainAnalyzer;

	private:

		BWAPI::TilePosition _depotPosition = BWAPI::TilePositions::None;
		Base * _base = nullptr;
		Bounds _bounds;
		bool _foundMinerals = false;
		bool _foundGas = false;

		BWAPI::PositionOrUnit target;
		BWAPI::Unitset _minerals;
		BWAPI::Unitset _gas;
		

	public:

		ResourcesArea();
		ResourcesArea(Base * base);
		ResourcesArea(const BWAPI::PositionOrUnit & target);

		void setBase(Base * base);
		void add(BWAPI::Unit resource);
		void addSet(const BWAPI::Unitset & set);
		void addMineral(BWAPI::Unit mineral);
		void addGas(BWAPI::Unit gas);
		
		
		void checkResources();

		Bounds getBounds() const;
		const BWAPI::Unitset & getMinerals();
		const BWAPI::Unitset & getGas();
		const Base * getBase() const;
		BWAPI::TilePosition getBaseLocation() const;

		std::pair<bool,bool> hasFoundResources() const;

	
	};

	typedef std::vector<ResourcesArea*> ResourcesAreasList;

}

#endif