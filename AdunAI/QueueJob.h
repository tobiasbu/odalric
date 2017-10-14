
#ifndef _OAI_QUEUEJOB_HPP_
#define _OAI_QUEUEJOB_HPP_

#include "Types.h"


namespace Odarich {

	//class PriotityQueue;

	class QueueJob
	{
		//friend class PriotityQueue;

	protected:

		int32 _type;
		int32 _priority;


	public:

		QueueJob(const int32 & type);
		//QueueJob(const uint8 & type, const int & priority);

		const int32 getType() const;
		const int32 getPriority() const;
		

	};

}

#endif