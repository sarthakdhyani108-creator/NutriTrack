#include "History.h"
#include <fstream>
#include <filesystem>
#include <iostream>
bool History::save(const DailyRecord& r,const std::string& folder){
    std::filesystem::create_directories(folder);
    std::ofstream o(folder+"/"+r.date+".txt"); 
    if(!o){
     return false;
    }
    o<<"NutriTrack Daily Record\nDate: "<<r.date
     <<"\nCalories: "<<r.total.calories
     <<" / "<<r.targets.nutrition.calories
     <<"\nProtein: "<<r.total.protein
     <<" / "<<r.targets.nutrition.protein
     <<"\nCarbs: "<<r.total.carbs
     <<" / "<<r.targets.nutrition.carbs
     <<"\nFat: "<<r.total.fat
     <<" / "<<r.targets.nutrition.fat<<"\n\n";
    for(auto& m : r.meals){
        o<<m.name<<":\n";
        for(auto& e : m.foods)
           o<<"  "<<e.food.name
            <<" - "<<e.grams
            <<"g - "
            <<e.nutrition.calories
            <<" kcal\n";
    }
    return true;
}
void History::list(const std::string& folder){
    namespace fs=std::filesystem;
     if(!fs::exists(folder)){
      std::cout<<"No history yet.\n";
      return;
    }
    bool any=false; 
    for(auto& p:fs::directory_iterator(folder))if(p.is_regular_file()){
        any=true;
        std::cout<<"\n--- "<<p.path().filename().string()<<" ---\n";
        std::ifstream in(p.path());
        std::cout<<in.rdbuf()<<"\n";
    }
    if(!any) std::cout<<"No history yet.\n";
}
