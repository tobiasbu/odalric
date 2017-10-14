

#ifndef _OAI_WORKERSINFO_H_
#define _OAI_WORKERSINFO_H_

#include <vector>
#include <unordered_map>
#include "BWAPI.h"

#include "Worker.h"

namespace Odarich {

	typedef std::unordered_map<BWAPI::Unit, Worker> WorkersMap;

	class WorkersInfo
	{
	private:

		BWAPI::Unitset _workers;
		WorkersMap _workersMap;
		//std::vector<Worker> _workersUnit;
		


	public:

		void addUnit(Worker & Worker);
		void add(BWAPI::Unit worker);
		void add(BWAPI::Unit worker, Worker::Job job);
		void destroy(BWAPI::Unit worker);

		WorkersMap & getWorkers();
		BWAPI::Unitset & getWorkersSet();
		int count() const;


	};

}

#endif