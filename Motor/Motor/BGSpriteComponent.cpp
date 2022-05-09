#include "BGSpriteComponent.h"
#include "Actor.h"
#include "Shader.h"

BGSpriteComponent::BGSpriteComponent(Actor* owner, int drawOrder) :
	SpriteComponent(owner, drawOrder),
	m_Scrollspeed(0.0f)
{
}

void BGSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	for (auto& bg : m_BGTextures)
	{
		bg.m_Offset.x += m_Scrollspeed * deltaTime;
		if (bg.m_Offset.x < -m_ScreenSize.x)
		{
			bg.m_Offset.x = (m_BGTextures.size() - 1) * m_ScreenSize.x - 1;
		}
	}
}

void BGSpriteComponent::Draw(Shader* shader)
{ 
	for (auto& bg : m_BGTextures)
	{
		glDrawElements(
			GL_TRIANGLES,		//Tipo de poligono a dibujar
			6,					//Cuantos indices hay en el bufer de indices
			GL_UNSIGNED_INT,	//Tipo de cada indice
			nullptr
		);
	}
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures)
{
	int count = 0;
	for (auto tex : textures)
	{
		BGTexture temp;
		temp.m_Texture = tex;
		temp.m_Offset.x = count * m_ScreenSize.x;
		temp.m_Offset.y = 0;
		m_BGTextures.emplace_back(temp);
		count++;
	}
}
