
#include <GameLibrary/GameLibrary.h>

namespace fl
{
	class Hitbox
	{
	public:
		Hitbox();

		size_t index;
		float x;
		float y;
		float radius;
	};

	class AnimationMetaPoint
	{
	public:
		AnimationMetaPoint();

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

		bool loadFromFile(const fgl::String& path, fgl::String* error);

		void drawFrame(double x, double y, double scale, size_t frameIndex, fgl::Graphics graphics);

		const fgl::String& getName() const;
		fgl::Animation* getAnimation() const;

	private:
		struct FrameData
		{
			fgl::ArrayList<Hitbox> hitboxes;
			fgl::ArrayList<AnimationMetaPoint> metaPoints;
		};

		fgl::String name;
		fgl::Animation* animation;
		fgl::ArrayList<FrameData> frameData;
	};
}
