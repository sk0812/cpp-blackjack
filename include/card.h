#pragma once
#include <string>
#include <vector>

enum class Suit {
    Diamonds,
    Hearts,
    Clubs,
    Spades,
};

enum class Rank { Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };


struct Card{Rank r; Suit s;};

std::string to_string(const Card& card);
int card_value(const Card& card);
Card deal(std::vector<Card>& deck);