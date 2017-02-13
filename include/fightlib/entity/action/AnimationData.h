
#include <GameLibrary/GameLibrary.hpp>

namespace fl
{
	typedef enum : fgl::byte
	{
		ANIMATIONORIENTATION_NEUTRAL,
		ANIMATIONORIENTATION_LEFT,
		ANIMATIONORIENTATION_RIGHT
	} AnimationOrientation;

	class AnimationHitbox
	{
	public:
		AnimationHitbox();

		bool loadFromDictionary(const fgl::Dictionary& dictionary, fgl::String* error);

		void draw(fgl::Graphics graphics) const;

		fgl::RectangleD getRect() const;

		size_t tag;
		float x;
		float y;
		float radius;
	};

	class AnimationMetaPoint
	{
	public:
		AnimationMetaPoint();

		bool loadFromDictionary(const fgl::Dictionary& dictionary, fgl::String* error);

		void draw(fgl::Graphics graphics) const;

		fgl::RectangleD getRect() const;

		typedef enum : fgl::byte
		{
			POINTTYPE_HEAD,
			POINTTYPE_LEFTHAND,
			POINTTYPE_RIGHTHAND,
			POINTTYPE_BOUNDS_TOPLEFT,
			POINTTYPE_BOUNDS_BOTTOMRIGHT,
			POINTTYPE_HANDLE
		} Type;
		//TODO work on a better system for custom metapoint types

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
		AnimationData();
		~AnimationData();

		bool loadFromFile(const fgl::String& path, fgl::AssetManager* assetManager, fgl::String* error=nullptr);

		void drawFrame(size_t frameIndex, fgl::Graphics graphics, AnimationOrientation drawnOrientation=ANIMATIONORIENTATION_NEUTRAL, bool showFrames=false) const;

		const fgl::String& getName() const;
		fgl::Animation* getAnimation() const;
		AnimationOrientation getOrientation() const;
		fgl::Vector2d getSize(size_t frameIndex, double scale) const;

		fgl::ArrayList<AnimationHitbox> getHitboxes(size_t frameIndex) const;
		fgl::ArrayList<AnimationMetaPoint> getMetaPoints(size_t frameIndex) const;
		fgl::ArrayList<AnimationMetaPoint> getMetaPoints(size_t frameIndex, AnimationMetaPoint::Type pointType) const;
		fgl::ArrayList<fgl::RectangleD> getBounds(size_t frameIndex, AnimationOrientation drawnOrientation=ANIMATIONORIENTATION_NEUTRAL) const;

		bool isMirrored(AnimationOrientation drawnOrientation) const;

	private:
		struct FrameData
		{
			fgl::ArrayList<AnimationHitbox> hitboxes;
			fgl::ArrayList<AnimationMetaPoint> metapoints;
		};

		fgl::String name;
		fgl::Animation* animation;
		AnimationOrientation orientation;
		fgl::ArrayList<FrameData> frameDatas;
	};
}
