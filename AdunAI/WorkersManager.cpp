
#include "WorkersManager.h"
#include "Commons.h"
#include "Utils.h"
#include "MicroCommands.h"
#include "Building.h"
#include "BaseManager.h"

namespace Odarich 
{
	WorkersManager::WorkersManager()
	{

	}

	void WorkersManager::update()
	{
		//std::vector<Worker> & workers = workersInfo.getWorkers();

		WorkersMap & map = workersInfo.getWorkers();
		WorkersMap::iterator it = map.begin();
		manageRefinaries();
		for (; it != map.end(); it++ )
		{
			

			//if (!w.get()->isBeingConstructed()) {
				

			if (!it->first)
				continue;

				
				if (!it->first->isCompleted())
				{
					continue;
				}

				const Worker::Job & job = it->second.getJob();

				if (job == Worker::Job::Scout)
				{
					continue;
				}

				checkWorkerConsistency(it->second);

				


				if (job == Worker::Job::Idle)
				{
					manageIdle(it->second);
				}
				else if (job == Worker::Job::ReturnBurden)
				{
					manageReturnBurden(it->second);
				}
			//}

		}

		
	}

	void WorkersManager::checkWorkerConsistency(Worker & worker)
	{
		const Worker::Job & job = worker.getJob();
		BWAPI::Unit unit = worker.get();

		if (job == Worker::Job::GatherMineral)
		{

			if (unit->getOrder() == BWAPI::Orders::MoveToGas ||
				unit->getOrder() == BWAPI::Orders::WaitForGas ||
				unit->getOrder() == BWAPI::Orders::ReturnGas)
			{
				setWorkerJob(&worker, Worker::Job::Idle);
				
			}
		}
		
		if (
			(unit->isIdle() || unit->getOrder() == BWAPI::Orders::PlayerGuard) &&
			job != Worker::Job::GatherMineral &&
			job != Worker::Job::Build &&
			/*job != WorkerJob::Move &&*/
			job != Worker::Job::Scout)
		{
			setWorkerJob(&worker, Worker::Job::Idle);
		}
		else if (job == Worker::Job::GatherGas)
		{
			// TODO 
		}
		else if (job == Worker::Job::GatherMineral)
		{
			// is any enemy around
			BWAPI::Unit target = enemyAround(unit);

			if (target)
			{
				MicroCommands::attack(unit, target);
			}
			else if (
				unit->getOrder() != BWAPI::Orders::MoveToMinerals &&
				unit->getOrder() != BWAPI::Orders::WaitForMinerals &&
				unit->getOrder() != BWAPI::Orders::MiningMinerals &&
				unit->getOrder() != BWAPI::Orders::ReturnMinerals &&
				unit->getOrder() != BWAPI::Orders::ResetCollision)
			{
				setWorkerJob(&worker, Worker::Job::Idle);
			}
		}
	}

	BWAPI::Unit WorkersManager::enemyAround(BWAPI::Unit worker)
	{
		BWAPI::Unit closeEnemy = nullptr;
		int closeDistance = 65;         // ignore anything farther away

		

		for (const auto enemy : BWAPI::Broodwar->enemy()->getUnits())
		{
			int dist;

			if (enemy->isVisible() &&
				(!enemy->isMoving() || enemy->isStuck()) &&
				!enemy->isFlying() &&
				enemy->isCompleted() &&
				enemy->isDetected() &&
				enemy->getPosition().isValid() &&
				(dist = enemy->getDistance(worker)) < closeDistance
				)
			{
				closeEnemy = enemy;
				closeDistance = dist;
			}
		}

		return closeEnemy;
	}
	
	void WorkersManager::manageReturnBurden(Worker & unit)
	{
		bool mineral = unit.get()->isCarryingMinerals();
		bool gas = unit.get()->isCarryingGas();
		if (!mineral)
		{
			setWorkerJobToClosestBase(unit, Worker::Job::GatherMineral);
		}
		else if (!gas)
		{
			setWorkerJobToClosestBase(unit, Worker::Job::GatherGas);
		}
	}

	void WorkersManager::manageIdle(Worker & unit)
	{

		if (unit.get()->isCarryingMinerals() || unit.get()->isCarryingGas())
		{
			//unit.setJob(WorkerJob::ReturnBurden,b);
			setWorkerJobToClosestBase(unit, Worker::Job::ReturnBurden);
		}
		else
		{

			setWorkerJobToClosestBase(unit, Worker::Job::GatherMineral);
			//unit.setJob(WorkerJob::GatherMineral, b);
		}

		/*Base * b = BaseManager::getInstance().getClosest(unit.get());

		if (b != nullptr)
		{

		
		}*/

	}

