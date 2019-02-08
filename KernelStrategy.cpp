//
// Created by nikita on 08.02.19.
//

#include "KernelStrategy.hpp"


strategy::Robot KernelStrategy::GetLeader() {
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

void KernelStrategy::UpdatePositions() {
    auto tmp_positions = localization.GetCurrentPositions();
    for (auto& current : robots_) {
        auto iter = current.number_;
        current.position_ = tmp_positions[iter];
    }
}
