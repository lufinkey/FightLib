
#include <fightlib/entity/draw/AnimationMetaPoint.hpp>

namespace fl
{
	#define return_error(err) if(error!=nullptr) {*error = err;} return false;
	
	bool AnimationOrientation_fromString(const fgl::String& string, AnimationOrientation* orientation)
	{
		if(string=="NEUTRAL")
		{
			*orientation = ANIMATIONORIENTATION_NEUTRAL;
			return true;
		}
		else if(string=="LEFT")
		{
			*orientation = ANIMATIONORIENTATION_LEFT;
			return true;
		}
		else if(string=="RIGHT")
		{
			*orientation = ANIMATIONORIENTATION_RIGHT;
			return true;
		}
		return false;
	}
	
	fgl::String AnimationOrientation_toString(AnimationOrientation orientation)
	{
		switch(orientation)
		{
			case ANIMATIONORIENTATION_NEUTRAL:
				return "NEUTRAL";
				
			case ANIMATIONORIENTATION_LEFT:
				return "LEFT";
				
			case ANIMATIONORIENTATION_RIGHT:
				return "RIGHT";
		}
		return "";
	}
	
	/*const MetaPointType METAPOINT_HITBOX;
	const MetaPointType METAPOINT_HEAD;
	const MetaPointType METAPOINT_LEFTHAND;
	const MetaPointType METAPOINT_RIGHTHAND;
	const MetaPointType METAPOINT_BOUNDS_TOPLEFT;
	const MetaPointType METAPOINT_BOUNDS_BOTTOMRIGHT;
	const MetaPointType METAPOINT_HANDLE;*/
	
	AnimationMetaPoint::AnimationMetaPoint()
		: tag(-1),
		x(0),
		y(0),
		radius(0),
		rotation(0),
		type(METAPOINT_HITBOX),
		orientation(ANIMATIONORIENTATION_NEUTRAL),
		behind(false),
		visible(true)
	{
		//
	}
	
	bool AnimationMetaPoint::operator==(const fl::AnimationMetaPoint& metaPoint) const
	{
		if(tag==metaPoint.tag && x==metaPoint.x && y==metaPoint.y && radius==metaPoint.radius
		   && rotation==metaPoint.rotation && type==metaPoint.type && orientation==metaPoint.orientation
		   && behind==metaPoint.behind && visible==metaPoint.visible)
		{
			return true;
		}
		return false;
	}
	
	bool AnimationMetaPoint::operator!=(const fl::AnimationMetaPoint& metaPoint) const
	{
		return !operator==(metaPoint);
	}
	
	bool AnimationMetaPointType_fromString(const fgl::String& string, MetaPointType* type)
	{
		if(string=="HITBOX")
		{
			*type = METAPOINT_HITBOX;
			return true;
		}
		if(string=="HEAD")
		{
			*type = METAPOINT_HEAD;
			return true;
		}
		else if(string=="LEFT_HAND")
		{
			*type = METAPOINT_LEFTHAND;
			return true;
		}
		else if(string=="RIGHT_HAND")
		{
			*type = METAPOINT_RIGHTHAND;
			return true;
		}
		else if(string=="BOUNDS_TOPLEFT")
		{
			*type = METAPOINT_BOUNDS_TOPLEFT;
			return true;
		}
		else if(string=="BOUNDS_BOTTOMRIGHT")
		{
			*type = METAPOINT_BOUNDS_BOTTOMRIGHT;
			return true;
		}
		else if(string=="HANDLE")
		{
			*type = METAPOINT_HANDLE;
			return true;
		}
		return false;
	}
	
	fgl::Any AnimationMetaPointType_toAny(MetaPointType type)
	{
		switch(type)
		{
			case METAPOINT_HITBOX:
				return fgl::String("HITBOX");
				
			case METAPOINT_HEAD:
				return fgl::String("HEAD");
				
			case METAPOINT_LEFTHAND:
				return fgl::String("LEFT_HAND");
				
			case METAPOINT_RIGHTHAND:
				return fgl::String("RIGHT_HAND");
				
			case METAPOINT_BOUNDS_TOPLEFT:
				return fgl::String("BOUNDS_TOPLEFT");
				
			case METAPOINT_BOUNDS_BOTTOMRIGHT:
				return fgl::String("BOUNDS_BOTTOMRIGHT");
				
			case METAPOINT_HANDLE:
				return fgl::String("HANDLE");
		}
		return fgl::Number(type);
	}
	
