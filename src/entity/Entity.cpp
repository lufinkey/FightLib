
#include <fightlib/entity/Entity.hpp>
#include <fightlib/stage/Platform.hpp>

namespace fl
{
#define setOptionalArg(arg, value) if(arg!=nullptr){ *arg = value; }
	
	Entity::Entity(const fgl::Vector2d& position, Orientation orientation)
		: StageObject(position),
		scale(1.0f),
		orientation(orientation),
		parentEntity(nullptr)
	{
		//
	}

	bool Entity::getFlag(const fgl::String& flag) const
	{
		if(flag=="Entity")
		{
			return true;
		}
		return StageObject::getFlag(flag);
	}

	void Entity::update(fgl::ApplicationData appData)
	{
		StageObject::update(appData);

		auto velocity = getVelocity();
		//make entities move when on top of other collidables
		auto bottomCollidables = getCollided(fgl::COLLISIONSIDE_BOTTOM);
		if(bottomCollidables.size() > 0)
		{
			if(movesWithGround())
			{
				auto ground = bottomCollidables[0];
				auto groundMovement = ground->getDisplacement();
				shift(fgl::Vector2d(groundMovement.x, 0));
			}
		}
		//update friction
		for(auto collisionSide : {fgl::COLLISIONSIDE_BOTTOM, fgl::COLLISIONSIDE_TOP, fgl::COLLISIONSIDE_LEFT, fgl::COLLISIONSIDE_RIGHT})
		{
			auto collidables = getCollided(collisionSide);
			auto platforms = collidables.filter([](fgl::Collidable* const & collidable) -> bool {
				if(collidable->getFlag("Platform"))
				{
					return true;
				}
				return false;
			});
			if(platforms.size() > 0)
			{
				auto platform = static_cast<Platform*>(platforms[0]);
				velocity += platform->getFriction(appData, this, CollisionSide_getOpposite(collisionSide));
			}
		}
		setVelocity(velocity);
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
		StageObject::draw(appData, graphics);
	}

