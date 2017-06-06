
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
		virtual void update(fgl::ApplicationData appData) override;
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const override;

		virtual fgl::Vector2d getPosition(float* rotation = nullptr) const override;
		virtual void setPosition(const fgl::Vector2d& position) override;
		virtual void shift(const fgl::Vector2d& offset) override;
		virtual fgl::Vector2d getTerminalVelocity() const;
		
		virtual HitboxInfo getHitboxInfo(size_t tag) const;

		float getScale() const;
		void setScale(float scale);

		Orientation getOrientation() const;
		void setOrientation(Orientation orientation);

		virtual bool isStaticCollisionBody() const override;
		virtual bool respondsToGravity() const;
		virtual bool movesWithGround() const;
		virtual bool usesHitboxes() const;

		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const override;

		Entity* getParentEntity() const;
		Stage* getStage() const;

		bool isOnGround() const;

	protected:
		virtual AnimationOrientation getAnimationOrientation() const override;

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

		virtual void onAddToStage(Stage* stage);
		virtual void onRemoveFromStage(Stage* stage);

		const fgl::ArrayList<Collidable*>& getCollided(CollisionSide side) const;
		bool isStaticCollidableOnSide(CollisionSide side) const;
		
		CollisionMethod getCollisionMethod() const;
		void setCollisionMethod(CollisionMethod method);

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
		CollisionRectManager collisionRectManager;

		fgl::ArrayList<Anchor> anchoredEntities;
		Entity* parentEntity;
		
		Stage* stage;

		fgl::ArrayList<Collidable*> leftCollidables;
		fgl::ArrayList<Collidable*> topCollidables;
		fgl::ArrayList<Collidable*> rightCollidables;
		fgl::ArrayList<Collidable*> bottomCollidables;

		Anchor getAnchor(const Entity* entity) const;
		bool getAnchorData(fgl::Vector2d* posOffset, float* rotation, fgl::Vector2d* rotationPoint, bool* behind, bool* visible) const;
	};
}
