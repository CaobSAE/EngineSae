#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder) :
	SpriteComponent(owner, drawOrder),
	m_CurrentFrame(0.0f),
	m_AnimFPS(24.0)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	m_CurrentFrame += m_AnimFPS * deltaTime;

	while (m_CurrentFrame >= m_AnimTextures.size())
	{
		m_CurrentFrame -= m_AnimTextures.size();
	}

	SetTexture(m_AnimTextures[static_cast<int>(m_CurrentFrame)]);
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	m_AnimTextures = textures;
	if (m_AnimTextures.size() > 0)
	{
		m_CurrentFrame = 0.0f;
		SetTexture(m_AnimTextures[0]);
	}
}
