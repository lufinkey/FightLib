
#include <fightlib/entity/action/AnimationData.h>

namespace fl
{
	#define return_error(err) if(error!=nullptr) {*error = err;} return false;

	AnimationHitbox::AnimationHitbox() :
		tag(-1),
		x(0),
		y(0),
		radius(0)
	{
		//
	}

	bool AnimationHitbox::loadFromDictionary(const fgl::Dictionary& dictionary, fgl::String* error)
	{
		if(dictionary.has("x") && dictionary.has("y") && dictionary.has("radius"))
		{
			float xValue = fgl::extract<fgl::Number>(dictionary, "x").toArithmeticValue<float>();
			float yValue = fgl::extract<fgl::Number>(dictionary, "y").toArithmeticValue<float>();
			float radiusValue = fgl::extract<fgl::Number>(dictionary, "radius").toArithmeticValue<float>();
			if(radiusValue<=0)
			{
				return_error("invalid radius value")
			}

			x = xValue;
			y = yValue;
			radius = radiusValue;
			if(dictionary.has("tag"))
			{
				tag = fgl::extract<fgl::Number>(dictionary, "tag").toArithmeticValue<size_t>();
			}
			return true;
		}
		else
		{
			return_error("missing required field")
		}
	}

	void AnimationHitbox::draw(fgl::Graphics graphics) const
	{
		graphics.setColor(fgl::Color::GREEN);
		graphics.drawRect(getRect());
	}

	fgl::RectangleD AnimationHitbox::getRect() const
	{
		return fgl::RectangleD(x-radius, y-radius, radius*2, radius*2);
	}

	AnimationMetaPoint::AnimationMetaPoint() :
		x(0),
		y(0),
		radius(0),
		rotation(0),
		type(POINTTYPE_HEAD),
		orientation(ORIENTATION_LEFT),
		behind(false),
		visible(true)
	{
		//
	}

	bool AnimationMetaPoint::loadFromDictionary(const fgl::Dictionary& dictionary, fgl::String* error)
	{
		if(dictionary.has("x") && dictionary.has("y") && dictionary.has("radius") && dictionary.has("type") && dictionary.has("orientation"))
		{
			float xValue = fgl::extract<fgl::Number>(dictionary, "x").toArithmeticValue<float>();
			float yValue = fgl::extract<fgl::Number>(dictionary, "y").toArithmeticValue<float>();
			float radiusValue = fgl::extract<fgl::Number>(dictionary, "radius").toArithmeticValue<float>();
			if(radiusValue <= 0)
			{
				return_error("invalid radius value")
			}
			fgl::String typeStr = fgl::extract<fgl::String>(dictionary, "type");
			fgl::String orientationStr = fgl::extract<fgl::String>(dictionary, "orientation");
			float rotationValue = fgl::extract<fgl::Number>(dictionary, "rotation", 0).toArithmeticValue<float>();

			AnimationMetaPoint::Type typeValue;
			if(typeStr=="HEAD")
			{
				typeValue = POINTTYPE_HEAD;
			}
			else if(typeStr=="LEFT_HAND")
			{
				typeValue = POINTTYPE_LEFTHAND;
			}
			else if(typeStr=="RIGHT_HAND")
			{
				typeValue = POINTTYPE_RIGHTHAND;
			}
			else if(typeStr=="HANDLE")
			{
				typeValue = POINTTYPE_HANDLE;
			}
			else
			{
				return_error("invalid \"type\" value: \""+typeStr+"\"")
			}

			AnimationMetaPoint::Orientation orientationValue;
			if(orientationStr=="LEFT")
			{
				orientationValue = ORIENTATION_LEFT;
			}
			else if(orientationStr=="RIGHT")
			{
				orientationValue = ORIENTATION_RIGHT;
			}
			else
			{
				return_error("invalid \"orientation\" value: \""+orientationStr+"\"")
			}

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
			case POINTTYPE_HEAD:
			graphics.setColor(fgl::Color::RED);
			break;

			case POINTTYPE_LEFTHAND:
			graphics.setColor(fgl::Color::PINK);
			break;

			case POINTTYPE_RIGHTHAND:
			graphics.setColor(fgl::Color::PURPLE);
			break;

			case POINTTYPE_HANDLE:
			graphics.setColor(fgl::Color::ORANGE);
			break;
		}
		fgl::RectangleD frame = getRect();
		graphics.drawRect(frame);

		graphics.setColor(graphics.getColor().negative());
		switch(orientation)
		{
			case ORIENTATION_LEFT:
			graphics.drawLine(frame.x, frame.y, frame.x, frame.y+frame.height);
			break;

			case ORIENTATION_RIGHT:
			graphics.drawLine(frame.x+frame.width, frame.y, frame.x+frame.width, frame.y);
			break;
		}
	}

