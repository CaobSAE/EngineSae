#include "Game.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include "Ship.h"
#include "BGSpriteComponent.h"
#include "Asteroid.h"
#include "GL/glew.h"

Game::Game() :
	m_Window(nullptr),
	m_Renderer(nullptr),
	m_IsRunning(true),
	m_TicksCount(0)
{
}

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE//cORE PARA APLIACCIONES DE ESCRITORIO
	);

	//Version openGL 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Pedir un búfer de color de 8 bits para cada canal RGBA
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // 2 ^ 8 nos da la cantidad de valores distintos para este canal (0-255)
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	// Activar double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Correr en GPU
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	m_Window = SDL_CreateWindow(
		"Motor SAE",
		100,	// Coordenadas de x, esquina superior izquierda
		100,	// Coordenadas de y, esquina superior izquierda
		1024,   // Ancho de la pantalla
		768,	// Altura de la pantalla
		SDL_WINDOW_OPENGL		// Bandera (0 para ninguna bandera)	
	);

	if (!m_Window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	m_Context = SDL_GL_CreateContext(m_Window);

	// Prevenir algunos errores de inicialización
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initalize GLEW");
		return false;
	}

	// Borrar ciertos errores benignos que hay en algunas plataformas
	glGetError();


	m_Renderer = SDL_CreateRenderer(
		m_Window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_Image: %s", SDL_GetError());
		return false;
	}

	LoadData();

	m_TicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop()
{
	while (m_IsRunning)
	{
		ProcessingInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyWindow(m_Window);
	SDL_DestroyRenderer(m_Renderer);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	if (m_UpdatingActors)
	{
		m_PendingActors.emplace_back(actor);
	}
	else
	{
		m_Actors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(m_PendingActors.begin(), m_PendingActors.end(), actor);
	if (iter != m_PendingActors.end())
	{
		std::iter_swap(iter, m_PendingActors.end() - 1);
		m_PendingActors.pop_back();
	}

	iter = std::find(m_Actors.begin(), m_Actors.end(), actor);
	if (iter != m_Actors.end())
	{
		std::iter_swap(iter, m_Actors.end() - 1);
		m_Actors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = m_Sprites.begin();
	for (; iter != m_Sprites.end(); ++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	m_Sprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(m_Sprites.begin(), m_Sprites.end(), sprite);
	m_Sprites.erase(iter);
}

void Game::AddAsteroid(Asteroid* ast)
{
	m_Asteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
 	auto iter = std::find(m_Asteroids.begin(), m_Asteroids.end(), ast);
	if (iter != m_Asteroids.end())
	{
		m_Asteroids.erase(iter);
	}
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	auto iter = m_Textures.find(fileName);
	if (iter != m_Textures.end())
	{
		tex = iter->second;
	}
	else
	{
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s ", SDL_GetError());
			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(m_Renderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s: ", fileName.c_str());
			return nullptr;
		}

		m_Textures.emplace(fileName.c_str(), tex);
	}

	return tex;
}

void Game::ProcessingInput()
{
	// Eventos de mouse
	SDL_Event event;

	// Revisamos todos los eventos de input que sucedieron entre la última llamada a
	// SDL_PollEvent y ésta
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_IsRunning = false;
			break;
		}
	}

	// Regresar el estado del teclado
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		m_IsRunning = false;
	}

	m_UpdatingActors = true;
	for (auto actor : m_Actors)
	{
		actor->ProcessInput(state);
	}
	m_UpdatingActors = false;
}

void Game::UpdateGame()
{
	// Frame limiting
	// Nuestro objetivo es que el juego corra a 60fps.
	// Esto significa que cada cuadro tiene que durar 16.6ms
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_TicksCount + 16));

	float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;
	m_TicksCount = SDL_GetTicks();

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05;
	}

	m_UpdatingActors = true;
	for (auto actor : m_Actors)
	{
		actor->Update(deltaTime);
	}
	m_UpdatingActors = false;

	for (auto pending : m_PendingActors)
	{
		m_Actors.emplace_back(pending);
	}
	m_PendingActors.clear();

	std::vector<Actor*> deadActors;
 	for (auto actor : m_Actors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(
		m_Renderer,
		0,
		0,
		255,
		255
	);

	// Resetear el búfer al color indicado
	SDL_RenderClear(m_Renderer);

	for (auto sprite : m_Sprites)
	{
		sprite->Draw(m_Renderer);
	}

	SDL_RenderPresent(m_Renderer);
}

void Game::LoadData()
{
	m_Ship = new Ship(this);
	m_Ship->SetPosition(Vector2(100.0f, 384.0f));
	m_Ship->SetScale(1.5f);

	Actor* temp = new Actor(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgTexs = {
		GetTexture("Assets/Images2D/Farback01.png"),
		GetTexture("Assets/Images2D/Farback02.png")
	};
	bg->SetBGTextures(bgTexs);
	bg->SetScrollSpeed(-100.0f);

	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgTexs = {
		GetTexture("Assets/Images2D/Stars.png"),
		GetTexture("Assets/Images2D/Stars.png")
	};
	bg->SetBGTextures(bgTexs);
	bg->SetScrollSpeed(-200.0f);

	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
	while (!m_Actors.empty())
	{
		delete m_Actors.back();
	}

	for (auto i : m_Textures)
	{
		SDL_DestroyTexture(i.second);
	}
	m_Textures.clear();
}
