
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
			POINTTYPE_RIGHTHAND
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

		void drawFrame(double x, double y, double scale, size_t frameIndex, fgl::Graphics graphics, bool showFrames=false) const;

		const fgl::String& getName() const;
		fgl::Animation* getAnimation() const;

		const fgl::ArrayList<AnimationHitbox>& getHitboxes(size_t frameNumber) const;
		const fgl::ArrayList<AnimationMetaPoint>& getMetaPoints(size_t frameNumber) const;

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
