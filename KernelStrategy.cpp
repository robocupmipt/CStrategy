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

bool KernelStrategy::Pause() {
    const bool answer = !is_paused_.load();
    is_paused_.store(true);
    return answer;
}

bool KernelStrategy::Start() {
    const bool answer = threads_.empty();
    const ProAngle horizontal(-60, 60, 10);
    const ProAngle vertical(-20, 20, 5);

    auto robot_routine = [&]() {
        while (true) {
            if (is_paused_.load()) {
                break;
            }
            if (is_terminated_.load()) {
                break;
            }

            for (double h_iter = horizontal.left_; h_iter < horizontal.right_;) {
                //head rotate in vertical direction
                for (double v_iter = vertical.left_; v_iter < vertical.right_;) {
                    //head rotate in horizontal direction
                    v_iter += vertical.step_;
                }
                h_iter += horizontal.step_;
            }

        }
    };
    threads_.emplace_back(robot_routine);
    return answer;
}

bool KernelStrategy::Terminate() {
    const bool answer = !is_terminated_.load();
    is_terminated_.store(true);
    return answer;
}
