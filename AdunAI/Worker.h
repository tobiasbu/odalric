
#ifndef _OAI_WORKER_H_
#define _OAI_WORKER_H_


#include <BWAPI.h>


namespace Odarich {

	class Base;
	class Building;


	class Worker
	{
	public:

		enum Job
		{
			Idle,
			GatherMineral,
			GatherGas,
			ReturnBurden,
			Build,
			Scout,
			ScoutResources,
			Repair
		};

	private:

		//BWAPI::Unitset::iterator _it;
		BWAPI::Unit _unit = nullptr;
		BWAPI::Unit _unitJob = nullptr;
		const Building * _building = nullptr;
		Base * _base = nullptr;
		Job _job;
		Job _previousJob;

	public:

		//Worker(BWAPI::Unitset::iterator & it);

		Worker(BWAPI::Unit bwunit);
		Worker();
		Worker(const Worker::Job & job);

		//void setJob(const WorkerJob & job, Base * base);

		// set building to build
		void setBuilding(const Building * building);
		void setUnitJob(BWAPI::Unit unitJob);
		void setBase(Base * base);
		void setJob(const Worker::Job & job);
		void setBuildJob();
		
		Worker::Job getPreviousJob() const;
		Worker::Job getJob() const;
		Base * getBase() const;
		BWAPI::Unit get();
		BWAPI::Unit getUnitJob();

		const Building * getBuilding() const;
	};

}

#endif