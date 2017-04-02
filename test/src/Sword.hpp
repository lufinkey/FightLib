
#include <fightlib/fightlib.hpp>

class Sword : public fl::Item
{
public:
	Sword(const fgl::Vector2d& position, fl::Orientation orientation);

	virtual fgl::ArrayList<fl::AnimationMetaPoint::Type> getAnchorPoints() const override;
};
