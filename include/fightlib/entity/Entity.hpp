
#pragma once

#include <functional>
#include "action/AnimationData.hpp"
#include "collision/rects/CollisionRect.hpp"
#include "collision/CollisionManager.hpp"

namespace fl
{
	typedef enum
	{
		ANIMATIONEVENT_FRAMECHANGED,
		ANIMATIONEVENT_FINISHED,
		ANIMATIONEVENT_CHANGED
	} AnimationEventType;

	class Entity
	{
	public:
		friend class CollisionManager;

		typedef enum : fgl::byte
		{
			ORIENTATION_LEFT,
			ORIENTATION_RIGHT,
		} Orientation;

		typedef enum : fgl::byte
		{
			COLLISIONMETHOD_NONE,
			COLLISIONMETHOD_FRAME,
			COLLISIONMETHOD_BOUNDS,
			COLLISIONMETHOD_PIXEL
		} CollisionMethod;

		Entity(const fgl::Vector2d& position, Entity::Orientation orientation);
		virtual ~Entity();

		virtual void update(fgl::ApplicationData appData);
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const;

		fgl::Vector2d getSize() const;
		fgl::Vector2d getPosition(float* rotation = nullptr) const;

		float getScale() const;
		void setScale(float scale);

		Entity::Orientation getOrientation() const;
		void setOrientation(Entity::Orientation orientation);

		Entity::CollisionMethod getCollisionMethod() const;
		void setCollisionMethod(Entity::CollisionMethod method);

		bool isStaticCollisionBody() const;
		void setStaticCollisionBody(bool staticCollisionBody);

		bool loadAnimation(const fgl::String& path, fgl::AssetManager* assetManager, fgl::String* error=nullptr);
		void changeAnimation(const fgl::String& name, std::function<void(AnimationEventType)> onevent=nullptr);
		fgl::Animation* getAnimation(const fgl::String& name) const;
		fgl::Animation* getCurrentAnimation() const;

		virtual fgl::ArrayList<CollisionRect*> createCollisionRects() const;

		void anchorChildEntity(Entity* child, AnimationMetaPoint::Type childPoint, size_t childPointIndex, AnimationMetaPoint::Type parentPoint, size_t parentPointIndex, const fgl::Vector2d& childOffset = fgl::Vector2d(0, 0));
		void removeAnchoredEntity(Entity* child);

	protected:
		void shift(const fgl::Vector2d& offset);
		void setVelocity(const fgl::Vector2d& velocity);
		const fgl::Vector2d& getVelocity() const;

		virtual void onCollision(Entity* entity, CollisionSide side);

	private:
		fgl::Vector2d offset;
		fgl::Vector2d velocity;

		float scale;

		Entity::Orientation orientation;
		Entity::CollisionMethod collisionMethod;
		bool staticCollisionBody;
		bool animationChanged;

		fgl::ArrayList<AnimationData*> animations;

		fgl::String currentAnimationName;
		size_t currentAnimationFrame;
		long long currentAnimationLastFrameTime;
		std::function<void(AnimationEventType)> currentAnimationEventHandler;

		AnimationData* getAnimationData(const fgl::String& name) const;

		struct Anchor
		{
			Entity* entity;
			AnimationMetaPoint::Type parentPoint;
			size_t parentPointIndex;
			AnimationMetaPoint::Type childPoint;
			size_t childPointIndex;
		};

		fgl::ArrayList<Anchor> anchoredEntities;
		Entity* parentEntity;

		Anchor getAnchor(const Entity* entity) const;
		bool getAnchorData(fgl::Vector2d* posOffset, float* rotation, fgl::Vector2d* rotationPoint, bool* behind, bool* visible) const;

		AnimationOrientation getAnimationOrientation() const;
	};
}
