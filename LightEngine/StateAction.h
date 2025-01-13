#pragma once

#include <iostream>
#include <functional>

class StateAction {
public:
    virtual ~StateAction() = default;
    virtual void start(Player& player, Ball& ball) = 0;
    virtual void update(Player& player, Ball& ball, float deltaTime) = 0;
};

class PossessionState : public StateAction {
public:
    void start(Player& player, Ball& ball) override;
    void update(Player& player, Ball& ball, float deltaTime) override;

private:
    void avoidDefenders(Player& player, float deltaTime);
};

class TeammateState : public StateAction {
public:
    void start(Player& player, Ball& ball) override;
    void update(Player& player, Ball& ball, float deltaTime) override;

private:
    void findOpenSpace(Player& player, Ball& ball, float deltaTime);
};

class OpponentState : public StateAction {
public:
    void start(Player& player, Ball& ball) override;
    void update(Player& player, Ball& ball, float deltaTime) override;

private:
    void positionStrategically(Player& player, Ball& ball, float deltaTime);
    void markPlayers(Player& player, float deltaTime);
};