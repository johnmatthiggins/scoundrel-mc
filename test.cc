#include "scoundrel.h"
#include "cppunit.h"

// Test examples.
class ScoundrelGameTests: public Cppunit {
    void test_game_starts_with_44_cards() {
        ScoundrelGame game(1);
        CHECK(40, game.get_deck()->size());
    }
};

int main(int argc, char *argv[]) {
    // Test invocation example.
    return (new ScoundrelGameTests)->run();
}
