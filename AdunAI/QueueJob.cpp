
#include "QueueJob.h"

namespace Odarich {

	QueueJob::QueueJob(const int32 & type)
		:
		_type(type),
		_priority(0)
		//_condition(nullptr)
		//_amount(0)

	{

	}
	const int32 QueueJob::getPriority() const
	{
		return _priority;
	}

	const int32 QueueJob::getType() const
	{
		return _type;
	}
	/*QueueJob::QueueJob(const uint8 & type, const int & priority)
		:
		_type(type),
		_priority(priority),
		//_condition(nullptr)
		//_amount(0)
	{

	}*/

	/*void QueueJob::setAmount(const int & amount)
	{
		_amount = amount;
	}*/

	
	
	/*const int QueueJob::getAmount() const
	{
		return _amount;
	}
	void * QueueJob::getCondition() const
	{
		return _condition;
	}*/

}