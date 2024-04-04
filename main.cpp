#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "SmoothSorter.cpp"
#include "HeapSorter.cpp"

int main() {
	const int num_vals = 1000000; // list size
	const int num_vals_round_diff = 1000000; // Amount to increase list size by each round.
	const int num_rounds = 10; // Each round, the list's size is increased.
	const int num_tests_per_round = 50;
	
	SmoothSorter sorter;
	std::vector<unsigned int> list;
	
	std::vector<float> test_samples(num_tests_per_round);
	std::vector<int> vals_in_rounds(num_rounds);
	std::vector<float> Q1(num_rounds);
	std::vector<float> Q2(num_rounds);
	std::vector<float> Q3(num_rounds);
	std::vector<float> avg(num_rounds);
	
	int num_vals_in_round = num_vals;
	for (int r = 0; r < num_rounds; r++) {
		printf("Round %d/%d: %d tests with %d items.\n", r+1, num_rounds, num_tests_per_round, num_vals_in_round);
		for (int t = 0; t < num_tests_per_round; t++) {
			for (int i = 0; i < num_vals_in_round; i++) {
				list.push_back(i);
			}
			for (int i = 0; i < num_vals; i++) {
				int min_i = std::max(0,                  (int) (i - num_vals * 1));
				int max_i = std::min((int) (num_vals-1), (int) (i + num_vals * 1));
				int other_i = rand() % (max_i - min_i) + min_i;
				
				unsigned int temp = list[i];
				list[i] = list[other_i];
				list[other_i] = temp;
			}
			
			clock_t start = clock();
			
			// sorter.heapSort(list);
			//sorter.smoothSort(list);
			std::sort(list.begin(), list.end());
			
			clock_t end = clock();
			
			clock_t elapsed_cycles = end - start;
			test_samples[t] = (double) elapsed_cycles / CLOCKS_PER_SEC;
			//printf("%d Cycles / %d Clocks per Second = %.3lfs\n", elapsed_cycles, CLOCKS_PER_SEC, (double) elapsed_cycles / CLOCKS_PER_SEC);
			
			// Validate
			for (int i = 0; i < list.size()-1; i++) {
				if (list.at(i) > list.at(i+1)) {
					printf("Not sorted.\n");
					break;
				}
			}
			
			list.clear();
		}
		
		vals_in_rounds[r] = num_vals_in_round;
		
		std::sort(test_samples.begin(), test_samples.end());
		Q1[r] = test_samples[(int) (num_tests_per_round * 0.25)];
		Q2[r] = test_samples[(int) (num_tests_per_round * 0.50)];
		Q3[r] = test_samples[(int) (num_tests_per_round * 0.75)];
		
		float running_avg = 0;
		for (int i = 0; i < test_samples.size(); i++) {
			running_avg += test_samples[i];
		}
		running_avg /= test_samples.size();
		avg[r] = running_avg;
		
		num_vals_in_round += num_vals_round_diff;
	}
	
	printf("| Num Items | Avg. (s) | Q1 (s) | Q2 (s) | Q3 (s) |\n");
	printf("|-----------+----------+--------+--------+--------+\n");
	for (int i = 0; i < num_rounds; i++) {
		printf("| %9ld | %6.3f   | %6.3f | %6.3f | %6.3f |\n", vals_in_rounds[i], avg[i], Q1[i], Q2[i], Q3[i]);
	}
	
	return 0;
}