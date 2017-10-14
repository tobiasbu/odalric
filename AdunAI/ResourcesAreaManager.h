
#ifndef _OAI_RESOURCESAREAMANAGER_H_
#define _OAI_RESOURCESAREAMANAGER_H_

#include "ResourcesArea.h"
#include "Singleton.hpp"
#include <thread>
#include <mutex>

namespace Odarich {

	

	class ResourcesAreaManager : public Singleton<ResourcesAreaManager>
	{
		friend class TerrainAnalyzer;

	private:

		BWAPI::Unitset _minerals;
		BWAPI::Unitset _gas;
		BWAPI::Unitset _unanalyzedResources;
		BWAPI::Unitset _analyzedResources;
		int _unanalyzedCount = 0;
		int _nextFrameAnalyze = 0;
		ResourcesAreasList _resourcesAreas;

		bool _runTask = true;
		bool _taskRunning = false;
		bool _taskFinished = false;
		short _taskStatus = 0;
		//std::mutex _mutex;
		std::thread * _taskAnalyzeResourcesArea = nullptr;

		void taskJoinClusters();
		bool checkResourceOnArea(ResourcesArea * area, BWAPI::Unit resource);
		

	public:

		ResourcesAreaManager();
		~ResourcesAreaManager();

		void init();

		void addSet(const BWAPI::Unitset & set);
		void addMineral(const BWAPI::Unit & mineral);		
		void addGas(const BWAPI::Unit & gas);

		void update();

		// only execute this for first base
		ResourcesArea * findResourcesAt(const BWAPI::PositionOrUnit & target, const int & maxRadius = 300);
		
		const ResourcesAreasList & getResourcesAreas();
		const BWAPI::Unitset & getMinerals();
		const BWAPI::Unitset & getGas();
		short getTaskStatus() const;
		bool isTaskFinished() const;
		static BWAPI::Unit getNearestResourceFromAll(const BWAPI::PositionOrUnit & from, const BWAPI::UnitType & type, const int & maxRadius = 10000);

		void onUnitShow(const BWAPI::Unit & unit);		

		void draw(const int & x = 200, const int & y = 16);
		void dispose();
	};

}

#endif