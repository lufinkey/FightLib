
#include <fightlib/entity/StageObject.hpp>

namespace fl
{
	StageObject::StageObject(const fgl::Vector2d& position)
		: Collidable(position),
		stage(nullptr)
	{
		setCollisionMethod(COLLISIONMETHOD_BOUNDS);
	}

	bool StageObject::getFlag(const fgl::String& flag) const
	{
		if(flag=="StageObject")
		{
			return true;
		}
		return Collidable::getFlag(flag);
	}

	void StageObject::update(fgl::ApplicationData appData)
	{
		auto velocity = getVelocity();
		//make sure velocity is not larger than terminal velocity
		auto terminalVelocity = getTerminalVelocity();
		terminalVelocity.x = fgl::Math::abs(terminalVelocity.x);
		terminalVelocity.y = fgl::Math::abs(terminalVelocity.y);
		if(velocity.x > terminalVelocity.x)
		{
			velocity.x = terminalVelocity.x;
		}
		else if(velocity.x < -terminalVelocity.x)
		{
			velocity.x = -terminalVelocity.x;
		}
		if(velocity.y > terminalVelocity.y)
		{
			velocity.y = terminalVelocity.y;
		}
		else if(velocity.y < -terminalVelocity.y)
		{
			velocity.y = -terminalVelocity.y;
		}
		setVelocity(velocity);

		Collidable::update(appData);
		collisionRectManager.update(appData, this);
	}

	fgl::Vector2d StageObject::getTerminalVelocity() const
	{
		return fgl::Vector2d(10000, 10000);
	}

	bool StageObject::respondsToGravity() const
	{
		return true;
	}

	bool StageObject::respondsToAirResistance() const
	{
		return true;
	}

	fgl::ArrayList<CollisionRect*> StageObject::getCollisionRects() const
	{
		return collisionRectManager.getCollisionRects();
	}

	Stage* StageObject::getStage() const
	{
		return stage;
	}

	CollisionMethod StageObject::getCollisionMethod() const
	{
		return collisionRectManager.getCollisionMethod();
	}

	void StageObject::setCollisionMethod(CollisionMethod method)
	{
		collisionRectManager.setCollisionMethod(method);
	}

    void StageObject::onAddToStage(Stage* stage)
    {
        //
    }

    void StageObject::onRemoveFromStage(Stage* stage)
    {
		//
    }
}
