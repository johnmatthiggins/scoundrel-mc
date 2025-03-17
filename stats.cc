#include "strategy.h"
#include "scoundrel.h"
#include <numeric>

// returns the number of turns in the game...
int play_game() {
	const int seed = rand();
	ScoundrelGame game(seed);
	FirstCardStrategy strategy(&game, false);

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


int main(int argc, char* argv[]) {
	const int sample_count = 1000000;
	std::vector<double> samples;
	samples.reserve(sample_count);

	for (int i = 0; i < sample_count; ++i) {
		samples.push_back((double)play_game());
	}

	double sum = std::accumulate(samples.begin(), samples.end(), 0.0);
	double mean = sum / samples.size();
	double sq_sum = std::inner_product(samples.begin(), samples.end(), samples.begin(), 0.0);
	double stddev = std::sqrt(sq_sum / samples.size() - (mean * mean));

	printf("STDDEV = %f\n", stddev);
	printf("MEAN = %f\n", mean);

	return 0;
}
