#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return m_AngularSpeed; }
	float GetForwardSpeed() const { return m_ForwardSpeed; }
	void SetAngularSpeed(float speed) { m_AngularSpeed = speed; }
	void SetForwardSpeed(float speed) { m_ForwardSpeed = speed; }
	void SetScreenWrap(bool screenWrap) { m_ScreenWrap = screenWrap; }
private:
	bool m_ScreenWrap;
	// La velocidad a la que rota el Actor
	float m_AngularSpeed;
	// La velocidad a la que se mueve hacia adelante el Actor
	float m_ForwardSpeed;
};