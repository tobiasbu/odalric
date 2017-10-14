
#ifndef _OAI_META_HPP_
#define _OAI_META_HPP_

#include <BWAPI.h>
#include <unordered_set>

#include "Types.h"
#include "MetaType.h"
#include "Singleton.hpp"


namespace Odarich {


	

	class MetaMap : public Singleton<MetaMap>
	{
	public:

	private:

		static BWAPI::UnitType getGroundUnit(int index);

		

	};
//	{

//	private:
//
//		std::unordered_set<MetaType> metaUnitTypes;
//		//std::unordered_set<BWAPI::TechType> metaTechTypes;
//		//std::unordered_set<BWAPI::UpgradeType> metaUpgradeTypes;
//
//	public:
//
//		MetaMap();
//		void init();
//
//	
//
//		template<class T>
//		const MetaType<T> * get(const int32 & id) const
//		{
//			return nullptr;
//		}
//
//		template<class T>
//		const T * getValue(const int32 & id) const
//		{
//			return get<T>();
//		}
//
//		template<>
//		const MetaType<BWAPI::UnitType> * get<BWAPI::UnitType>(const int32 & id) const
//		{
//			std::unordered_set<MetaType<BWAPI::UnitType>>::const_iterator it = metaUnitTypes.find(id);
//
//			if (it == metaUnitTypes.end()) // not found
//				return nullptr;
//			else 
//				return (&(*it));
//		}
//
//		template<>
//		const BWAPI::UnitType * getValue(const int32 & id) const
//		{
//			return &get<BWAPI::UnitType>(id)->value();
//		}
//		
//
//		const BWAPI::UnitType * getUnit(const int32 & id) const
//		{
//			std::unordered_set<MetaType>::iterator it = metaUnitTypes.find(id);
//
//			if (it == metaUnitTypes.end()) // not found
//				return nullptr;
//			else
//				return &it->value();
//		}
//		//BWAPI::TechType * getTechType(const int32 & id);
//		//BWAPI::TechType & getTechType(const int32 & id);
	};
//
//	
//}
//
//
//

#endif