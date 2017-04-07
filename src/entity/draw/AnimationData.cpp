
#include <fightlib/entity/draw/AnimationData.hpp>

namespace fl
{
	#define return_error(err) if(error!=nullptr) {*error = err;} return false;

	AnimationData::AnimationData()
		: animation(new fgl::Animation(1.0)),
		orientation(ANIMATIONORIENTATION_NEUTRAL)
	{
		//
	}
	
	AnimationData::AnimationData(const AnimationData& animationData)
		: name(animationData.name),
		animation(new fgl::Animation(*animationData.animation)),
		orientation(animationData.orientation),
		frameDatas(animationData.frameDatas)
	{
		//
	}
	
	AnimationData::AnimationData(AnimationData&& animationData)
		: name(animationData.name),
		animation(animationData.animation),
		orientation(animationData.orientation),
		frameDatas(animationData.frameDatas)
	{
		animationData.animation = nullptr;
	}

	AnimationData::~AnimationData()
	{
		if(animation!=nullptr)
		{
			delete animation;
		}
	}
	
	AnimationData& AnimationData::operator=(const fl::AnimationData& animationData)
	{
		name = animationData.name;
		if(animation!=nullptr)
		{
			delete animation;
			animation = nullptr;
		}
		if(animationData.animation!=nullptr)
		{
			animation = new fgl::Animation(*animationData.animation);
		}
		orientation = animationData.orientation;
		frameDatas = animationData.frameDatas;
		return *this;
	}
	
	AnimationData& AnimationData::operator=(fl::AnimationData&& animationData)
	{
		name = animationData.name;
		if(animation!=nullptr)
		{
			delete animation;
			animation = nullptr;
		}
		if(animationData.animation!=nullptr)
		{
			animation = animationData.animation;
			animationData.animation = nullptr;
		}
		orientation = animationData.orientation;
		frameDatas = animationData.frameDatas;
		return *this;
	}
	
