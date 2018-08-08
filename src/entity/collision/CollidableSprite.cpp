
#include <fightlib/entity/collision/CollidableSprite.hpp>

namespace fl
{
	CollidableSprite::CollidableSprite(const fgl::Vector2d& position)
		: Sprite(position),
		Collidable(),
		velocity(0,0) {
		//
	}
	
	bool CollidableSprite::getFlag(const fgl::String& flag) const {
		if(flag=="CollidableSprite") {
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
	
	fgl::TransformState CollidableSprite::getTransformState() const {
		return fgl::TransformState(Sprite::getPosition(), Sprite::getRotation());
	}
	
	void CollidableSprite::shift(const fgl::Vector2d& offset)
	{
		auto position = Sprite::getPosition();
		position += offset;
		Sprite::setPosition(position);
	}
	
	fgl::ArrayList<const fgl::CollisionRect*> CollidableSprite::createCollisionRectsFromAnimation(CollidableSprite* collidable, const fgl::ArrayList<const fgl::CollisionRect*>& prevRects) {
		auto animData = collidable->getCurrentAnimationData();
		if(animData == nullptr) {
			return {};
		}
		auto anim = animData->getAnimation();
		if(anim == nullptr) {
			return {};
		}
		auto frameIndex = collidable->getCurrentAnimationFrameIndex();
		bool mirroredHorizontal = animData->isMirrored(collidable->getAnimationOrientation());
		bool mirroredVertical = false;
		return { fgl::CollisionRectBuilder::fromAnimation("all", anim, frameIndex, prevRects, collidable->getOrigin(), collidable->getSize(), mirroredHorizontal, mirroredVertical) };
	}
	
	fgl::ArrayList<const fgl::CollisionRect*> CollidableSprite::createCollisionRectsFromBoundsMetapoints(CollidableSprite* collidable, const fgl::ArrayList<const fgl::CollisionRect*>& prevRects) {
		auto transformState = collidable->getTransformState();
		auto origin = collidable->getOrigin();
		auto scale = collidable->getDrawScale();
		AnimationData* animData = collidable->getCurrentAnimationData();
		if(animData==nullptr) {
			return {};
		}
		size_t frameIndex = collidable->getCurrentAnimationFrameIndex();
		AnimationOrientation animationOrientation = collidable->getAnimationOrientation();
		fgl::ArrayList<TaggedBox> boundsList = animData->getBounds(frameIndex, animationOrientation);
		if(boundsList.size()==0) {
			return {};
		}
		fgl::ArrayList<const fgl::CollisionRect*> newCollisionRects;
		newCollisionRects.reserve(boundsList.size());
		for(size_t i=0; i<boundsList.size(); i++) {
			auto& metaBounds = boundsList[i];
			auto rect = fgl::RectangleD((metaBounds.rect.x*scale.x), (metaBounds.rect.y*scale.y), (metaBounds.rect.width*scale.x), (metaBounds.rect.height*scale.y));
			auto topLeft = fgl::Vector2d(rect.x-origin.x, rect.y-origin.y);
			rect = fgl::RectangleD(topLeft.x, topLeft.y, rect.width, rect.height);
			fgl::String tag;
			if(metaBounds.tag!=-1) {
				tag = (fgl::String)"bounds:"+metaBounds.tag;
			}
			else {
				tag = (fgl::String)"bounds:index"+i;
			}
			newCollisionRects.add(fgl::CollisionRectBuilder::fromRect(tag, rect, prevRects, transformState.rotation, scale));
		}
		return newCollisionRects;
	}
}
