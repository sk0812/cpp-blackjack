#include "deck.h"
#include <random>

std::vector<Card> make_deck(int num_decks = 1) {
    std::vector<Card> deck;
    deck.reserve(52 * num_decks);

    for (int d = 0; d < num_decks; ++d) {
        for (int s = 0; s < 4; ++s){
            for (int r = 2; r <= 14; ++r) {
                Rank rank;
                switch (r){
                    case 11: rank = Rank::Jack; break;
                    case 12: rank = Rank::Queen; break;
                    case 13: rank = Rank::King; break;
                    case 14: rank = Rank::Ace; break;
                    default: rank = static_cast<Rank>(r);
                }
                deck.push_back({rank, static_cast<Suit>(s)});
            }
        }
    }   
    return deck;
}


void shuffle_deck(std::vector<Card>& deck) {
    std::mt19937 rng(std::random_device{}());
    std::shuffle(deck.begin(), deck.end(), rng);
}
