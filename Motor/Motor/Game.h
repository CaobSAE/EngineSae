#pragma once
#include "SDL.h"
#include "CMath.h"
#include <vector>
#include <SDL_image.h>
#include <ostream>
#include <unordered_map>

class Game
{
public:
	Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return m_Asteroids; }

	SDL_Texture* GetTexture(const std::string& fileName);
private:
	void ProcessingInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, SDL_Texture*> m_Textures;

	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;

	std::vector<class Actor*> m_Actors;
	std::vector<class Actor*> m_PendingActors;
	bool m_UpdatingActors;

	bool m_IsRunning;
	Uint32 m_TicksCount;

	std::vector<class SpriteComponent*> m_Sprites;

	// Game-specific
	class Ship* m_Ship;
	std::vector<class Asteroid*> m_Asteroids;
};