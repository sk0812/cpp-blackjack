#include <iostream>
#include "ui.h"


void print_player(const Hand& h) {
    std::cout << "Player: ";
    for (const auto& c : h) std::cout << to_string(c) << " ";
    std::cout << '\n';
}

void print_dealer(const Hand& h, bool hide_hole) {
    std::cout << "Dealer: ";
    for (std::size_t i = 0; i < h.size(); ++i) {
        if (hide_hole && i == 1) {
            std::cout << "?? ";
        } else {
            std::cout << to_string(h[i]) << ' ';
        }
    }
    std::cout << "\n";
}

void print_hand_value(const Hand& hand) {
    int total {hand_value(hand)};
    std::cout << "Hand Value: " << total << "\n";
}

