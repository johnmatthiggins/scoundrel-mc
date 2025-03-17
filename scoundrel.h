#ifndef SCOUNDREL_H
#define SCOUNDREL_H

#include <cstdint>
#include <cstdio>
#include <memory>
#include <optional>
#include <random>
#include <vector>

enum CardSuit: uint32_t {
	CLUBS,
	SPADES,
	DIAMONDS,
	HEARTS,
};

typedef struct {
	uint32_t suit: 2;
	uint32_t rank: 30;
} Card;

constexpr uint32_t STARTING_DECK_SIZE = 44;
constexpr int32_t STARTING_HEALTH = 20;

using CardStack = std::vector<Card>;

std::string card_to_string(const Card& card);

// the object that holds the state of the game...
class ScoundrelGame {
private:
	// starts at 20...
	int32_t _health_points;

	std::unique_ptr<std::mt19937> _mt19937;

	std::unique_ptr<std::vector<Card>> _deck;
	std::unique_ptr<std::vector<Card>> _discard;

	// The current weapon that is equipped...
	std::optional<Card> _equipped_weapon;

	// The list of monsters killed with the current weapon...
	std::unique_ptr<std::vector<Card>> _killed_monsters;

	// The 2-4 cards in front of the player...
	std::unique_ptr<std::vector<Card>> _room;

	// A player cannot press the RUN button twice in a row...
	bool _can_run;

	// A player cannot use more than one potion per dungeon room.
	bool _can_heal;

	// starts at zero and goes up to before limit (non-inclusive)
	uint32_t _random_int(uint32_t limit);
	void _shuffle();
	CardStack _starting_deck() const;

	void _next_room();
public:
	ScoundrelGame(uint32_t seed);
	~ScoundrelGame();

	int32_t get_health() const;
	std::vector<Card>* get_room() const;
	std::vector<Card>* get_deck() const;
	std::vector<Card>* get_discard() const;
	std::vector<Card>* get_killed_monsters() const;
	const std::optional<Card>& get_equipped_weapon() const;

	bool has_died() const;
	bool has_exited_dungeon() const;
	bool has_weapon() const;
	bool can_drink_potion() const;
	bool can_run_away() const;
	bool can_fight_monster_with_weapon_at(uint32_t room_index) const;
	bool can_drink_potion_at(uint32_t room_index) const;

	void fight_monster_barehanded_at(uint32_t room_index);
	void fight_with_weapon_at(uint32_t room_index);
	void equip_weapon_at(uint32_t room_index);
	void drink_potion_at(uint32_t room_index);
	void run_away();
};

#endif
