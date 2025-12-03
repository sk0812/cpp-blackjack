#include "hand.h"

int hand_value(const Hand& hand) {
    int ace_count {0};
    int total {0};
    for (const auto& c : hand) {
        total += card_value(c);
        if (c.r == Rank::Ace) ++ace_count;
    }

    while (total > 21 && ace_count > 0) {
        total-=10;
        --ace_count;
    } 

    return total;
}

void hit(std::vector<Card>& deck, Hand& hand) {
    hand.push_back(deal(deck));
}