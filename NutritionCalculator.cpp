#include "FoodDatabase.h"
#include <algorithm>
#include "NutritionCalculator.h"
double NutritionCalculator::bmi(const User& u) {
    double h=u.heightCm/100.0; return h>0 ? u.weightKg/(h*h) : 0;
}
std::string NutritionCalculator::bmiCategory(double x) {
    if(x<18.5) return "Underweight";
    if(x<25) return "Normal Weight";
    if(x<30) return "Overweight";
    return "Obese";
}
double NutritionCalculator::bmr(const User& u) {
    double base=10*u.weightKg+6.25*u.heightCm-5*u.age;
    std::string g=u.gender; std::transform(g.begin(),g.end(),g.begin(),::tolower);
    return base+(g=="female"||g=="f"?-161:5);
}
double NutritionCalculator::activityMultiplier(const std::string& a) {
    if(a=="Sedentary") return 1.2;
    if(a=="Lightly Active") return 1.375;
    if(a=="Moderately Active") return 1.55;
    return 1.725;
}
double NutritionCalculator::tdee(const User& u) { return bmr(u)*activityMultiplier(u.activity); }
Targets NutritionCalculator::calculateTargets(const User& u) {
    Targets t; t.bmi=bmi(u); t.bmr=bmr(u); t.tdee=tdee(u);
    double c=t.tdee;
    if(u.goal=="Fat Loss") c-=500;
    else if(u.goal=="Muscle Gain") c+=300;
    c=std::max(1200.0,c);
    t.nutrition.calories=c;
    t.nutrition.protein=u.weightKg*(u.goal=="Muscle Gain"?2.0:(u.goal=="Fat Loss"?1.8:1.6));
    t.nutrition.fat=u.weightKg*0.8;
    t.nutrition.carbs=std::max(0.0,(c-t.nutrition.protein*4-t.nutrition.fat*9)/4);
    t.nutrition.fiber=30; t.nutrition.calcium=1000; t.nutrition.iron=18; t.nutrition.vitaminC=75;
    t.nutrition.sodium=2300; t.nutrition.potassium=3500;
    return t;
}
Nutrition NutritionCalculator::scale(const Food& f,double grams) {
    Nutrition n=f.per100g; double k=grams/100.0;
    n.calories*=k;n.protein*=k;n.carbs*=k;n.fat*=k;n.fiber*=k;n.calcium*=k;n.iron*=k;n.vitaminC*=k;n.sodium*=k;n.potassium*=k;
    return n;
}
Nutrition NutritionCalculator::total(const std::vector<Meal>& meals) {
    Nutrition t;
    for(const auto& m:meals) for(const auto& e:m.foods) t+=e.nutrition;
    return t;
}
