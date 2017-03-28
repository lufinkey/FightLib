
#include <fightlib/entity/Entity.hpp>
#include <fightlib/entity/collision/rects/BoxCollisionRect.hpp>
#include <fightlib/entity/collision/rects/PixelCollisionRect.hpp>

namespace fl
{
#define setOptionalArg(arg, value) if(arg!=nullptr){ *arg = value; }

	Entity::Entity(const fgl::Vector2d& position, Orientation orientation)
		: offset(position),
		velocity(0, 0),
		scale(1.0f),
		orientation(orientation),
		parentEntity(nullptr)
	{
		setCollisionMethod(COLLISIONMETHOD_BOUNDS);
	}

	void Entity::update(const fgl::ApplicationData& appData)
	{
		Collidable::update(appData);

		//offset for velocity
		offset += (velocity*appData.getFrameSpeedMultiplier());

		collisionRectManager.update(appData, this, velocity*appData.getFrameSpeedMultiplier());
	}

	fgl::Vector2d Entity::getDrawPosition(float* rotation) const
	{
		//TODO find some better way to actually use this when I'm sober
		if(rotation!=nullptr)
		{
			*rotation = 0;
		}
		return fgl::Vector2d(0, 0);
	}
	
	float Entity::getDrawScale() const
	{
		return scale;
	}

	void Entity::draw(const fgl::ApplicationData& appData, fgl::Graphics graphics) const
	{
		if(parentEntity==nullptr)
		{
			graphics.translate(offset.x, offset.y);
		}
		else
		{
			AnimationOrientation parentOrientation = parentEntity->getAnimationOrientation();
			if(parentOrientation==ANIMATIONORIENTATION_RIGHT)
			{
				graphics.translate(-offset.x, offset.y);
			}
			else
			{
				graphics.translate(offset.x, offset.y);
			}
		}

		struct AnchorDrawData
		{
			Entity* entity;
			fgl::Vector2d offset;
			float rotation;
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
					fgl::Graphics entityGraphics = graphics;
					entityGraphics.translate(anchorData.offset);
					if(anchorData.rotation!=0)
					{
						entityGraphics.rotate(anchorData.rotation, anchorData.rotationPoint-anchorData.offset);
					}
					anchorData.entity->draw(appData, entityGraphics);
				}
				else
				{
					frontAnchorDatas.add(anchorData);
				}
			}
		}

		Collidable::draw(appData, graphics);

		for(size_t i=0; i<frontAnchorDatas.size(); i++)
		{
			const AnchorDrawData& anchorData = frontAnchorDatas[i];
			fgl::Graphics entityGraphics = graphics;
			entityGraphics.translate(anchorData.offset);
			if(anchorData.rotation!=0)
			{
				entityGraphics.rotate(anchorData.rotation, anchorData.rotationPoint-anchorData.offset);
			}
			anchorData.entity->draw(appData, entityGraphics);
		}
	}

	fgl::Vector2d Entity::getPosition(float* rotation) const
	{
		fgl::Vector2d childOffset = fgl::Vector2d(offset.x, offset.y);
		if(parentEntity!=nullptr)
		{
			AnimationOrientation parentOrientation = parentEntity->getAnimationOrientation();
			if(parentOrientation==ANIMATIONORIENTATION_RIGHT)
			{
				childOffset.x = -childOffset.x;
			}
		}

		fgl::Vector2d parentOffset;
		float anchorRotation;
		fgl::Vector2d anchorRotationPoint;
		getAnchorData(&parentOffset, &anchorRotation, &anchorRotationPoint, nullptr, nullptr);
		fgl::Vector2d fullOffset = parentOffset+childOffset;
		if(anchorRotation!=0)
		{
			fgl::TransformD rotationTransform;
			rotationTransform.rotate(anchorRotation, anchorRotationPoint);
			fullOffset = rotationTransform.transform(fullOffset);
		}
		if(parentEntity!=nullptr)
		{
			float parentRotation = 0;
			fgl::Vector2d parentPosition = parentEntity->getPosition(&parentRotation);
			setOptionalArg(rotation, parentRotation+anchorRotation)
				return parentPosition+fullOffset;
		}
		setOptionalArg(rotation, anchorRotation)
			return fullOffset;
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

	CollisionMethod Entity::getCollisionMethod() const
	{
		return collisionRectManager.getCollisionMethod();
	}

	void Entity::setCollisionMethod(CollisionMethod collisionMethod)
	{
		collisionRectManager.setCollisionMethod(collisionMethod);
	}

	bool Entity::isStaticCollisionBody() const
	{
		return false;
	}

	fgl::ArrayList<CollisionRect*> Entity::getCollisionRects() const
	{
		return collisionRectManager.getCollisionRects();
	}
	
	void Entity::anchorChildEntity(Entity* child, AnimationMetaPoint::Type childPoint, size_t childPointIndex, AnimationMetaPoint::Type parentPoint, size_t parentPointIndex, const fgl::Vector2d& childOffset)
	{
		if(child->parentEntity!=nullptr)
		{
			throw fgl::IllegalArgumentException("child", "already anchored to a parent");
		}
		//TODO check for another item attached at the same place? idk revisit this when I'm sober
		Anchor anchor;
		child->offset.x = childOffset.x;
		child->offset.y = childOffset.y;
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
				child->offset.x = position.x;
				child->offset.y = position.y;
				child->parentEntity = nullptr;
				anchoredEntities.remove(i);
			}
		}
	}

	void Entity::shift(const fgl::Vector2d& shiftOffset)
	{
		if(parentEntity!=nullptr)
		{
			parentEntity->shift(shiftOffset);
			return;
		}
		offset.x += shiftOffset.x;
		offset.y += shiftOffset.y;

		offset.y = fgl::Math::round(offset.y, 12);
		offset.x = fgl::Math::round(offset.x, 12);
	}

	void Entity::setVelocity(const fgl::Vector2d& velocity_arg)
	{
		velocity = velocity_arg;
	}

	const fgl::Vector2d& Entity::getVelocity() const
	{
		return velocity;
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

	bool Entity::getAnchorData(fgl::Vector2d* posOffset, float* rotation, fgl::Vector2d* rotationPoint, bool* behind, bool* visible) const
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

			fgl::Vector2d parentSize = parentEntity->getSize();
			fgl::Vector2d parentPointOffset = fgl::Vector2d(((double)parentMetaPoint.x*parentEntity->scale)-(parentSize.x/2), ((double)parentMetaPoint.y*parentEntity->scale)-(parentSize.y/2));
			AnimationOrientation parentOrientation = parentEntity->getAnimationOrientation();
			AnimationOrientation parentAnimationOrientation = parentAnimData->getOrientation();
			if(parentOrientation!=parentAnimationOrientation && parentAnimationOrientation!=ANIMATIONORIENTATION_NEUTRAL)
			{
				parentPointOffset.x = -parentPointOffset.x;
				parentRotation = -parentRotation;
			}

			fgl::Vector2d childSize = getSize();
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
}
