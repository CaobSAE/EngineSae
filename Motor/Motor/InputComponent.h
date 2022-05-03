#pragma once
#include "MoveComponent.h"
#include <cstdint>

class InputComponent : public MoveComponent
{
public:
	InputComponent(class Actor* owner);

	void ProcessInput(const uint8_t* keyState) override;

	float GetMaxForward() const { return m_MaxForwardSpeed; }
	float GetMaxAngular() const { return m_MaxAngularSpeed; }
	int GetForwardKey() const { return m_ForwardKey; }
	int GetBackKey() const { return m_BackKey; }
	int GetClockwiseKey() const { return m_ClockWiseKey; }
	int GetCounterClockwiseKey() const { return m_CounterClockwiseKey; }

	void SetMaxForward(float speed) { m_MaxForwardSpeed = speed; }
	void SetMaxAngular(float speed) { m_MaxAngularSpeed = speed; }
	void SetForwardKey(int key) { m_ForwardKey = key; }
	void SetBackKey(int key) { m_BackKey = key; }
	void SetClockwiseKey(int key) { m_ClockWiseKey = key; }
	void SetCounterClockwiseKey(int key) { m_CounterClockwiseKey = key; }
private:
	float m_MaxForwardSpeed;
	float m_MaxAngularSpeed;

	int m_ForwardKey;
	int m_BackKey;

	int m_ClockWiseKey;
	int m_CounterClockwiseKey;
};