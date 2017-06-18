
#pragma once

#include "collision/Collidable.hpp"
#include "hitbox/HitboxInfo.hpp"
#include "hitbox/HitboxClashEvent.hpp"
#include "hitbox/HitboxCollisionEvent.hpp"

class b2WeldJoint;

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

		Entity* getParentEntity() const;
		Stage* getStage() const;

		bool isOnGround() const;

	protected:
		virtual void drawMain(fgl::ApplicationData appData, fgl::Graphics graphics) const;

		virtual AnimationOrientation getAnimationOrientation() const override;

		virtual fgl::Vector2d getDrawScale() const override;
		virtual bool shouldUseParentMetaPointRotation() const;

		virtual void onBeginCollisionUpdates() override;
		
		virtual bool respondsToHitboxClash(Entity* clashedEntity) const;
		virtual bool canCollideWithEntityHitbox(Entity* collidedEntity) const;
		
		virtual void onBeginHitboxUpdates();

		virtual void onHitboxClash(const HitboxClashEvent& clashEvent);
		virtual void onHitboxClashUpdate(const HitboxClashEvent& clashEvent);
		virtual void onHitboxClashFinish(const HitboxClashEvent& clashEvent);
		
		virtual void onHitboxCollision(const HitboxCollisionEvent& collisionEvent);
		virtual void onHitboxCollisionUpdate(const HitboxCollisionEvent& collisionEvent);
		virtual void onHitboxCollisionFinish(const HitboxCollisionEvent& collisionEvent);
		
		virtual void onFinishHitboxUpdates();

		virtual void onAddToStage(Stage* stage);
		virtual void onRemoveFromStage(Stage* stage);

		fgl::ArrayList<Collidable*> getCollided(CollisionSide side) const;

		struct Anchor
		{
			Entity* entity;
			MetaPointType parentPoint;
			size_t parentPointIndex;
			MetaPointType childPoint;
			size_t childPointIndex;
			b2WeldJoint* joint;
		};

		void anchorChildEntity(Entity* child, MetaPointType childPoint, size_t childPointIndex, MetaPointType parentPoint, size_t parentPointIndex, const fgl::Vector2d& childOffset = fgl::Vector2d(0, 0));
		void removeAnchoredEntity(Entity* child);
		const fgl::ArrayList<Anchor>& getAnchoredEntities() const;

		void setNeedsAnchorsUpdate();
		void updateAnchors();

	private:
		Anchor getAnchor(const Entity* entity) const;
		bool getAnchorData(fgl::Vector2d* posOffset, double* rotation, fgl::Vector2d* rotationPoint, bool* behind, bool* visible) const;
		void updateAnchor(Anchor& anchor);
		fgl::Vector2d getMetaPointOffset(MetaPointType metaPointType, size_t metaPointIndex, double* rotation) const;
		fgl::Vector2d getIntendedEntityPosition(double* rotation) const;

		float scale;

		Orientation orientation;
		bool needsAnchorsUpdate;

		fgl::ArrayList<Anchor> anchoredEntities;
		Entity* parentEntity;
		
		Stage* stage;
	};
}
