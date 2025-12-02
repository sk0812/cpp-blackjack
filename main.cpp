#include <iostream>
#include <vector>
#include <random>

enum class Suit {
    Diamonds,
    Hearts,
    Clubs,
    Spades,
};

enum class Rank {
    Two = 2,
    Three,
    Four,
    Five, 
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack = 11,
    Queen = 12,
    King = 13,
    Ace = 14
};

struct Card{Rank r; Suit s;};

using Hand = std::vector<Card>;

std::vector<Card> make_deck() {
    std::vector<Card> deck;
    deck.reserve(52);

    for (int s = 0; s < 4; s++){
        for (int r = 2; r <= 14; r++) {
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

    return deck;
}


void shuffle_deck(std::vector<Card>& deck) {
    std::mt19937 rng(std::random_device{}());
    std::shuffle(deck.begin(), deck.end(), rng);
}


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


Card deal(std::vector<Card>& deck) {
    Card c {deck.back()};
    deck.pop_back();

    return c;
}

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

void print_hand_value(Hand& hand) {
    int total {hand_value(hand)};
    std::cout << "Hand Value: " << total << "\n";
}

void hit(std::vector<Card>& deck, Hand& hand) {
    hand.push_back(deal(deck));
}


int main() {
    std::cout << "\n\n====================Welcome to CLI Blackjack=====================\n\n";
    std::vector<Card> deck {make_deck()};
    shuffle_deck(deck);

    while (true) {
        char play;
        std::cout << "\nDo you want to play? (Y/N): ";
        std::cin >> play;
        play = std::toupper(static_cast<unsigned char>(play));

        if (play == 'N') break;
        if (play != 'Y') continue;

        Hand player, dealer;
        player.push_back(deal(deck));
        dealer.push_back(deal(deck));
        player.push_back(deal(deck));
        dealer.push_back(deal(deck));

        print_player(player);
        print_hand_value(player);
        print_dealer(dealer, true);

        bool player_blackjack = (hand_value(player) == 21);
        bool dealer_blackjack = (hand_value(dealer) == 21);
        
        if (player_blackjack || dealer_blackjack) {
            print_dealer(dealer, false);
            print_hand_value(dealer);

            if (player_blackjack && dealer_blackjack)
                std::cout << "Both have blackjack — Push!\n";
            else if (player_blackjack)
                std::cout << "Player has blackjack!\n";
            else
                std::cout << "Dealer has blackjack!\n";

            continue; 
        }
        
        bool round_over = false;

        while (!round_over) {
            char decision;
            std::cout << "\nHit or Stand (H/S): ";
            std::cin >> decision;
            decision = std::toupper(static_cast<unsigned char>(decision));

            if (decision == 'H') {
                hit(deck, player);
                print_player(player);
                print_hand_value(player);
                if (hand_value(player) > 21) {
                    std::cout << "Player bust. Dealer wins!\n";
                    round_over = true;
                }
            } else if (decision == 'S') {
                break;
            }
        }

        if (round_over) continue; 

        
        print_dealer(dealer, false);
        print_hand_value(dealer);

        while (hand_value(dealer) < 17) {
            hit(deck, dealer);
            print_dealer(dealer, false);
            print_hand_value(dealer);
            if (hand_value(dealer) > 21) {
                std::cout << "Dealer bust. Player wins!\n";
                round_over = true;
                break;
            }
        }

        if (round_over) continue;

        
        int p = hand_value(player);
        int d = hand_value(dealer);
        if (p > d) std::cout << "Player wins!\n";
        else if (d > p) std::cout << "Dealer wins!\n";
        else std::cout << "Push (tie)!\n";
    }

    return 0;
}