	bool AnimationMetaPoint::loadFromDictionary(const fgl::Dictionary& dictionary, fgl::String* error)
	{
		if(dictionary.has("type") && dictionary.has("x") && dictionary.has("y"))
		{
			fgl::Any typeAny = dictionary.get("type", fgl::Any());
			MetaPointType typeValue = -1;
			if(typeAny.isEmpty())
			{
				return_error("missing required field");
			}
			else if(typeAny.is<fgl::String>())
			{
				fgl::String typeStr = typeAny.as<fgl::String>();
				if(!AnimationMetaPointType_fromString(typeStr, &typeValue))
				{
					return_error("invalid \"type\" value: \""+typeStr+"\"");
				}
			}
			else if(typeAny.is<fgl::Number>())
			{
				long long typeNum = typeAny.as<fgl::Number>().toArithmeticValue<long long>();
				if(typeNum >= 255 || typeNum < 0)
				{
					return_error((fgl::String)"invalid \"type\" value outside of allowed range: \""+typeNum+"\"");
				}
				typeValue = (MetaPointType)typeNum;
			}
			else
			{
				return_error("invalid \"type\" datatype");
			}
			
			size_t tagValue = fgl::extract<fgl::Number>(dictionary, "tag", -1).toArithmeticValue<size_t>();
			
			float xValue = fgl::extract<fgl::Number>(dictionary, "x").toArithmeticValue<float>();
			float yValue = fgl::extract<fgl::Number>(dictionary, "y").toArithmeticValue<float>();
			float radiusValue = fgl::extract<fgl::Number>(dictionary, "radius", 0).toArithmeticValue<float>();
			if(radiusValue < 0)
			{
				return_error("invalid radius value");
			}
			fgl::String orientationStr = fgl::extract<fgl::String>(dictionary, "orientation", "NEUTRAL");
			AnimationOrientation orientationValue;
			if(!AnimationOrientation_fromString(orientationStr, &orientationValue))
			{
				return_error("invalid \"orientation\" value: \""+orientationStr+"\"");
			}
			float rotationValue = fgl::extract<fgl::Number>(dictionary, "rotation", 0).toArithmeticValue<float>();
			
			tag = tagValue;
			x = xValue;
			y = yValue;
			radius = radiusValue;
			type = typeValue;
			orientation = orientationValue;
			rotation = rotationValue;
			if(dictionary.has("behind"))
			{
				behind = fgl::extract<fgl::Number>(dictionary, "behind").toArithmeticValue<bool>();
			}
			if(dictionary.has("visible"))
			{
				visible = fgl::extract<fgl::Number>(dictionary, "visible").toArithmeticValue<bool>();
			}
			return true;
		}
		else
		{
			return_error("missing required field")
		}
	}
	
	bool AnimationMetaPoint::saveToDictionary(fgl::Dictionary* dictionary, fgl::String* error) const
	{
		fgl::Dictionary dst;
		dst["type"] = AnimationMetaPointType_toAny(type);
		if(orientation!=ANIMATIONORIENTATION_NEUTRAL)
		{
			fgl::String orientationStr = AnimationOrientation_toString(orientation);
			if(orientationStr.length()==0)
			{
				return_error("invalid orientation value");
			}
			dst["orientation"] = orientationStr;
		}
		dst["x"] = fgl::Number(x);
		dst["y"] = fgl::Number(y);
		if(radius < 0)
		{
			return_error("invalid radius value");
		}
		if(radius!=0)
		{
			dst["radius"] = fgl::Number(radius);
		}
		if(rotation!=0)
		{
			dst["rotation"] = fgl::Number(rotation);
		}
		if(tag!=-1)
		{
			dst["tag"] = fgl::Number(tag);
		}
		dst["visible"] = fgl::Number(visible);
		dst["behind"] = fgl::Number(behind);
		*dictionary = dst;
		return true;
	}
	
	void AnimationMetaPoint::draw(fgl::Graphics graphics) const
	{
		if(behind)
		{
			graphics.compositeAlpha(0.7);
		}
		if(!visible)
		{
			graphics.compositeAlpha(0.3);
		}
		
		graphics.rotate((double)rotation, (double)x, (double)y);
		switch(type)
		{
			case METAPOINT_HITBOX:
				graphics.setColor(fgl::Color::GREEN);
				break;
				
			case METAPOINT_HEAD:
				graphics.setColor(fgl::Color::RED);
				break;
				
			case METAPOINT_LEFTHAND:
				graphics.setColor(fgl::Color::PINK);
				break;
				
			case METAPOINT_RIGHTHAND:
				graphics.setColor(fgl::Color::VIOLET);
				break;
				
			case METAPOINT_BOUNDS_TOPLEFT:
				graphics.setColor(fgl::Color::BLUE);
				break;
				
			case METAPOINT_BOUNDS_BOTTOMRIGHT:
				graphics.setColor(fgl::Color::SKYBLUE);
				break;
				
			case METAPOINT_HANDLE:
				graphics.setColor(fgl::Color::ORANGE);
				break;
		}
		fgl::RectangleD frame = getRect();
		graphics.drawRect(frame);
		graphics.fillRect(x-0.5, y-0.5, 1.0, 1.0);
		
		graphics.setColor(graphics.getColor().negative());
		switch(orientation)
		{
			case ANIMATIONORIENTATION_NEUTRAL:
				graphics.fillRect(frame.x+(frame.width/2)-0.5, frame.y, 1.0, frame.height);
				break;
				
			case ANIMATIONORIENTATION_LEFT:
				graphics.fillRect(frame.x, frame.y, 1, frame.height);
				break;
				
			case ANIMATIONORIENTATION_RIGHT:
				graphics.fillRect(frame.x+frame.width-1, frame.y, 1, frame.height);
				break;
		}
	}
	
	fgl::RectangleD AnimationMetaPoint::getRect() const
	{
		return fgl::RectangleD(x-radius, y-radius, radius*2, radius*2);
	}
}
