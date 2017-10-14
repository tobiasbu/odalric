
#include "BuildManager.h"
#include "Commander.h"
#include "BuildingManager.h"

namespace Odalric {


	BuildManager::BuildManager()
		:
		resources(Resources::getInstance())
	{

	}


	void BuildManager::setupBuildJob()
	{
		if (!buildOrder.empty())
		{
			// has job to do, but is not work in progress
			if (_currentJob == nullptr) {


				//_jobsToDo.push(JobToDo(queue.front()));

				Log::info("START build");
				_currentJob = buildOrder.front();
				//buildOrder.setCurrentJob(&buildOrder..value); // static_cast<BuildJob*>(
				resources.update();				 //_currentJob.set(queue.front());

				if (_currentJob->getType().isUnit()) { //  == BuildJob::BuildJobType::Unit



													   //Log::info("unit");
													   // std::pair<std::unordered_set<BWAPI::UnitType>::iterator, bool> it 
													   //std::pair<std::unordered_set<BWAPI::UnitType>::iterator, bool> it = _metaUnitTypes.insert(_currentJob->getType());
													   //BWAPI::UnitType unit = _currentJob->getType();

					const BWAPI::UnitType & type = _currentJob->getType().getUnitType();///meta.getUnit(_currentJob->getType());
																						//const BWAPI::UnitType * type = &SELF.race().getWorker();

																						//Log::info(type->toString());

					if (_currentJob->getAction()->getBuildCondition() == QueueConditions::reachMaxSupplies)
					{
						//Log::info("has condition");
						int max = resources.getAvailbleSupplies() / type.supplyRequired();
						Log::info("max: %d", max);
						_currentJob->setMaxAmount(max);  //_availbleSupplies / type.supplyRequired();
														 //_reservedSupplies += _currentJob->_maxAmount;
						resources.reserveSupplies(max);
					}


				}
				/*else if (_currentJob->getType().isBuilding())
				{

				}*/
			}
			else
				return;

		}
		else
		{
			_currentJob = nullptr;
		}
	}

	void BuildManager::updateBuildOrder()
	{
		//BuildJob * currentJob = buildOrder.getCurrentJob();

		// check for build order
		if (_currentJob != nullptr)
		{
			resources.update();
			int mineral = SELF.me()->minerals();
			int gas = SELF.me()->gas();
			//bool loop = true;
			bool pop = false;
			bool updateGen = true;
			BWAPI::Unit generator;

			while (true)
			{
				//Log::info("building %d", (int)_currentJob->getType());

				const MetaType & metaType = _currentJob->getType();

				if (updateGen) {
					generator = getGenerator(metaType);
					updateGen = false;
				}

				bool canBuild = generator && canBeBuilded(generator, metaType);

				
				/*if (metaType.isBuilding() && 
					!canBuild &&
					metaType.getProducer().isWorker() &&
					//BWAPI::Broodwar->getFrameCount() >= _delayBuildingPredictionUntilFrame
					)
				{

				}*/
				

				if (canBuild)
				{
					//mineral >= mineralPrice && gas >= gasPrice

					if (generate(generator, _currentJob))
					{
						int mineralPrice = metaType.mineralPrice();
						int gasPrice = metaType.gasPrice();
						mineral -= mineralPrice;
						gas -= gasPrice;
						_currentJob->amount++;
						resources.update();

						

					}

					


					/*if (type.isBuilding())
					{

					}
					else if (type.isUnit()) {
						if (doTrain(type)) { // if can train

							
							//_reservedSupplies -= type.getUnitType().supplyRequired();
							resources.unreserveSupplies(type.supplyRequired());
							

						}
						else
							break;
					}*/

				}
				else
					break;



				//else
				//	break;

				// check for amount condition
				if (_currentJob->amount /* _currentJob->type.supplyRequired()*/ >= _currentJob->getMaxAmount())
				{
					pop = true;
				}


				// check for amount condition
				/*if (_currentJob->getAction()->getBuildCondition() == nullptr)
				{
				if (_currentJob->amount >= _currentJob->getMaxAmount())
				pop = true;

				}
				else // check for special condition
				{

				if (_currentJob->getAction()->getBuildCondition() == QueueConditions::reachMaxSupplies)
				{

				}


				}*/

				// pop the queue for finished job
				if (pop)
				{
					Log::info("next");

					jobFinished();



					if (_currentJob == nullptr)
						break;
					else {
						// check the last job if was building
						if (metaType.isBuilding())
						{
							break;
						}
					}

				}
			}

		}
		else
			return;
	}

