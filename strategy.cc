#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>

#include "scoundrel.h"
#include "strategy.h"

FirstCardStrategy::FirstCardStrategy(bool show_output) {
  if (game == nullptr) {
    throw std::invalid_argument(
        "You can't pass a null pointer game to a strategy!");
  }
  this->show_output = show_output;
}

void FirstCardStrategy::load_game(ScoundrelGame *game) { this->game = game; }

FirstCardStrategy::~FirstCardStrategy() {
  // pass
}

bool FirstCardStrategy::play_next_turn() {
  if (game->has_died() || game->has_exited_dungeon()) {
    throw std::invalid_argument(
        "You are trying to play a game that has already been decided!");
  }
  const Card first_card = game->get_room()->at(0);
  if (first_card.suit == CardSuit::CLUBS ||
      first_card.suit == CardSuit::SPADES) {
    if (game->has_weapon()) {
      if (game->can_fight_monster_with_weapon_at(0)) {
        game->fight_with_weapon_at(0);
        if (this->show_output) {
          std::cout << "[" << game->get_health() << "]"
                    << " Fought monster with weapon... "
                    << card_to_string(first_card) << std::endl;
        }
      } else {
        game->fight_monster_barehanded_at(0);
        if (this->show_output) {
          std::cout << "[" << game->get_health() << "]"
                    << " Fought monster with bare hands... "
                    << card_to_string(first_card) << std::endl;
        }
      }
    } else {
      game->fight_monster_barehanded_at(0);
      if (this->show_output) {
        std::cout << "[" << game->get_health() << "]"
                  << " Fought monster with bare hands... "
                  << card_to_string(first_card) << std::endl;
      }
    }
  } else if (first_card.suit == CardSuit::DIAMONDS) {
    game->equip_weapon_at(0);
    if (this->show_output) {
      std::cout << "[" << game->get_health() << "]"
                << " Equipped weapon... " << card_to_string(first_card)
                << std::endl;
    }
  } else {
    game->drink_potion_at(0);
    if (this->show_output) {
      std::cout << "[" << game->get_health() << "]"
                << " Drank potion... " << card_to_string(first_card)
                << std::endl;
    }
  }

  const bool game_decided =
      this->game->has_died() || this->game->has_exited_dungeon();
  return game_decided;
}

RandomStrategy::RandomStrategy() {
  time_t now = time(0);
  this->_rng = std::make_unique<std::mt19937>(now);
}

RandomStrategy::~RandomStrategy() {}

bool RandomStrategy::play_next_turn() {
  const std::vector<Card> *room = this->game->get_room();
  std::vector<RandomStrategy::GameAction *> actions;

  // compile list of possible actions...
  for (uint32_t i = 0; i < room->size(); ++i) {
    const Card &card = room->at(i);
    if (card.suit == CardSuit::CLUBS || card.suit == CardSuit::SPADES) {
      RandomStrategy::GameAction *action1 =
          new RandomStrategy::FightMonsterBareHanded(i);
      actions.push_back(action1);
      if (this->game->can_fight_monster_with_weapon_at(i)) {
        RandomStrategy::GameAction *action2 =
            new RandomStrategy::FightMonsterWithWeapon(i);
        actions.push_back(action2);
      }
    } else if (card.suit == CardSuit::HEARTS) {
      actions.push_back(new RandomStrategy::DrinkPotion(i));
    } else {
      actions.push_back(new RandomStrategy::EquipWeapon(i));
    }
  }
  if (this->game->can_run_away()) {
    actions.push_back(new RandomStrategy::RunAway());
    actions.push_back(new RandomStrategy::RunAway());
    actions.push_back(new RandomStrategy::RunAway());
    actions.push_back(new RandomStrategy::RunAway());
  }

  std::uniform_int_distribution<uint32_t> dist(0, actions.size() - 1);
  uint32_t choice = dist(*this->_rng);
  actions.at(choice)->act(this->game);

  // clear pointers that were allocated...
  for (int i = 0; i < actions.size(); ++i) {
    delete actions.at(i);
  }

  return this->game->has_died() || this->game->has_exited_dungeon();
}

void RandomStrategy::load_game(ScoundrelGame *game) { this->game = game; }

RandomStrategy::DrinkPotion::DrinkPotion(uint32_t target) {
  this->_target = target;
}

RandomStrategy::DrinkPotion::~DrinkPotion() {}

void RandomStrategy::DrinkPotion::act(ScoundrelGame *game) {
  game->drink_potion_at(this->_target);
}

RandomStrategy::FightMonsterBareHanded::FightMonsterBareHanded(
    uint32_t target) {
  this->_target = target;
}

RandomStrategy::FightMonsterBareHanded::~FightMonsterBareHanded() {}

void RandomStrategy::FightMonsterBareHanded::act(ScoundrelGame *game) {
  game->fight_monster_barehanded_at(this->_target);
}

RandomStrategy::FightMonsterWithWeapon::FightMonsterWithWeapon(
    uint32_t target) {
  this->_target = target;
}

RandomStrategy::FightMonsterWithWeapon::~FightMonsterWithWeapon() {}

void RandomStrategy::FightMonsterWithWeapon::act(ScoundrelGame *game) {
  game->fight_with_weapon_at(this->_target);
}

RandomStrategy::EquipWeapon::EquipWeapon(uint32_t target) {
  this->_target = target;
}

RandomStrategy::EquipWeapon::~EquipWeapon() {}

void RandomStrategy::EquipWeapon::act(ScoundrelGame *game) {
  game->equip_weapon_at(this->_target);
}

void RandomStrategy::RunAway::act(ScoundrelGame *game) { game->run_away(); }
