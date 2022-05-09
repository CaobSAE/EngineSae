#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "CMath.h"
#include "Shader.h"

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

void SpriteComponent::Draw(Shader* shader)
{
	if (m_Texture){
		glDrawElements(
			GL_TRIANGLES,		//Tipo de poligono a dibujar
			6,					//Cuantos indices hay en el bufer de indices
			GL_UNSIGNED_INT,	//Tipo de cada indice
			nullptr
		);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	m_Texture = texture;
	SDL_QueryTexture(texture, nullptr, nullptr, &m_TexWidth, &m_TexHeight);
}
