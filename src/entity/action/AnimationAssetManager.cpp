
#include <fightlib/entity/action/AnimationAssetManager.hpp>

namespace fl
{
	AnimationAssetManager::AnimationAssetManager(fgl::AssetManager* assetManager) : AssetManager(assetManager->getWindow(), assetManager->getRootDirectory())
	{
		addAssetManager(assetManager);
	}

	AnimationAssetManager::~AnimationAssetManager()
	{
		unloadAllAnimationData();
	}

	bool AnimationAssetManager::loadAnimationData(const fgl::String& path, fgl::String* error)
	{
		for(auto& animPair : animationDatas)
		{
			if(animPair.first.equals(path))
			{
				return true;
			}
		}

		//TODO load from dependent asset managers maybe?

		AnimationData* animData = new AnimationData();
		bool success = animData->loadFromFile(path, this, error);
		if(success)
		{
			if(error!=nullptr)
			{
				error->clear();
			}
			animationDatas.add(std::pair<fgl::String, AnimationData*>(path, animData));
			return true;
		}
		else
		{
			delete animData;
			return false;
		}
	}

	void AnimationAssetManager::unloadAnimationData(const fgl::String& path)
	{
		for(size_t animationDatas_size=animationDatas.size(), i=0; i<animationDatas_size; i++)
		{
			std::pair<fgl::String, AnimationData*>& pair = animationDatas[i];
			if(pair.first.equals(path))
			{
				delete pair.second;
				animationDatas.remove(i);
				return;
			}
		}
	}

	void AnimationAssetManager::unloadAllAnimationData()
	{
		for(auto& animPair : animationDatas)
		{
			delete animPair.second;
		}
		animationDatas.clear();
	}

	AnimationData* AnimationAssetManager::getAnimationData(const fgl::String& path) const
	{
		for(auto& animPair : animationDatas)
		{
			if(animPair.first.equals(path))
			{
				return animPair.second;
			}
		}
		//TODO try to get it from a dependent asset manager?
		return nullptr;
	}

	size_t AnimationAssetManager::getAssetCount() const
	{
		return animationDatas.size()+AssetManager::getAssetCount();
	}

	size_t AnimationAssetManager::reload(bool safely)
	{
		if(safely)
		{
			size_t successCounter = AssetManager::reload(safely);

			auto oldAnimationDatas = animationDatas;
			animationDatas.clear();

			for(auto& animPair : animationDatas)
			{
				if(loadAnimationData(animPair.first))
				{
					successCounter++;
					delete animPair.second;
				}
				else
				{
					animationDatas.add(animPair);
				}
			}
			return successCounter;
		}
		else
		{
			fgl::ArrayList<fgl::String> animPaths;
			animPaths.reserve(animationDatas.size());

			for(auto& animPair : animationDatas)
			{
				animPaths.add(animPair.first);
			}

			size_t successCounter = reload(safely);

			for(auto& animPath : animPaths)
			{
				if(loadAnimationData(animPath))
				{
					successCounter++;
				}
			}
			return successCounter;
		}
	}

	void AnimationAssetManager::unload()
	{
		AssetManager::unload();
		unloadAllAnimationData();
	}
}
