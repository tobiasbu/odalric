
#ifndef _OAI_METATYPE_HPP_
#define _OAI_METATYPE_HPP_

#include <BWAPI.h>
#include "Types.h"
//#include "Meta.h"
#include <xhash>


/*
namespace std
{
	template <>
	struct hash<Odarich::MetaTypeBase>
	{
		Odarich::int32 operator()(Odarich::MetaTypeBase const & x) const
		{
			return x.getType();

		}


	};

	template <>
	struct equal_to<Odarich::MetaTypeBase> : public unary_function<Odarich::MetaTypeBase, bool>
	{
		bool operator()(const Odarich::MetaTypeBase& x, const Odarich::MetaTypeBase& y) const
		{
			return  (x.getType() == y.getType() && x.getID() == y.getID());
		}
	};
}*/

namespace Meta
{
	enum Type { Unit, Tech, Upgrade, Command, Default };

}

namespace Odarich 
{
	
	class MetaType
	{
	protected:

		Meta::Type _type;
		int32 _id;
		BWAPI::UnitType _unit;
		BWAPI::UpgradeType _upgrade;
		BWAPI::TechType _tech;

	public:

		MetaType(const BWAPI::UnitType & value);
		MetaType(const BWAPI::UpgradeType & value);
		MetaType(const BWAPI::TechType & value);

		const Meta::Type getType() const;
		const int32 getID() const;
		int mineralPrice() const;
		int gasPrice() const;
		int supplyRequired() const;
		int buildTime() const;
		bool isUnit() const;
		bool isTech() const;
		bool isUpgrade() const;
		bool isBuilding() const;
		bool isAddon() const;
		bool isRefinary() const;

		// returns what building or unit type produce the current meta type
		BWAPI::UnitType getProducer() const;
		

		const BWAPI::UnitType & getUnitType() const;
		const BWAPI::TechType & getTechType() const;
		const BWAPI::UpgradeType & getUpgradeType() const;


	};



	/*template<class T>
	const T& MetaTypeBase::value() const
	{
		return dynamic_cast<const MetaType<T>&>(*this).value();
	}

	MetaType<BWAPI::UnitType>::MetaType(const BWAPI::UnitType & unitType)
		:
		MetaTypeBase(unitType)
	{
		_value = unitType;
		_type = Meta::Type::Unit;
	}

	MetaType<BWAPI::UnitType>::MetaType(const int32 & unitID)
		:
		MetaTypeBase(unitID)
	{
		_value = unitID;
		_type = Meta::Type::Unit;
	}

	


	//template<BWAPI::UnitType>
	int MetaType<BWAPI::UnitType>::mineralPrice() const
	{
		return _value.mineralPrice();
	}

	int MetaType<BWAPI::UnitType>::gasPrice() const
	{
		return _value.mineralPrice();
	}

	
	*/

	/*template<>
	class MetaType<BWAPI::UnitType> : public MetaTypeBase
	{
	public:

		MetaType(const BWAPI::UnitType & value)
			:
			MetaTypeBase(value)
		{
			_value = value;
		}
	};*/
}


namespace std
{
	
	template<>
	struct hash<Odarich::MetaType>
	{
		Odarich::int32 operator()(Odarich::MetaType const & x) const
		{
			return x.getType();

		}


	};

	//template <class U>
	template<>
	struct equal_to<Odarich::MetaType> : public unary_function<Odarich::MetaType, bool>
	{
		bool operator()(const Odarich::MetaType& x, const Odarich::MetaType& y) const
		{
			return  (x.getType() == y.getType() && x.getID() == y.getID());
		}
	};
}

#endif