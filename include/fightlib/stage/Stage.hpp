
#pragma once

#include <fightlib/entity/collision/CollisionManager.hpp>
#include <fightlib/entity/hitbox/HitboxCollisionManager.hpp>
#include <fightlib/entity/draw/DrawManager.hpp>
#include <fightlib/entity/StageObject.hpp>
#include <fightlib/entity/Character.hpp>
#include <fightlib/entity/Entity.hpp>
#include <fightlib/entity/Item.hpp>
#include "Platform.hpp"
#include "StageSection.hpp"
#include "StageController.hpp"

namespace fl
{
	class Fight;

	class Stage : public Drawable
	{
		friend class Fight;
		friend class Character;
	public:
		Stage();
		virtual ~Stage();

		virtual bool getFlag(const fgl::String& flag) const override;
		virtual void update(fgl::ApplicationData appData) override;
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const override;

		virtual fgl::Vector2d getGravity(StageObject* object) const;
		virtual fgl::Vector2d getAirResistance(StageObject* object) const;

		Fight* getFight() const;
		Stage* getParentStage() const;

		CollisionManager* getCollisionManager();
		const CollisionManager* getCollisionManager() const;
		HitboxCollisionManager* getHitboxCollisionManager();
		const HitboxCollisionManager* getHitboxCollisionManager() const;
		DrawManager* getDrawManager();
		const DrawManager* getDrawManager() const;

		fgl::ArrayList<Item*> getAccessibleItems(Character* character) const;

		const fgl::ArrayList<StageObject*>& getObjects() const;
		const fgl::ArrayList<Entity*>& getEntities() const;
		const fgl::ArrayList<Item*>& getItems() const;
		const fgl::ArrayList<Character*>& getCharacters() const;
		const fgl::ArrayList<Platform*>& getPlatforms() const;

		void addObject(StageObject* object, double zLayer=0.5);
		void removeObject(StageObject* object);

		void addSection(StageSection* section);
		void removeSection(StageSection* section);

		void addSubStage(Stage* stage, double zLayer=0.5);
		void removeSubStage(Stage* stage);

		void addController(StageController* controller);
		void removeController(StageController* controller);
		
	protected:
		virtual void onAddObject(StageObject* object);
		virtual void onRemoveObject(StageObject* object);

	private:
		void loadSection(StageSection* section);
		void unloadSection(StageSection* section);

		Fight* fight;
		Stage* parentStage;

		fgl::ArrayList<StageObject*> objects;
		fgl::ArrayList<Platform*> platforms;
		fgl::ArrayList<Entity*> entities;
		fgl::ArrayList<Item*> items;
		fgl::ArrayList<Character*> characters;

		fgl::ArrayList<StageSection*> sections;
		fgl::ArrayList<Stage*> subStages;
		fgl::ArrayList<StageController*> controllers;

		fgl::BasicDictionary<Character*, fgl::ArrayList<Item*>> characterAccessibleItems;

		CollisionManager collisionManager;
		HitboxCollisionManager hitboxCollisionManager;
		DrawManager drawManager;

		void removeAccessibleItem(Item* item);
	};
}
