#include <iostream>
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <optional>
#include <random>
#include <stdexcept>
#include <vector>

#include "scoundrel.h"

std::string card_to_string(const Card& card) {
	char buf[0x10] = {0};
	char c[4] = {0};
	switch (card.suit) {
		case CardSuit::CLUBS:
			c[0] = 0xE2;
			c[1] = 0x99;
			c[2] = 0xA3;
			break;
		case CardSuit::SPADES:
			c[0] = 0xE2;
			c[1] = 0x99;
			c[2] = 0xA0;
			break;
		case CardSuit::HEARTS:
			c[0] = 0xE2;
			c[1] = 0x99;
			c[2] = 0xA5;
			break;
		case CardSuit::DIAMONDS:
			c[0] = 0xE2;
			c[1] = 0x99;
			c[2] = 0xA6;
			break;
		default:
			break;
	}
	snprintf(buf, sizeof buf, "%s %d", c, card.rank);
	return std::string(buf);
}

ScoundrelGame::ScoundrelGame(uint32_t seed) {
	this->_mt19937 = std::make_unique<std::mt19937>(std::mt19937(seed));
	this->_health_points = STARTING_HEALTH;

	this->_deck = std::make_unique<CardStack>(
		this->_starting_deck()
	);
	_shuffle();

	this->_discard = std::make_unique<CardStack>(
		CardStack()
	);
	this->_equipped_weapon = std::nullopt;
	this->_killed_monsters = std::make_unique<CardStack>(
		CardStack()
	);
	this->_room = std::make_unique<CardStack>(
		CardStack()
	);

	// load first four cards into room
	for (size_t i = 0; i < 4; ++i) {
		const Card next = this->_deck->back();
		this->_deck->pop_back();
		this->_room->push_back(next);
	}
	this->_can_run = true;
	this->_can_heal = true;
}

ScoundrelGame::~ScoundrelGame() {
	// I think everything gets cleaned up because I'm
	// using unique_ptr but we should definitely test
	// that later...
}

uint32_t ScoundrelGame::_random_int(uint32_t limit) {
	std::uniform_int_distribution<uint32_t> distribution(0, limit);
	std::mt19937 rng = *this->_mt19937;
	uint32_t random_int = (uint32_t)distribution(rng);
	return random_int;
}

void ScoundrelGame::_shuffle() {
	std::shuffle(this->_deck->begin(), this->_deck->end(), *this->_mt19937);
}

CardStack ScoundrelGame::_starting_deck() const  {
	std::vector<Card> _cards;
	_cards.reserve(STARTING_DECK_SIZE);

	// load monsters...
	for (uint32_t i = 2; i <= 14; ++i) {
		const Card next_clubs = {
			.suit = CardSuit::CLUBS,
			.rank = i,
		};
		const Card next_spades = {
			.suit = CardSuit::SPADES,
			.rank = i,
		};
		_cards.push_back(next_clubs);
		_cards.push_back(next_spades);
	}

	// load health potions and weapons...
	for (uint32_t i = 2; i <= 10; ++i) {
		const Card next_diamond = {
			.suit = CardSuit::DIAMONDS,
			.rank = i,
		};
		const Card next_heart = {
			.suit = CardSuit::HEARTS,
			.rank = i,
		};
		_cards.push_back(next_diamond);
		_cards.push_back(next_heart);
	}

	return _cards;
}

void ScoundrelGame::_next_room() {
	constexpr size_t ROOM_CARD_COUNT = 4;
	const size_t limit = std::min(ROOM_CARD_COUNT - 1, this->_deck->size());
	for (size_t i = 0; i < limit; ++i) {
		this->_room->push_back(this->_deck->back());
		this->_deck->pop_back();
	}
	this->_can_run = true;
}

int32_t ScoundrelGame::get_health() const {
	return this->_health_points;
}

CardStack* ScoundrelGame::get_room() const {
	return this->_room.get();
}

CardStack* ScoundrelGame::get_deck() const {
	return this->_deck.get();
}

CardStack* ScoundrelGame::get_discard() const {
	return this->_discard.get();
}

CardStack* ScoundrelGame::get_killed_monsters() const {
	return this->_killed_monsters.get();
}

const std::optional<Card>& ScoundrelGame::get_equipped_weapon() const {
	return this->_equipped_weapon;
}

bool ScoundrelGame::has_died() const {
	return this->_health_points <= 0;
}

bool ScoundrelGame::has_exited_dungeon() const {
	return std::count_if(
		this->_discard->begin(),
		this->_discard->end(),
		[](Card card) {
			return card.suit == CardSuit::SPADES || card.suit == CardSuit::CLUBS;
		}
	) == 26;
}

