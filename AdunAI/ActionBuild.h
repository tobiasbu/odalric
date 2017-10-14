
#ifndef _OAI_ACTIONBUILD_H_
#define _OAI_ACTIONBUILD_H_

#include "MetaType.h"
#include "Action.h"

namespace Odarich {


	class ActionBuild : public Action
	{
		friend class ActionContainer;
		

	private:

		MetaType _meta;
		const void * _buildCondition = nullptr;
		int _startTime = 0;
		int _amount = 1;

	public:

		//ActionAttributes attributes;

		ActionBuild(const MetaType & metaType);
		ActionBuild(const MetaType & metaType, const int & id);
		
		void setBuildCondition(const void * buildCondition);
		void setStartTime(const int & startTime);
		void setAmount(const int & amount);

		const MetaType & getMetaType() const;
		const void * getBuildCondition() const;
		int getRemainingTime() const;
		int getAmount() const;

		static Action * makeBuildAction(const MetaType & metaType, void * buildCondition = nullptr);


		//void setAttributes(ActionAttributes * _attributes);


		/*template<class Func, class... Args>
		void setPrereqiosteFunction(Func && func, Args&&... args)
		{
			_prerequisteCondition = func;

		}*/

		//void setPrerequisiteFunction(void 
		
		
		/*Action(const value_type& rhs)
			:
			value(value_type) 
		{

		}

		const value_type & get() const
		{ 
			return _meta.value();
		}*/

		
	};

	

}

#endif