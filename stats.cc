#include "scoundrel.h"
#include "strategy.h"
#include <numeric>

int play_game_with_strategy(Strategy &strategy) {
  int turns = 0;
  int max_turns = 1000;
  while (!strategy.play_next_turn()) {
    turns++;
    if (turns >= max_turns) {
      break;
    }
  }

  return turns;
}

// returns the number of turns in the game...
void evaluate_strategy(Strategy &strategy, std::string name) {
  const int sample_count = 100000;
  std::vector<double> samples;
  samples.reserve(sample_count);

  for (int i = 0; i < sample_count; ++i) {
    const int seed = rand();
    ScoundrelGame game(seed);
    strategy.load_game(&game);
    samples.push_back((double)play_game_with_strategy(strategy));
  }

  double sum = std::accumulate(samples.begin(), samples.end(), 0.0);
  double mean = sum / samples.size();
  double sq_sum =
      std::inner_product(samples.begin(), samples.end(), samples.begin(), 0.0);
  double stddev = std::sqrt(sq_sum / samples.size() - (mean * mean));

  std::sort(samples.begin(), samples.end());

  double median;
  if (samples.size() % 2 == 0) {
    median = (samples.at(samples.size() / 2) +
              samples.at((samples.size() / 2) + 1)) /
             2;
  } else {
    median = samples.at(samples.size() / 2);
  }

  printf("-------------------------------------\n");
  printf("STRATEGY(%s):\n", name.c_str());
  printf("STDDEV = %f\n", stddev);
  printf("MEAN = %f\n", mean);
  printf("MEDIAN = %f\n", median);
  printf("MAX = %f\n", samples.back());
  printf("MIN = %f\n", samples.front());
  printf("-------------------------------------\n");
}

int main(int argc, char *argv[]) {
  FirstCardStrategy strategy(false);
  evaluate_strategy(strategy, std::string("ALWAYS PICK FIRST CARD"));

  RandomStrategy rng_strategy;
  evaluate_strategy(rng_strategy, std::string("PICK RANDOM CARD"));
  return 0;
}
