#pragma once
#include "Models.h"
class NutritionCalculator {
public:
    static double bmi(const User&);
    static std::string bmiCategory(double);
    static double bmr(const User&);
    static double activityMultiplier(const std::string&);
    static double tdee(const User&);
    static Targets calculateTargets(const User&);
    static Nutrition scale(const Food&, double grams);
    static Nutrition total(const std::vector<Meal>&);
};
