
#include "BuildJob.h"
#include "Assert.h"

namespace Odarich {


	/*BuildJob::BuildJob(const BWAPI::TechType & techType)
		:
		QueueJob(techType)
	{
		_jobType = Tech;
	}

	BuildJob::BuildJob(const BWAPI::UpgradeType & upgradeType)
		:
		QueueJob(upgradeType)
	{
		_jobType = Upgrade;
	}*/

	BuildJob::BuildJob(ActionBuild * action, const int & maxAmount)

	{
		OAI_ASSERT(action != nullptr);

		_action = action;

		_maxAmount = maxAmount;

		if (_maxAmount < 0)
			_maxAmount = 1;

		//_jobType = Unit;
	}

	/*BuildJob::BuildJob(const int8 & jobType)
		:
		QueueJob(jobType),
		_condition(nullptr)
	{
		this->_maxAmount = 1;
	}

	BuildJob::BuildJob(const int8 & unitType, const short & maxAmount)
		:
		QueueJob(unitType),
		_condition(nullptr),
		_maxAmount(maxAmount)

	{
		
		
		//if (unitType.isWorker())
		//{
		//	_jobType = Worker;

		//} 
		//else if (unitType.isRefinery())
		//{
		//	_jobType = Refinery;
		//}
		//else if (unitType.isBuilding())
		//{
		//	_jobType = Building;
		//}


	}*/


	void BuildJob::setMaxAmount(const int & maxAmount)
	{
		_maxAmount = maxAmount;
	}

	const int BuildJob::getMaxAmount() const
	{
		return _maxAmount;
	}

	/*void * BuildJob::getCondition() const
	{
		return _condition;
	}*/

	ActionBuild * BuildJob::getAction() const
	{
		return _action;
	}

	const MetaType & BuildJob::getType() const
	{
		return _action->getMetaType();
	}

	/*BuildJob::BuildJobType BuildJob::getJobType() const
	{
		return _jobType;
	}*/



	//bool BuildJob::isWorker() const;

	

}