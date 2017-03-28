
#pragma once

#include "AnimationData.hpp"
#include "AnimationAssetManager.hpp"
#include "Drawable.hpp"
#include <functional>

namespace fl
{
	typedef enum
	{
		ANIMATIONEVENT_FRAMECHANGED,
		ANIMATIONEVENT_FINISHED,
		ANIMATIONEVENT_CHANGED
	} AnimationEventType;

	class Sprite : public Drawable
	{
	public:
		Sprite();
		virtual ~Sprite();

		virtual void update(const fgl::ApplicationData& appData) override;
		virtual void draw(const fgl::ApplicationData& appData, fgl::Graphics graphics) const override;

		fgl::Vector2d getSize() const;
		virtual fgl::Vector2d getPosition(float* rotation = nullptr) const = 0;

		bool loadAnimation(const fgl::String& path, AnimationAssetManager* assetManager, fgl::String* error=nullptr);
		void changeAnimation(const fgl::String& name, const std::function<void(AnimationEventType)>& onevent=nullptr);
		fgl::Animation* getAnimation(const fgl::String& name) const;
		fgl::Animation* getCurrentAnimation() const;
		fgl::String getCurrentAnimationName() const;
		size_t getCurrentAnimationFrameIndex() const;

	protected:
		virtual AnimationOrientation getAnimationOrientation() const;

		AnimationData* getCurrentAnimationData() const;
		AnimationData* getAnimationData(const fgl::String& name) const;
		
		virtual fgl::Vector2d getDrawPosition(float* rotation = nullptr) const = 0;
		virtual float getDrawScale() const;

	private:
		fgl::ArrayList<AnimationData*> animations;

		AnimationData* currentAnimationData;
		size_t currentAnimationFrame;
		long long currentAnimationLastFrameTime;
		std::function<void(AnimationEventType)> currentAnimationEventHandler;
		bool animationChanged;
	};
}
