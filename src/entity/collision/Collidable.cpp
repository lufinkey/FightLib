
#include <fightlib/entity/collision/Collidable.hpp>
#include <fightlib/entity/collision/CollisionManager.hpp>
#include <fightlib/entity/collision/PhysicsConstants.hpp>
#include <Box2D/Box2D.hpp>

namespace fl
{
	Collidable::Collidable(const fgl::Vector2d& position)
		: collisionManager(nullptr),
		collisionMethod(COLLISIONMETHOD_PIXEL),
		needsPolygonsUpdate(false)
	{
		physicalState = new PhysicalState();
		physicalState->position = position;
	}

	Collidable::~Collidable()
	{
		if(collisionManager!=nullptr)
		{
			auto world = collisionManager->getWorld();
			world->Destroy(physicsBody);
		}
		else
		{
			delete physicalState;
		}
	}

	bool Collidable::isRotationFixed() const
	{
		return true;
	}

	double Collidable::getFriction(size_t polygonIndex) const
	{
		return 0.3;
	}

	fgl::Vector2d Collidable::getPosition() const
	{
		if(collisionManager!=nullptr)
		{
			auto pos = physicsBody->GetLocation();
			return fgl::Vector2d((double)pos.x * METERS_TO_PIXELS, (double)pos.y * METERS_TO_PIXELS);
		}
		else
		{
			return physicalState->position;
		}
	}

	void Collidable::setPosition(const fgl::Vector2d& position)
	{
		if(collisionManager!=nullptr)
		{
			box2d::Length2D pos((box2d::RealNum)(position.x * PIXELS_TO_METERS), (box2d::RealNum)(position.y * PIXELS_TO_METERS));
			auto angle = physicsBody->GetAngle();
			physicsBody->SetTransform(pos, angle);
		}
		else
		{
			physicalState->position = position;
		}
	}

	double Collidable::getRotation() const
	{
		if(collisionManager!=nullptr)
		{
			return fgl::Math::radtodeg((double)physicsBody->GetAngle());
		}
		else
		{
			return physicalState->rotation;
		}
	}

	void Collidable::setRotation(double degrees)
	{
		if(collisionManager!=nullptr)
		{
			auto pos = physicsBody->GetLocation();
			physicsBody->SetTransform(pos, (box2d::RealNum)fgl::Math::degtorad(degrees));
		}
		else
		{
			physicalState->rotation = degrees;
		}
	}

	fgl::Vector2d Collidable::getVelocity() const
	{
		if(collisionManager!=nullptr)
		{
			auto vel = physicsBody->GetVelocity().linear;
			return fgl::Vector2d((double)vel.x * METERS_TO_PIXELS, (double)vel.y * METERS_TO_PIXELS);
		}
		else
		{
			return physicalState->velocity;
		}
	}

	void Collidable::setVelocity(const fgl::Vector2d& velocity)
	{
		if(collisionManager!=nullptr)
		{
			auto vel = physicsBody->GetVelocity();
			vel.linear = box2d::LinearVelocity2D((box2d::RealNum)(velocity.x * PIXELS_TO_METERS), (box2d::RealNum)(velocity.y * PIXELS_TO_METERS));
			physicsBody->SetVelocity(vel);
		}
		else
		{
			physicalState->velocity = velocity;
		}
	}

	double Collidable::getRotationalVelocity() const
	{
		if(collisionManager!=nullptr)
		{
			return fgl::Math::radtodeg((double)physicsBody->GetVelocity().angular);
		}
		else
		{
			return physicalState->rotationalVelocity;
		}
	}

	void Collidable::setRotationalVelocity(double degreesPerSecond)
	{
		if(collisionManager!=nullptr)
		{
			auto vel = physicsBody->GetVelocity();
			vel.angular = (box2d::RealNum)fgl::Math::degtorad(degreesPerSecond);
			physicsBody->SetVelocity(vel);
		}
		else
		{
			physicalState->rotationalVelocity = degreesPerSecond;
		}
	}
	
