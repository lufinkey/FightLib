
#pragma once

#include "AnimationData.hpp"

namespace fl
{
	class AnimationAssetManager : public fgl::AssetManager
	{
	public:
		AnimationAssetManager(fgl::Window& window, const fgl::String& root="");
		virtual ~AnimationAssetManager();

		bool loadAnimationData(const fgl::String& path, fgl::String* error=nullptr);
		void unloadAnimationData(const fgl::String& path);
		void unloadAllAnimationData();
		AnimationData* getAnimationData(const fgl::String& path) const;

		virtual size_t getAssetCount() const override;
		virtual size_t reload(bool safely=true) override;
		virtual void unload() override;

	private:
		fgl::ArrayList<std::pair<fgl::String, AnimationData*>> animationDatas;
	};
}
