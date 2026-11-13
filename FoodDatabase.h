#pragma once
#include "Models.h"
#include <vector>
#include <unordered_map>
class FoodDatabase {
    std::vector<Food> foods;
    std::unordered_map<std::string,size_t> index;
public:
    bool load(const std::string& path);
    bool save(const std::string& path) const;
    std::vector<Food> search(const std::string&) const;
    const Food* findExact(const std::string&) const;
    const std::vector<Food>& all() const { return foods; }
};
