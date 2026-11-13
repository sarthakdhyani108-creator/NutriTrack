#pragma once
#include <string>
#include <vector>

struct Nutrition {
    double calories=0, protein=0, carbs=0, fat=0, fiber=0, calcium=0, iron=0, vitaminC=0, sodium=0, potassium=0;
    Nutrition& operator+=(const Nutrition& n) {
        calories+=n.calories; protein+=n.protein; carbs+=n.carbs; fat+=n.fat; fiber+=n.fiber;
        calcium+=n.calcium; iron+=n.iron; vitaminC+=n.vitaminC; sodium+=n.sodium; potassium+=n.potassium;
        return *this;
    }
};

struct Food {
    std::string name, category;
    Nutrition per100g;
};

struct FoodEntry {
    Food food;
    double grams=0;
    Nutrition nutrition;
};

struct Meal {
    std::string name;
    std::vector<FoodEntry> foods;
};

struct User {
    std::string name, gender, activity, goal;
    int age=0, mealCount=0;
    double heightCm=0, weightKg=0;
};

struct Targets {
    double bmi=0, bmr=0, tdee=0;
    Nutrition nutrition;
};

struct DailyRecord {
    std::string date;
    std::vector<Meal> meals;
    Nutrition total;
    Targets targets;
};
