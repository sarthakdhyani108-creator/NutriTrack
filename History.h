#pragma once
#include "Models.h"
#include <string>
class History {
public: 
    static bool save(const DailyRecord&, const std::string& folder);
    static void list(const std::string& folder);
};
