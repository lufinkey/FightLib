
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
		Sprite(const fgl::Vector2d& position);
		virtual ~Sprite();

		virtual bool getFlag(const fgl::String& flag) const override;
		virtual void update(fgl::ApplicationData appData) override;
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const override;

		fgl::RectangleD getFrame() const;
		fgl::Vector2d getSize() const;
		virtual fgl::Vector2d getOrigin() const;
		virtual double getRotation() const;
		virtual fgl::Vector2d getPosition() const;
		virtual void setPosition(const fgl::Vector2d& position);

		bool loadAnimation(const fgl::String& path, AnimationAssetManager* assetManager, fgl::String* error=nullptr);
		void changeAnimation(const fgl::String& name, const std::function<void(AnimationEventType)>& onevent=nullptr);
		fgl::Animation* getAnimation(const fgl::String& name) const;
		fgl::Animation* getCurrentAnimation() const;
		fgl::String getCurrentAnimationName() const;
		size_t getCurrentAnimationFrameIndex() const;
		AnimationData* getCurrentAnimationData() const;
		AnimationData* getAnimationData(const fgl::String& name) const;

		fgl::ArrayList<TaggedBox> getMetaPointBoxes(MetaPointType metaPointType) const;

	protected:
		virtual AnimationOrientation getAnimationOrientation() const;
		
		virtual void onAnimationFrameChange();
		virtual void onAnimationChange();

		virtual fgl::TransformState getDrawTransformState() const;
		virtual fgl::Vector2d getDrawScale() const;

	private:
		fgl::Vector2d position;

		fgl::ArrayList<AnimationData*> animations;

		AnimationData* currentAnimationData;
		size_t currentAnimationFrame;
		long long currentAnimationLastFrameTime;
		std::function<void(AnimationEventType)> currentAnimationEventHandler;
		bool animationChanged;
	};
}