	void Entity::draw(fgl::ApplicationData appData, fgl::Graphics graphics) const
	{
		fgl::Graphics childGraphics = graphics;
		auto transformStateOffset = getDrawTransformState();
		auto offset = transformStateOffset.position;
		if(parentEntity==nullptr)
		{
			childGraphics.translate(offset.x, offset.y);
		}
		else
		{
			//don't draw if entity is not being drawn from parent
			if(!fgl::extract<bool>(appData.additionalData, "DrawnFromEntityParent", false))
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
		childrenAppData.additionalData["DrawnFromEntityParent"] = true;

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

	fgl::TransformState Entity::getTransformState() const
	{
		if(parentEntity==nullptr)
		{
			return StageObject::getTransformState();
		}
		auto childOffsetTransformState = StageObject::getTransformState();
		auto childOffset = childOffsetTransformState.position;
		auto childRotation = childOffsetTransformState.rotation;

		//if parent orientation is right, the offset should be flipped
		AnimationOrientation parentOrientation = parentEntity->getAnimationOrientation();
		if(parentOrientation==ANIMATIONORIENTATION_RIGHT)
		{
			childOffset.x = -childOffset.x;
		}

		fgl::Vector2d parentOffset;
		double anchorRotation = 0;
		fgl::Vector2d anchorRotationPoint;
		getAnchorData(&parentOffset, &anchorRotation, &anchorRotationPoint, nullptr, nullptr);
		fgl::Vector2d fullOffset = parentOffset+childOffset;
		if(anchorRotation!=0)
		{
			fgl::TransformD rotationTransform;
			rotationTransform.rotate(anchorRotation, anchorRotationPoint);
			fullOffset = rotationTransform.transform(fullOffset);
		}

		auto parentTransformState = parentEntity->getTransformState();
		auto parentPosition= parentTransformState.position;
		auto parentRotation = parentTransformState.rotation;
		
		return fgl::TransformState(parentPosition+fullOffset, parentRotation+anchorRotation+childRotation);
	}
	
	void Entity::shift(const fgl::Vector2d& offset)
	{
		if(parentEntity != nullptr)
		{
			// shift parent
			parentEntity->shift(offset);
		}
		else
		{
			StageObject::shift(offset);
		}
	}
	
	double Entity::getRotation() const
	{
		return Entity::getTransformState().rotation;
	}
	
	fgl::Vector2d Entity::getPosition() const
	{
		return Entity::getTransformState().position;
	}

	void Entity::setPosition(const fgl::Vector2d& position_arg)
	{
		if(parentEntity!=nullptr)
		{
			//position can't be changed while anchored to a parent entity (atleast for now...)
			return;
		}
		StageObject::setPosition(position_arg);
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
		if(parentEntity!=nullptr)
		{
			return false;
		}
		return StageObject::respondsToGravity();
	}

	bool Entity::respondsToAirResistance() const
	{
		if(isOnGround() || parentEntity!=nullptr)
		{
			return false;
		}
		return StageObject::respondsToAirResistance();
	}

	bool Entity::movesWithGround() const
	{
		return true;
	}

	bool Entity::usesHitboxes() const
	{
		return true;
	}

	void Entity::onCollision(const fgl::CollisionEvent& collisionEvent)
	{
		CollidedObject collidedObject;
		collidedObject.collidable = collisionEvent.getCollided();
		collidedObject.side = collisionEvent.getCollisionSide();
		collidedObjects.add(collidedObject);
		Collidable::onCollision(collisionEvent);
	}

	void Entity::onCollisionFinish(const fgl::CollisionEvent& collisionEvent)
	{
		auto collidable = collisionEvent.getCollided();
		auto side = collisionEvent.getCollisionSide();
		for(size_t i=0; i<collidedObjects.size(); i++)
		{
			auto& collidedObject = collidedObjects[i];
			if(collidedObject.collidable==collidable && collidedObject.side==side)
			{
				collidedObjects.remove(i);
				break;
			}
		}
		Collidable::onCollisionFinish(collisionEvent);
	}
	
	void Entity::onFinishCollisionUpdates()
	{
		StageObject::onFinishCollisionUpdates();
		auto velocity = getVelocity();
		if(isStaticCollidableOnSide(fgl::COLLISIONSIDE_LEFT))
		{
			if(velocity.x < 0)
			{
				velocity.x = 0;
			}
		}
		if(isStaticCollidableOnSide(fgl::COLLISIONSIDE_TOP))
		{
			if(velocity.y < 0)
			{
				velocity.y = 0;
			}
		}
		if(isStaticCollidableOnSide(fgl::COLLISIONSIDE_RIGHT))
		{
			if(velocity.x > 0)
			{
				velocity.x = 0;
			}
		}
		if(isStaticCollidableOnSide(fgl::COLLISIONSIDE_BOTTOM))
		{
			if(velocity.y > 0)
			{
				velocity.y = 0;
			}
		}
		setVelocity(velocity);
	}

	bool Entity::respondsToHitboxClash(Entity* clashedEntity) const
	{
		return false;
	}

	bool Entity::canCollideWithEntityHitbox(Entity* collidedEntity) const
	{
		return false;
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

	bool Entity::isOnGround() const
	{
		for(auto& collidedObject : collidedObjects)
		{
			if(collidedObject.side==fgl::COLLISIONSIDE_BOTTOM)
			{
				return true;
			}
		}
		return false;
	}

	fgl::ArrayList<fgl::Collidable*> Entity::getCollided(fgl::CollisionSide side) const
	{
		fgl::ArrayList<fgl::Collidable*> collidables;
		for(auto& collidedObject : collidedObjects)
		{
			if(collidedObject.side==side)
			{
				collidables.add(collidedObject.collidable);
			}
		}
		return collidables;
	}

	bool Entity::isStaticCollidableOnSide(fgl::CollisionSide side) const
	{
		for(auto collidedObject : collidedObjects)
		{
			if(collidedObject.side==side)
			{
				auto collidable = collidedObject.collidable;
				if(collidable->isStaticCollisionBody())
				{
					return true;
				}
				else if(collidable->getFlag("Entity") && static_cast<Entity*>(collidable)->isStaticCollidableOnSide(side))
				{
					return true;
				}
			}
		}
		return false;
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
		anchoredEntities.add(anchor);
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
			float parentRotation = parentMetaPoint.rotation;
			float childRotation = childMetaPoint.rotation;

			if(!shouldUseParentMetaPointRotation())
			{
				parentRotation = 0;
			}

			fgl::Vector2d parentOrigin = parentEntity->getOrigin();
			//get the offset of the parent point from the parent entity's center
			fgl::Vector2d parentPointOffset = fgl::Vector2d(((double)parentMetaPoint.x*parentEntity->scale)-parentOrigin.x, ((double)parentMetaPoint.y*parentEntity->scale)-parentOrigin.y);
			AnimationOrientation parentOrientation = parentEntity->getAnimationOrientation();
			AnimationOrientation parentAnimationOrientation = parentAnimData->getOrientation();
			if(parentOrientation!=parentAnimationOrientation && parentAnimationOrientation!=ANIMATIONORIENTATION_NEUTRAL)
			{
				parentPointOffset.x = -parentPointOffset.x;
				parentRotation = -parentRotation;
			}

			fgl::Vector2d childOrigin = getOrigin();
			//get the offset of the child point from the child entity's (this entity's) center
			fgl::Vector2d childPointOffset = fgl::Vector2d(((double)childMetaPoint.x*scale)-childOrigin.x, ((double)childMetaPoint.y*scale)-childOrigin.y);
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
}
