
#ifndef _OAI_QUEUECONDITIONS_HPP_
#define _OAI_QUEUECONDITIONS_HPP_

#include "Commons.h"
#include "Log.h"
#include "FunctionTraits.h"


namespace Odarich {

	typedef bool(*func)(...);
	typedef bool(*oai_condition)(...);

	namespace QueueConditions
	{
		static bool reachMaxSupplies() // const int & atLeast = 3
		{
			
			return (SELF.me()->supplyUsed(SELF.race()) >= SELF.me()->supplyTotal(SELF.race())) ? true : false;
			
		}

		static bool test(int test)
		{
			Log::info("%d ola",test);// << std::endl;
			return true;
		}
	}

}

#endif


/*template<typename R, class... Args>
struct Function
{
typedef R result_type;

const int _argCount;

Function()
{

}



template<class Func>
explicit Function(Func&& f, Args&&... args)
{
//_f = f;
}

operator=;

result_type operator();
result_type operator() const;
};*/