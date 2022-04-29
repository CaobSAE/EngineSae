#pragma once

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	virtual void Update(float deltaTime);

	int GetUpdateOrder() const { return m_UpdateOrder; }
protected:
	class Actor* m_Owner;
	int m_UpdateOrder;
};