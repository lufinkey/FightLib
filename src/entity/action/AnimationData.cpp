
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

			PointType typeValue;
			if(typeStr=="head")
			{
				typeValue = POINTTYPE_HEAD;
			}
			else if(typeStr=="lefthand")
			{
				typeValue = POINTTYPE_LEFTHAND;
			}
			else if(typeStr=="righthand")
			{
				typeValue = POINTTYPE_RIGHTHAND;
			}
			else
			{
				return_error("invalid \"type\" value: \""+typeStr+"\"")
			}

			Orientation orientationValue;
			if(orientationStr=="left")
			{
				orientationValue = ORIENTATION_LEFT;
			}
			else if(orientationStr=="right")
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
			return_error("Unable to load plist: "+plistError)
		}

		fgl::String animName = fgl::extract<fgl::String>(plist, "name");
		if(animName.length())
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
			fgl::String fileName = fgl::extract<fgl::String>(file, "name");
			if(fileName.length()==0)
			{
				delete anim;
				return_error((fgl::String)"file at index "+i+" has no name");
			}
			fgl::String filePath = fgl::FileTools::combinePathStrings(animDirectory, name);
			fgl::Number rows = fgl::extract<fgl::Number>(file, "rows", 1);
			fgl::Number cols = fgl::extract<fgl::Number>(file, "columns", 1);
			unsigned int rowsValue = rows.toArithmeticValue<unsigned int>();
			unsigned int colsValue = cols.toArithmeticValue<unsigned int>();
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

		fgl::ArrayList<FrameData> frmData;
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

			frmData.add(frame);
		}

		//load all the animation frames
		for(size_t i=0; i<loadingFunctions.size(); i++)
		{
			std::function<void()> func = loadingFunctions[i];
			func();
		}
		name = animName;
		animation = anim;
		frameData = frmData;
		return true;
	}

	void AnimationData::drawFrame(double x, double y, double scale, size_t frameIndex, fgl::Graphics graphics)
	{
		if(animation!=nullptr)
		{
			graphics.translate(x, y);
			graphics.scale(scale, scale);
			animation->drawFrame(graphics, frameIndex);
			//TODO draw metapoints and hitboxes if preprocessor says so
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
}
