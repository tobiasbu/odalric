
#ifndef _OAI_BASESMANAGER_H_
#define _OAI_BASESMANAGER_H_

#include <BWAPI.h>
#include "Base.h"

#include "Singleton.hpp"
#include <unordered_map>



namespace Odarich {

	typedef std::unordered_map<BWAPI::Unit, Base> BasesMap;

	class BaseManager : public Singleton<BaseManager>
	{
	private:

		
		BasesMap _bases;
		Base * _mainBase;
		
		

	public:

		BaseManager();

		void add(const BWAPI::Unit & base);
		
		Base * getMainBase() const;
		BasesMap & getBases();
		Base * getClosest(const BWAPI::PositionOrUnit & unit);
		bool hasBuildBounds() const;

		int count() const;

		void onUnitShow(const BWAPI::Unit & unit);

		void draw();

	};

}

#endif