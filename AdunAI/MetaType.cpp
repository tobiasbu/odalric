
#include "MetaType.h"

namespace Odarich {

	MetaType::MetaType(const BWAPI::UnitType & value)
	{
		_unit = value;
		_id = value;
		_type = Meta::Type::Unit;
	}

	MetaType::MetaType(const BWAPI::UpgradeType & value)
	{
		_upgrade = value;
		_id = value;
		_type = Meta::Type::Upgrade;
	}

	MetaType::MetaType(const BWAPI::TechType & value)
	{
		_tech = value;
		_id = value;
		_type = Meta::Type::Tech;
	}

	const Meta::Type MetaType::getType() const
	{
		return _type;
	}

	const int32 MetaType::getID() const
	{
		return _id;
	}

	int MetaType::mineralPrice() const
	{
		return isUnit() ? _unit.mineralPrice() : (isTech() ? _tech.mineralPrice() : (isUpgrade() ? _upgrade.mineralPrice() : 0));
	}
	int MetaType::gasPrice() const
	{
		return isUnit() ? _unit.gasPrice() : (isTech() ? _tech.gasPrice() : (isUpgrade() ? _upgrade.gasPrice() : 0));
	}

	int MetaType::supplyRequired() const
	{
		if (isUnit())
		{
			return _unit.supplyRequired();
		}
		else
			return 0;
	}

	int MetaType::buildTime() const
	{
		return isUnit() ? _unit.buildTime() : (isTech() ? _tech.researchTime() : (isUpgrade() ? _upgrade.upgradeTime() : 0));
	}

	bool MetaType::isUnit() const
	{
		return _type == Meta::Type::Unit;
	}

	bool MetaType::isTech() const
	{
		return _type == Meta::Type::Tech;
	}

	bool MetaType::isUpgrade() const
	{
		return _type == Meta::Type::Upgrade;
	}

	bool MetaType::isBuilding() const
	{
		if (isUnit())
		{
			return _unit.isBuilding();
		}
		else 
		{
			return false;
		}
	}

	bool MetaType::isAddon() const
	{
		if (isUnit())
		{
			return _unit.isAddon();
		}
		else 
		{
			return false;
		}
	}

	bool  MetaType::isRefinary() const
	{
		if (isUnit())
		{
			return _unit.isRefinery();
		}
		else
		{
			return false;
		}
	}

	BWAPI::UnitType MetaType::getProducer() const
	{
		return isUnit() ? _unit.whatBuilds().first : (isTech() ? _tech.whatResearches() : _upgrade.whatUpgrades());
	}


	const BWAPI::UnitType & MetaType::getUnitType() const
	{
		return _unit;
	}

	const BWAPI::TechType & MetaType::getTechType() const
	{
		return _tech;
	}

	const BWAPI::UpgradeType & MetaType::getUpgradeType() const
	{
		return _upgrade;
	}




	/*MetaTypeBase::MetaTypeBase(const int32 & id)
	{
		_id = id;
	}

	Meta::Type MetaTypeBase::getType() const
	{
		return _type;
	}

	int32 MetaTypeBase::getID() const
	{
		return _id;
	}*/

}