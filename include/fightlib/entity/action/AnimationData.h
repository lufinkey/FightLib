
#include <GameLibrary/GameLibrary.h>

namespace fl
{
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
			POINTTYPE_HANDLE
		} PointType;

		typedef enum : fgl::byte
		{
			ORIENTATION_LEFT,
			ORIENTATION_RIGHT
		} Orientation;

		float x;
		float y;
		float radius;
		float rotation;
		PointType type;
		Orientation orientation;
		bool behind;
		bool visible;
	};

	class AnimationData
	{
	public:
		AnimationData();
		~AnimationData();

		bool loadFromFile(const fgl::String& path, fgl::AssetManager* assetManager, fgl::String* error=nullptr);

		void drawFrame(size_t frameIndex, fgl::Graphics& graphics, bool showFrames=false) const;

		const fgl::String& getName() const;
		fgl::Animation* getAnimation() const;
		fgl::Vector2d getSize(size_t frameIndex, double scale) const;

		fgl::ArrayList<AnimationHitbox> getHitboxes(size_t frameIndex) const;
		fgl::ArrayList<AnimationMetaPoint> getMetaPoints(size_t frameIndex) const;
		fgl::ArrayList<AnimationMetaPoint> getMetaPoints(size_t frameIndex, AnimationMetaPoint::PointType pointType) const;

	private:
		struct FrameData
		{
			fgl::ArrayList<AnimationHitbox> hitboxes;
			fgl::ArrayList<AnimationMetaPoint> metapoints;
		};

		fgl::String name;
		fgl::Animation* animation;
		fgl::ArrayList<FrameData> frameDatas;
	};
}
