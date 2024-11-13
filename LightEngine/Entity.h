#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace sf 
{
	class Shape;
    class Color;
}

class Scene;

class Entity
{
    struct Target 
    {
		sf::Vector2f position;
        float distance;
		bool isSet;
    };

protected:
    sf::CircleShape mShape;
    sf::Vector2f mDirection;
	Target mTarget;
    float mSpeed;
    bool mToDestroy;
    int mTag;

public:
	bool GoToDirection(float x, float y, float speed = -1.f);
    bool GoToPosition(float x, float y, float speed = -1.f);
    void SetPosition(float x, float y, float ratioX = 0.f, float ratioY = 0.f);
	void SetDirection(float x, float y, float speed = -1.f);
	void SetSpeed(float speed) { mSpeed = speed; }
	void SetTag(int tag) { mTag = tag; }

    sf::Vector2f GetPosition(float ratioX = 0.f, float ratioY = 0.f) const;
	sf::Shape* GetShape() { return &mShape; }

	bool IsTag(int tag) const { return mTag == tag; }
    bool IsColliding(Entity* other) const;
	bool IsInside(float x, float y) const;

	void Destroy() { mToDestroy = true; }
	bool ToDestroy() const { return mToDestroy; }
	
	template<typename T>
	T* GetScene() const;

    Scene* GetScene() const;

protected:
    virtual ~Entity() {};
    Entity(float radius, const sf::Color& color);

    virtual void OnUpdate() {};
    virtual void OnCollision(Entity* collidedWith) {};
	
private:
    void Update();


    friend class GameManager;
};

#include "Entity.inl"