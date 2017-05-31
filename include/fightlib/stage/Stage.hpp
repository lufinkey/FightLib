
#pragma once

#include <fightlib/entity/collision/CollisionManager.hpp>
#include <fightlib/entity/hitbox/HitboxCollisionManager.hpp>
#include <fightlib/entity/draw/DrawManager.hpp>
#include <fightlib/entity/Character.hpp>
#include <fightlib/entity/Entity.hpp>
#include <fightlib/entity/Item.hpp>
#include "Platform.hpp"
#include "StageSection.hpp"

namespace fl
{
	class Fight;
	
	class Stage : Drawable
	{
		friend class Fight;
		friend class Character;
	public:
		Stage();
		virtual ~Stage();
		
		virtual void update(fgl::ApplicationData appData) override;
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const override;
		
		virtual fgl::Vector2d getGravity(Entity* entity) const;
		virtual fgl::Vector2d getAirResistance(Entity* entity) const;
		
		Fight* getFight() const;
		
		fgl::ArrayList<Item*> getAccessibleItems(Character* character) const;
		
		const fgl::ArrayList<Entity*>& getEntities() const;
		const fgl::ArrayList<Item*>& getItems() const;
		const fgl::ArrayList<Character*>& getCharacters() const;
		const fgl::ArrayList<Platform*>& getPlatforms() const;

		void addPlatform(Platform* platform, double zLayer=0.5);
		void removePlatform(Platform* platform);

		void addDrawable(Drawable* drawable, double zLayer=0.5);
		void removeDrawable(Drawable* drawable);
		
		void addEntity(Entity* entity, double zLayer=0.5);
		void removeEntity(Entity* entity);

		void addItem(Item* item, double zLayer=0.5);
		void removeItem(Item* item);

		void addCharacter(Character* character, double zLayer=0.5);
		void removeCharacter(Character* character);

		void addSection(StageSection* section);
		void removeSection(StageSection* section);

		void addSubStage(Stage* stage, double zLayer=0.5);
		void removeSubStage(Stage* stage);
		
	private:
		void loadSection(StageSection* section);
		void unloadSection(StageSection* section);

		Fight* fight;
		Stage* parentStage;
		
		fgl::ArrayList<Platform*> platforms;
		fgl::ArrayList<Entity*> entities;
		fgl::ArrayList<Item*> items;
		fgl::ArrayList<Character*> characters;

		fgl::ArrayList<StageSection*> sections;
		fgl::ArrayList<Stage*> subStages;
		
		fgl::BasicDictionary<Character*, fgl::ArrayList<Item*>> characterAccessibleItems;
		
		CollisionManager collisionManager;
		HitboxCollisionManager hitboxCollisionManager;
		DrawManager drawManager;
		
		void removeAccessibleItem(Item* item);
	};
}
