#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "CMath.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder) :
	Component(owner),
	m_Texture(nullptr),
	m_DrawOrder(drawOrder),
	m_TexWidth(0),
	m_TexHeight(0)
{
	m_Owner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	m_Owner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (m_Texture)
	{
		SDL_Rect r;

		r.w = static_cast<int>(m_TexWidth * m_Owner->GetScale());
		r.h = static_cast<int>(m_TexHeight * m_Owner->GetScale());

		r.x = static_cast<int>(m_Owner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(m_Owner->GetPosition().y - r.h / 2);

		SDL_RenderCopyEx(
			renderer,									
			m_Texture,
			nullptr,									// Qué parte de la textura renderizar (nullptr para toda la textura)
			&r,											
			-CMath::ToDegrees(m_Owner->GetRotation()),	// Nosotres tenemos la rotación en radianes, SDL la pide en grados.
			// Nosotres utilizamos el unit circle para definir la rotación. En el Unit Circle los valores positivos se encuentran
			// en sentido opuesto de las manecillas del reloj. SDL los pide en el sentido de las manecillas del reloj. Por lo tanto
			// tenemos que negar el signo
			nullptr,									// Punto sobre el cual rotar (nullptr para el centro)
			SDL_FLIP_NONE								// Cómo rotar la textura (normalmente es SDL_FLIP_NONE)
		);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	m_Texture = texture;
	SDL_QueryTexture(texture, nullptr, nullptr, &m_TexWidth, &m_TexHeight);
}
