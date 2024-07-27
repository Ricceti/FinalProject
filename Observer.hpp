#pragma once

class Observer {
public:
    virtual void update(int player1Health, int player2Health) = 0;
    virtual ~Observer() = default;
};

class Subject {
private:
    std::vector<Observer*> observers;
public:
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }
    void notifyObservers(int player1Health, int player2Health) {
        for (auto& observer : observers) {
            observer->update(player1Health, player2Health);
        }
    }
};  
