//
// Created by nikita on 08.02.19.
//

#include "KernelStrategy.hpp"


strategy::Robot KernelStrategy::GetLeader(const std::vector<strategy::Robot> &robots_) {
    strategy::Robot best{};
    for (auto& current : robots_) {
        if (current.position_.y > best.position_.y ) {
            best = current;
        }
    }
    return best;
}

strategy::Command KernelStrategy::WhatToDo() {
    return {};
}
