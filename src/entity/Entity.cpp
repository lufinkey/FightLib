
#include <fightlib/entity/Entity.hpp>
#include <fightlib/entity/collision/CollisionManager.hpp>
#include <Box2D/Box2D.h>

namespace fl
{
#define setOptionalArg(arg, value) if(arg!=nullptr){ *arg = value; }

	Entity::Entity(const fgl::Vector2d& position, Orientation orientation)
		: Collidable(position),
		scale(1.0f),
		orientation(orientation),
		parentEntity(nullptr),
		stage(nullptr)
	{
		setCollisionMethod(COLLISIONMETHOD_BOUNDS);
	}
	
	bool Entity::getFlag(const fgl::String& flag) const
	{
		if(flag=="Entity")
		{
			return true;
		}
		return Collidable::getFlag(flag);
	}

	void Entity::update(fgl::ApplicationData appData)
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
	}
	
	fgl::Vector2d Entity::getDrawScale() const
	{
		return fgl::Vector2d((double)scale, (double)scale);
	}
	
	bool Entity::shouldUseParentMetaPointRotation() const
	{
		return true;
	}

	void Entity::drawMain(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		Collidable::draw(appData, graphics);
	}

	void Entity::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		fgl::Graphics childGraphics = graphics;
		fgl::Vector2d offset = getDrawPosition();
		if(parentEntity==nullptr)
		{
			childGraphics.translate(offset.x, offset.y);
		}
		else
		{
			//don't draw if entity is not being drawn from parent
			if(!fgl::extract<bool>(appData.additionalData, "DrawnFromParent", false))
			{
				return;
			}
			
			AnimationOrientation parentOrientation = parentEntity->getAnimationOrientation();
			if(parentOrientation==ANIMATIONORIENTATION_RIGHT)
			{
				childGraphics.translate(-offset.x, offset.y);
			}
			else
			{
				childGraphics.translate(offset.x, offset.y);
			}
		}
		
		auto childrenAppData = appData;
		childrenAppData.additionalData["DrawnFromParent"] = true;

		struct AnchorDrawData
		{
			Entity* entity;
			fgl::Vector2d offset;
			double rotation;
			fgl::Vector2d rotationPoint;
			bool behind;
			bool visible;
		};
		fgl::ArrayList<AnchorDrawData> frontAnchorDatas;
		for(size_t anchoredEntities_size=anchoredEntities.size(), i=0; i<anchoredEntities_size; i++)
		{
			AnchorDrawData anchorData;
			anchorData.entity = anchoredEntities[i].entity;
			bool drawEntity = anchorData.entity->getAnchorData(&anchorData.offset, &anchorData.rotation, &anchorData.rotationPoint, &anchorData.behind, &anchorData.visible);
			if(drawEntity && anchorData.visible)
			{
				if(anchorData.behind)
				{
					fgl::Graphics entityGraphics = childGraphics;
					entityGraphics.translate(anchorData.offset);
					if(anchorData.rotation!=0)
					{
						entityGraphics.rotate(anchorData.rotation, anchorData.rotationPoint-anchorData.offset);
					}
					anchorData.entity->draw(childrenAppData, entityGraphics);
				}
				else
				{
					frontAnchorDatas.add(anchorData);
				}
			}
		}

		drawMain(appData, graphics);

		for(size_t i=0; i<frontAnchorDatas.size(); i++)
		{
			const AnchorDrawData& anchorData = frontAnchorDatas[i];
			fgl::Graphics entityGraphics = childGraphics;
			entityGraphics.translate(anchorData.offset);
			if(anchorData.rotation!=0)
			{
				entityGraphics.rotate(anchorData.rotation, anchorData.rotationPoint-anchorData.offset);
			}
			anchorData.entity->draw(childrenAppData, entityGraphics);
		}
	}

	fgl::Vector2d Entity::getTerminalVelocity() const
	{
		return fgl::Vector2d(10000, 10000);
	}
	
	HitboxInfo Entity::getHitboxInfo(size_t tag) const
	{
		return NULL_HITBOX_INFO;
	}

	float Entity::getScale() const
	{
		return scale;
	}

	void Entity::setScale(float scale_arg)
	{
		scale = scale_arg;
	}

	Orientation Entity::getOrientation() const
	{
		return orientation;
	}

	void Entity::setOrientation(Orientation orientation_arg)
	{
		orientation = orientation_arg;
	}

	bool Entity::isStaticCollisionBody() const
	{
		return false;
	}
	
	bool Entity::respondsToGravity() const
	{
		return true;
	}
	
	bool Entity::movesWithGround() const
	{
		return true;
	}
	
	bool Entity::usesHitboxes() const
	{
		return true;
	}

	void Entity::setNeedsAnchorsUpdate()
	{
		needsAnchorsUpdate = true;
	}

	void Entity::updateAnchors()
	{
		for(auto& anchor : anchoredEntities)
		{
			updateAnchor(anchor);
		}
		needsAnchorsUpdate = false;
	}

	void Entity::updateAnchor(Anchor& anchor)
	{
		auto collisionMgr = getCollisionManager();
		if(collisionMgr==nullptr)
		{
			return;
		}
		auto world = collisionMgr->getWorld();

		//destroy the old joint
		if(anchor.joint!=nullptr)
		{
			world->DestroyJoint(anchor.joint);
			anchor.joint = nullptr;
		}

		//place the anchored entity where it should be when anchored
		double intendedRotation = 0;
		fgl::Vector2d intendedChildPos = anchor.entity->getIntendedEntityPosition(&intendedRotation);
		anchor.entity->setPosition(intendedChildPos);
		anchor.entity->setRotation(intendedRotation);
		anchor.entity->setVelocity(fgl::Vector2d(0, 0));
		anchor.entity->setRotationalVelocity(0);

		//get the anchor points and rotations
		double parentRotation = 0;
		fgl::Vector2d parentOffset = getMetaPointOffset(anchor.parentPoint, anchor.parentPointIndex, &parentRotation);
		double childRotation = 0;
		fgl::Vector2d childOffset = anchor.entity->getMetaPointOffset(anchor.childPoint, anchor.childPointIndex, &childRotation);

		//create the new joint
		auto parentBody = getPhysicsBody();
		auto childBody = anchor.entity->getPhysicsBody();
		if(parentBody==nullptr || childBody==nullptr)
		{
			return;
		}
		b2WeldJointDef jointDef;
		jointDef.bodyA = parentBody;
		jointDef.bodyB = childBody;
		jointDef.collideConnected = false;
		if(anchor.entity->shouldUseParentMetaPointRotation())
		{
			jointDef.referenceAngle = fgl::Math::degtorad(parentRotation+childRotation);
		}
		else
		{
			jointDef.referenceAngle = fgl::Math::degtorad(childRotation);
		}
		jointDef.localAnchorA = b2Vec2((float32)parentOffset.x, (float32)parentOffset.y);
		jointDef.localAnchorB = b2Vec2((float32)childOffset.x, (float32)childOffset.y);
		anchor.joint = static_cast<b2WeldJoint*>(world->CreateJoint(&jointDef));
	}

	fgl::Vector2d Entity::getMetaPointOffset(MetaPointType metaPointType, size_t metaPointIndex, double* rotation) const
	{
		auto animData = getCurrentAnimationData();
		if(animData==nullptr)
		{
			setOptionalArg(rotation, 0)
				return fgl::Vector2d(0, 0);
		}
		size_t frameIndex = getCurrentAnimationFrameIndex();
		auto metaPoints = animData->getMetaPoints(frameIndex, metaPointType);
		if(metaPointIndex >= metaPoints.size())
		{
			setOptionalArg(rotation, 0);
			return fgl::Vector2d(0, 0);
		}
		auto metaPoint = metaPoints[metaPointIndex];

		auto size = getSize();
		//get the offset of the point from the entity's center
		fgl::Vector2d offset = fgl::Vector2d(((double)metaPoint.x*scale)-(size.x/2), ((double)metaPoint.y*scale)-(size.y/2));
		double metaRotation = (double)metaPoint.rotation;

		//flip the offset if the orientation is flipped
		AnimationOrientation orientation = getAnimationOrientation();
		AnimationOrientation animOrientation = animData->getOrientation();
		if(orientation!=animOrientation && animOrientation!=ANIMATIONORIENTATION_NEUTRAL)
		{
			offset.x = -offset.x;
			metaRotation = -metaRotation;
		}

		setOptionalArg(rotation, metaRotation);
		return offset;
	}

	fgl::Vector2d Entity::getIntendedEntityPosition(double* rotation) const
	{
		if(parentEntity==nullptr)
		{
			setOptionalArg(rotation, getRotation());
			return getPosition();
		}

		fgl::Vector2d offset;
		double anchorRotation;
		fgl::Vector2d anchorRotationPoint;
		getAnchorData(&offset, &anchorRotation, &anchorRotationPoint, nullptr, nullptr);
		if(anchorRotation!=0)
		{
			fgl::TransformD rotationTransform;
			rotationTransform.rotate(anchorRotation, anchorRotationPoint);
			offset = rotationTransform.transform(offset);
		}

		double parentRotation = parentEntity->getRotation();
		fgl::Vector2d parentPosition = parentEntity->getPosition();
		setOptionalArg(rotation, parentRotation+anchorRotation)
			return parentPosition+offset;
	}

	void Entity::onBeginCollisionUpdates()
	{
		Collidable::onBeginCollisionUpdates();
		if(needsAnchorsUpdate)
		{
			updateAnchors();
		}
	}
	
	bool Entity::respondsToHitboxClash(Entity* clashedEntity) const
	{
		return false;
	}
	
	bool Entity::canCollideWithEntityHitbox(Entity* collidedEntity) const
	{
		return false;
	}

	void Entity::onBeginHitboxUpdates()
	{
		if(needsCollisionPolygonsUpdate())
		{
			updateCollisionPolygons();
		}
		if(needsAnchorsUpdate)
		{
			updateAnchors();
		}
	}

	void Entity::onHitboxClash(const HitboxClashEvent& clashEvent)
	{
		//
	}
	
	void Entity::onHitboxClashUpdate(const fl::HitboxClashEvent& clashEvent)
	{
		//
	}
	
	void Entity::onHitboxClashFinish(const fl::HitboxClashEvent& clashEvent)
	{
		//
	}
	
	void Entity::onHitboxCollision(const HitboxCollisionEvent& collisionEvent)
	{
		//
	}
	
	void Entity::onHitboxCollisionUpdate(const fl::HitboxCollisionEvent& collisionEvent)
	{
		//
	}
	
	void Entity::onHitboxCollisionFinish(const fl::HitboxCollisionEvent& collisionEvent)
	{
		//
	}
	
	void Entity::onFinishHitboxUpdates()
	{
		//
	}

	void Entity::onAddToStage(Stage* stage)
	{
		//
	}

	void Entity::onRemoveFromStage(Stage* stage)
	{
		//
	}

	bool Entity::isOnGround() const
	{
		//TODO make this be perpendicular to gravity with a 90 degree threshold
		if(getCollided(COLLISIONSIDE_BOTTOM).size() > 0)
		{
			return true;
		}
		return false;
	}

	fgl::ArrayList<Collidable*> Entity::getCollided(CollisionSide side) const
	{
		auto collisionManager = getCollisionManager();
		if(collisionManager==nullptr)
		{
			return {};
		}
		fgl::ArrayList<Collidable*> collided;
		for(auto& pair : collisionManager->getCollisionPairs())
		{
			if(pair.collidable1==this)
			{
				for(auto& data : pair.previousCollisions)
				{
					if(data.getSide()==side)
					{
						collided.add(pair.collidable2);
						break;
					}
				}
			}
			else if(pair.collidable2==this)
			{
				CollisionSide oppSide = CollisionSide_getOpposite(side);
				for(auto& data : pair.previousCollisions)
				{
					if(data.getSide()==oppSide)
					{
						collided.add(pair.collidable1);
						break;
					}
				}
			}
		}
		return collided;
	}
	
	void Entity::anchorChildEntity(Entity* child, MetaPointType childPoint, size_t childPointIndex, MetaPointType parentPoint, size_t parentPointIndex, const fgl::Vector2d& childOffset)
	{
		if(child->parentEntity!=nullptr)
		{
			throw fgl::IllegalArgumentException("child", "already anchored to a parent");
		}
		//TODO check for another item attached at the same place? idk revisit this when I'm sober
		Anchor anchor;
		child->setPosition(childOffset);
		child->parentEntity = this;
		anchor.entity = child;
		anchor.childPoint = childPoint;
		anchor.childPointIndex = childPointIndex;
		anchor.parentPoint = parentPoint;
		anchor.parentPointIndex = parentPointIndex;
		anchor.joint = nullptr;
		anchoredEntities.add(anchor);
		updateAnchor(anchor);
	}
	
	void Entity::removeAnchoredEntity(Entity* child)
	{
		for(size_t anchoredEntities_size=anchoredEntities.size(), i=0; i<anchoredEntities_size; i++)
		{
			const Anchor& anchor = anchoredEntities[i];
			if(anchor.entity==child)
			{
				fgl::Vector2d position = child->getPosition();
				//TODO do something about rotation when it gets released? maybe pass a pointer as an optional parameter? idk
				child->parentEntity = nullptr;
				anchoredEntities.remove(i);
				child->setPosition(position);
				return;
			}
		}
	}

	const fgl::ArrayList<Entity::Anchor>& Entity::getAnchoredEntities() const
	{
		return anchoredEntities;
	}
	
	Entity* Entity::getParentEntity() const
	{
		return parentEntity;
	}

	Entity::Anchor Entity::getAnchor(const Entity* entity) const
	{
		for(size_t anchoredEntities_size=anchoredEntities.size(), i=0; i<anchoredEntities_size; i++)
		{
			if(anchoredEntities[i].entity)
			{
				return anchoredEntities[i];
			}
		}
		throw fgl::IllegalArgumentException("entity", "no anchor exists for the given entity");
	}

	bool Entity::getAnchorData(fgl::Vector2d* posOffset, double* rotation, fgl::Vector2d* rotationPoint, bool* behind, bool* visible) const
	{
		//TODO determine how offset should behave with orientation
		if(parentEntity!=nullptr)
		{
			Anchor anchor = parentEntity->getAnchor(this);
			AnimationData* parentAnimData = parentEntity->getCurrentAnimationData();
			AnimationData* childAnimData = getCurrentAnimationData();
			if(parentAnimData==nullptr || childAnimData==nullptr)
			{
				setOptionalArg(posOffset, fgl::Vector2d(0,0))
				setOptionalArg(rotation, 0)
				setOptionalArg(rotationPoint, fgl::Vector2d(0, 0))
				setOptionalArg(behind, false)
				setOptionalArg(visible, false)
				return false;
			}
			size_t parentFrameIndex = parentEntity->getCurrentAnimationFrameIndex();
			size_t frameIndex = getCurrentAnimationFrameIndex();
			fgl::ArrayList<AnimationMetaPoint> parentMetaPoints = parentAnimData->getMetaPoints(parentFrameIndex, anchor.parentPoint);
			fgl::ArrayList<AnimationMetaPoint> childMetaPoints = childAnimData->getMetaPoints(frameIndex, anchor.childPoint);
			if(anchor.parentPointIndex >= parentMetaPoints.size() || anchor.childPointIndex >= childMetaPoints.size())
			{
				setOptionalArg(posOffset, fgl::Vector2d(0, 0))
				setOptionalArg(rotation, 0)
				setOptionalArg(rotationPoint, fgl::Vector2d(0, 0))
				setOptionalArg(behind, false)
				setOptionalArg(visible, false)
				return false;
			}
			AnimationMetaPoint parentMetaPoint = parentMetaPoints[anchor.parentPointIndex];
			AnimationMetaPoint childMetaPoint = childMetaPoints[anchor.childPointIndex];
			double parentRotation = (double)parentMetaPoint.rotation;
			double childRotation = (double)childMetaPoint.rotation;
			
			if(!shouldUseParentMetaPointRotation())
			{
				parentRotation = 0;
			}

			fgl::Vector2d parentSize = parentEntity->getSize();
			//get the offset of the parent point from the parent entity's center
			fgl::Vector2d parentPointOffset = fgl::Vector2d(((double)parentMetaPoint.x*parentEntity->scale)-(parentSize.x/2), ((double)parentMetaPoint.y*parentEntity->scale)-(parentSize.y/2));
			AnimationOrientation parentOrientation = parentEntity->getAnimationOrientation();
			AnimationOrientation parentAnimationOrientation = parentAnimData->getOrientation();
			if(parentOrientation!=parentAnimationOrientation && parentAnimationOrientation!=ANIMATIONORIENTATION_NEUTRAL)
			{
				parentPointOffset.x = -parentPointOffset.x;
				parentRotation = -parentRotation;
			}

			fgl::Vector2d childSize = getSize();
			//get the offset of the child point from the child entity's (this entity's) center
			fgl::Vector2d childPointOffset = fgl::Vector2d(((double)childMetaPoint.x*scale)-(childSize.x/2), ((double)childMetaPoint.y*scale)-(childSize.y/2));
			AnimationOrientation childOrientation = getAnimationOrientation();
			AnimationOrientation childAnimationOrientation = childAnimData->getOrientation();
			if(childOrientation!=childAnimationOrientation && childAnimationOrientation!=ANIMATIONORIENTATION_NEUTRAL)
			{
				childPointOffset.x = -childPointOffset.x;
				childRotation = -childRotation;
			}

			fgl::Vector2d totalOffset = parentPointOffset - childPointOffset;
			
			setOptionalArg(posOffset, totalOffset)
			setOptionalArg(rotation, parentRotation+childRotation)
			setOptionalArg(rotationPoint, parentPointOffset)
			setOptionalArg(behind, parentMetaPoint.behind)
			setOptionalArg(visible, parentMetaPoint.visible)
			return true;
		}
		setOptionalArg(posOffset, fgl::Vector2d(0, 0))
		setOptionalArg(rotation, 0)
		setOptionalArg(rotationPoint, fgl::Vector2d(0, 0))
		setOptionalArg(behind, false)
		setOptionalArg(visible, true)
		return true;
	}

	AnimationOrientation Entity::getAnimationOrientation() const
	{
		if(orientation==ORIENTATION_LEFT)
		{
			return ANIMATIONORIENTATION_LEFT;
		}
		else if(orientation==ORIENTATION_RIGHT)
		{
			return ANIMATIONORIENTATION_RIGHT;
		}
		throw fgl::IllegalStateException("Entity::orientation has an invalid value");
	}
	
	Stage* Entity::getStage() const
	{
		return stage;
	}
}
