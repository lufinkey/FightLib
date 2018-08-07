
#include <fightlib/entity/collision/CollidableSprite.hpp>

namespace fl
{
	CollidableSprite::CollidableSprite(const fgl::Vector2d& position)
		: Sprite(position),
		Collidable(),
		velocity(0,0)
	{
		//
	}
	
	bool CollidableSprite::getFlag(const fgl::String& flag) const
	{
		if(flag=="CollidableSprite")
		{
			return true;
		}
		return Sprite::getFlag(flag) || Collidable::getFlag(flag);
	}
	
	void CollidableSprite::update(fgl::ApplicationData appData) {
		auto position = getPosition();
		position += (velocity * appData.getFrameSpeedMultiplier());
		setPosition(position);
		Sprite::update(appData);
	}
	
	fgl::Vector2d CollidableSprite::getVelocity() const {
		return velocity;
	}
	
	void CollidableSprite::setVelocity(const fgl::Vector2d& velocity_arg) {
		velocity = velocity_arg;
	}
	
	void CollidableSprite::applyForce(const fgl::Vector2d& force) {
		velocity += (force / getMass());
	}
	
	fgl::TransformState CollidableSprite::getTransformState() const
	{
		return fgl::TransformState(Sprite::getPosition(), Sprite::getRotation());
	}
	
	void CollidableSprite::shift(const fgl::Vector2d& offset)
	{
		auto position = Sprite::getPosition();
		position += offset;
		Sprite::setPosition(position);
	}
	
	fgl::ArrayList<fgl::CollisionRect*> CollidableSprite::createCollisionRectsFromAnimation(CollidableSprite* collidable, const fgl::ArrayList<fgl::CollisionRect*>& prevRects)
	{
		auto animData = collidable->getCurrentAnimationData();
		if(animData == nullptr)
		{
			return {};
		}
		auto anim = animData->getAnimation();
		if(anim == nullptr)
		{
			return {};
		}
		auto frameIndex = collidable->getCurrentAnimationFrameIndex();
		bool mirroredHorizontal = animData->isMirrored(collidable->getAnimationOrientation());
		bool mirroredVertical = false;
		return { fgl::CollisionRectBuilder::fromAnimation("all", collidable, prevRects, collidable->getSize(), collidable->getOrigin(), anim, mirroredHorizontal, mirroredVertical) };
	}
	
	fgl::ArrayList<fgl::CollisionRect*> CollidableSprite::createCollisionRectsFromBoundsMetapoints(CollidableSprite* collidable, const fgl::ArrayList<fgl::CollisionRect*>& prevRects)
	{
		auto origin = collidable->getOrigin();
		auto transformState = collidable->getTransformState();
		auto prevTransformState = collidable->getPreviousTransformState();
		auto displacement = transformState.position - prevTransformState.position;
		auto scale = collidable->getDrawScale();
		AnimationData* animData = collidable->getCurrentAnimationData();
		if(animData==nullptr)
		{
			return {};
		}
		size_t frameIndex = collidable->getCurrentAnimationFrameIndex();
		AnimationOrientation animationOrientation = collidable->getAnimationOrientation();
		fgl::ArrayList<TaggedBox> boundsList = animData->getBounds(frameIndex, animationOrientation);
		if(boundsList.size()==0)
		{
			return {};
		}
		fgl::ArrayList<fgl::CollisionRect*> newCollisionRects;
		newCollisionRects.reserve(boundsList.size());
		for(size_t i=0; i<boundsList.size(); i++)
		{
			auto& metaBounds = boundsList[i];
			auto rect = fgl::RectangleD((metaBounds.rect.x*scale.x), (metaBounds.rect.y*scale.y), (metaBounds.rect.width*scale.x), (metaBounds.rect.height*scale.y));
			auto boundOrigin = fgl::Vector2d(rect.x-origin.x, rect.y-origin.y);
			rect = fgl::RectangleD(transformState.position.x+boundOrigin.x, transformState.position.y+boundOrigin.y, rect.width, rect.height);
			fgl::String tag;
			if(metaBounds.tag!=-1)
			{
				tag = (fgl::String)"bounds:"+metaBounds.tag;
			}
			else
			{
				tag = (fgl::String)"bounds:index"+i;
			}
			auto lastRect = fgl::CollisionRectBuilder::getMatchingRect(prevRects, tag, rect, displacement);
			if(transformState.rotation!=0.0)
			{
				newCollisionRects.add(new fgl::BoxCollisionRect(tag, rect, lastRect, transformState.rotation, boundOrigin, scale));
			}
			newCollisionRects.add(new fgl::BoxCollisionRect(tag, rect, lastRect, scale));
		}
		return newCollisionRects;
	}
}
