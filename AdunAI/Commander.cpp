
#include "Commander.h"
#include "WorkersManager.h"
#include "BuildManager.h"
#include "ResourcesAreaManager.h"
#include "Resources.h"
#include "Log.h"
#include "Self.h"
#include "QueueConditions.h"
#include "Strategy.h"
#include "ScoutManager.h"

#include <thread>

namespace Odalric 
{
	void Commander::init()
	{


		// check for initial units
		for each(auto u in BWAPI::Broodwar->self()->getUnits())
		{
			_units.add(u);
		}

		// check initial resources
		Base * mainBase = BaseManager::getInstance().getMainBase();
		ResourcesArea * area = ResourcesAreaManager::getInstance().findResourcesAt(mainBase->get(), 300);
		area->setBase(mainBase);
		mainBase->setupResources(area);

		// INITAL STAGE
		// set inital actions

		// build workers reaching the maximal supply
		Action * action = Strategy::makeBuildAction(MetaType(SELF.race().getWorker()), QueueConditions::reachMaxSupplies);
		// then build a supply provider
		Action * action2 = Strategy::makeBuildAction(MetaType(SELF.me()->getRace().getSupplyProvider()));
		action2->setPrerequisiteAction(action);
		action2->setPrerequisteCondition(Action::PreRequisite::ActionOrdered);
		// and for finish the first stage, add first scout
		Action * action3 = Strategy::makeCommandAction(ActionCommand::ActionCommandType::SendScout);
		action3->setPrerequisiteAction(action2);
		// future implementation
		//action3->setPrerequisteCondition(Action::PreRequisite::BuildRemainingTime, 10);

		_actions.add(action);
		_actions.add(action2);
		_actions.add(action3);
		

		// retrieve main base resources
		/*ResourcesArea * res = new ResourcesArea(BaseManager::getInstance().getMainBase()->get()->getPosition());
		res->findResources();
		*/

		//_buildOrder.meta.init();

		//_buildOrder.add(BuildJob(SELF.me()->getRace().getWorker()), QueueConditions::reachMaxSupplies);
	}

	void Commander::update()
	{
		ActionSet & actions = _actions.getUnitializedActions();
		ActionSet::iterator it = actions.begin();

		while (it != actions.end())
		{
			if (performAction((*it)))
			{
				it = actions.erase(it);
			}
			else
			{
				it++;
			}

			/*bool perform = true;

			Action * prerequiste = (*it)->getPrerequisteAction();

			if (prerequiste != nullptr) {
				if (prerequiste->getState() != Action::ActionState::Done)
				{
					perform = false;
				}
			}

			if (perform) {
				if ((*it)->getActionType() == Action::ActionType::Build)
				{
					BuildManager::getInstance().buildOrder.add((*it));
					_actions.inProgress((*it));
					it = actions.erase(it);
				}
			}
			else
				it++;*/
		}
	}

	// FUTURE IMPLEMENTATION - ActionTRACKER
	void Commander::trackOrderedActions()
	{
		for each (Action * action in _actions.getOrderedActions())
		{
			if (action->getActionType() == Action::ActionType::Build)
			{
				//static_cast<ActionBuild*>(action)
			}
			else
				continue;
		}
	}

	bool Commander::performAction(Action * action)
	{
		bool perform = true;

		// check if this action has a prerequiste action
		const Action * prerequiste = action->getPrerequisteAction();

		if (prerequiste != nullptr) {

			// check if we have a pre requisite condition
			if (action->getPrerequisiteCondition() == Action::PreRequisite::None)
			{
				perform = (prerequiste->getState() == Action::ActionState::Done);
				
			}
			else 
			{
				perform = action->checkPrerequiste();
				
			}
		}

		
		if (perform) {
			if (action->getActionType() == Action::ActionType::Build) // is build action
			{
				BuildManager::getInstance().buildOrder.add(action);
				_actions.inProgress(action);
				_perfomedActions++;
			}
			else if (action->getActionType() == Action::ActionType::Command)
			{
				performCommand(static_cast<ActionCommand*>(action));
				_perfomedActions++;
			}
		}

		return perform;
	}
	
	void Commander::performCommand(ActionCommand * action)
	{
		if (action->getCommand() == ActionCommand::ActionCommandType::SendScout)
		{
			Worker & worker = WorkersManager::getInstance().getFreeWorker();
			ScoutManager::getInstance().add(worker.get());
			WorkersManager::getInstance().setWorkerJob(&worker, Worker::Job::Scout);
		}

		_actions.ordered(action);
	}

	// callback when a action is unitialzed (append new action)
	void Commander::onActionUnitialized(Action * action)
	{
		_actions.add(action);
	}

	// callback when a action is in progress
	void Commander::onActionInProgress(Action * action)
	{
		_actions.removeUnitialized(action);
		_actions.inProgress(action);
	}



	void Commander::onActionDone(Action * action)
	{
		_actions.removeOrdered(action);
		_actions.done(action);
	}

	void Commander::onActionOrdered(Action * action)
	{
		_actions.removeInProgress(action);
		_actions.ordered(action);
	}

	int Commander::getPerfomedActions() const
	{
		return _perfomedActions;
	}

	const ActionContainer & Commander::getActionsContainer()
	{
		return _actions;
	}
}