
#ifndef _OAI_RESOURCES_HPP_
#define _OAI_RESOURCES_HPP_

#include "Singleton.hpp"

namespace Odalric {

	class Resources : public Singleton<Resources>
	{
	private:

		int _reservedMinerals;
		int _reservedGas;
		int _reservedSupplies;
		int _availbleSupplies;

	public:

		Resources();
		void clear();
		void update();

		void reserveMinerals(const int & value);
		void reserveGas(const int & value);
		void reserveSupplies(const int & value);

		void unreserveMinerals(const int & value);
		void unreserveGas(const int & value);
		void unreserveSupplies(const int & value);

		int getReservedMinerals() const;
		int getReservedGas() const;
		int getReservedSupplies() const;

		int getAvailbleSupplies() const;
		int getAvailbleMinerals() const;
		int getAvailbleGas() const;
		void draw();
	};

}

#endif