	bool AnimationData::operator==(const fl::AnimationData& animationData) const
	{
		if(animation!=nullptr)
		{
			if(animationData.animation!=nullptr)
			{
				if(*animation != *animationData.animation)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else if(animationData.animation!=nullptr)
		{
			return false;
		}
		
		if(name!=animationData.name || orientation!=animationData.orientation)
		{
			return false;
		}
		
		if(frameDatas.size() != animationData.frameDatas.size())
		{
			return false;
		}
		
		for(size_t i=0; i<frameDatas.size(); i++)
		{
			auto& frameData = frameDatas[i];
			auto& cmpFrameData = animationData.frameDatas[i];
			if(frameData.metapoints.size() != cmpFrameData.metapoints.size())
			{
				return false;
			}
			for(size_t j=0; j<frameData.metapoints.size(); j++)
			{
				auto& metaPoint = frameData.metapoints[j];
				auto& cmpMetaPoint = cmpFrameData.metapoints[j];
				if(metaPoint != cmpMetaPoint)
				{
					return false;
				}
			}
		}
		
		return true;
	}
	
	bool AnimationData::operator!=(const fl::AnimationData& animationData) const
	{
		return !operator==(animationData);
	}

	bool AnimationData::loadFromFile(const fgl::String& path, fgl::AssetManager* assetManager, fgl::String* error)
	{
		fgl::Dictionary plist;
		fgl::String plistError;
		fgl::String resolvedPath;
		FILE* plistFile = assetManager->openFile(path, "rb", &resolvedPath);
		if(plistFile==nullptr)
		{
			return_error("unable to open file");
		}
		if(!fgl::Plist::loadFromFile(&plist, plistFile, &plistError))
		{
			std::fclose(plistFile);
			return_error("unable to load plist: "+plistError);
		}
		std::fclose(plistFile);

		fgl::String fullPath;
		if(fgl::FileTools::isPathAbsolute(resolvedPath))
		{
			fullPath = resolvedPath;
		}
		else
		{
			fullPath = fgl::FileTools::getAbsolutePath(resolvedPath);
			if(fullPath.length()==0)
			{
				return_error("unable to resolve animation path")
			}
		}

		fgl::String animName = fgl::extract<fgl::String>(plist, "name");
		if(animName.length()==0)
		{
			return_error("missing \"name\" field");
		}

		fgl::String orientationStr = fgl::extract<fgl::String>(plist, "orientation");
		if(orientationStr.length()==0)
		{
			return_error("missing \"orientation\" field");
		}
		AnimationOrientation orientationValue;
		if(!AnimationOrientation_fromString(orientationStr, &orientationValue))
		{
			return_error("invalid \"orientation\" value: "+orientationStr);
		}

		fgl::Number fps = fgl::extract<fgl::Number>(plist, "fps");
		float fpsValue = fps.toArithmeticValue<float>();
		if(fpsValue <= 0)
		{
			return_error("fps must specified and must be a value greater than 0");
		}

		fgl::ArrayList<fgl::Any> files = fgl::extract<fgl::ArrayList<fgl::Any>>(plist, "files");

		fgl::String animDirectory = fgl::FileTools::getDirectoryComponent(fullPath);
		if(animDirectory.length()==0)
		{
			return_error("unable to resolve animation directory")
		}
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
			fgl::String filePath;
			if(fgl::FileTools::isPathAbsolute(fileName))
			{
				filePath = fileName;
			}
			else
			{
				filePath = fgl::FileTools::combinePathStrings(animDirectory, fileName);
			}
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
						if(metapoint.tag!=(size_t)-1)
						{
							for(auto cmpMetapoint : frame.metapoints)
							{
								if(metapoint.tag==cmpMetapoint.tag && metapoint.type==cmpMetapoint.type)
								{
									delete anim;
									return_error((fgl::String)"duplicate metapoint tag "+metapoint.tag+" for type "+metapoint.type+" in metapoint at index "+j+" for frame "+i);
								}
							}
						}
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
		if(animation!=nullptr)
		{
			delete animation;
			animation = nullptr;
		}
		name = animName;
		animation = anim;
		orientation = orientationValue;
		frameDatas = frmDatas;
		return true;
	}

	bool AnimationData::saveToFile(const fgl::String& path, fgl::String* error) const
	{
		fgl::Dictionary plist;

		//save the main properties
		plist["name"] = name;
		fgl::String orientationStr = AnimationOrientation_toString(orientation);
		if(orientationStr.length()==0)
		{
			return_error("invalid orientation value");
		}
		plist["orientation"] = orientationStr;
		plist["fps"] = fgl::Number(animation->getFPS());
		
		fgl::String animDirectory = fgl::FileTools::getDirectoryComponent(path);

		fgl::ArrayList<fgl::Any> files;
		
		//save "files"
		fgl::String lastFile;
		unsigned int lastRows = 0;
		unsigned int lastCols = 0;
		unsigned int imageFrameCount = 0;
		fgl::ArrayList<fgl::Vector2u> sequence;
		bool framesNeedSequence = false;
		bool sequenceFinished = false;
		for(auto& frame : animation->getFrames())
		{
			if(frame.file!=lastFile || frame.rows!=lastRows || frame.cols!=lastCols)
			{
				if(lastFile.length()!=0)
				{
					//save the file into the files array
					fgl::ArrayList<fgl::Any> sequenceArr;
					if(framesNeedSequence || !sequenceFinished)
					{
						for(auto& seq : sequence)
						{
							fgl::Dictionary seqDict = {{"row",fgl::Number(seq.y)},{"column",fgl::Number(seq.x)}};
							sequenceArr.add(seqDict);
						}
					}
					fgl::Dictionary file;
					fgl::String relativePath = fgl::FileTools::getRelativePath(animDirectory, lastFile);
					if(relativePath.length()==0)
					{
						return_error("unable to resolve image path "+lastFile);
					}
					file["filename"] = relativePath;
					file["rows"] = fgl::Number(lastRows);
					file["columns"] = fgl::Number(lastCols);
					if(framesNeedSequence || !sequenceFinished)
					{
						file["sequence"] = sequenceArr;
					}
					files.add(file);
				}

				lastFile = frame.file;
				lastRows = frame.rows;
				lastCols = frame.cols;
				imageFrameCount = 0;
				sequence = {};
				framesNeedSequence = false;
				sequenceFinished = false;
			}
			if(sequenceFinished)
			{
				//we had more frames than expected, so we need the sequence specified
				framesNeedSequence = true;
			}
			else if(!framesNeedSequence)
			{
				//make sure the frame falls in sequence
				unsigned int seqIndex = (frame.cols*frame.y)+frame.x;
				if(seqIndex!=imageFrameCount)
				{
					//sequence needs to be specified
					framesNeedSequence = true;
				}
				else if(imageFrameCount==((frame.rows*frame.cols)-1))
				{
					//sequence should be complete
					sequenceFinished = true;
				}
			}
			sequence.add(fgl::Vector2u(frame.x, frame.y));
			imageFrameCount++;
		}
		//add the current file to the files array
		if(lastFile.length()!=0)
		{
			//save the file into the files array
			fgl::ArrayList<fgl::Any> sequenceArr;
			if(framesNeedSequence || !sequenceFinished)
			{
				for(auto& seq : sequence)
				{
					fgl::Dictionary seqDict ={{"row",fgl::Number(seq.y)},{"column",fgl::Number(seq.x)}};
					sequenceArr.add(seqDict);
				}
			}
			fgl::Dictionary file;
			fgl::String relativePath = fgl::FileTools::getRelativePath(animDirectory, lastFile);
			if(relativePath.length()==0)
			{
				return_error("unable to resolve image path "+lastFile);
			}
			file["filename"] = relativePath;
			file["rows"] = fgl::Number(lastRows);
			file["columns"] = fgl::Number(lastCols);
			if(framesNeedSequence || !sequenceFinished)
			{
				file["sequence"] = sequenceArr;
			}
			files.add(file);
		}
		plist["files"] = files;

		fgl::ArrayList<fgl::Any> frameDatasArray;
		size_t framedataIndex = 0;
		for(auto& framedata : frameDatas)
		{
			fgl::Dictionary frameDataDict;
			fgl::ArrayList<fgl::Any> metaPointsArray;
			size_t metapointIndex = 0;
			for(auto& metapoint : framedata.metapoints)
			{
				fgl::Dictionary metapointDict;
				fgl::String metapointError;
				if(!metapoint.saveToDictionary(&metapointDict, &metapointError))
				{
					return_error((fgl::String)"unable to save metapoint at index "+metapointIndex+" for frame at index "+framedataIndex+": "+metapointError);
				}
				metaPointsArray.add(metapointDict);
				metapointIndex++;
			}
			frameDataDict["metapoints"] = metaPointsArray;
			frameDatasArray.add(frameDataDict);

			framedataIndex++;
		}
		plist["framedata"] = frameDatasArray;
		
		return fgl::Plist::saveToFile(plist, path, error);
	}

	void AnimationData::drawFrame(size_t frameIndex, fgl::Graphics graphics, AnimationOrientation drawnOrientation, bool showFrames) const
	{
		if(animation!=nullptr && animation->getTotalFrames() > 0)
		{
			if(isMirrored(drawnOrientation))
			{
				graphics.scale(-1.0, 1.0);
			}

			animation->drawFrame(graphics, frameIndex);

			if(showFrames)
			{
				fgl::RectangleD animFrame = animation->getRect(frameIndex);
				drawMetaPoints(frameIndex, animFrame, graphics, drawnOrientation);
				
				graphics.setColor(fgl::Color::BLACK);
				graphics.drawRect(animFrame);
			}
		}
	}
	
	void AnimationData::drawMetaPoints(size_t frameIndex, const fgl::RectangleD& dstRect, fgl::Graphics graphics, AnimationOrientation drawnOrientation) const
	{
		if(animation!=nullptr && animation->getTotalFrames() > 0)
		{
			graphics.translate(dstRect.x, dstRect.y);

			fgl::RectangleD animFrame = animation->getRect(frameIndex);
			graphics.scale(dstRect.width/animFrame.width, dstRect.height/animFrame.height);

			if(frameIndex < frameDatas.size())
			{
				const FrameData& frameData = frameDatas[frameIndex];
				for(auto& metapoint : frameData.metapoints)
				{
					metapoint.draw(graphics);
				}
			}

			fgl::ArrayList<TaggedBox> bounds = getBounds(frameIndex);
			graphics.setColor(fgl::Color::SKYBLUE);
			for(size_t i=0; i<bounds.size(); i++)
			{
				graphics.drawRect(bounds[i].rect);
			}
		}
	}
	
	void AnimationData::drawMetaPoints(size_t frameIndex, const fgl::RectangleD& dstRect, fgl::Graphics graphics, AnimationOrientation drawnOrientation, MetaPointType metaPointType) const
	{
		if(animation!=nullptr && animation->getTotalFrames() > 0)
		{
			graphics.translate(dstRect.x, dstRect.y);
			
			fgl::RectangleD animFrame = animation->getRect(frameIndex);
			graphics.scale(dstRect.width/animFrame.width, dstRect.height/animFrame.height);
			
			if(frameIndex < frameDatas.size())
			{
				const FrameData& frameData = frameDatas[frameIndex];
				for(auto& metapoint : frameData.metapoints)
				{
					if(metapoint.type==metaPointType)
					{
						metapoint.draw(graphics);
					}
				}
			}
			
			if(metaPointType==METAPOINT_BOUNDS_TOPLEFT || metaPointType==METAPOINT_BOUNDS_BOTTOMRIGHT)
			{
				fgl::ArrayList<TaggedBox> bounds = getBounds(frameIndex);
				graphics.setColor(fgl::Color::SKYBLUE);
				for(auto& metaBounds : bounds)
				{
					if(metaPointType==METAPOINT_BOUNDS_TOPLEFT)
					{
						graphics.fillRect(metaBounds.rect.x, metaBounds.rect.y, metaBounds.rect.width, 1.0);
						graphics.fillRect(metaBounds.rect.x, metaBounds.rect.y, 1.0, metaBounds.rect.height);
					}
					else if(metaPointType==METAPOINT_BOUNDS_BOTTOMRIGHT)
					{
						graphics.fillRect(metaBounds.rect.x+metaBounds.rect.width-1.0, metaBounds.rect.y, 1.0, metaBounds.rect.height);
						graphics.fillRect(metaBounds.rect.x, metaBounds.rect.y+metaBounds.rect.height-1.0, metaBounds.rect.width, 1.0);
					}
				}
			}
		}
	}

	const fgl::String& AnimationData::getName() const
	{
		return name;
	}

	void AnimationData::setName(const fgl::String& name_arg)
	{
		name = name_arg;
	}

	fgl::Animation* AnimationData::getAnimation() const
	{
		return animation;
	}

	void AnimationData::setAnimation(fgl::Animation* animation_arg)
	{
		animation = animation_arg;
	}

	AnimationOrientation AnimationData::getOrientation() const
	{
		return orientation;
	}

	void AnimationData::setOrientation(AnimationOrientation orientation_arg)
	{
		orientation = orientation_arg;
	}

	fgl::Vector2u AnimationData::getSize(size_t frameIndex) const
	{
		if(animation==nullptr)
		{
			return fgl::Vector2u(0, 0);
		}
		return animation->getFrameSize(frameIndex);
	}

	fgl::ArrayList<AnimationMetaPoint> AnimationData::getMetaPoints(size_t frameIndex) const
	{
		if(frameIndex >= frameDatas.size())
		{
			return {};
		}
		return frameDatas[frameIndex].metapoints;
	}
	
	void AnimationData::setMetaPoints(size_t frameIndex, const fgl::ArrayList<AnimationMetaPoint>& metaPoints)
	{
		while(frameIndex >= frameDatas.size())
		{
			frameDatas.add(FrameData());
		}
		frameDatas[frameIndex].metapoints = metaPoints;
	}

	fgl::ArrayList<AnimationMetaPoint> AnimationData::getMetaPoints(size_t frameIndex, MetaPointType pointType) const
	{
		if(frameIndex >= frameDatas.size())
		{
			return {};
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

	const AnimationMetaPoint& AnimationData::getMetaPoint(size_t frameIndex, size_t metaPointIndex) const
	{
		return frameDatas[frameIndex].metapoints[metaPointIndex];
	}

	void AnimationData::setMetaPoint(size_t frameIndex, size_t metaPointIndex, const AnimationMetaPoint& metaPoint)
	{
		frameDatas[frameIndex].metapoints[metaPointIndex] = metaPoint;
	}

	size_t AnimationData::addMetaPoint(size_t frameIndex, const AnimationMetaPoint& metaPoint)
	{
		while(frameIndex >= frameDatas.size())
		{
			frameDatas.add(FrameData());
		}
		size_t metaPointIndex = frameDatas[frameIndex].metapoints.size();
		frameDatas[frameIndex].metapoints.add(metaPoint);
		return metaPointIndex;
	}

	void AnimationData::removeMetaPoint(size_t frameIndex, size_t metaPointIndex)
	{
		frameDatas[frameIndex].metapoints.remove(metaPointIndex);
	}

	fgl::ArrayList<TaggedBox> AnimationData::getBounds(size_t frameIndex, AnimationOrientation drawnOrientation) const
	{
		if(frameIndex >= frameDatas.size())
		{
			return {};
		}
		bool mirrored = isMirrored(drawnOrientation);
		const FrameData& frame = frameDatas[frameIndex];
		fgl::ArrayList<AnimationMetaPoint> topLefts;
		fgl::ArrayList<AnimationMetaPoint> bottomRights;
		for(size_t metapoints_size=frame.metapoints.size(), i=0; i<metapoints_size; i++)
		{
			if(frame.metapoints[i].type==METAPOINT_BOUNDS_TOPLEFT)
			{
				topLefts.add(frame.metapoints[i]);
			}
			else if(frame.metapoints[i].type==METAPOINT_BOUNDS_BOTTOMRIGHT)
			{
				bottomRights.add(frame.metapoints[i]);
			}
		}
		fgl::Vector2u size = getSize(frameIndex);
		fgl::ArrayList<TaggedBox> bounds;
		while(topLefts.size()>0)
		{
			const AnimationMetaPoint& topLeft = topLefts[0];
			for(size_t i=0; i<bottomRights.size(); i++)
			{
				const AnimationMetaPoint& bottomRight = bottomRights[i];
				if(topLeft.tag==bottomRight.tag)
				{
					fgl::RectangleD rect = fgl::RectangleD((double)topLeft.x, (double)topLeft.y, (double)(bottomRight.x-topLeft.x), (double)(bottomRight.y - topLeft.y));
					if(mirrored)
					{
						rect.x = (double)size.x - (rect.x + rect.width);
					}
					TaggedBox metaBounds = { .tag=topLeft.tag, .rect=rect };
					bounds.add(metaBounds);
					bottomRights.remove(i);
					break;
				}
			}
			topLefts.remove(0);
		}
		return bounds;
	}

	bool AnimationData::isMirrored(AnimationOrientation drawnOrientation) const
	{
		if(orientation!=drawnOrientation && drawnOrientation!=ANIMATIONORIENTATION_NEUTRAL)
		{
			return true;
		}
		return false;
	}
}