	BWAPI::Unit BuildManager::getGenerator(const MetaType & metaType)
	{
		const BWAPI::UnitType & generatorType = metaType.getProducer();

		// is a resource depot
		if (generatorType.isResourceDepot())
		{
			return BaseManager::getInstance().getClosest(BWAPI::Positions::None)->get();
		}
		else {
			BWAPI::Unitset candidates;

			for (const auto unit : SELF.me()->getUnits())
			{
				// Reasons that a unit cannot produce the desired type:

				if (generatorType != unit->getType()) { continue; }

				if (!unit->isCompleted())  { continue; }
				if (unit->isTraining())    { continue; }
				if (unit->isLifted())      { continue; }
				if (!unit->isPowered())    { continue; }
				if (unit->isUpgrading())   { continue; }
				if (unit->isResearching()) { continue; }

				candidates.insert(unit);

			}

			if (metaType.isUnit() && metaType.getUnitType().isWorker())
			{
				return getFarthest(candidates, BaseManager::getInstance().getMainBase()->get()->getPosition());
			}
			else 
			{
				return getClosest(candidates, BaseManager::getInstance().getMainBase()->get()->getPosition());
			}

			return nullptr;
		}
	}

	bool BuildManager::canBeBuilded(BWAPI::Unit generator, const MetaType & metaType)
	{
		OAI_ASSERT_MSG(generator != nullptr, "BuildManager: Generator is null");

		// check first for current resources
		bool can = (metaType.mineralPrice() <= Resources::getInstance().getAvailbleMinerals()) &&
				   (metaType.gasPrice() <= Resources::getInstance().getAvailbleGas());

		if (can)
		{
			if (metaType.isUnit())
			{
				can = BWAPI::Broodwar->canMake(metaType.getUnitType(), generator);
			}
			else if (metaType.isTech())
			{
				can = BWAPI::Broodwar->canResearch(metaType.getTechType(), generator);
			}
			else if (metaType.isUpgrade())
			{
				can = BWAPI::Broodwar->canUpgrade(metaType.getUpgradeType(), generator);
			}
			else
			{
				OAI_ASSERT_MSG(false, "BuildManager: On canBeBuilded has a unknown Meta Type");
			}
		}

		return can;

	}

	bool BuildManager::generate(BWAPI::Unit generator, BuildJob * job)
	{
		if (!generator)
		{
			return false;
		}

		const MetaType & meta = job->getAction()->getMetaType();

		if (meta.isBuilding())
		{
			if (meta.isAddon())
			{
				return generator->buildAddon(meta.getUnitType());
			}
			else 
			{

				BuildingManager::getInstance().addFuture(job->getAction(), SELF.startPosition());
				return true;
			}

		}
		else if (meta.isUnit())
		{
			resources.unreserveSupplies(meta.supplyRequired());

			// zergs morph, not train
			if (SELF.race() == BWAPI::Races::Zerg)
			{
				return generator->morph(meta.getUnitType());
			}
			else {

				
				return generator->train(meta.getUnitType());
			}
		}
		else if (meta.isTech())
		{
			return generator->research(meta.getTechType());

		}
		else if (meta.isUpgrade())
		{
			return  generator->upgrade(meta.getUpgradeType());
		}
		else 
		{
			return false;
		}
	}

	void BuildManager::jobFinished()
	{
		buildOrder.pop();

		//if (!_jobsToDo.empty()) {
		/*if (_currentJob == &_jobsToDo.top())
		{

		_jobsToDo.pop();
		}*/
		//}

		
		Commander::getInstance().onActionOrdered(_currentJob->getAction());

		_currentJob = nullptr;

		// check for next job 

		setupBuildJob();
	}

	void BuildManager::update()
	{
		// setup next build job if we have one
		setupBuildJob();
		// update build order
		updateBuildOrder();

	}

	bool BuildManager::doTrain(const MetaType & type)
	{
		// check for requeriments
		if (resources.getAvailbleSupplies() == 0)
			return false;


		if (type.getUnitType().isWorker())
		{
			return true;

			//return BaseManager::getInstance().getMainBase()->get()->train(type.getUnitType());
		}
	}

	bool BuildManager::doBuild(const MetaType & type)
	{
		Worker & worker = WorkersManager::getInstance().getFreeWorker();

		return false;
	}

	BWAPI::Unit BuildManager::getFarthest(const BWAPI::Unitset & units, BWAPI::Position position)
	{
		if (units.empty())
		{
			return nullptr;
		}

		if (position == BWAPI::Positions::None)
		{
			return *(units.begin());
		}

		BWAPI::Unit farthest = nullptr;
		int maxDist = -1;

		for (auto unit : units)
		{
			OAI_ASSERT_MSG(unit != nullptr, "Unit was null");

			int distance = unit->getDistance(position);

			if (distance > maxDist)
			{
				farthest = unit;
				maxDist = distance;
			}
		}

		return farthest;
	}

	BWAPI::Unit BuildManager::getClosest(const BWAPI::Unitset & units, BWAPI::Position position)
	{
		if (units.empty())
		{
			return nullptr;
		}

		if (position == BWAPI::Positions::None)
		{
			return *(units.begin());
		}

		BWAPI::Unit closeUnit = nullptr;
		int dist = 1000000;

		for (const auto unit : units)
		{
			int distance = unit->getDistance(position);

			if (distance < dist)
			{
				closeUnit = unit;
				dist = distance;
			}
		}

		return closeUnit;
	}
}