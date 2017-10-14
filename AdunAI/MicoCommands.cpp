
#include "MicroCommands.h"

namespace Odalric {

	namespace MicroCommands
	{
		BWAPI::Unit getClosestEnemy(BWAPI::Unit & myUnit, const int & minRadius)
		{
			BWAPI::Unit closeUnit = nullptr;
			int closeDist = minRadius;

			for (auto & unit : BWAPI::Broodwar->enemy()->getUnits())
			{
				int dist = unit->getDistance(myUnit);

				if ((dist < minRadius) && (!closeUnit || (dist < minRadius)))
				{
					closeUnit = unit;
					closeDist = dist;
				}
			}

			return closeUnit;
		}

		void attack(BWAPI::Unit & myUnit, BWAPI::Unit & enemyUnit)
		{
			if (!myUnit || !enemyUnit)
			{
				Log::error("attack: Unit and Target is null");
				return;
			}

			if (myUnit->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount() || myUnit->isAttackFrame())
			{
				return;
			}

			BWAPI::UnitCommand currentCommand(myUnit->getLastCommand());

			if (currentCommand.getType() == BWAPI::UnitCommandTypes::Attack_Unit &&	currentCommand.getTarget() == enemyUnit)
			{
				return;
			}

			myUnit->attack(enemyUnit);

		}

		// totally from ualbertabot
		void rightClick(BWAPI::Unit unit, BWAPI::Unit target)
		{
			if (!unit || !target)
			{
				Log::error("rightClick: Unit and Target is null");
				return;
			}

			// if we have issued a command to this unit already this frame, ignore this one
			int lastFrame = unit->getLastCommandFrame();
			if (lastFrame > 0)
				if (lastFrame >= BWAPI::Broodwar->getFrameCount() || unit->isAttackFrame())
				{
					
					return;
				}

			// get the unit's current command
			BWAPI::UnitCommand currentCommand(unit->getLastCommand());

			// if we've already told this unit to move to this position, ignore this command
			if ((currentCommand.getType() == BWAPI::UnitCommandTypes::Right_Click_Unit) && (currentCommand.getTargetPosition() == target->getPosition()))
			{
				return;
			}

			// if nothing prevents it, click on the target
			unit->rightClick(target);

		}

		void move(BWAPI::Unit unit, const BWAPI::Position & target)
		{

			if (!unit || !unit->exists() || unit->getPlayer() != BWAPI::Broodwar->self() || !target.isValid())
			{
				// UAB_ASSERT(false, "bad arg");  // TODO restore this after the bugs are solved; can make too many beeps
				return;
			}

			// if we have issued a command to this unit already this frame, ignore this one
			if (unit->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount() || unit->isAttackFrame())
			{
				return;
			}

			BWAPI::UnitCommand currentCommand(unit->getLastCommand());

			// if we've already told this unit to move to this position, ignore this command
			if ((currentCommand.getType() == BWAPI::UnitCommandTypes::Move) && (currentCommand.getTargetPosition() == target) && unit->isMoving())
			{
				return;
			}

			
			// if nothing prevents it, move the target position
			unit->move(target);
			//Log::info("asda3");
		}
	}

}