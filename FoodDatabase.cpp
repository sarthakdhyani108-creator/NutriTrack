#include "FoodDatabase.h"
#include <fstream>
#include <sstream>
#include <algorithm>
static std::string lower(std::string s){std::transform(s.begin(),s.end(),s.begin(),::tolower);return s;}
bool FoodDatabase::load(const std::string& path) {
    foods.clear(); index.clear(); std::ifstream in(path); if(!in) return false;
    std::string line; std::getline(in,line);
    while(std::getline(in,line)) {
        std::stringstream ss(line); std::string x; std::vector<std::string> v;
        while(std::getline(ss,x,',')) v.push_back(x);
        if(v.size()<12) continue;
        try {
            Food f; f.name=v[0]; f.category=v[1];
            double* p[]={&f.per100g.calories,&f.per100g.protein,&f.per100g.carbs,&f.per100g.fat,&f.per100g.fiber,&f.per100g.calcium,&f.per100g.iron,&f.per100g.vitaminC,&f.per100g.sodium,&f.per100g.potassium};
            for(int i=0;i<10;i++) *p[i]=std::stod(v[i+2]);
            index[lower(f.name)]=foods.size(); foods.push_back(f);
        } catch(...) {}
    } return !foods.empty();
}
bool FoodDatabase::save(const std::string& path) const {
    std::ofstream o(path); if(!o) return false;
    o<<"Name,Category,Calories,Protein,Carbs,Fat,Fiber,Calcium,Iron,VitaminC,Sodium,Potassium\n";
    for(auto& f:foods) { auto n=f.per100g;
        o<<f.name<<","<<f.category<<","<<n.calories<<","<<n.protein<<","<<n.carbs<<","<<n.fat<<","<<n.fiber<<","<<n.calcium<<","<<n.iron<<","<<n.vitaminC<<","
         <<n.sodium<<","<<n.potassium<<"\n";
    } return true;
}
std::vector<Food> FoodDatabase::search(const std::string& q) const {
    std::vector<Food> r; auto needle=lower(q);
    for(auto& f:foods) if(lower(f.name).find(needle)!=std::string::npos) r.push_back(f);
    return r;
}
const Food* FoodDatabase::findExact(const std::string& q) const {
    auto it=index.find(lower(q)); return it==index.end()?nullptr:&foods[it->second];
}
