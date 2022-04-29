#pragma once
#include <vector>
#include "CMath.h"

class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	~Actor();

	// Este método lo vamos a llamar directamente desde Game (no se puede sobreescribir)
	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	// Específico del actor (sí se puede sobreescribir)
	virtual void UpdateActor(float deltaTime);

	// Getters/Setters
	const Vector2& GetPosition() const { return m_Position; }
	void SetPosition(const Vector2& pos) { m_Position = pos; }
	float GetScale() const { return m_Scale; }
	void SetScale(float scale) { m_Scale = scale; }
	float GetRotation() const { return m_Rotation; }
	void SetRotation(float rotation) { m_Rotation = rotation; }

	State GetState() const { return m_State; }
	void SetState(State state) { m_State = state; }

	class Game* GetGame() { return m_Game; }

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	State m_State;

	// Transform
	Vector2 m_Position;
	float m_Scale;
	float m_Rotation;

	std::vector<class Component*> m_Components;
	class Game* m_Game;
};