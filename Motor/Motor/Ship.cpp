#include "Ship.h"
#include "Game.h"
#include "AnimSpriteComponent.h"

Ship::Ship(Game* game) :
	Actor(game),
	m_RightSpeed(0.0f),
	m_DownSpeed(0.0f)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Images2D/Ship01.png"),
		game->GetTexture("Assets/Images2D/Ship02.png"),
		game->GetTexture("Assets/Images2D/Ship03.png"),
		game->GetTexture("Assets/Images2D/Ship04.png")
	};

	asc->SetAnimTextures(anims);
}

void Ship::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	Vector2 pos = GetPosition();
	pos.x += m_RightSpeed * deltaTime;
	pos.y += m_DownSpeed * deltaTime;

	SetPosition(pos);
}

void Ship::ProcessKeyboard(const uint8_t* state)
{
	m_RightSpeed = 0.0f;
	m_DownSpeed = 0.0f;

	if (state[SDL_SCANCODE_D])
	{
		m_RightSpeed += 250.0f;
	}

	if (state[SDL_SCANCODE_A])
	{
		m_RightSpeed -= 250.0f;
	}

	if (state[SDL_SCANCODE_S])
	{
		m_DownSpeed += 300.0f;
	}

	if (state[SDL_SCANCODE_W])
	{
		m_DownSpeed -= 300.0f;
	}
}