	fgl::RectangleD AnimationMetaPoint::getRect() const
	{
		return fgl::RectangleD(x-radius, y-radius, radius*2, radius*2);
	}

	AnimationData::AnimationData() : animation(nullptr)
	{
		//
	}

	AnimationData::~AnimationData()
	{
		if(animation!=nullptr)
		{
			delete animation;
		}
	}

	bool AnimationData::loadFromFile(const fgl::String& path, fgl::AssetManager* assetManager, fgl::String* error)
	{
		fgl::Dictionary plist;
		fgl::String plistError;
		if(!fgl::Plist::loadFromFile(&plist, path, &plistError))
		{
			return_error("unable to load plist: "+plistError)
		}

		fgl::String animName = fgl::extract<fgl::String>(plist, "name");
		if(animName.length()==0)
		{
			return_error("animation must have a name")
		}

		fgl::Number fps = fgl::extract<fgl::Number>(plist, "fps");
		float fpsValue = fps.toArithmeticValue<float>();
		if(fpsValue <= 0)
		{
			return_error("fps must specified and must be a value greater than 0")
		}

		fgl::ArrayList<fgl::Any> files = fgl::extract<fgl::ArrayList<fgl::Any>>(plist, "files");
		if(files.size()==0)
		{
			return_error("animation must have atleast one frame")
		}

		fgl::String animDirectory = fgl::FileTools::getDirectoryComponent(path);
		fgl::Animation* anim = new fgl::Animation(fpsValue);

		//use list of functions so that files aren't loaded into the asset manager before the loading potentially fails
		fgl::ArrayList<std::function<void()>> loadingFunctions;

		for(size_t i=0; i<files.size(); i++)
		{
			fgl::Dictionary file = fgl::extract<fgl::Dictionary>(files, i);
			fgl::String fileName = fgl::extract<fgl::String>(file, "filename");
			if(fileName.length()==0)
			{
				delete anim;
				return_error((fgl::String)"file at index "+i+" has is missing \"filename\" field");
			}
			fgl::String filePath = fgl::FileTools::combinePathStrings(animDirectory, fileName);
			unsigned int rows = fgl::extract<fgl::Number>(file, "rows", 1).toArithmeticValue<unsigned int>();
			unsigned int cols = fgl::extract<fgl::Number>(file, "columns", 1).toArithmeticValue<unsigned int>();
			if(file.has("sequence"))
			{
				fgl::ArrayList<fgl::Any> sequenceArray = fgl::extract<fgl::ArrayList<fgl::Any>>(file, "sequence");
				fgl::ArrayList<fgl::Vector2u> sequence;
				for(size_t j=0; j<sequenceArray.size(); j++)
				{
					fgl::Dictionary seq = fgl::extract<fgl::Dictionary>(sequenceArray, j);
					if(!seq.has("row") || !seq.has("column"))
					{
						delete anim;
						return_error((fgl::String)"sequence index "+j+" for file at index "+i+" is missing a required field");
					}
					fgl::Number row = fgl::extract<fgl::Number>(seq, "row");
					fgl::Number column = fgl::extract<fgl::Number>(seq, "column");
					sequence.add(fgl::Vector2u(row, column));
				}
				loadingFunctions.add([anim, assetManager, filePath, rows, cols, sequence]{
					anim->addFrames(assetManager, filePath, rows, cols, sequence);
				});
			}
			else
			{
				loadingFunctions.add([anim, assetManager, filePath, rows, cols]{
					anim->addFrames(assetManager, filePath, rows, cols);
				});
			}
		}

		fgl::ArrayList<FrameData> frmDatas;
		fgl::ArrayList<fgl::Any> frameDataArray = fgl::extract<fgl::ArrayList<fgl::Any>>(plist, "framedata");
		for(size_t i=0; i<frameDataArray.size(); i++)
		{
			FrameData frame;
			fgl::Dictionary frameDict = fgl::extract<fgl::Dictionary>(frameDataArray, i);

			fgl::ArrayList<fgl::Any> hitboxesArray = fgl::extract<fgl::ArrayList<fgl::Any>>(frameDict, "hitboxes");
			for(size_t j=0; j<hitboxesArray.size(); j++)
			{
				fgl::Dictionary hitboxDict = fgl::extract<fgl::Dictionary>(hitboxesArray, j);
				if(!hitboxDict.isEmpty())
				{
					AnimationHitbox hitbox;
					fgl::String hitboxError;
					bool hitboxSuccess = hitbox.loadFromDictionary(hitboxDict, &hitboxError);
					if(hitboxSuccess)
					{
						for(size_t k=0; k<frame.hitboxes.size(); k++)
						{
							if(hitbox.tag!=(size_t)-1 && hitbox.tag==frame.hitboxes[k].tag)
							{
								delete anim;
								return_error((fgl::String)"duplicate hitbox tag "+hitbox.tag+" in hitbox at index "+j+" for frame "+i)
							}
						}
						frame.hitboxes.add(hitbox);
					}
					else
					{
						delete anim;
						return_error((fgl::String)"unable to load hitbox at index "+j+" for frame "+i+": "+hitboxError)
					}
				}
			}

			fgl::ArrayList<fgl::Any> metapointsArray = fgl::extract<fgl::ArrayList<fgl::Any>>(frameDict, "metapoints");
			for(size_t j=0; j<metapointsArray.size(); j++)
			{
				fgl::Dictionary metapointDict = fgl::extract<fgl::Dictionary>(metapointsArray, j);
				if(!metapointDict.isEmpty())
				{
					AnimationMetaPoint metapoint;
					fgl::String metapointError;
					bool metapointSuccess = metapoint.loadFromDictionary(metapointDict, &metapointError);
					if(metapointSuccess)
					{
						frame.metapoints.add(metapoint);
					}
					else
					{
						delete anim;
						return_error((fgl::String)"unable to load metapoint at index "+j+" for frame "+i+": "+metapointError);
					}
				}
			}

			frmDatas.add(frame);
		}

		//load all the animation frames
		for(size_t i=0; i<loadingFunctions.size(); i++)
		{
			std::function<void()> func = loadingFunctions[i];
			func();
		}
		name = animName;
		animation = anim;
		frameDatas = frmDatas;
		return true;
	}

