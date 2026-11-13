#include "UI.h"
#include "NutritionCalculator.h"
#include "History.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <chrono>
#include <ctime>
#include <sstream>
static void clearInput(){std::cin.clear();std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');}
static std::string today(){
    auto t=std::time(nullptr); std::tm tm=*std::localtime(&t); char b[20]; std::strftime(b,sizeof(b),"%Y-%m-%d",&tm); return b;
}
static void line(){std::cout<<"============================================================\n";}
static void progress(const std::string& name,double x,double target,const std::string& unit){
    double pct=target>0?x/target*100:0; int bars=(int)std::min(20.0,std::max(0.0,pct/5));
    std::cout<<std::left<<std::setw(14)<<name<<std::right<<std::fixed<<std::setprecision(1)<<x<<" / "<<target<<" "<<unit<<" [";
    for(int i=0;i<20;i++)std::cout<<(i<bars?'#':'-'); std::cout<<"] "<<pct<<"%\n";
}
void UI::createProfile(){
    clearInput(); std::cout<<"Name: ";std::getline(std::cin,user.name);
    std::cout<<"Age: ";std::cin>>user.age;
    std::cout<<"Gender (Male/Female): ";std::cin>>user.gender;
    std::cout<<"Height (cm): ";std::cin>>user.heightCm;
    std::cout<<"Weight (kg): ";std::cin>>user.weightKg;
    int a,g; std::cout<<"Activity: 1.Sedentary 2.Light 3.Moderate 4.Very Active\nChoose: ";std::cin>>a;
    user.activity=(a==1?"Sedentary":a==2?"Lightly Active":a==3?"Moderately Active":"Very Active");
    std::cout<<"Goal: 1.Fat Loss 2.Maintain 3.Muscle Gain\nChoose: ";std::cin>>g;
    user.goal=(g==1?"Fat Loss":g==2?"Maintain":"Muscle Gain");
    std::cout<<"Number of meals (3-6): ";std::cin>>user.mealCount;
    if(user.mealCount<1)user.mealCount=3;
    record.date=today(); record.targets=NutritionCalculator::calculateTargets(user); setMeals(); profileReady=true;
    std::cout<<"\nProfile created successfully.\n";
}
void UI::setMeals(){
    record.meals.clear(); std::vector<std::string> defaults={"Breakfast","Lunch","Snack","Dinner","Meal 5","Meal 6"};
    clearInput();
    for(int i=0;i<user.mealCount;i++){Meal m;m.name=defaults[i]; std::string custom;
        std::cout<<"Meal "<<i+1<<" name (Enter for "<<m.name<<"): ";std::getline(std::cin,custom); if(!custom.empty())m.name=custom; record.meals.push_back(m);}
}
void UI::showTargets() const{
    line(); std::cout<<"PROFILE: "<<user.name<<" | Goal: "<<user.goal<<"\nBMI: "<<record.targets.bmi<<" ("<<NutritionCalculator::bmiCategory(record.targets.bmi)<<")\n";
    std::cout<<"BMR: "<<record.targets.bmr<<" kcal | TDEE: "<<record.targets.tdee<<" kcal\n";
    progress("Calories",0,record.targets.nutrition.calories,"kcal");progress("Protein",0,record.targets.nutrition.protein,"g");
    progress("Carbs",0,record.targets.nutrition.carbs,"g");progress("Fat",0,record.targets.nutrition.fat,"g");line();
}
void UI::addFood(){
    if(!profileReady){std::cout<<"Create profile first.\n";return;}
    for(size_t i=0;i<record.meals.size();i++)std::cout<<i+1<<". "<<record.meals[i].name<<"\n";
    int mi;std::cout<<"Select meal: ";std::cin>>mi;if(mi<1||mi>(int)record.meals.size())return;
    clearInput();std::string q;std::cout<<"Search food: ";std::getline(std::cin,q);auto results=db.search(q);
    if(results.empty()){std::cout<<"No food found.\n";return;}
    for(size_t i=0;i<results.size();i++)std::cout<<i+1<<". "<<results[i].name<<" ("<<results[i].per100g.calories<<" kcal/100g)\n";
    int fi;std::cout<<"Choose food: ";std::cin>>fi;if(fi<1||fi>(int)results.size())return;
    double grams;std::cout<<"Quantity in grams: ";std::cin>>grams;if(grams<=0)return;
    FoodEntry e{results[fi-1],grams,NutritionCalculator::scale(results[fi-1],grams)};
    record.meals[mi-1].foods.push_back(e);record.total=NutritionCalculator::total(record.meals);
    std::cout<<"Added "<<e.food.name<<" | "<<e.nutrition.calories<<" kcal, "<<e.nutrition.protein<<"g protein\n";
}
void UI::removeFood(){
    if(!profileReady)return; for(size_t i=0;i<record.meals.size();i++)std::cout<<i+1<<". "<<record.meals[i].name<<"\n";
    int m;std::cin>>m;if(m<1||m>(int)record.meals.size())return;auto& meal=record.meals[m-1];
    if(meal.foods.empty()){std::cout<<"Meal is empty.\n";return;}for(size_t i=0;i<meal.foods.size();i++)std::cout<<i+1<<". "<<meal.foods[i].food.name<<" "<<meal.foods[i].grams<<"g\n";
    int f;std::cout<<"Remove which: ";std::cin>>f;if(f<1||f>(int)meal.foods.size())return;meal.foods.erase(meal.foods.begin()+f-1);record.total=NutritionCalculator::total(record.meals);
}
void UI::dashboard(){
    if(!profileReady)return;record.total=NutritionCalculator::total(record.meals);auto n=record.total,t=record.targets.nutrition;line();std::cout<<"TODAY'S NUTRITION - "<<record.date<<"\n";
    progress("Calories",n.calories,t.calories,"kcal");progress("Protein",n.protein,t.protein,"g");progress("Carbs",n.carbs,t.carbs,"g");progress("Fat",n.fat,t.fat,"g");
    progress("Fiber",n.fiber,t.fiber,"g");progress("Calcium",n.calcium,t.calcium,"mg");progress("Iron",n.iron,t.iron,"mg");progress("Vitamin C",n.vitaminC,t.vitaminC,"mg");
    std::cout<<"Remaining calories: "<<t.calories-n.calories<<"\nRemaining protein: "<<t.protein-n.protein<<" g\n";line();
}
void UI::recommendations() const{
    if(!profileReady)return;double remain=record.targets.nutrition.calories-record.total.calories;
    std::cout<<"Suggested foods for "<<user.goal<<" (about "<<std::max(0.0,remain)<<" kcal remaining):\n";
    auto all=db.all();int shown=0;
    for(auto& f:all){bool ok=false;
        if(user.goal=="Muscle Gain")ok=f.per100g.protein>=10||f.category=="Carbohydrate";
        else if(user.goal=="Fat Loss")ok=f.per100g.protein>=8||f.category=="Vegetable"||f.category=="Fruit";
        else ok=f.category=="Protein"||f.category=="Fruit"||f.category=="Vegetable";
        if(ok){std::cout<<"- "<<f.name<<" ["<<f.category<<"] "<<f.per100g.calories<<" kcal/100g\n";if(++shown==10)break;}
    }
}
void UI::showFoodDatabase() const{int i=1;for(auto&f:db.all())std::cout<<i++<<". "<<f.name<<" | "<<f.category<<" | "<<f.per100g.calories<<" kcal/100g | P "<<f.per100g.protein<<"g\n";}
void UI::saveToday(){if(!profileReady)return;record.total=NutritionCalculator::total(record.meals);std::cout<<(History::save(record,"history")?"Saved successfully.\n":"Save failed.\n");}
void UI::run(){
    if(!db.load("data/foods.csv")){std::cout<<"ERROR: data/foods.csv could not be loaded.\n";return;}
    while(true){line();std::cout<<"NUTRITRACK - C++ DIET TRACKER\n1. Create Profile\n2. View BMI, BMR, TDEE & Targets\n3. Add Food to Meal\n4. Remove Food from Meal\n5. View Today's Dashboard\n6. Food Recommendations\n7. View Food Database\n8. Save Today's Record\n9. View History\n0. Exit\nChoose: ";int c;if(!(std::cin>>c)){clearInput();continue;}
        switch(c){case 1:createProfile();break;case 2:if(profileReady)showTargets();else std::cout<<"Create profile first.\n";break;case 3:addFood();break;case 4:removeFood();break;case 5:dashboard();break;case 6:recommendations();break;case 7:showFoodDatabase();break;case 8:saveToday();break;case 9:History::list("history");break;case 0:saveToday();std::cout<<"Goodbye!\n";return;default:std::cout<<"Invalid choice.\n";}}
}
