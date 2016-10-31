
#include <GameLibrary/GameLibrary.h>
#include "collision/Hitbox.h"

namespace fl
{
	class Entity : public fgl::UpdateDrawable
	{
	public:
		Entity(double x, double y);

		void addAnimation(const fgl::String& path);

		virtual void update(fgl::ApplicationData appData) override;
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const override;

	private:
		struct AnimationData
		{
			fgl::String name;
			fgl::Animation* animation;
			fgl::ArrayList<Hitbox> hitboxes;
		};

		fgl::String currentAnimation;

		double x;
		double y;
	};
}
