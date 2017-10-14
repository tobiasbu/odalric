
#include "PriorityQueue.h"

namespace Odarich {


	/*PriotityQueue::PriotityQueue()
	{

	}

	void PriotityQueue::add(const QueueJob & job)
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
	}

	void PriotityQueue::add(QueueJob & job, const int & quantity)
	{
		if (quantity < 1)
			job._amount = 1;

		add(job);

	}

	void PriotityQueue::add(QueueJob & job, void * endCondition)
	{
		if (endCondition || endCondition != nullptr)
			job._condition = endCondition;

		add(job);
	}

	bool PriotityQueue::empty() const
	{
		return _queue.empty();
	}

	void PriotityQueue::clear()
	{
		_queue.clear();
	}

	uint32 PriotityQueue::count() const
	{
		return _queue.size();
	}

	QueueJob * PriotityQueue::front()
	{
		if (!_queue.empty())
			return &_queue.front();
		else
			return nullptr;
	}

	QueueJob * PriotityQueue::frontNext()
	{
		if (_queue.size() > 1)
			return &_queue[1];
		else
			return nullptr;
	}
	
	QueueJob & PriotityQueue::pop_back()
	{
		QueueJob & job = _queue.back();
		_queue.pop_back();
		return job;
	}

	QueueJob & PriotityQueue::pop_front()
	{
		QueueJob & job = _queue.front();
		_queue.pop_front();
		return job;
	}

	void PriotityQueue::pop()
	{
		_queue.pop_front();
	}*/


}