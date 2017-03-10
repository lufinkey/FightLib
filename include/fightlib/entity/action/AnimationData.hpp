
#pragma once

#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	typedef enum : fgl::Uint8
	{
		ANIMATIONORIENTATION_NEUTRAL,
		ANIMATIONORIENTATION_LEFT,
		ANIMATIONORIENTATION_RIGHT
	} AnimationOrientation;

	class AnimationMetaPoint
	{
	public:
		AnimationMetaPoint();

		bool loadFromDictionary(const fgl::Dictionary& dictionary, fgl::String* error);

		void draw(fgl::Graphics graphics) const;

		fgl::RectangleD getRect() const;

		typedef fgl::Uint8 Type;
		static const Type POINTTYPE_HITBOX = 0;
		static const Type POINTTYPE_HEAD = 1;
		static const Type POINTTYPE_LEFTHAND = 2;
		static const Type POINTTYPE_RIGHTHAND = 3;
		static const Type POINTTYPE_BOUNDS_TOPLEFT = 4;
		static const Type POINTTYPE_BOUNDS_BOTTOMRIGHT = 5;
		static const Type POINTTYPE_HANDLE = 6;

		size_t tag;
		float x;
		float y;
		float radius;
		float rotation;
		AnimationMetaPoint::Type type;
		AnimationOrientation orientation;
		bool behind;
		bool visible;
	};

	class AnimationData
	{
	public:
		struct MetaBounds
		{
			size_t tag;
			fgl::RectangleD rect;
		};

		AnimationData();
		~AnimationData();

		bool loadFromFile(const fgl::String& path, fgl::AssetManager* assetManager, fgl::String* error=nullptr);

		void drawFrame(size_t frameIndex, fgl::Graphics graphics, AnimationOrientation drawnOrientation=ANIMATIONORIENTATION_NEUTRAL, bool showFrames=false) const;
		void drawMetaPoints(size_t frameIndex, const fgl::RectangleD& dstRect, fgl::Graphics graphics, AnimationOrientation drawnOrientation) const;

		const fgl::String& getName() const;
		fgl::Animation* getAnimation() const;
		AnimationOrientation getOrientation() const;
		fgl::Vector2d getSize(size_t frameIndex, double scale) const;

		fgl::ArrayList<AnimationMetaPoint> getMetaPoints(size_t frameIndex) const;
		fgl::ArrayList<AnimationMetaPoint> getMetaPoints(size_t frameIndex, AnimationMetaPoint::Type pointType) const;
		fgl::ArrayList<MetaBounds> getBounds(size_t frameIndex, AnimationOrientation drawnOrientation=ANIMATIONORIENTATION_NEUTRAL) const;
		
		void setMetaPoints(size_t frameIndex, const fgl::ArrayList<AnimationMetaPoint>& metaPoints);
		
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
