#pragma once

#include <iostream>
#include <functional>

class PlayerEntity;
class BallEntity;

class StateAction {
public:
    virtual ~StateAction() = default;
    virtual void start(PlayerEntity& player, BallEntity& ball) = 0;
    virtual void update(PlayerEntity& player, BallEntity& ball, float deltaTime) = 0;
};

class PossessionState : public StateAction {
public:
    void start(PlayerEntity& player, BallEntity& ball) override;
    void update(PlayerEntity& player, BallEntity& ball, float deltaTime) override;

private:
    void avoidDefenders(PlayerEntity& player, float deltaTime);
};

class TeammateState : public StateAction {
public:
    void start(PlayerEntity& player, BallEntity& ball) override;
    void update(PlayerEntity& player, BallEntity& ball, float deltaTime) override;

private:
    void findOpenSpace(PlayerEntity& player, BallEntity& ball, float deltaTime);
};

class OpponentState : public StateAction {
public:
    void start(PlayerEntity& player, BallEntity& ball) override;
    void update(PlayerEntity& player, BallEntity& ball, float deltaTime) override;

private:
    void positionStrategically(PlayerEntity& player, BallEntity& ball, float deltaTime);
    void markPlayers(PlayerEntity& player, float deltaTime);
};