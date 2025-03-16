#include <cstdio>
#include <cstdlib>

#include "scoundrel.h"

char _suit_char(CardSuit suit) {
	switch (suit) {
		case CardSuit::CLUBS:
			return 'C';
		case CardSuit::SPADES:
			return 'S';
		case CardSuit::DIAMONDS:
			return 'D';
		case CardSuit::HEARTS:
			return 'H';
		default:
			return '-';
	}
}

int main(int argc, char* argv[]) {
	const int seed = rand();
	ScoundrelGame game(seed);
	for (int i = 0; i < 40; ++i) {
		const Card card = game.get_room()->at(i);
		printf("%d %c ", card.rank,_suit_char((CardSuit)card.suit));
	}
	printf("\n");

	game.run_away();

	for (int i = 0; i < 4; ++i) {
		const Card card = game.get_room()->at(i);
		printf("%d %c ", card.rank, _suit_char((CardSuit)card.suit));
	}
	printf("\n");
	return 0;
}

