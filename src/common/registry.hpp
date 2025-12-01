// registry.hpp
#pragma once

#include "solution.hpp"
#include <map>
#include <memory>
#include <functional>

namespace aoc {

class Registry {
public:
    using Factory = std::function<std::unique_ptr<Solution>()>;
    
    static inline Registry& instance() {
        static Registry reg;
        return reg;
    }
    
    inline void add(int year, int day, Factory factory) {
        solutions_[{year, day}] = std::move(factory);
    }
    
    inline std::unique_ptr<Solution> get(int year, int day) {
        auto it = solutions_.find({year, day});
        if (it == solutions_.end()) return nullptr;
        return it->second();
    }
    
    inline std::vector<std::pair<int, int>> list() const {
        std::vector<std::pair<int, int>> result;
        for (const auto& [key, _] : solutions_) {
            result.push_back(key);
        }
        return result;
    }
    
private:
    std::map<std::pair<int, int>, Factory> solutions_;
};

#define REGISTER_SOLUTION(cls) \
    namespace { \
        static bool cls##_registered = []() { \
            aoc::Registry::instance().add( \
                cls().year(), cls().day(), \
                []() { return std::make_unique<cls>(); }); \
            return true; \
        }(); \
    }

} // namespace aoc