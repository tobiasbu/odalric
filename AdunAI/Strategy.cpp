


#include "Strategy.h"
#include "Commander.h"
#include "Self.h"
#include "BaseManager.h"
#include "Resources.h"
#include "Log.h"

bool refinary = false;

namespace Odarich {

	ActionBuild * Strategy::makeBuildAction(const MetaType & metaType, const void * buildCondition)
	{
		ActionBuild * action = new ActionBuild(metaType);
		action->setBuildCondition(buildCondition);
		return action;
	}

	Action * Strategy::makeCommandAction(const ActionCommand::ActionCommandType & commandType)
	{
		ActionCommand * action = new ActionCommand(commandType);
		return action;
	}

	void Strategy::populateCountMap()
	{
		// populate with basic types
		_counter.set(SELF.race().getCenter().getID(), 0);
		_counter.set(SELF.race().getWorker().getID(), 0);
		_counter.set(SELF.race().getSupplyProvider().getID(), 0);
		_counter.set(SELF.race().getRefinery().getID(), 0);

		if (SELF.race() == BWAPI::Races::Terran)
		{
			_counter.set(BWAPI::UnitTypes::Terran_Barracks.getID(), 0);
			_counter.set(BWAPI::UnitTypes::Terran_Academy.getID(), 0);
			_counter.set(BWAPI::UnitTypes::Terran_Marine.getID(), 0);
			_counter.set(BWAPI::UnitTypes::Terran_Medic.getID(), 0);
			_counter.set(BWAPI::UnitTypes::Terran_Bunker.getID(), 0);
		}
	
	}

	void Strategy::init()
	{
		populateCountMap();
	}

	void Strategy::countUnits()
	{
		_counter.resetAll();

		for each (auto u in SELF.me()->getUnits())
		{
			_counter(u->getType().getID())++;

			if (u->getRemainingTrainTime() > 0)
			{
				BWAPI::UnitType trainType = u->getLastCommand().getUnitType();

				if (trainType == u->getType() && u->getRemainingTrainTime() == trainType.buildTime())
				{
					_counter(u->getType().getID())++;
				}
			}


		}
	}

	void Strategy::update()
	{
		if (Commander::getInstance().getPerfomedActions() < 3)
			return;
		else 
		{
			if (BWAPI::Broodwar->getFrameCount() >= _nextSearch)
			{
				// count current units
				countUnits();

				basicGoals();

				if (SELF.race() == BWAPI::Races::Terran)
				{
					terranGoals();
				}

				checkAppendedActions();

				// schedule next actions
				_nextSearch = BWAPI::Broodwar->getFrameCount() + 48;
			}
		}
	}

	void Strategy::basicGoals()
	{
		BasesMap & bases = BaseManager::getInstance().getBases();

		int mineralWorkers = 0;
		int gasWorkers = 0;
		int otherWorkers = 0;
		bool depotFull = false;

		// check for each base the number of mineral and gas workers
		for each (auto base in bases)
		{
			ResourcesArea * area = base.second.getResources();
			int mineralsFieldCount = area->getMinerals().size();
			int geysersCount = area->getGas().size();

			mineralWorkers = base.second.getWorkersJobCount(Worker::GatherMineral);
			gasWorkers = base.second.getWorkersJobCount(Worker::GatherGas);

			//Log::info("%d , %d",mineralWorkers, gasWorkers);
			 
			if (mineralsFieldCount > 10)
			{
				mineralsFieldCount = 10;
			}

			if (!base.second.isFull(Worker::GatherMineral))
			{
				if (mineralWorkers < mineralsFieldCount)
				{
					ActionBuild * action = makeBuildAction(SELF.race().getWorker());
					action->setAmount(mineralsFieldCount - mineralWorkers);
					appendFutureAction(action, MineralWorker);
				}
			}

			if (!base.second.isFull(Worker::GatherGas))
			{
				int refinaries =  base.second.refinariesCount();

				if (geysersCount >= 1) {

					if (refinaries == 0) {

						if (base.second.refinariesUnderConstruction() == 0)
						{
							if (!refinary)
							{
								Log::info("refinary");
								ActionBuild * action = makeBuildAction(SELF.race().getRefinery());
								appendFutureAction(action, Refinary);


								ActionBuild * action2 = makeBuildAction(SELF.race().getWorker());
								action2->setAmount(3);
								appendFutureAction(action2, GasWorker);
								refinary = true;
							}
							
						}
					} 

					
				}
			}
		}

		
	}

	void Strategy::terranGoals()
	{

	}

	void Strategy::checkAppendedActions()
	{
		
		_actionsSent = 0;
		Resources::getInstance().update();
		int futureMineralSpent = 0;
		int futureGasSpent = 0;
		int futureSuppliesSpent = 0;
		

		for each (auto & act in _futureActions)
		{
			if (_actionsSent < 20)
			{

				if (act.first->getActionType() == Action::ActionType::Build)
				{

					ActionBuild * actBuild = static_cast<ActionBuild*>(act.first);
					const MetaType & meta = actBuild->getMetaType();
					bool requireSupply = false;

					if (futureMineralSpent <= Resources::getInstance().getAvailbleMinerals() &&
						futureGasSpent <= Resources::getInstance().getAvailbleMinerals()) 
					{
						if (meta.isUnit())
						{
							int suppliesSpent = meta.supplyRequired() * actBuild->getAmount();

							if (suppliesSpent+futureSuppliesSpent > Resources::getInstance().getAvailbleSupplies())
							{
								requireSupply = true;
							}
							else {
								futureSuppliesSpent += suppliesSpent;
							}
						}

						if (!requireSupply)
						{
							int mineralAction = actBuild->getAmount() * actBuild->getMetaType().mineralPrice();
							int gasAction = actBuild->getAmount() * actBuild->getMetaType().gasPrice();

							if (mineralAction + futureMineralSpent <= Resources::getInstance().getAvailbleMinerals() &&
								gasAction + futureGasSpent <= Resources::getInstance().getAvailbleMinerals())
							{
								futureMineralSpent += mineralAction;
								futureGasSpent += gasAction;
							}

							sendToCommander(act.first);

							if (act.second == MineralWorker)
							{
								
							}
						}
					}

				}
			}
			else
				break;

		}
	}

	void Strategy::sendToCommander(Action * action)
	{
		Commander::getInstance().onActionUnitialized(action);
		_actionsSent++;
	}

	void Strategy::appendFutureAction(Action * action, const FutureActionType & type)
	{
		_futureActions.push_back(std::make_pair(action,type));
	}



}