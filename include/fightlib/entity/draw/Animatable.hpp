
#pragma once

#include "../action/AnimationData.hpp"
#include "../action/AnimationAssetManager.hpp"
#include <functional>

namespace fl
{
	typedef enum : fgl::Uint8
	{
		ORIENTATION_LEFT,
		ORIENTATION_RIGHT,
	} Orientation;

	typedef enum
	{
		ANIMATIONEVENT_FRAMECHANGED,
		ANIMATIONEVENT_FINISHED,
		ANIMATIONEVENT_CHANGED
	} AnimationEventType;

	class Animatable
	{
	public:
		Animatable(const fgl::Vector2d& position);
		virtual ~Animatable();

		virtual void update(const fgl::ApplicationData& appData);
		virtual void draw(const fgl::ApplicationData& appData, fgl::Graphics graphics) const;

		fgl::Vector2d getSize() const;
		virtual fgl::Vector2d getPosition(float* rotation = nullptr) const;

		bool loadAnimation(const fgl::String& path, AnimationAssetManager* assetManager, fgl::String* error=nullptr);
		void changeAnimation(const fgl::String& name, const std::function<void(AnimationEventType)>& onevent=nullptr);
		fgl::Animation* getAnimation(const fgl::String& name) const;
		fgl::Animation* getCurrentAnimation() const;
		fgl::String getCurrentAnimationName() const;

	protected:
		virtual float getScale() const;
		virtual AnimationOrientation getAnimationOrientation() const;

		AnimationData* getCurrentAnimationData() const;

	private:
		AnimationData* getAnimationData(const fgl::String& name) const;

		virtual fgl::Vector2d getDrawPosition() const;

		fgl::ArrayList<AnimationData*> animations;

		AnimationData* currentAnimationData;
		size_t currentAnimationFrame;
		long long currentAnimationLastFrameTime;
		std::function<void(AnimationEventType)> currentAnimationEventHandler;
		bool animationChanged;
	};
}
