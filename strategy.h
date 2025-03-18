#include "scoundrel.h"
#include <memory>
#include <random>

/**
 * Strategy interface...
 */
class Strategy {
public:
  virtual bool play_next_turn() = 0;
  virtual void load_game(ScoundrelGame *game) = 0;
};

/**
 * Always picks the card furthest to the left...
 */
class FirstCardStrategy : public Strategy {
  ScoundrelGame *game;
  bool show_output;

public:
  FirstCardStrategy(bool show_output);
  ~FirstCardStrategy();
  // returns true when game result has been decided...
  bool play_next_turn();
  void load_game(ScoundrelGame *game);
};

/**
 * Takes random actions...
 */
class RandomStrategy : public Strategy {
  ScoundrelGame *game;
  std::unique_ptr<std::mt19937> _rng;

public:
  RandomStrategy();
  ~RandomStrategy();
  bool play_next_turn();
  void load_game(ScoundrelGame *game);

  class GameAction {
  public:
    GameAction() {}
    virtual ~GameAction() {}
    virtual void act(ScoundrelGame *game) = 0;
  };

  class FightMonsterWithWeapon final : public GameAction {
    uint32_t _target;

  public:
    FightMonsterWithWeapon(uint32_t target);
    ~FightMonsterWithWeapon();
    virtual void act(ScoundrelGame *game) override;
  };

  class FightMonsterBareHanded final : public GameAction {
    uint32_t _target;

  public:
    FightMonsterBareHanded(uint32_t target);
    ~FightMonsterBareHanded();
    virtual void act(ScoundrelGame *game) override;
  };

  class DrinkPotion final : public GameAction {
    uint32_t _target;

  public:
    DrinkPotion(uint32_t target);
    ~DrinkPotion();
    virtual void act(ScoundrelGame *game) override;
  };

  class EquipWeapon final : public GameAction {
    uint32_t _target;

  public:
    EquipWeapon(uint32_t target);
    ~EquipWeapon();
    virtual void act(ScoundrelGame *game) override;
  };

  class RunAway final : public GameAction {
  public:
    RunAway() {};
    ~RunAway() {};
    virtual void act(ScoundrelGame *game) override;
  };
};

/**
 * Greedy algorithm that checks all ordering of cards in current room and takes
 * the ordering with the best score. (Highest health?) (Best ordering cannot lead to death)
 * TODO: Figure out scoring function...
 */

/**
 * Algorithm that fights every monster barehanded if they can survive it.
 * - Start with the highest monster. If the player can't survive a fight
 *   they will opt for the next lowest monster. If no monsters exist, perform
 *   a random action.
 */
