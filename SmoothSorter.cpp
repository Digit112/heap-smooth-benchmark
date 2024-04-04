#include <cstdio>
#include <cstdint>
#include <vector>

class SmoothSorter {
public:
	// Because of the limits of the arrays, sorts up to 9,615,053,904 items.
	// Increasing this number raises that limit, but the uint32[] members must be upgraded to uint64.
	static const size_t ARRAY_SIZE = 46;
	
	uint32_t sizes[ARRAY_SIZE];
	uint8_t indices[ARRAY_SIZE];
	uint32_t leo[ARRAY_SIZE];
	
	// Number of trees in the Leonardo forest.
	uint8_t numTrees;
	
	// Number of nodes in the Leonardo forests.
	size_t forestSize;
	
	SmoothSorter() {
		leo[0] = 1;
		leo[1] = 1;
		
		for (int i = 2; i < ARRAY_SIZE; i++){
			leo[i] = leo[i-2] + leo[i-1] + 1;
		}
	}
	
	void smoothSort(std::vector<unsigned int>& list) {
		// Using the pointer directly may increase performance.
		// TODO: benchmark against standard version.
		unsigned int* data = &(list.at(0));
		size_t size = list.size();
		
		numTrees = 0;
		forestSize = 0;
		
		heapify(data, size);
		sort(data, size);
	}

private:
	void heapify(unsigned int* data, size_t size) {
		for (int i = 0; i < size; i++) {
			// The first insertion adds Lt(1)
			if (numTrees == 0) {
				sizes[numTrees] = 1;
				indices[numTrees] = 1;
				numTrees++;
			}
			// If there is only one tree, add Lt(1) or Lt(0) depending on whether the existing tree is Lt(1) or not.
			else if (numTrees == 1) {
				if (indices[numTrees-1] == 1) {
					sizes[numTrees] = 1;
					indices[numTrees] = 0;
				}
				else {
					sizes[numTrees] = 1;
					indices[numTrees] = 1;
				}
				numTrees++;
			}
			else {
				// If the last two entries are consecutive Leonardo numbers, combine them into one tree.
				if (indices[numTrees-2] - indices[numTrees-1] == 1) {
					sizes[numTrees-2] += sizes[numTrees-1] + 1;
					indices[numTrees-2] += 1;
					numTrees--;
				}
				// Otherwise, add Lt(1) or Lt(0) depending on whether the previous tree is Lt(1) or not.
				else {
					if (indices[numTrees-1] == 1) {
						sizes[numTrees] = 1;
						indices[numTrees] = 0;
					}
					else {
						sizes[numTrees] = 1;
						indices[numTrees] = 1;
					}
					numTrees++;
				}
			}
			
			forestSize++;
			sift(data, i, numTrees - 1);
		}
	}
	
	// Re-orders the roots (I call this sift left) and sifts down whichever tree the new root ends up in.
	// currTree is the index of the tree whose root we'd like to sift.
	void sift(unsigned int* data, int rootIndex, int currTree) {
		// Sift root left.
		while (currTree > 0) {
			int leftRootInd = rootIndex - sizes[currTree];
			int rootVal = data[rootIndex];
			int leftRootVal = data[leftRootInd];
			
			// The left tree's root is larger than currTree's root and (larger than currTree's children or currTree's root has no children).
			if (
				leftRootVal > rootVal &&
				(
					sizes[currTree] == 1 ||
					(leftRootVal > data[rootIndex - 1] && leftRootVal > data[rootIndex - 1 - leo[indices[currTree]-2]])
				)
			) {
				data[rootIndex] = leftRootVal;
				data[leftRootInd] = rootVal;
				
				currTree--;
				rootIndex = leftRootInd;
			}
			else {
				break;
			}
		}
		
		// Sift down.
		int treeLeoIndex = indices[currTree];
		while (treeLeoIndex > 1) {
			int rightInd = rootIndex - 1;
			int leftInd = rootIndex - 1 - leo[treeLeoIndex - 2];
			
			int rootVal = data[rootIndex];
			int leftVal = data[leftInd];
			int rightVal = data[rightInd];
			
			if (leftVal > rootVal) {
				if (rightVal >= leftVal) {
					data[rightInd] = rootVal;
					data[rootIndex] = rightVal;
					rootIndex = rightInd;
					treeLeoIndex -= 2;
				}
				else {
					data[leftInd] = rootVal;
					data[rootIndex] = leftVal;
					rootIndex = leftInd;
					treeLeoIndex -= 1;
				}
			}
			else if (rightVal > rootVal) {
				data[rightInd] = rootVal;
				data[rootIndex] = rightVal;
				rootIndex = rightInd;
				treeLeoIndex -= 2;
			}
			else {
				break;
			}
		}
	}
	
	void sort(unsigned int* data, size_t size) {
		for (int i = size - 1; i >= 0; i--) {
			if (sizes[numTrees-1] == 1) {
				numTrees--;
				continue;
			}
			
			numTrees++;
			indices[numTrees-2]--;
			indices[numTrees-1] = indices[numTrees-2] - 1;
			
			sizes[numTrees-2] = leo[indices[numTrees-2]];
			sizes[numTrees-1] = leo[indices[numTrees-1]];
			
			sift(data, i - sizes[numTrees-1] - 1, numTrees-2);
			sift(data, i - 1, numTrees-1);
		}
	}
};