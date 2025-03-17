#include "scoundrel.h"
#include "cppunit.h"

class Cppunit_tests: public Cppunit {
    void test_list() {
        correct_starting_pile_sizes();
        player_run_away_changes_cards();
        player_cannot_run_away_twice_in_a_row();
        does_not_win_or_lose_game_instantly();
        can_kill_monster_with_weapon();
        cannot_kill_monster_with_weapon_when_no_weapon();
        clearing_room_loads_next_room();
    }

    void correct_starting_pile_sizes() {
        ScoundrelGame game(1);

        CHECK(4, game.get_room()->size());
        CHECK(40, game.get_deck()->size());
        CHECK(0, game.get_discard()->size());
        CHECK(0, game.get_killed_monsters()->size());
    }

    void player_run_away_changes_cards() {
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

        CHECK(card0.suit, CardSuit::CLUBS);
        CHECK(card1.suit, CardSuit::SPADES);
        CHECK(card2.suit, CardSuit::SPADES);
        CHECK(card3.suit, CardSuit::HEARTS);
    }

    void player_cannot_run_away_twice_in_a_row() {
        ScoundrelGame game(3);
        game.run_away();
        CHECKT(!game.can_run_away());
    }

    void does_not_win_or_lose_game_instantly() {
        ScoundrelGame game(128);
        CHECKT(!game.has_died());
        CHECKT(!game.has_exited_dungeon());
    }

    void can_kill_monster_with_weapon() {
        ScoundrelGame game(124);
        CHECK(game.get_room()->at(0).suit, CardSuit::SPADES);
        CHECK(game.get_room()->at(0).rank, 14);

        CHECK(game.get_room()->at(1).suit, CardSuit::DIAMONDS);
        CHECK(game.get_room()->at(1).rank, 10);

        CHECK(game.get_health(), 20);

        game.equip_weapon_at(1);
        CHECKT(game.get_equipped_weapon().has_value())
        CHECKT(game.can_fight_monster_with_weapon_at(0));
        game.fight_with_weapon_at(0);
        CHECK(game.get_health(), 16);

        CHECK(game.get_discard()->at(0).suit, CardSuit::DIAMONDS);
        CHECK(game.get_discard()->at(0).rank, 10);
        CHECK(game.get_discard()->at(1).suit, CardSuit::SPADES);
        CHECK(game.get_discard()->at(1).rank, 14);
    }

    void cannot_kill_monster_with_weapon_when_no_weapon() {
        ScoundrelGame game(124);
        CHECK(game.get_room()->at(0).suit, CardSuit::SPADES);
        CHECK(game.get_room()->at(0).rank, 14);

        CHECK(game.get_room()->at(1).suit, CardSuit::DIAMONDS);
        CHECK(game.get_room()->at(1).rank, 10);

        CHECK(game.get_health(), 20);
        CHECKT(!game.can_fight_monster_with_weapon_at(0));
    }

    void clearing_room_loads_next_room() {
        ScoundrelGame game(124);
        CHECK(game.get_room()->at(0).suit, CardSuit::SPADES);
        CHECK(game.get_room()->at(0).rank, 14);

        CHECK(game.get_room()->at(1).suit, CardSuit::DIAMONDS);
        CHECK(game.get_room()->at(1).rank, 10);

        game.equip_weapon_at(1);
        CHECKT(game.get_equipped_weapon().has_value())
        CHECKT(game.can_fight_monster_with_weapon_at(0));

        game.fight_with_weapon_at(0);
        CHECK(game.get_deck()->at(1).suit, CardSuit::HEARTS);
        game.fight_with_weapon_at(1);

        CHECK(game.get_room()->size(), 4);
    }
};

int main(int argc, char *argv[]) {
    // Test invocation example.
    return (new Cppunit_tests)->run();
}
