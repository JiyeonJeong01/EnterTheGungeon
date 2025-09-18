#pragma once

class CTransform;
class CCollider;
class CRenderer;

class CObject
{
public:
	CObject();
	virtual ~CObject();

public:
	virtual void Initialize();
	virtual int Update() = 0;
	virtual void Late_Update();
	virtual void Render(HDC _hDC) = 0;
	virtual void Release() = 0;

public: // Components
	virtual void Update_Transform() = 0;
	virtual void Update_Renderer();
	virtual void Update_Collider();

public:
	CTransform* Get_Transform() { return pTransform; }
	CCollider* Get_Collider() { return pCollider; }
	CRenderer* Get_Renderer() { return pRenderer; }

protected:
	CTransform* pTransform;
	CCollider* pCollider;
	CRenderer* pRenderer;

protected:
	bool bAlive;
};

