
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

	class Animatable : public Drawable
	{
	public:
		Animatable();
		virtual ~Animatable();

		virtual void update(fgl::ApplicationData appData) override;
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const override;

		fgl::Vector2d getSize() const;

		virtual fgl::Vector2d getPosition() const = 0;
		virtual void setPosition(const fgl::Vector2d& position) = 0;
		virtual double getRotation() const = 0;
		virtual void setRotation(double degrees) = 0;

		bool loadAnimation(const fgl::String& path, AnimationAssetManager* assetManager, fgl::String* error=nullptr);
		void changeAnimation(const fgl::String& name, const std::function<void(AnimationEventType)>& onevent=nullptr);
		fgl::Animation* getAnimation(const fgl::String& name) const;
		fgl::Animation* getCurrentAnimation() const;
		fgl::String getCurrentAnimationName() const;
		size_t getCurrentAnimationFrameIndex() const;
		
		fgl::ArrayList<TaggedBox> getMetaPointBoxes(MetaPointType metaPointType) const;

	protected:
		virtual AnimationOrientation getAnimationOrientation() const;

		AnimationData* getCurrentAnimationData() const;
		AnimationData* getAnimationData(const fgl::String& name) const;
		
		virtual fgl::Vector2d getDrawPosition() const = 0;
		virtual double getDrawRotation() const = 0;
		virtual fgl::Vector2d getDrawScale() const = 0;

	private:
		fgl::ArrayList<AnimationData*> animations;

		AnimationData* currentAnimationData;
		size_t currentAnimationFrame;
		long long currentAnimationLastFrameTime;
		std::function<void(AnimationEventType)> currentAnimationEventHandler;
		bool animationChanged;
	};
}
