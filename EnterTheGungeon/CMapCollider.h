#pragma once

class CObject;

class CMapCollider
{
public:
	CMapCollider() {}
	CMapCollider(const RECT& rect);
	virtual ~CMapCollider();

public :
	void Initialize();
	void Render(HDC hDC); // Only used for debugging
	void Release();

public : 
	const RECT& Get_Collider() { return rBound; }
	void Set_Collider(const RECT& rBound) {
		this->rBound.left = rBound.left;
		this->rBound.top = rBound.top;
		this->rBound.right = rBound.right;
		this->rBound.bottom = rBound.bottom;

		vPosition = { (rBound.right - rBound.left) * 0.5f + rBound.left, (rBound.bottom - rBound.top) * 0.5f + rBound.top };
	}

private :
	RECT rBound;
	Vector2 vPosition;

public :
	Vector2 Get_Position() { return vPosition; }
	void Add_OnCollision(function<void(CObject*, Vector2)> listener) { onCollision = listener; }
	function<void(CObject*, Vector2)>* Get_OnCollision() { return &onCollision; }

private :
	function<void(CObject*, Vector2)> onCollision;
};

