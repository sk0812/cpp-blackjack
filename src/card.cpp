#include "card.h"

std::string to_string(const Card& card) {
    std::string rank_str;
    switch(card.r) {
        case Rank::Jack: rank_str = "J"; break;
        case Rank::Queen: rank_str = "Q"; break;
        case Rank::King: rank_str = "K"; break;
        case Rank::Ace: rank_str = "A"; break;
        default: rank_str = std::to_string(static_cast<int>(card.r));
    }

    std::string suit_str;
    switch (card.s) {
        case Suit::Clubs: suit_str = "♣"; break;
        case Suit::Diamonds: suit_str = "♦"; break;
        case Suit::Hearts: suit_str = "♥"; break;
        case Suit::Spades: suit_str = "♠"; break;
    }

    return rank_str + suit_str;
}

int card_value(const Card& card) {
    switch (card.r) {
        case Rank::Jack:
        case Rank::Queen:
        case Rank::King:
            return 10;
        case Rank::Ace:
            return 11;
        default:
            return static_cast<int>(card.r);
    }
}

Card deal(std::vector<Card>& deck) {
    Card c {deck.back()};
    deck.pop_back();

    return c;
}