	void WorkersManager::manageRefinaries()
	{
		BasesMap & baseMap = BaseManager::getInstance().getBases();
		BasesMap::iterator it = baseMap.begin();

		for (; it != baseMap.end(); it++)
		{
			//ResourcesArea * r = base.second.getResources();

			int refnariesCount = it->second.refinariesCount();

			if (refnariesCount >= 1)
			{
				int assingGas = it->second.getWorkersJobCount(Worker::Job::GatherGas);
				
				

				if (assingGas < 3)
				{
					assingGas = 3 - assingGas;


					for (int i = 0; i < assingGas; i++)
					{
						BWAPI::Unit refinary = it->second.getBestRefinary();

						if (refinary != nullptr)
						{

							Worker * gasWorker = nullptr;
							
							
							//while (gasWorker != nullptr && workersInfo.count() > 0)
							
								gasWorker = getFreeWorkerForGas(refinary);
							
							

							if (gasWorker != nullptr)
							{

								if (gasWorker->getBase() != nullptr)
								{
									it->second.unassignWorker(gasWorker);
								}
								
								
								
								
								
								it->second.assignGasWorker(gasWorker, refinary);
								it->second.addRefinaryPath(refinary, 1);
								
								gasWorker->setUnitJob(refinary);
								gasWorker->setBase(&it->second);
								MicroCommands::rightClick(gasWorker->get(), refinary);
								setWorkerJob(gasWorker, Worker::Job::GatherGas);
								
							}
							else {
								//Log::info("null");
							}
						}
					}


				}
				

				/*for each (auto refinary in r->getGas())
				{
					if (refinary && refinary->isCompleted() && refinary->getType().isRefinery())
					{
						
					}
					else continue;
				}*/

				
			}

		}
	}

	void WorkersManager::setWorkerJob(Worker * worker, const Worker::Job & job)
	{
		OAI_ASSERT(worker != nullptr);

		BWAPI::Unit unit = worker->get();

		if (!unit || !unit->exists())
			return;

		

		if (job == Worker::Job::Idle || job == Worker::Job::Scout)
		{
			Base * b = worker->getBase();

			if (b != nullptr)
			{
				b->unassignWorker(worker);
				//worker.setBase(nullptr);
			}
		}
		

		worker->setJob(job);
	}

	void WorkersManager::setBuilderWorkerJob(Worker & unit, const Building & building)
	{
		BWAPI::Unit worker = unit.get();

		if (!worker || !worker->exists())
			return;

		Base * b = unit.getBase();

		if (b != nullptr)
		{
			b->unassignWorker(&unit);
			unit.setBase(nullptr);
		}

		unit.setJob(Worker::Job::Build);
		unit.setBuilding(&building);
	}

	void WorkersManager::setWorkerJobToClosestBase(Worker & unit, const Worker::Job & job)
	{
		BWAPI::Unit worker = unit.get();

		if (!worker || !worker->exists())
			return;

		

		if (job == Worker::Job::Idle)
		{
			Base * b = unit.getBase();

			if (b != nullptr)
			{
				b->unassignWorker(&unit);
				unit.setBase(nullptr);
			}
		}
		else 
		{

			Base * b = BaseManager::getInstance().getClosest(unit.get());

			if (b != nullptr)
			{

				if (job == Worker::Job::GatherMineral)
				{
					BWAPI::Unit mine = b->getBestMine(worker);

					// TODO improve this
					if (mine == nullptr) // fallback
					{
						mine = worker->getClosestUnit(BWAPI::Filter::IsMineralField);

						/*if (!worker->gather(worker->getClosestUnit(BWAPI::Filter::IsMineralField)))
						{
						// If the call fails, then print the last error message
						Log::error(BWAPI::Broodwar->getLastError().toString());

						// fallback
						// check to all knowed mineral fields
						BWAPI::Unit mineralField = ResourcesAreaManager::getNearestResourceFromAll(base->get(), BWAPI::UnitTypes::Resource_Mineral_Field, 300);


						if (worker->gather(mineralField))
						{
						//base->assignMineralWorker(this);

						}
						}*/

					}

					if (mine != nullptr)
					{
						b->addMineralPath(mine, 1);
						b->assignMineralWorker(&unit, mine);
						unit.setUnitJob(mine);
						MicroCommands::rightClick(worker, mine);
					}


					

				}
				else if (job == Worker::Job::GatherGas)
				{
					//b->assignGasWorker(&unit);
				}
				else if (job == Worker::Job::ReturnBurden)
				{
					MicroCommands::rightClick(unit.get(), b->get());
				}
			}
		}

		unit.setJob(job);
	}

	void WorkersManager::workComplete(Worker & worker)
	{
		setWorkerJob(&worker, Worker::Job::Idle);
	}

