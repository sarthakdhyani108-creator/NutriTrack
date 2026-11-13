#pragma once
#include "Models.h"
#include "FoodDatabase.h"
class UI {
    FoodDatabase db;
    User user;
    DailyRecord record;
    bool profileReady=false;
    void createProfile();
    void showTargets() const;
    void setMeals();
    void addFood();
    void removeFood();
    void dashboard();
    void recommendations() const;
    void showFoodDatabase() const;
    void saveToday();
public:
    void run();
};
