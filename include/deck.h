#pragma once
#include <vector>
#include "card.h"

std::vector<Card> make_deck(int num_decks);
void shuffle_deck(std::vector<Card>& deck);