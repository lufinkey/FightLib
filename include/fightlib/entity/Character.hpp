
#pragma once

#include "ActionEntity.hpp"

namespace fl
{
	class Character : public ActionEntity
	{
	public:
		Character(const fgl::Vector2d& position, Orientation orientation);

		virtual void update(const fgl::ApplicationData& appData) override;

		virtual fgl::String getIdleAnimationName() const;
		virtual fgl::String getMoveAnimationName(double amount) const;
		virtual double getMoveSpeed(double amount) const;

		void setDirection(const fgl::Vector2f& direction);
		const fgl::Vector2f& getDirection() const;

	protected:
		virtual void updateMovement(const fgl::ApplicationData& appData);
		virtual void updateMoveAnimation();
		
		virtual void onActionEnd(Action* action) override;

	private:
		fgl::Vector2f direction;
	};
}