	Worker & WorkersManager::getFreeWorker()
	{
		WorkersMap & map = workersInfo.getWorkers();
		WorkersMap::iterator it = map.begin();

		for (; it != map.end(); it++)
		{
			const BWAPI::Unit u = it->first;

			if (!u || !u->exists()) // ->exists()
				continue;

			// Ignore the unit if it is incomplete or busy constructing
			if (!u->isCompleted() || u->isConstructing())
				continue;

			// Ignore the unit if it has one of the following status ailments
			if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
				continue;

			// Ignore the unit if it is in one of the following states
			if (u->isLoaded() || !u->isPowered() || u->isStuck())
				continue;

			if (it->second.getJob() == Worker::Job::Build || it->second.getJob() == Worker::Job::Repair || it->second.getJob() == Worker::Job::Scout)
				continue;

			if (it->second.getJob() == Worker::Job::Idle)
				return it->second;
			else {

				if (it->second.getJob() == Worker::Job::GatherMineral || it->second.getJob() == Worker::Job::GatherGas)
				{
					if (!u->isBeingGathered() && !u->isCarryingGas() && !u->isCarryingMinerals())
					{
						return it->second;
					}
				}
				else if (it->second.getJob() == Worker::Job::ReturnBurden)
				{
					return it->second;
				}


			}

		}

	}

	Worker * WorkersManager::getBuilderWorker(const Building & building)
	{
		WorkersMap & map = workersInfo.getWorkers();
		WorkersMap::iterator it = map.begin();

		const BWAPI::Position & from = BWAPI::Position(building.getPosition());

		int closeDist = 0;
		Worker * closeWorker = nullptr;

		for (; it != map.end(); it++)
		{
			const BWAPI::Unit u = it->first;

			if (!u || !u->exists()) // ->exists()
				continue;

			// Ignore the unit if it is incomplete or busy constructing
			if (!u->isCompleted() || u->isConstructing())
				continue;

			// Ignore the unit if it has one of the following status ailments
			if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
				continue;

			// Ignore the unit if it is in one of the following states
			if (u->isLoaded() || !u->isPowered() || u->isStuck())
				continue;

			if (it->second.getJob() == Worker::Job::Build || it->second.getJob() == Worker::Job::Repair || it->second.getJob() == Worker::Job::Scout)
				continue;
			else if (it->second.getJob() == Worker::Job::Idle) {
				closeWorker = &it->second;
				break;
			}
			else if (it->second.getJob() == Worker::Job::GatherMineral)
			{
				int distance = u->getDistance(from);

				if (u->isCarryingMinerals() || u->isCarryingGas() ||
					u->getOrder() == BWAPI::Orders::MiningMinerals)
				{
					distance += 96;
				}

				if (!closeWorker || distance < closeDist)
				{
					closeWorker = &it->second;
					closeDist = distance;
				}
			}
		}

		

		return closeWorker;
	}

	Worker * WorkersManager::getFreeWorkerForGas(BWAPI::Unit refinary)
	{

		WorkersMap & map = workersInfo.getWorkers();
		WorkersMap::iterator it = map.begin();

		Worker * closeWorker = nullptr;
		int closeDist = 0; // std::numeric_limits<int>::max();

		for (; it != map.end(); it++)
		{
			const BWAPI::Unit unit = it->first;

			if (!unit || !unit->exists())
				continue;

			if (!unit->isCompleted() || unit->isConstructing())
				continue;

			if (unit->isLoaded() || !unit->isPowered() || unit->isStuck())
				continue;

			if (it->second.getJob() == Worker::Job::Build || it->second.getJob() == Worker::Job::Repair || it->second.getJob() == Worker::Job::Scout || it->second.getJob() == Worker::Job::GatherGas)
				continue;

			

			//UAB_ASSERT(unit != nullptr, "Unit was null");

			if (it->second.getJob() == Worker::Job::GatherMineral ||
				it->second.getJob() == Worker::Job::Idle)
			{
			

				
				if (unit->isCarryingMinerals() || unit->isCarryingGas() ||
					unit->getOrder() == BWAPI::Orders::MiningMinerals)
				{
					continue;
				}

				int distance = unit->getDistance(refinary);
				if (!closeWorker || distance < closeDist)
				{
					//Log::info("noit nul");
					closeWorker = &it->second;
					closeDist = distance;
				}
			}
			
		}

		return closeWorker;
	}
			//Log::info("get base");
			//Log::info("gather mineral");
			


	void WorkersManager::onUnitShow(BWAPI::Unit unit)
	{

		if (unit->getType().isWorker() && unit->getPlayer() == SELF.me() /*&& !unit->isBeingConstructed()*/)
		{
			workersInfo.add(unit);
		}
	}

	void WorkersManager::onUnitMorph(BWAPI::Unit unit)
	{

		if (unit->getType().isWorker() && unit->getPlayer() == SELF.me() /*&& !unit->isBeingConstructed()*/)
		{
			workersInfo.add(unit);
		}

		if (SELF.race() == BWAPI::Races::Zerg)
		{
			if (unit->getType().isBuilding() && unit->getPlayer() == SELF.me())
			{
				workersInfo.destroy(unit);
			}
		}
	}

	void WorkersManager::onUnitDestroy(BWAPI::Unit unit)
	{
		if (unit->getType().isWorker() && unit->getPlayer() == SELF.me())
		{
			workersInfo.destroy(unit);
		}
	}
		
	

	void WorkersManager::draw(const int & x, const int & y)
	{
		BWAPI::Broodwar->drawTextScreen(x, y, "WorkersManager: %d", workersInfo.count());
	

	}

}