	void Collidable::applyForce(const fgl::Vector2d& force)
	{
		physicsBody->ApplyForceToCenter(b2Vec2((float32)(force.x * PIXELS_TO_METERS), (float32)(force.y * PIXELS_TO_METERS)), true);
	}

	void Collidable::applyForce(const fgl::Vector2d& force, const fgl::Vector2d& point)
	{
		physicsBody->ApplyForce(b2Vec2((float32)(force.x * PIXELS_TO_METERS), (float32)(force.y * PIXELS_TO_METERS)),
								b2Vec2((float32)(point.x * PIXELS_TO_METERS), (float32)(point.y * PIXELS_TO_METERS)),
								true);
	}

	fgl::ArrayList<fgl::PolygonD> Collidable::getTransformedCollisionPolygons() const
	{
		fgl::TransformD transform;
		transform.translate(getPosition());
		transform.rotate(getRotation());

		auto polygons = getCollisionPolygons();
		for(auto& polygon : polygons)
		{
			polygon = transform.transform(polygon);
		}
		return polygons;
	}

	b2Body* Collidable::getPhysicsBody()
	{
		if(collisionManager!=nullptr)
		{
			return physicsBody;
		}
		return nullptr;
	}

	CollisionManager* Collidable::getCollisionManager() const
	{
		return collisionManager;
	}

	void Collidable::initPhysicsBody()
	{
		if(collisionManager==nullptr)
		{
			//boy whatchu doin
			throw fgl::IllegalStateException("cannot call initPhysicsBody when not added to a CollisionManager");
		}

		auto world = collisionManager->getWorld();

		PhysicalState state = *physicalState;
		delete physicalState;

		b2BodyDef bodyDef;
		bodyDef.userData = this;
		bodyDef.fixedRotation = isRotationFixed();
		bodyDef.type = isStaticCollisionBody() ? b2_kinematicBody : b2_dynamicBody;
		bodyDef.position = b2Vec2((float32)(state.position.x * PIXELS_TO_METERS), (float32)(state.position.y * PIXELS_TO_METERS));
		bodyDef.linearVelocity = b2Vec2((float32)(state.velocity.x * PIXELS_TO_METERS), (float32)(state.velocity.y * PIXELS_TO_METERS));
		bodyDef.angle = (float32)fgl::Math::degtorad(state.rotation);
		bodyDef.angularVelocity = (float32)fgl::Math::degtorad(state.rotationalVelocity);

		physicsBody = world->CreateBody(&bodyDef);

		updateCollisionPolygons();
	}

	void Collidable::deinitPhysicsBody()
	{
		if(collisionManager==nullptr)
		{
			//damn you really fuckin up
			throw fgl::IllegalStateException("cannot call deinitPhysicsBody when collisionManager is null");
		}

		auto world = collisionManager->getWorld();

		auto position = getPosition();
		auto velocity = getVelocity();
		auto rotation = getRotation();
		auto rotationalVelocity = getRotationalVelocity();

		world->DestroyBody(physicsBody);

		physicalState = new PhysicalState();
		physicalState->position = position;
		physicalState->velocity = velocity;
		physicalState->rotation = rotation;
		physicalState->rotationalVelocity = rotationalVelocity;
	}

	fgl::Vector2d Collidable::getDrawPosition() const
	{
		return Collidable::getPosition();
	}

	double Collidable::getDrawRotation() const
	{
		return Collidable::getRotation();
	}

	fgl::Vector2d Collidable::getDrawScale() const
	{
		return fgl::Vector2d(1.0, 1.0);
	}

	void Collidable::onBeginCollisionUpdates()
	{
		if(needsPolygonsUpdate)
		{
			updateCollisionPolygons();
		}
	}

	bool Collidable::respondsToCollision(Collidable* collided, CollisionData data) const
	{
		return true;
	}
	
