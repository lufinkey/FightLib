
#include <functional>
#include "action/AnimationData.h"

namespace fl
{
	typedef enum
	{
		ANIMATIONEVENT_FRAMECHANGED,
		ANIMATIONEVENT_FINISHED,
		ANIMATIONEVENT_CANCELLED
	} AnimationEventType;

	class Entity : public fgl::UpdateDrawable
	{
	public:
		Entity(double x, double y);

		void loadAnimation(const fgl::String& path, fgl::AssetManager* assetManager);
		void changeAnimation(const fgl::String& name, std::function<void(AnimationEventType)> onevent=nullptr);

		virtual void update(fgl::ApplicationData appData) override;
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const override;

	private:
		double x;
		double y;

		fgl::ArrayList<AnimationData*> animations;

		AnimationData* getAnimationData(const fgl::String& name) const;

		fgl::String currentAnimationName;
		size_t currentAnimationFrame;
		long long currentAnimationLastFrameTime;
		std::function<void(AnimationEventType)> currentAnimationEventHandler;
		bool animationChanged;
	};
}
