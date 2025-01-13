#include "StateAction.h"

void IdleState::start() {
    std::cout << "Entrée dans l'état Idle." << std::endl;
}

void IdleState::update(float deltaTime) {
}

ShootingState::ShootingState(int& ammo, float& shootProgress, float shootTime, std::function<void()> onComplete)
    : mAmmo(ammo), mShootProgress(shootProgress), mShootTime(shootTime), mOnComplete(onComplete) {}

void ShootingState::start() {
    if (mAmmo > 0) {
        mAmmo--;
        std::cout << "Bang! Munitions restantes : " << mAmmo << std::endl;
    }
    else {
        std::cout << "Pas de munitions." << std::endl;
    }
}

void ShootingState::update(float deltaTime) {
    mShootProgress += deltaTime;
    if (mShootProgress >= mShootTime) {
        mShootProgress = 0;
        mOnComplete();
    }
}

ReloadingState::ReloadingState(int& ammo, int capacity, float& reloadProgress, float reloadTime, std::function<void()> onComplete)
    : mAmmo(ammo), mCapacity(capacity), mReloadProgress(reloadProgress), mReloadTime(reloadTime), mOnComplete(onComplete) {}

void ReloadingState::start() {
    std::cout << "Rechargement en cours..." << std::endl;
}

void ReloadingState::update(float deltaTime) {
    mReloadProgress += deltaTime;
    if (mReloadProgress >= mReloadTime) {
        mReloadProgress = 0;
        mAmmo = mCapacity;
        std::cout << "Rechargement terminé." << std::endl;
        mOnComplete();
    }
}

void EmptyState::start() {
    std::cout << "Munitions vides, rechargez !" << std::endl;
}

void EmptyState::update(float deltaTime) {
}