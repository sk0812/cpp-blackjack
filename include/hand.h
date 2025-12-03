#pragma once
#include <vector>
#include "card.h"

using Hand = std::vector<Card>;

int hand_value(const Hand& hand);
void hit(std::vector<Card>& deck, Hand& hand);