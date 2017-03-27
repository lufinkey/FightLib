
#pragma once

#include "collision/Collidable.hpp"

namespace fl
{
	typedef enum : fgl::Uint8
	{
		ORIENTATION_LEFT,
		ORIENTATION_RIGHT,
	} Orientation;

	typedef enum : fgl::Uint8
	{
		COLLISIONMETHOD_NONE,
		COLLISIONMETHOD_FRAME,
		COLLISIONMETHOD_BOUNDS,
		COLLISIONMETHOD_PIXEL
	} CollisionMethod;

	class Entity : public Collidable
	{
	public:
		Entity(const fgl::Vector2d& position, Orientation orientation);
		virtual ~Entity();

		virtual void update(const fgl::ApplicationData& appData) override;
		virtual void draw(const fgl::ApplicationData& appData, fgl::Graphics graphics) const override;

		virtual fgl::Vector2d getPosition(float* rotation = nullptr) const override;

		virtual float getScale() const override;
		void setScale(float scale);

		Orientation getOrientation() const;
		void setOrientation(Orientation orientation);

		CollisionMethod getCollisionMethod() const;
		void setCollisionMethod(CollisionMethod method);

		virtual bool isStaticCollisionBody() const override;
		void setStaticCollisionBody(bool staticCollisionBody);

		virtual fgl::ArrayList<CollisionRect*> getCollisionRects() const override;

		void anchorChildEntity(Entity* child, AnimationMetaPoint::Type childPoint, size_t childPointIndex, AnimationMetaPoint::Type parentPoint, size_t parentPointIndex, const fgl::Vector2d& childOffset = fgl::Vector2d(0, 0));
		void removeAnchoredEntity(Entity* child);
		
		void setVelocity(const fgl::Vector2d& velocity);
		const fgl::Vector2d& getVelocity() const;

	protected:
		virtual void shift(const fgl::Vector2d& offset) override;

		virtual fgl::ArrayList<CollisionRect*> createCollisionRects(const fgl::ApplicationData& appData, const fgl::ArrayList<CollisionRect*>& previousRects={}) const;

		virtual AnimationOrientation getAnimationOrientation() const override;

	private:
		virtual fgl::Vector2d getDrawPosition(float* rotation) const override;

		fgl::Vector2d offset;
		fgl::Vector2d velocity;

		float scale;

		Orientation orientation;
		CollisionMethod collisionMethod;
		bool staticCollisionBody;

		fgl::ArrayList<CollisionRect*> collisionRects;

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
	};
}
