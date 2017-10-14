
#ifndef _OAI_BUILDING_H_
#define _OAI_BUILDING_H_

#include "ActionBuild.h"
#include "Types.h"
#include "Worker.h"

namespace Odalric {

	class Building
	{
	public:

		enum State : uint8
		{
			Unassigned = 0,
			Assigned = 1,
			UnderConstruction = 2,
			Size = 3
		};
		
		

	private:

		State _state;
		ActionBuild * _action = nullptr;
		Worker * _worker = nullptr;
		BWAPI::Unit _unit = nullptr;
		BWAPI::UnitType _type;
		BWAPI::TilePosition _position;
		bool _sentCommandToWorker = false;
		bool _obsolete = false;

	public:

		Building();
		Building(ActionBuild * action, const BWAPI::TilePosition & position);

		void assignWorker(Worker * worker);
		void setPosition(const BWAPI::TilePosition & position);
		void setState(const Building::State & state);
		void setWorker(Worker * worker);
		void setSentCommandToWorker(const bool & flag);
		void setUnit(BWAPI::Unit unit);
		void markAsObsolte();
		
		bool isDone() const;
		bool isUnderConstruction() const;
		bool isObsolete() const;
		bool canBuild() const;
		BWAPI::Unit get() const;
		BWAPI::UnitType getType() const;
		ActionBuild * getAction() const;
		Building::State getState() const;
		Worker * getWorker() const;
		bool getSentCommandToWorker() const;
		
		int getRemainingTime() const;
		BWAPI::TilePosition getPosition() const;

		bool operator==(const Building & rhs) const
		{
			return (this->_unit == rhs._unit || this->_worker == rhs._worker);
		}
	};

}

#endif