	void Collidable::onCollision(const CollisionEvent& collisionEvent)
	{
		//
	}
	
	void Collidable::onCollisionUpdate(const CollisionEvent& collisionEvent)
	{
		//
	}
	
	void Collidable::onCollisionFinish(const CollisionEvent& collisionEvent)
	{
		//
	}
	
	void Collidable::onFinishCollisionUpdates()
	{
		//
	}

	fgl::ArrayList<fgl::PolygonD> Collidable::getCollisionPolygons() const
	{
		if(collisionMethod==COLLISIONMETHOD_PIXEL)
		{
			auto animData = getCurrentAnimationData();
			if(animData==nullptr)
			{
				return {};
			}
			auto frameIndex = getCurrentAnimationFrameIndex();
			auto polygons = animData->getPolygons(frameIndex);
			auto size = (fgl::Vector2d)animData->getSize(frameIndex);

			fgl::TransformD transform;
			transform.scale(getDrawScale());
			transform.translate(-size/2.0);
			for(auto& polygon : polygons)
			{
				polygon = transform.transform(polygon).makeConvex();
				polygon.simplify((size_t)b2_maxPolygonVertices);
			}
			return polygons;
		}
		else if(collisionMethod==COLLISIONMETHOD_BOUNDS)
		{
			auto animData = getCurrentAnimationData();
			if(animData==nullptr)
			{
				return {};
			}
			auto frameIndex = getCurrentAnimationFrameIndex();
			auto boundList = animData->getBounds(frameIndex, getAnimationOrientation());
			auto size = (fgl::Vector2d)animData->getSize(frameIndex);

			fgl::ArrayList<fgl::PolygonD> polygons;
			polygons.reserve(boundList.size());
			fgl::TransformD transform;
			transform.scale(getDrawScale());
			transform.translate(-size/2.0);
			for(auto& bounds : boundList)
			{
				auto polygon = transform.transform(bounds.rect.toPolygon()).makeConvex();
				polygon.simplify((size_t)b2_maxPolygonVertices);
				polygons.add(polygon);
			}
			return polygons;
		}
		else if(collisionMethod==COLLISIONMETHOD_FRAME)
		{
			auto size = getSize();
			auto frame = fgl::RectangleD(-size.x/2, -size.y/2, size.x, size.y);
			return {frame.toPolygon()};
		}
		return {};
	}

	void Collidable::setNeedsCollisionPolygonsUpdate()
	{
		needsPolygonsUpdate = true;
	}

	bool Collidable::needsCollisionPolygonsUpdate() const
	{
		return needsPolygonsUpdate;
	}

	void Collidable::updateCollisionPolygons()
	{
		if(collisionManager==nullptr)
		{
			//TODO should I throw an exception here?
			return;
		}
		//destroy old fixtures
		while(auto fixture = physicsBody->GetFixtureList())
		{
			physicsBody->DestroyFixture(fixture);
		}
		//create new fixtures
		auto polygons = getCollisionPolygons();
		size_t index = 0;
		for(auto& polygon : polygons)
		{
			b2PolygonShape shape;
			auto& points = polygon.getPoints();
			size_t vertexCount = points.size();
			b2Vec2* vertices = new b2Vec2[vertexCount];
			for(size_t i=0; i<vertexCount; i++)
			{
				auto point = points[i];
				vertices[i] = b2Vec2((float32)(point.x * PIXELS_TO_METERS), (float32)(point.y * PIXELS_TO_METERS));
			}
			shape.Set(vertices, (int32)vertexCount);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.friction = getFriction(index);
			physicsBody->CreateFixture(&fixtureDef);

			index++;
		}
		needsPolygonsUpdate = false;
	}

	void Collidable::setCollisionMethod(CollisionMethod collisionMethod_arg)
	{
		collisionMethod = collisionMethod_arg;
	}

	CollisionMethod Collidable::getCollisionMethod() const
	{
		return collisionMethod;
	}
}
