
#pragma once

#include "Camera.hpp"

namespace fl
{
	class Character;

	class FollowerCamera : public Camera
	{
	public:
		FollowerCamera();

		void setFocus(Character* focus);
		Character* getFocus() const;

		void setFollowRegion(const fgl::RectD& region);
		const fgl::RectD& getFollowRegion() const;

	protected:
		virtual void update(fgl::ApplicationData appData) override;
		virtual void drawStage(fgl::ApplicationData appData, fgl::Graphics graphics) const override;

	private:
		fgl::RectangleD getRect() const;

		Character* focus;
		fgl::Vector2d center;
		fgl::RectD followRegion;
	};
}
