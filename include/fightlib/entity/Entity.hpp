
#pragma once

#include "collision/Collidable.hpp"
#include "collision/CollisionRectManager.hpp"
#include "hitbox/HitboxInfo.hpp"
#include "hitbox/HitboxClashEvent.hpp"
#include "hitbox/HitboxCollisionEvent.hpp"

namespace fl
{
	class Stage;
	
	typedef enum : fgl::Uint8
	{
		ORIENTATION_LEFT,
		ORIENTATION_RIGHT,
	} Orientation;

	class Entity : public Collidable
	{
		friend class Stage;
		friend class HitboxCollisionManager;
	public:
		Entity(const fgl::Vector2d& position, Orientation orientation);
		
		virtual bool getFlag(const fgl::String& flag) const override;

		virtual void update(const fgl::ApplicationData& appData) override;
		virtual void draw(const fgl::ApplicationData& appData, fgl::Graphics graphics) const override;

		virtual fgl::Vector2d getPosition(float* rotation = nullptr) const override;
		
		virtual HitboxInfo getHitboxInfo(size_t tag) const;

		float getScale() const;
		void setScale(float scale);

		Orientation getOrientation() const;
		void setOrientation(Orientation orientation);

		CollisionMethod getCollisionMethod() const;
		void setCollisionMethod(CollisionMethod method);

		virtual bool isStaticCollisionBody() const override;
		virtual bool respondsToGravity() const;
		virtual bool movesWithGround() const;
		virtual bool usesHitboxes() const;

		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const override;

		Entity* getParentEntity() const;
		Stage* getStage() const;

		bool isOnGround() const;

	protected:
		virtual void shift(const fgl::Vector2d& offset) override;

		virtual AnimationOrientation getAnimationOrientation() const override;

		virtual fgl::Vector2d getDrawPosition(float* rotation) const override;
		virtual float getDrawScale() const override;
		virtual bool shouldUseParentMetaPointRotation() const;

		virtual void onCollision(const CollisionEvent& collisionEvent) override;
		virtual void onCollisionFinish(const CollisionEvent& collisionEvent) override;
		
		virtual bool respondsToHitboxClash(Entity* clashedEntity) const;
		virtual bool canCollideWithEntityHitbox(Entity* collidedEntity) const;
		
		virtual void onHitboxClash(const HitboxClashEvent& clashEvent);
		virtual void onHitboxClashUpdate(const HitboxClashEvent& clashEvent);
		virtual void onHitboxClashFinish(const HitboxClashEvent& clashEvent);
		
		virtual void onHitboxCollision(const HitboxCollisionEvent& collisionEvent);
		virtual void onHitboxCollisionUpdate(const HitboxCollisionEvent& collisionEvent);
		virtual void onHitboxCollisionFinish(const HitboxCollisionEvent& collisionEvent);
		
		virtual void onFinishHitboxUpdates();

		const fgl::ArrayList<Collidable*>& getGroundCollidables() const;

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
		fgl::Vector2d offset;

		float scale;

		Orientation orientation;
		CollisionRectManager collisionRectManager;

		fgl::ArrayList<Anchor> anchoredEntities;
		Entity* parentEntity;
		
		Stage* stage;

		fgl::ArrayList<Collidable*> groundCollidables;

		Anchor getAnchor(const Entity* entity) const;
		bool getAnchorData(fgl::Vector2d* posOffset, float* rotation, fgl::Vector2d* rotationPoint, bool* behind, bool* visible) const;
	};
}
