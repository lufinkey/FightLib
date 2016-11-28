
#include <functional>
#include "action/AnimationData.h"

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
		typedef enum
		{
			ORIENTATION_LEFT,
			ORIENTATION_RIGHT,
		} Orientation;

		Entity(double x, double y, Entity::Orientation orientation);
		virtual ~Entity();

		virtual void update(fgl::ApplicationData appData);
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const;

		fgl::Vector2d getPosition(float* rotation = nullptr) const;

		float getScale() const;
		void setScale(float scale);

		Entity::Orientation getOrientation() const;
		void setOrientation(Entity::Orientation orientation);

		bool loadAnimation(const fgl::String& path, fgl::AssetManager* assetManager, fgl::String* error=nullptr);
		void changeAnimation(const fgl::String& name, std::function<void(AnimationEventType)> onevent=nullptr);
		fgl::Animation* getAnimation(const fgl::String& name) const;
		fgl::Animation* getCurrentAnimation() const;

		void anchorChildEntity(Entity* child, AnimationMetaPoint::Type childPoint, size_t childPointIndex, AnimationMetaPoint::Type parentPoint, size_t parentPointIndex, const fgl::Vector2d& offset = fgl::Vector2d(0,0));
		void removeAnchoredEntity(Entity* child);

	private:
		double x;
		double y;
		Entity::Orientation orientation;

		float scale;

		fgl::ArrayList<AnimationData*> animations;

		fgl::String currentAnimationName;
		size_t currentAnimationFrame;
		long long currentAnimationLastFrameTime;
		std::function<void(AnimationEventType)> currentAnimationEventHandler;
		bool animationChanged;

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
		bool getAnchorData(fgl::Vector2d* offset, float* rotation, fgl::Vector2d* rotationPoint, bool* behind, bool* visible) const;

		AnimationOrientation getAnimationOrientation() const;
	};
}
