
#pragma once

#include "AnimationMetaPoint.hpp"

namespace fl
{
	struct TaggedBox
	{
		size_t tag;
		fgl::RectangleD rect;
	};
	
	class AnimationData
	{
	public:
		AnimationData();
		AnimationData(const AnimationData&);
		AnimationData(AnimationData&&);
		~AnimationData();
		
		AnimationData& operator=(const AnimationData&);
		AnimationData& operator=(AnimationData&&);
		
		bool operator==(const AnimationData&) const;
		bool operator!=(const AnimationData&) const;

		bool loadFromFile(const fgl::String& path, fgl::AssetManager* assetManager, fgl::String* error=nullptr);
		bool saveToFile(const fgl::String& path, fgl::String* error=nullptr) const;

		void drawFrame(size_t frameIndex, fgl::Graphics graphics, AnimationOrientation drawnOrientation=ANIMATIONORIENTATION_NEUTRAL, bool showFrames=false) const;
		void drawMetaPoints(size_t frameIndex, const fgl::RectangleD& dstRect, fgl::Graphics graphics, AnimationOrientation drawnOrientation) const;
		void drawMetaPoints(size_t frameIndex, const fgl::RectangleD& dstRect, fgl::Graphics graphics, AnimationOrientation drawnOrientation, MetaPointType metaPointType) const;

		const fgl::String& getName() const;
		fgl::Animation* getAnimation() const;
		AnimationOrientation getOrientation() const;
		fgl::Vector2u getSize(size_t frameIndex) const;

		void setName(const fgl::String& name);
		void setAnimation(fgl::Animation* animation);
		void setOrientation(AnimationOrientation orientation);

		fgl::ArrayList<AnimationMetaPoint> getMetaPoints(size_t frameIndex) const;
		fgl::ArrayList<AnimationMetaPoint> getMetaPoints(size_t frameIndex, MetaPointType pointType) const;
		const AnimationMetaPoint& getMetaPoint(size_t frameIndex, size_t metaPointIndex) const;
		void setMetaPoints(size_t frameIndex, const fgl::ArrayList<AnimationMetaPoint>& metaPoints);
		void setMetaPoint(size_t frameIndex, size_t metaPointIndex, const AnimationMetaPoint& metaPoint);
		size_t addMetaPoint(size_t frameIndex, const AnimationMetaPoint& metaPoint);
		void removeMetaPoint(size_t frameIndex, size_t metaPointIndex);

		fgl::ArrayList<TaggedBox> getBounds(size_t frameIndex, AnimationOrientation drawnOrientation=ANIMATIONORIENTATION_NEUTRAL) const;
		
		bool isMirrored(AnimationOrientation drawnOrientation) const;

	private:
		struct FrameData
		{
			fgl::ArrayList<AnimationMetaPoint> metapoints;
		};

		fgl::String name;
		fgl::Animation* animation;
		AnimationOrientation orientation;
		fgl::ArrayList<FrameData> frameDatas;
	};
}
