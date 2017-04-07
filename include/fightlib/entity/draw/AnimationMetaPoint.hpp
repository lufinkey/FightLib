
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
	
	bool AnimationOrientation_fromString(const fgl::String& string, AnimationOrientation* orientation);
	fgl::String AnimationOrientation_toString(AnimationOrientation orientation);
	
	
	typedef fgl::Uint8 MetaPointType;
	static const MetaPointType METAPOINT_HITBOX = 0;
	static const MetaPointType METAPOINT_HEAD = 1;
	static const MetaPointType METAPOINT_LEFTHAND = 2;
	static const MetaPointType METAPOINT_RIGHTHAND = 3;
	static const MetaPointType METAPOINT_BOUNDS_TOPLEFT = 4;
	static const MetaPointType METAPOINT_BOUNDS_BOTTOMRIGHT = 5;
	static const MetaPointType METAPOINT_HANDLE = 6;
	
	class AnimationMetaPoint
	{
	public:
		AnimationMetaPoint();
		
		bool operator==(const AnimationMetaPoint&) const;
		bool operator!=(const AnimationMetaPoint&) const;
		
		bool loadFromDictionary(const fgl::Dictionary& dictionary, fgl::String* error);
		bool saveToDictionary(fgl::Dictionary* dictionary, fgl::String* error) const;
		
		void draw(fgl::Graphics graphics) const;
		
		fgl::RectangleD getRect() const;
		
		size_t tag;
		float x;
		float y;
		float radius;
		float rotation;
		MetaPointType type;
		AnimationOrientation orientation;
		bool behind;
		bool visible;
	};
}
