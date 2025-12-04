#include <iostream>
#include <vector>
#include <limits>
#include "card.h"
#include "hand.h"
#include "ui.h"
#include "deck.h"


int get_valid_bet(const int& MIN_BET, const int& MAX_BET, int& bankroll ) {
        int bet;
        double temp;
        while (true) {
            std::cout << "Place bet: ";
            std::cin >> temp;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            if (temp!= static_cast<int>(temp)) {
                std::cout << "Enter whole number.\n";
                continue;
            }

            bet = static_cast<int>(temp);

            if (bet < MIN_BET) {
                std::cout << "Min Bet is £" << MIN_BET << '\n';
                continue;
            } else if (bet > MAX_BET) {
                std::cout << "Max Bet is £" << MAX_BET << '\n';
                continue;
            } else if (bet > bankroll) {
                std::cout << "Bankroll is only £" << bankroll << '\n';
                continue;
            } else if (bet % 10 != 0) {
                std::cout << "Bet needs to be a multiple of 10!\n";
                continue;
            } else {
                bankroll-=bet;
                break;
            }
        }

        return bet;
}

int main() {
    
    const int NUM_DECKS {6};
    const double RESHUFFLE_THRESHOLD {0.25};
    constexpr int CARDS_PER_DECK {52};
    const int MIN_BET{20};
    const int MAX_BET{500};
    int bankroll{1000};
    
    std::cout << "\n\n====================Welcome to CLI Blackjack=====================\n\n";
    std::cout << "- Blackjack payout 3:2\n";
    std::cout << "- Dealer stands on soft 17\n";
    std::cout << "- Table Minimum: £" << MIN_BET << '\n';
    std::cout << "- Table Maximum: £" << MAX_BET << '\n';
    std::cout << "- Starting Balance: £" << bankroll << "\n\n";


    std::vector<Card> deck {make_deck(NUM_DECKS)};
    shuffle_deck(deck);
    int player_wins {0};
    int played {0};
    while (true && bankroll>0) {
        std::cout << "Player wins: " << player_wins << " out of " << played << '\n';
        std::cout << "Bankroll: £" << bankroll << '\n';


        char play;

        std::cout << "\nDo you want to play? (Y/N): ";
        std::cin >> play;
        play = std::toupper(static_cast<unsigned char>(play));

        if (play == 'N') break;
        if (play != 'Y') continue;

        int bet {get_valid_bet(MIN_BET, MAX_BET, bankroll)};


        if (deck.size() < RESHUFFLE_THRESHOLD*NUM_DECKS*CARDS_PER_DECK) {
            std::cout << "Shuffling Deck...\n";
            deck = make_deck(NUM_DECKS);
            shuffle_deck(deck);
        }

        ++played;

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

            if (player_blackjack && dealer_blackjack) {
                std::cout << "Both have blackjack - Push!\n";
                bankroll+=bet;
            } else if (player_blackjack) {
                std::cout << "Player has blackjack!\n";
                ++player_wins;
                bankroll+=bet*2.5;
            } else {
                std::cout << "Dealer has blackjack!\n";;
            }
            continue; 
        }
        
        bool round_over = false;

        while (!round_over) {
            char decision;
            bool double_enabled {true};
            if (player.size() > 2) {
                double_enabled = false;
                std::cout << "\nHit, Stand (H/S): ";
            } else {
                std::cout << "\nHit, Stand or Double (H/S/D): ";
            }
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
            } else if (decision == 'D' && double_enabled) {
                if (bet > bankroll) {
                    std::cout << "Bankroll is: £" << bankroll << ". Not enough to double\n";
                    continue;
                } else {
                    bankroll-=bet;
                    bet*=2;
                    std::cout << "Doubling bet to £"<< bet <<"\n";
                    hit(deck, player);
                    print_player(player);
                    print_hand_value(player);
                    if (hand_value(player) > 21) {
                        std::cout << "Player bust. Dealer wins!\n";
                        round_over = true;
                    } else {
                        break;
                    }
                }
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
                ++player_wins;
                bankroll+=bet*2;
                round_over = true;
                break;
            }
        }

        if (round_over) continue;

        
        int p = hand_value(player);
        int d = hand_value(dealer);
        if (p > d) {
            std::cout << "Player wins!\n";
            ++player_wins;
            bankroll+=bet*2;
        } else if (d > p) { 
            std::cout << "Dealer wins!\n";
        } else {
            std::cout << "Push (tie)!\n";
            bankroll+=bet;
        }
    }

    if (bankroll <= 0) {
        std::cout << "Out of money!\n";
    }

    return 0;
}
