
#ifndef _OAI_BUILDMANAGER_H_
#define _OAI_BUILDMANAGER_H_

#include "Resources.h"
#include "BuildOrder.h"
#include "BuildingInfo.h"
#include "Singleton.hpp"

//BuildingInfo buildingInfo;

namespace Odalric {

	// manages build operations:
	// retrieves build order, perform constructing actions
	class BuildManager : public Singleton<BuildManager>
	{
	private:

		Resources & resources;

		BuildJob * _currentJob = nullptr;

		void setupBuildJob();
		void jobFinished();

		void updateBuildOrder();

		BWAPI::Unit getGenerator(const MetaType & type);
		BWAPI::Unit getFarthest(const BWAPI::Unitset & units, BWAPI::Position farthest);
		BWAPI::Unit getClosest(const BWAPI::Unitset & units, BWAPI::Position position);

		bool canBeBuilded(BWAPI::Unit generator, const MetaType & type);
		bool generate(BWAPI::Unit generator, BuildJob * job);

		bool doTrain(const MetaType & type);
		bool doBuild(const MetaType & type);

	public:

		BuildOrder buildOrder;
		

		BuildManager();

		void update();
		
	};

}

#endif
