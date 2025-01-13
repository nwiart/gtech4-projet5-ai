#pragma once

#include <iostream>
#include <functional>

class StateAction {
public:
    virtual ~StateAction() = default;
    virtual void start() = 0;
    virtual void update(float deltaTime) = 0;
};

class IdleState : public StateAction {
public:
    void start() override;
    void update(float deltaTime) override;
};

class ShootingState : public StateAction {
private:
    int& mAmmo;
    float& mShootProgress;
    float mShootTime;
    std::function<void()> mOnComplete;

public:
    ShootingState(int& ammo, float& shootProgress, float shootTime, std::function<void()> onComplete);
    void start() override;
    void update(float deltaTime) override;
};

class ReloadingState : public StateAction {
private:
    int& mAmmo;
    int mCapacity;
    float& mReloadProgress;
    float mReloadTime;
    std::function<void()> mOnComplete;

public:
    ReloadingState(int& ammo, int capacity, float& reloadProgress, float reloadTime, std::function<void()> onComplete);
    void start() override;
    void update(float deltaTime) override;
};

class EmptyState : public StateAction {
public:
    void start() override;
    void update(float deltaTime) override;
};