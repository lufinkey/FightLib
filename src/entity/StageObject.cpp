
#include <fightlib/entity/StageObject.hpp>

namespace fl
{
	StageObject::StageObject(const fgl::Vector2d& position)
		: CollidableSprite(position),
		stage(nullptr),
		collisionMethod(COLLISIONMETHOD_BOUNDS)
	{
		//
	}
	
	StageObject::~StageObject()
	{
		for(auto collisionRect : collisionRects)
		{
			delete collisionRect;
		}
		collisionRects.clear();
	}

	bool StageObject::getFlag(const fgl::String& flag) const
	{
		if(flag=="StageObject")
		{
			return true;
		}
		return CollidableSprite::getFlag(flag);
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

		CollidableSprite::update(appData);
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

	Stage* StageObject::getStage() const
	{
		return stage;
	}

	CollisionMethod StageObject::getCollisionMethod() const
	{
		return collisionMethod;
	}

	void StageObject::setCollisionMethod(CollisionMethod method)
	{
		collisionMethod = method;
	}
	
	fgl::ArrayList<const fgl::CollisionRect*> StageObject::getCollisionRects() const
	{
		return collisionRects;
	}
	
	void StageObject::updateCollisionRects()
	{
		// save old collision rects
		auto prevCollisionRects = collisionRects;
		// update rects using collision method
		fgl::Collidable* collidable = static_cast<fgl::Collidable*>(this);
		switch(collisionMethod)
		{
			case COLLISIONMETHOD_NONE:
				collisionRects = {};
				break;
				
			case COLLISIONMETHOD_FRAME:
				{
					auto origin = getOrigin();
					auto size = getSize();
					auto rect = fgl::RectangleD(-origin.x, -origin.y, size.x, size.y);
					auto rotation = getTransformState().rotation;
					collisionRects = { fgl::CollisionRectBuilder::fromRect("all", rect, prevCollisionRects, rotation, getDrawScale()) };
				}
				break;
				
			case COLLISIONMETHOD_PIXEL:
				collisionRects = CollidableSprite::createCollisionRectsFromAnimation(this, prevCollisionRects);
				break;
				
			case COLLISIONMETHOD_BOUNDS:
				collisionRects = CollidableSprite::createCollisionRectsFromBoundsMetapoints(this, prevCollisionRects);
				break;
				
			default:
				throw new fgl::IllegalArgumentException("collisionMethod", "invalid enum value");
		}
		// delete old collision rects
		for(auto collisionRect : prevCollisionRects)
		{
			delete collisionRect;
		}
	}

	void StageObject::onAddToStage(Stage* stage)
	{
		//
	}

	void StageObject::onRemoveFromStage(Stage* stage)
	{
		//
	}
	
	void StageObject::onBeginCollisionUpdates()
	{
		updateCollisionRects();
		CollidableSprite::onBeginCollisionUpdates();
	}
}
