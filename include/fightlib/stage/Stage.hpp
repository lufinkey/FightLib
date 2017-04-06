
#pragma once

#include <fightlib/entity/collision/CollisionManager.hpp>
#include <fightlib/entity/draw/DrawManager.hpp>
#include <fightlib/entity/Character.hpp>
#include <fightlib/entity/Entity.hpp>
#include <fightlib/entity/Item.hpp>
#include <fightlib/entity/Platform.hpp>

namespace fl
{
	class Fight;
	
	class Stage
	{
		friend class Fight;
	public:
		Stage();
		virtual ~Stage();
		
		virtual void update(fgl::ApplicationData appData);
		virtual void draw(fgl::ApplicationData appData, fgl::Graphics graphics) const;
		
		virtual fgl::Vector2d getGravity(Entity* entity) const;
		
		Fight* getFight() const;
		
		fgl::ArrayList<Item*> getAccessibleItems(Character* character) const;
		
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
		Fight* fight;
		
		fgl::ArrayList<Entity*> entities;
		fgl::ArrayList<Platform*> platforms;

		fgl::ArrayList<Item*> items;
		fgl::ArrayList<Character*> characters;
		
		fgl::BasicDictionary<Character*, fgl::ArrayList<Item*>> characterAccessibleItems;
		
		CollisionManager collisionManager;
		DrawManager drawManager;
	};
}
