
#ifndef _OAI_BUILDJOB_HPP_
#define _OAI_BUILDJOB_HPP_

#include <BWAPI.h>
#include "Types.h"
#include "QueueJob.h"
#include "ActionBuild.h"

namespace Odarich {


	/*
	enum BuildJobType : uint8
	{
		Unit,
		Upgrade,
		Tech
	};
*/

	class BuildOrder;

	class BuildJob //: public QueueJob
	{
		friend class BuildOrder;

	public:


	private:

		//BuildJobType _jobType = Unit;
	
		ActionBuild * _action = nullptr;
		//void *_condition = nullptr;
		int _maxAmount = 0;

	public:

		BuildJob(ActionBuild * action, const int & maxAmount = 1);
		
		/*BuildJob(const BWAPI::UnitType & unitType, const short & maxAmount = 1);
		
		//template<typename &&Func, typename ...&&Args> explicit
		// BuildJob(const BWAPI::UnitType & unitType, void * condition);

		BuildJob(const BWAPI::TechType & techType);
		BuildJob(const BWAPI::UpgradeType & upgradeType);*/
		
		void setMaxAmount(const int & maxAmount);

		const int getMaxAmount() const;
		//void * getCondition() const;
		ActionBuild * getAction() const;
		const MetaType & getType() const;

		//BuildJob::BuildJobType getJobType() const;
		int amount = 0;
		

	};

}

#endif