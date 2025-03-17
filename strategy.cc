#include <iostream>
#include <stdexcept>

#include "strategy.h"
#include "scoundrel.h"

FirstCardStrategy::FirstCardStrategy(ScoundrelGame* game) {
	if (game == nullptr) {
		throw std::invalid_argument("You can't pass a null pointer game to a strategy!");
	}
	this->game = game;
}

FirstCardStrategy::~FirstCardStrategy() {
	// pass
}

bool FirstCardStrategy::play_next_turn() {
	if (game->has_died() || game->has_exited_dungeon()) {
		throw new std::invalid_argument("You are trying to play a game that has already been decided!");
	}
	const Card first_card = game->get_room()->at(0);
	if (first_card.suit == CardSuit::CLUBS || first_card.suit == CardSuit::SPADES) {
		if (game->has_weapon()) {
			if (game->can_fight_monster_with_weapon_at(0)) {
				game->fight_with_weapon_at(0);
				std::cout << "[" << game->get_health() << "]" << " Fought monster with weapon... " << card_to_string(first_card) << std::endl;
			} else {
				game->fight_monster_barehanded_at(0);
				std::cout << "[" << game->get_health() << "]" << " Fought monster with bare hands... " << card_to_string(first_card) << std::endl;
			}
		} else {
			game->fight_monster_barehanded_at(0);
			std::cout << "[" << game->get_health() << "]" << " Fought monster with bare hands... " << card_to_string(first_card) << std::endl;
		}
	} else if (first_card.suit == CardSuit::DIAMONDS) {
		game->equip_weapon_at(0);
		std::cout << "[" << game->get_health() << "]"  << " Equipped weapon... " << card_to_string(first_card) << std::endl;
	} else {
		game->drink_potion_at(0);
		std::cout << "[" << game->get_health() << "]" << " Drank potion... " << card_to_string(first_card) << std::endl;
	}

	const bool game_decided = game->has_died() || game->has_exited_dungeon();
	return game_decided;
}
