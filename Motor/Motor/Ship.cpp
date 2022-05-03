#include "Ship.h"
#include "Game.h"
#include "AnimSpriteComponent.h"
#include "InputComponent.h"
#include "Laser.h"

Ship::Ship(Game* game) :
	Actor(game)
{
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Images2D/Ship01.png"),
		game->GetTexture("Assets/Images2D/Ship02.png"),
		game->GetTexture("Assets/Images2D/Ship03.png"),
		game->GetTexture("Assets/Images2D/Ship04.png")
	};

	asc->SetAnimTextures(anims);

	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForward(300.0f);
	ic->SetMaxAngular(CMath::TwoPi);
	ic->SetScreenWrap(true);
}

void Ship::UpdateActor(float deltaTime)
{
	m_LaserCooldown -= deltaTime;
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && m_LaserCooldown <= 0.0f)
	{
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		m_LaserCooldown = 0.5f;
	}
}
