
#include "Resources.h"
#include "Self.h"

namespace Odalric {



	Resources::Resources()
	{
		clear();
	}

	void Resources::clear()
	{
		_reservedMinerals = 0;
		_reservedGas = 0;
		_reservedSupplies = 0;
		_availbleSupplies = 0;
	}

	void Resources::update()
	{
		_availbleSupplies = SELF.me()->supplyTotal(SELF.race()) - SELF.me()->supplyUsed(SELF.race());

		if (_reservedSupplies < 0)
			_reservedSupplies = 0;
	}

	void Resources::reserveMinerals(const int & value)
	{
		_reservedMinerals += value; // *(unreserve) ? -1 : 1;

	}

	void Resources::reserveGas(const int & value)
	{

		_reservedGas += value; // *(unreserve) ? -1 : 1;


	}

	void Resources::reserveSupplies(const int & value)
	{
		_reservedSupplies += value; // *(unreserve) ? -1 : 1;
	}

	void Resources::unreserveMinerals(const int & value)
	{
		_reservedMinerals -= value;

		if (_reservedMinerals < 0)
			_reservedMinerals = 0;
	}

	void Resources::unreserveGas(const int & value)
	{
		_reservedGas -= value;

		if (_reservedGas < 0)
			_reservedGas = 0;
	}

	void Resources::unreserveSupplies(const int & value)
	{
		_reservedSupplies -= value;

		if (_reservedSupplies < 0)
			_reservedSupplies = 0;
	}

	int Resources::getReservedMinerals() const
	{
		return _reservedMinerals;
	}

	int Resources::getReservedGas() const
	{
		return _reservedGas;
	}

	int Resources::getReservedSupplies() const
	{
		return _reservedSupplies;
	}

	int Resources::getAvailbleSupplies() const
	{
		return _availbleSupplies;
	}

	int Resources::getAvailbleMinerals() const
	{
		return SELF.me()->minerals() - _reservedMinerals;
	}

	int Resources::getAvailbleGas() const
	{
		return SELF.me()->gas() - _reservedGas;
	}

	void Resources::draw()
	{
		BWAPI::Broodwar->drawTextScreen(32, 16, "Supplies: %d/%d/%d", SELF.me()->supplyUsed(), SELF.me()->supplyTotal(), _availbleSupplies);
	}

	

}