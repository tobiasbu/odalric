
#include "OdalricAIModule.h"
#include "TerrainAnalyzer.h"

namespace Odalric {

	OdalricAIModule::OdalricAIModule()
		:
		commander(Commander::getInstance()),
		workers(WorkersManager::getInstance()),
		bases(BaseManager::getInstance()),
		resourcesArea(ResourcesAreaManager::getInstance()),
		build(BuildManager::getInstance()),
		building(BuildingManager::getInstance()),
		tactical(TacticalData::getInstance()),
		scout(ScoutManager::getInstance()),
		strategy(Strategy::getInstance())
	{
		
	}

	OdalricAIModule::~OdalricAIModule()
	{
		Log::info("APPLICATION END");
		resourcesArea.dispose();
		TerrainAnalyzer::getInstance().dispose();
	}


	/*void OdalricAIModule::update()
	{
		// update workers
		BWAPI::Broodwar->drawTextScreen(200, 0, "FPS: %d", BWAPI::Broodwar->getFPS());
		BWAPI::Broodwar->drawTextScreen(200, 20, "WorkersManager: %d", workers.workersInfo.count());
		//BWAPI::Broodwar->drawTextScreen(200, 40, "Supplies: %d/%d/%d", SELF.me()->supplyUsed(), SELF.me()->supplyTotal(), (SELF.me()->supplyTotal() - SELF.me()->supplyUsed()));
		BWAPI::Broodwar->drawCircle(BWAPI::CoordinateType::Map, SELF.me()->getStartLocation().x, SELF.me()->getStartLocation().y, 350, BWAPI::Colors::Cyan);
		commander.update();

		WorkersManager::getInstance().update();


		
	}*/


	void OdalricAIModule::onStart()
	{
		// initialize log
		Log::start();

		// initialize self
		SELF.init();

		// initialize commander
		commander.init();

		// initialize resources
		resourcesArea.init();

		// intialize terrain analyzer
		TerrainAnalyzer::getInstance().init();

		// initialize tactical data
		tactical.init();

		strategy.init();

		// initiliaze metamap
		//MetaMap::getInstance().init();

		
		BWAPI::Broodwar->printf("Odalric bot. v%d.%d - bwapi 4.1.2", ODARICH_VERSION_MAJOR, ODARICH_VERSION_MINOR);
		//BWAPI::Broodwar->sendText("gl hf", text.c_str());

		BWAPI::Player enemy = BWAPI::Broodwar->enemy();

		if (enemy)
			BWAPI::Broodwar->sendText("gl hf, %s!", enemy->getName().c_str());


		// Print the map name.
		// BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
		//BWAPI::Broodwar << "The map is " << BWAPI::Broodwar->mapName() << "!" << std::endl;

		// Enable the UserInput flag, which allows us to control the bot and type messages.
		//BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

		// Uncomment the following line and the bot will know about everything through the fog of war (cheat).
		//Broodwar->enableFlag(Flag::CompleteMapInformation);

		// Set the command optimization level so that common commands can be grouped
		// and reduce the bot's APM (Actions Per Minute).
		BWAPI::Broodwar->setCommandOptimizationLevel(2);

		// Check if this is a replay
		/*if (BWAPI::Broodwar->isReplay())
		{

			// Announce the players in the replay
			BWAPI::Broodwar << "The following players are in this replay:" << std::endl;

			// Iterate all the players in the game using a std:: iterator
			BWAPI::Playerset players = BWAPI::Broodwar->getPlayers();


			for (auto p : players)
			{
				// Only print the player if they are not an observer
				if (!p->isObserver())
					BWAPI::Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;


			}

		}*/
		

	}

	void OdalricAIModule::onEnd(bool isWinner)
	{
		resourcesArea.dispose();
		TerrainAnalyzer::getInstance().dispose();

		// Called when the game ends
		if (isWinner)
		{
			// Log your win here!
		}
	}

	void OdalricAIModule::onFrame()
	{
		// Called once every game frame



		commander.update();

		workers.update();

		resourcesArea.update();

		build.update();

		building.update();

		strategy.update();

		scout.update();

		//TerrainAnalyzer::getInstance().draw(200, 16);

		//resourcesArea.draw(400,16);
		//bases.draw();
	
		//workers.draw(32, 64);
		
		//Resources::getInstance().draw();

		
	}

	void OdalricAIModule::onSendText(std::string text)
	{

		// Send the text to the game if it is not being processed.
		BWAPI::Broodwar->sendText("%s", text.c_str());


		// Make sure to use %s and pass the text as a parameter,
		// otherwise you may run into problems when you use the %(percent) character!

	}

	void OdalricAIModule::onReceiveText(BWAPI::Player player, std::string text)
	{
		// Parse the received text
		BWAPI::Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
	}

	void OdalricAIModule::onPlayerLeft(BWAPI::Player player)
	{
		// Interact verbally with the other players in the game by
		// announcing that the other player has left.
		BWAPI::Broodwar->sendText("Bye %s!", player->getName().c_str());
	}

	void OdalricAIModule::onNukeDetect(BWAPI::Position target)
	{

		// Check if the target is a valid position
		/*if (target)
		{
			// if so, print the location of the nuclear strike target
			BWAPI::Broodwar << "Nuclear Launch Detected at " << target << std::endl;
		}
		else
		{
			// Otherwise, ask other players where the nuke is!
			BWAPI::Broodwar->sendText("Where's the nuke?");
		}*/

		// You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
	}

	void OdalricAIModule::onUnitDiscover(BWAPI::Unit unit)
	{
	}

	void OdalricAIModule::onUnitEvade(BWAPI::Unit unit)
	{
	}

	void OdalricAIModule::onUnitShow(BWAPI::Unit unit)
	{
		workers.onUnitShow(unit);

		bases.onUnitShow(unit);

		tactical.onUnitShow(unit);
		//resourcesArea.onUnitShow(unit);
		//bases.onUnitShow(unit);
	}


	void OdalricAIModule::onUnitHide(BWAPI::Unit unit)
	{
	}

	void OdalricAIModule::onUnitCreate(BWAPI::Unit unit)
	{
		/*if (BWAPI::Broodwar->isReplay())
		{
			// if we are in a replay, then we will print out the build order of the structures
			if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
			{
				int seconds = BWAPI::Broodwar->getFrameCount() / 24;
				int minutes = seconds / 60;
				seconds %= 60;
				BWAPI::Broodwar->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
			}
		}*/
	}


	void OdalricAIModule::onUnitDestroy(BWAPI::Unit unit)
	{
		workers.onUnitDestroy(unit);
	}

	void OdalricAIModule::onUnitMorph(BWAPI::Unit unit)
	{
		workers.onUnitMorph(unit);

		/*if (BWAPI::Broodwar->isReplay())
		{
			// if we are in a replay, then we will print out the build order of the structures
			if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
			{
				int seconds = BWAPI::Broodwar->getFrameCount() / 24;
				int minutes = seconds / 60;
				seconds %= 60;
				BWAPI::Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
			}
		}*/
	}

	void OdalricAIModule::onUnitRenegade(BWAPI::Unit unit)
	{
	}

	void OdalricAIModule::onSaveGame(std::string gameName)
	{
		BWAPI::Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
	}

	void OdalricAIModule::onUnitComplete(BWAPI::Unit unit)
	{
	}


	
}