#include "scoundrel.h"
#include "cppunit.h"

// Test examples.
class Cppunit_tests: public Cppunit {
    void test_list() {
        correct_starting_pile_sizes_test();
        player_run_away_changes_cards_test();
    }

    void correct_starting_pile_sizes_test() {
        ScoundrelGame game(1);

        CHECK(4, game.get_room()->size());
        CHECK(40, game.get_deck()->size());
        CHECK(0, game.get_discard()->size());
        CHECK(0, game.get_killed_monsters()->size());
    }

    void player_run_away_changes_cards_test() {
        ScoundrelGame game(2);

        Card card0 = game.get_room()->at(0);
        Card card1 = game.get_room()->at(1);
        Card card2 = game.get_room()->at(2);
        Card card3 = game.get_room()->at(3);

        CHECK(card0.suit, CardSuit::SPADES);
        CHECK(card1.suit, CardSuit::DIAMONDS);
        CHECK(card2.suit, CardSuit::HEARTS);
        CHECK(card3.suit, CardSuit::SPADES);

        game.run_away();

        card0 = game.get_room()->at(0);
        card1 = game.get_room()->at(1);
        card2 = game.get_room()->at(2);
        card3 = game.get_room()->at(3);

        // 0 1 1 3
        CHECK(card0.suit, CardSuit::CLUBS);
        CHECK(card1.suit, CardSuit::SPADES);
        CHECK(card2.suit, CardSuit::SPADES);
        CHECK(card3.suit, CardSuit::HEARTS);
    }
};

int main(int argc, char *argv[]) {
    // Test invocation example.
    return (new Cppunit_tests)->run();
}
