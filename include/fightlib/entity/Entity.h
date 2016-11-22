
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
		Entity(double x, double y);
		virtual ~Entity();

		virtual void update(fgl::ApplicationData appData);
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const;

		bool loadAnimation(const fgl::String& path, fgl::AssetManager* assetManager, fgl::String* error=nullptr);
		void changeAnimation(const fgl::String& name, std::function<void(AnimationEventType)> onevent=nullptr);

	private:
		AnimationData* getAnimationData(const fgl::String& name) const;

		double x;
		double y;

		fgl::ArrayList<AnimationData*> animations;

		fgl::String currentAnimationName;
		size_t currentAnimationFrame;
		long long currentAnimationLastFrameTime;
		std::function<void(AnimationEventType)> currentAnimationEventHandler;
		bool animationChanged;
	};
}
