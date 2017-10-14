
#ifndef _OAI_AIMODULE_H_
#define _OAI_AIMODULE_H_

#include <BWAPI.h>
#include "Commander.h"
#include "WorkersManager.h"
#include "BaseManager.h"
//#include "MetaMap.h"
#include "ResourcesAreaManager.h"
#include "BuildManager.h"
#include "BuildingManager.h"
#include "TacticalData.h"
#include "ScoutManager.h"
#include "Strategy.h"

namespace Odalric {

	class OdalricAIModule : public BWAPI::AIModule
	{
	private:

		Commander & commander;
		WorkersManager & workers;
		BaseManager & bases;
		ResourcesAreaManager & resourcesArea;
		BuildManager & build;
		BuildingManager & building;
		TacticalData & tactical;
		ScoutManager & scout;
		Strategy & strategy;

	public:

		

		OdalricAIModule();
		~OdalricAIModule();

		//void init();
		//void update();

		virtual void onStart();
		virtual void onEnd(bool isWinner);
		virtual void onFrame();
		virtual void onSendText(std::string text);
		virtual void onReceiveText(BWAPI::Player player, std::string text);
		virtual void onPlayerLeft(BWAPI::Player player);
		virtual void onNukeDetect(BWAPI::Position target);
		virtual void onUnitDiscover(BWAPI::Unit unit);
		virtual void onUnitEvade(BWAPI::Unit unit);
		virtual void onUnitShow(BWAPI::Unit unit);
		virtual void onUnitHide(BWAPI::Unit unit);
		virtual void onUnitCreate(BWAPI::Unit unit);
		virtual void onUnitDestroy(BWAPI::Unit unit);
		virtual void onUnitMorph(BWAPI::Unit unit);
		virtual void onUnitRenegade(BWAPI::Unit unit);
		virtual void onSaveGame(std::string gameName);
		virtual void onUnitComplete(BWAPI::Unit unit);

	};

}

#endif