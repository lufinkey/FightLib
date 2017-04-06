
#pragma once

#include <fightlib/entity/collision/CollisionManager.hpp>
#include <fightlib/entity/draw/DrawManager.hpp>
#include <fightlib/entity/Character.hpp>
#include <fightlib/entity/Entity.hpp>
#include <fightlib/entity/Item.hpp>
#include <fightlib/entity/Platform.hpp>

namespace fl
{
	class Stage
	{
	public:
		Stage();
		virtual ~Stage();
		
		virtual void update(fgl::ApplicationData appData);
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const;
		
		virtual double getGravity(Entity* entity) const;

		
		const fgl::ArrayList<Entity*>& getEntities() const;
		const fgl::ArrayList<Item*>& getItems() const;
		const fgl::ArrayList<Character*>& getCharacters() const;
		const fgl::ArrayList<Platform*>& getPlatforms() const;
		
		void addEntity(Entity* entity, double zLayer=0.5);
		void removeEntity(Entity* entity);

		void addItem(Item* item, double zLayer=0.5);
		void removeItem(Item* item);

		void addCharacter(Character* character, double zLayer=0.5);
		void removeCharacter(Character* character);

	protected:
		void addPlatform(Platform* platform, double zLayer=0.5);
		void removePlatform(Platform* platform);
		
	private:
		fgl::ArrayList<Entity*> entities;
		fgl::ArrayList<Platform*> platforms;

		fgl::ArrayList<Item*> items;
		fgl::ArrayList<Character*> characters;
		
		CollisionManager collisionManager;
		DrawManager drawManager;
	};
}
