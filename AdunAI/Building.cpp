
#include "Building.h"
#include "Assert.h"

namespace Odalric {

	Building::Building()
		:
		_state(Unassigned),
		_action(nullptr),
		_worker(nullptr),
		_unit(nullptr),
		_sentCommandToWorker(false),
		_type(BWAPI::UnitTypes::Unknown)
	{
		_position = BWAPI::TilePositions::None;
	}

	Building::Building(ActionBuild * action, const BWAPI::TilePosition & position)
		:
		_state(Unassigned),
		_worker(nullptr),
		_unit(nullptr),
		_sentCommandToWorker(false)
	{
		OAI_ASSERT(action != nullptr);

		_position = position;
		_action = action;
		_type = action->getMetaType().getUnitType();
	}

	void Building::assignWorker(Worker * worker)
	{
		//OAI_ASSERT(worker != nullptr);

		_worker = worker;
	}

	void Building::setState(const Building::State & state)
	{
		_state = state;
	}

	void Building::setPosition(const BWAPI::TilePosition & position)
	{
		_position = position;
	}

	void Building::setWorker(Worker * worker)
	{
		_worker = worker;
	}

	void Building::setSentCommandToWorker(const bool & flag)
	{
		_sentCommandToWorker = flag;
	}

	void Building::setUnit(BWAPI::Unit unit)
	{
		_unit = unit;
	}

	void Building::markAsObsolte()
	{
		_obsolete = true;
	}

	bool Building::isDone() const
	{
		if (!_unit)
			return false;
		
		return _unit->isCompleted();
	}

	bool Building::isObsolete() const
	{
		return _obsolete;
	}

	bool Building::isUnderConstruction() const
	{
		return _state == UnderConstruction;
	}

	BWAPI::Unit Building::get() const
	{
		return _unit;
	}

	BWAPI::UnitType Building::getType() const
	{
		return _type;
	}

	ActionBuild * Building::getAction() const
	{
		return _action;
	}

	Worker * Building::getWorker() const
	{
		return _worker;
	}

	Building::State Building::getState() const
	{
		return _state;
	}

	BWAPI::TilePosition Building::getPosition() const
	{
		return _position;
	}

	bool Building::getSentCommandToWorker() const
	{
		return _sentCommandToWorker;
	}


}
