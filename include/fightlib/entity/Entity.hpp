
#pragma once

#include "StageObject.hpp"
#include "hitbox/HitboxInfo.hpp"
#include "hitbox/HitboxClashEvent.hpp"
#include "hitbox/HitboxCollisionEvent.hpp"

namespace fl
{
	typedef enum : fgl::Uint8
	{
		ORIENTATION_LEFT,
		ORIENTATION_RIGHT,
	} Orientation;

	class Entity : public StageObject
	{
		friend class HitboxCollisionManager;
	public:
		Entity(const fgl::Vector2d& position, Orientation orientation);

		virtual bool getFlag(const fgl::String& flag) const override;
		virtual void update(fgl::ApplicationData appData) override;
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const override;

		virtual fgl::TransformState getTransformState() const override;
		virtual void shift(const fgl::Vector2d& offset) override;
		
		virtual double getRotation() const override;
		virtual fgl::Vector2d getPosition() const override;
		virtual void setPosition(const fgl::Vector2d& position) override;

		virtual HitboxInfo getHitboxInfo(size_t tag) const;

		float getScale() const;
		void setScale(float scale);

		Orientation getOrientation() const;
		void setOrientation(Orientation orientation);

		virtual bool isStaticCollisionBody() const override;
		virtual bool respondsToGravity() const override;
		virtual bool respondsToAirResistance() const override;
		virtual bool movesWithGround() const;
		virtual bool usesHitboxes() const;

		Entity* getParentEntity() const;

		bool isOnGround() const;

	protected:
		virtual void drawMain(fgl::ApplicationData appData, fgl::Graphics graphics) const;

		virtual AnimationOrientation getAnimationOrientation() const override;

		virtual fgl::Vector2d getDrawScale() const override;
		virtual bool shouldUseParentMetaPointRotation() const;

		virtual void onCollision(const fgl::CollisionEvent& collisionEvent) override;
		virtual void onCollisionFinish(const fgl::CollisionEvent& collisionEvent) override;
		
		virtual void onFinishCollisionUpdates() override;

		virtual bool respondsToHitboxClash(Entity* clashedEntity) const;
		virtual bool canCollideWithEntityHitbox(Entity* collidedEntity) const;

		virtual void onHitboxClash(const HitboxClashEvent& clashEvent);
		virtual void onHitboxClashUpdate(const HitboxClashEvent& clashEvent);
		virtual void onHitboxClashFinish(const HitboxClashEvent& clashEvent);

		virtual void onHitboxCollision(const HitboxCollisionEvent& collisionEvent);
		virtual void onHitboxCollisionUpdate(const HitboxCollisionEvent& collisionEvent);
		virtual void onHitboxCollisionFinish(const HitboxCollisionEvent& collisionEvent);

		virtual void onFinishHitboxUpdates();

		fgl::ArrayList<fgl::Collidable*> getCollided(fgl::CollisionSide side) const;
		bool isStaticCollidableOnSide(fgl::CollisionSide side) const;

		struct Anchor
		{
			Entity* entity;
			MetaPointType parentPoint;
			size_t parentPointIndex;
			MetaPointType childPoint;
			size_t childPointIndex;
		};

		void anchorChildEntity(Entity* child, MetaPointType childPoint, size_t childPointIndex, MetaPointType parentPoint, size_t parentPointIndex, const fgl::Vector2d& childOffset = fgl::Vector2d(0, 0));
		void removeAnchoredEntity(Entity* child);
		const fgl::ArrayList<Anchor>& getAnchoredEntities() const;

	private:
		float scale;

		Orientation orientation;

		fgl::ArrayList<Anchor> anchoredEntities;
		Entity* parentEntity;

		struct CollidedObject
		{
			fgl::Collidable* collidable;
			fgl::CollisionSide side;
		};
		fgl::ArrayList<CollidedObject> collidedObjects;

		Anchor getAnchor(const Entity* entity) const;
		bool getAnchorData(fgl::Vector2d* posOffset, double* rotation, fgl::Vector2d* rotationPoint, bool* behind, bool* visible) const;
	};
}
