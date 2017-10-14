
#include "BuildOrder.h"

namespace Odarich {

	BuildOrder::BuildOrder()
	{
		
	}

	/*void BuildOrder::add(const BuildJob & job)
	{
		if (!_queue.empty()) {

			if (job.getPriority() > _queue.front().getPriority())
				_queue.push_front(job);
			else
				_queue.push_back(job);
		}
		else
		{
			_queue.push_back(job);
		}
	}*/

	void BuildOrder::add(Action * action)
	{
		//if (action->attributes.maxAmount < 1)
		//	action->attributes.maxAmount = 1;

		PriotityQueue<BuildJob>::add(BuildJob(static_cast<ActionBuild*>(action)));
	}

	void BuildOrder::add(BuildJob & job, const int & quantity)
	{
		if (quantity < 1)
			job._maxAmount = 1;
		else
			job._maxAmount = quantity;

		PriotityQueue<BuildJob>::add(job);
	}

	void BuildOrder::add(BuildJob & job, void * endCondition)
	{
		//if (endCondition || endCondition != nullptr)
		//	job._condition = endCondition;

		PriotityQueue<BuildJob>::add(job);
	}

	

	/*void BuildOrder::updateAvailbleResources()
	{
		//SELF.me()->minerals();

		_availbleSupplies = SELF.me()->supplyTotal(SELF.race()) - SELF.me()->supplyUsed(SELF.race());

		if (_reservedSupplies < 0)
			_reservedSupplies = 0;
	}

	void BuildOrder::doBuild()
	{
		
	}

	bool BuildOrder::doTrain(const MetaType & unitType)
	{
		
	}


	void BuildOrder::jobFinished()
	{
		
	}*/

	

	void BuildOrder::draw(const int & x, const int & y)
	{
		BWAPI::Broodwar->drawTextScreen(x, y, "Supplies: %d/%d/%d", SELF.me()->supplyUsed(), SELF.me()->supplyTotal(), (SELF.me()->supplyTotal() - SELF.me()->supplyUsed()));
		//BWAPI::Broodwar->drawTextScreen(x, y + 16, "Availble: %d", this->_availbleSupplies);

		if (_currentJob != nullptr) {
			//BWAPI::Broodwar->drawTextScreen(x, y + (16 * 2), "Price: %d", _currentJob->type.supplyRequired());
			BWAPI::Broodwar->drawTextScreen(x, y + (16 * 3), "Current Amount: %d/%d", _currentJob->amount, _currentJob->getMaxAmount());
		}
	}
	
}