bool ScoundrelGame::has_weapon() const {
	return this->_equipped_weapon.has_value();
}

bool ScoundrelGame::can_drink_potion() const {
	return this->_can_heal;
}

bool ScoundrelGame::can_run_away() const {
	// all cards need to be present in room in order for a
	// player to run.
	return this->_can_run && this->_room->size() == 4;
}

// don't do any bounds checking...
bool ScoundrelGame::can_fight_monster_with_weapon_at(uint32_t room_index) const {
	bool can_fight = false;

	// the player can only fight a monster with a weapon if they:
	// - have a weapon
	// - have not killed any monsters with that weapon that have a lower
	//   rank than the current monster
	if (this->has_weapon()) {
		const Card monster_card = this->_room->at(room_index);
		if (monster_card.suit == CardSuit::CLUBS || monster_card.suit == CardSuit::SPADES) {
			if (this->_killed_monsters->size() == 0) {
				can_fight = true;
			} else if (this->_killed_monsters->back().rank >= monster_card.rank) {
				can_fight = true;
			}
		}
	}
	return can_fight;
}

bool ScoundrelGame::can_drink_potion_at(uint32_t room_index) const {
	bool can_drink_potion = false;
	if (this->_can_heal) {
		const Card card = this->_room->at(room_index);
		can_drink_potion = card.suit == CardSuit::HEARTS;
	}
	return can_drink_potion;
}

void ScoundrelGame::fight_monster_barehanded_at(uint32_t room_index) {
	const Card monster = this->_room->at(room_index);
	if (monster.suit != CardSuit::SPADES && monster.suit != CardSuit::CLUBS) {
		throw std::invalid_argument("Cannot fight monster if the card is not a monster!");
	}

	// decremented health and added monster to discard pile...
	this->_health_points -= monster.rank;
	this->_room->erase(this->_room->begin() + room_index);
	this->_discard->push_back(monster);
	
	// increment the number of killed monsters...
	// check if all monsters have been killed
	// if they have all been killed then set the win state or whatever...

	if (this->get_room()->size() == 1) {
		this->_next_room();
	}
}

void ScoundrelGame::fight_with_weapon_at(uint32_t room_index) {
	if (!this->can_fight_monster_with_weapon_at(room_index)) {
		throw std::invalid_argument("Cannot fight monster card!");
	}

	const Card monster = this->_room->at(room_index);

	// decrement health and add monster to discard pile...
	this->_health_points -= std::max(monster.rank - this->_equipped_weapon.value().rank, 0);
	this->_killed_monsters->push_back(monster);
	this->_room->erase(this->_room->begin() + room_index);
	this->_discard->push_back(monster);

	if (this->get_room()->size() == 1) {
		this->_next_room();
	}
}

void ScoundrelGame::equip_weapon_at(uint32_t room_index) {
	const Card weapon = this->_room->at(room_index);
	if (weapon.suit != CardSuit::DIAMONDS) {
		throw new std::invalid_argument("Cannot equip weapon card because it is not a weapon!");
	}
	// reset killed monsters because we have a new weapon...
	this->_killed_monsters->clear();
	this->_equipped_weapon = std::optional(weapon);

	// delete from room and move it to the discard pile...
	this->_room->erase(this->_room->begin() + room_index);
	this->_discard->push_back(weapon);

	if (this->get_room()->size() == 1) {
		this->_next_room();
	}
}

void ScoundrelGame::drink_potion_at(uint32_t room_index) {
	if (!this->can_drink_potion_at(room_index)) {
		throw std::invalid_argument("Cannot drink potion card!");
	}
	const Card potion = this->_room->at(room_index);

	// don't increase it too much...
	this->_health_points = std::min(this->_health_points + potion.rank, STARTING_HEALTH);
	this->_room->erase(this->_room->begin() + room_index);
	this->_discard->push_back(potion);

	if (this->get_room()->size() == 1) {
		this->_next_room();
	}
}

void ScoundrelGame::run_away() {
	if (!this->can_run_away()) {
		throw std::invalid_argument("You can't run away you damn coward!");
	}

	// move cards back into deck, shuffle, and deal 4
	for (size_t i = 0; i < 4; ++i) {
		const Card card = this->_room->back();
		this->_room->pop_back();
		this->_deck->push_back(card);
	}
	_shuffle();

	for (size_t i = 0; i < 4; ++i) {
		const Card card = this->_deck->back();
		this->_deck->pop_back();
		this->_room->push_back(card);
	}

	// make sure the player can't run away twice in a row...
	this->_can_run = false;
}
