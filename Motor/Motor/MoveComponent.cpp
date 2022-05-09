#include "MoveComponent.h"
#include "CMath.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder) :
	Component(owner, updateOrder),
	m_AngularSpeed(0.0f),
	m_ForwardSpeed(0.0f),
	m_ScreenWrap(false)
{
}

void MoveComponent::Update(float deltaTime)
{
	if (!CMath::NearZero(m_AngularSpeed))
	{
		float rot = m_Owner->GetRotation();
		rot += m_AngularSpeed * deltaTime;
		m_Owner->SetRotation(rot);
	}

	if (!CMath::NearZero(m_ForwardSpeed))
	{
		Vector2 pos = m_Owner->GetPosition();
		pos += m_Owner->GetForward() * m_ForwardSpeed * deltaTime;

		/*
			TAREA 1
			Obtener mi aceleración utilizando masa y suma de fuerzas
			Obtener la velocidad a partir de la aceleración
			Aplicar el drag sobre la velocidad
			Limitar la velocidad a la velocidad máxima
			Obtener la nueva posición a partir de la velocidad
		*/

		if (m_ScreenWrap)
		{
			if (pos.x < 0.0f) { pos.x = 1024.0f; }
			else if (pos.x > 1024.f) { pos.x = 0.0f; }

			if (pos.y < 0.0f) { pos.y = 768.0f; }
			else if (pos.y > 768.0f) { pos.y = 0.0f; }
		}

		m_Owner->SetPosition(pos);
	}
}
