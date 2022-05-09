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
	virtual ~Actor();

	// Este método lo vamos a llamar directamente desde Game (no se puede sobreescribir)
	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	// Específico del actor (sí se puede sobreescribir)
	virtual void UpdateActor(float deltaTime);

	void ProcessInput(const uint8_t* keyState);
	virtual void ActorInput(const uint8_t* keyState) {}

	// Getters/Setters
	const Vector2& GetPosition() const { return m_Position; }
	void SetPosition(const Vector2& pos) {
		m_Position = pos;
		m_RecomputeWorldTransform = true;
	}
	float GetScale() const { return m_Scale; }
	void SetScale(float scale) {
		m_Scale = scale;
		m_RecomputeWorldTransform = true;
	}
	float GetRotation() const { return m_Rotation; }
	void SetRotation(float rotation) {
		m_Rotation = rotation;
		m_RecomputeWorldTransform = true;
	}

	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform() const { return m_WorldTransform; }


	// En el Círculo Unitario, +y es arriba.
	// Para SDL +y es abajo.
	// Tenemos que negar el valor de y
	Vector2 GetForward() const{
		return Vector2(CMath::Cos(m_Rotation), CMath::Sin(m_Rotation));
	}
	
	State GetState() const { return m_State; }
	void SetState(State state) { m_State = state; }
	
	class Game* GetGame() { return m_Game; }

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);


private:
	State m_State;
	
	//Coordenadas Homogeneas de la posicion 3D son (x,y,z,w)
	Matrix4 m_WorldTransform;

	// Transform
	Vector2 m_Position;
	float m_Scale;
	float m_Rotation;
	bool m_RecomputeWorldTransform;


	std::vector<class Component*> m_Components;
	class Game* m_Game;
};