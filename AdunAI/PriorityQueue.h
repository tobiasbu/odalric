
#ifndef _OAI_PRIORITYQUEUE_HPP_
#define _OAI_PRIORITYQUEUE_HPP_

#include <deque>
//#include "Commons.h"
#include "QueueJob.h"

namespace Odalric {

	

	/*class PriotityQueue
	{
	protected:

		std::deque<QueueJob> _queue;

	public:

		PriotityQueue();

		void add(const QueueJob & job);
	

		bool empty() const;
		void clear();
		uint32 count() const;

		QueueJob * front();
		QueueJob * frontNext();
		void pop();
		QueueJob & pop_back();
		QueueJob & pop_front();

	};*/

	template<class T>
	class PriotityQueue
	{
		typedef T value_type;

	protected:

		struct Tval
		{
			value_type value;
			int priority;

			Tval(value_type & value, const int & priority = 0)
				:
				value(value),
				priority(priority)
			{
				
			}
		};

		std::deque<Tval> _queue;

	public:

		PriotityQueue()
		{

		}

		void add(value_type & job)
		{
			_queue.push_back(Tval(job));
		}

		void add(value_type & job, const int & priority)
		{
			Tval v(job, priority);

			if (!_queue.empty()) {

				if (priority > _queue.front().priority)
					_queue.push_front(v);
				else
					_queue.push_back(v);
			}
			else
			{
				_queue.push_back(v);
			}
		}

		bool empty() const
		{
			return _queue.empty();
		}

		void clear()
		{
			_queue.clear();
		}

		uint32 size() const
		{
			return _queue.size();
		}

		value_type * front()
		{
			return &_queue.front().value;
		}

		value_type  * frontNext()
		{
			if (_queue.size() > 1)
				return &_queue[1].value;
			else
				return nullptr;
		}

		void pop()
		{
			_queue.pop_front();
		}

		value_type & pop_back()
		{
			value_type job = *_queue.front().value;
			_queue.pop_front();
			return job;
		}

		value_type & pop_front()
		{
			value_type & job = *_queue.back().value;
			_queue.pop_back();
			return job;
		}

	};

}

#endif