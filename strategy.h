#include "scoundrel.h"
#include <memory>

// just pick the first card and interact with it...
class FirstCardStrategy {
    ScoundrelGame* game;
    bool show_output;
public:
	FirstCardStrategy(ScoundrelGame* game, bool show_output);
	~FirstCardStrategy();
	// returns true when game result has been decided...
	bool play_next_turn();
};