	void AnimationData::drawFrame(size_t frameIndex, fgl::Graphics& graphics, bool showFrames) const
	{
		if(animation!=nullptr)
		{
			animation->drawFrame(graphics, frameIndex);

			if(showFrames)
			{
				fgl::Graphics frameGraphics = graphics;
				fgl::RectangleD animFrame = animation->getRect(frameIndex);
				frameGraphics.setColor(fgl::Color::BLACK);
				frameGraphics.drawRect(animFrame);

				frameGraphics.translate(animFrame.x, animFrame.y);

				if(frameIndex < frameDatas.size())
				{
					const FrameData& frameData = frameDatas[frameIndex];
					for(size_t hitboxes_size=frameData.hitboxes.size(), i=0; i<hitboxes_size; i++)
					{
						frameData.hitboxes[i].draw(frameGraphics);
					}
					for(size_t metapoints_size=frameData.metapoints.size(), i=0; i<metapoints_size; i++)
					{
						frameData.metapoints[i].draw(frameGraphics);
					}
				}
			}
		}
	}

	const fgl::String& AnimationData::getName() const
	{
		return name;
	}

	fgl::Animation* AnimationData::getAnimation() const
	{
		return animation;
	}

	fgl::Vector2d AnimationData::getSize(size_t frameIndex, double scale) const
	{
		if(animation==nullptr)
		{
			return fgl::Vector2d(0, 0);
		}
		fgl::RectangleD animRect = animation->getRect(frameIndex);
		return fgl::Vector2d(animRect.width*scale, animRect.height*scale);
	}

	fgl::ArrayList<AnimationHitbox> AnimationData::getHitboxes(size_t frameIndex) const
	{
		if(frameIndex >= frameDatas.size())
		{
			return fgl::ArrayList<AnimationHitbox>();
		}
		return frameDatas[frameIndex].hitboxes;
	}

	fgl::ArrayList<AnimationMetaPoint> AnimationData::getMetaPoints(size_t frameIndex) const
	{
		if(frameIndex >= frameDatas.size())
		{
			return fgl::ArrayList<AnimationMetaPoint>();
		}
		return frameDatas[frameIndex].metapoints;
	}

	fgl::ArrayList<AnimationMetaPoint> AnimationData::getMetaPoints(size_t frameIndex, AnimationMetaPoint::Type pointType) const
	{
		if(frameIndex >= frameDatas.size())
		{
			return fgl::ArrayList<AnimationMetaPoint>();
		}
		const FrameData& frame = frameDatas[frameIndex];
		fgl::ArrayList<AnimationMetaPoint> metaPoints;
		for(size_t metapoints_size=frame.metapoints.size(), i=0; i<metapoints_size; i++)
		{
			if(frame.metapoints[i].type==pointType)
			{
				metaPoints.add(frame.metapoints[i]);
			}
		}
		return metaPoints;
	}
}
