#include "scoundrel.h"
#include <memory>

// just pick the first card and interact with it...
class FirstCardStrategy {
    ScoundrelGame* game;
public:
	FirstCardStrategy(ScoundrelGame* game);
	~FirstCardStrategy();
	// returns true when game result has been decided...
	bool play_next_turn();
};
