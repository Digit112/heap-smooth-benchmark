#include <cstdio>
#include <vector>

class HeapSorter {
public:
	void heapSort(std::vector<unsigned int>& list) {
		unsigned int* data = &(list.at(0));
		size_t size = list.size();
		
		//heapify(data, size, 0);
		for (int i = size/2; i >= 0; i--) {
			siftdown(data, size, i);
		}
		
		for (int i = size-1; i >= 2; i--) {
			unsigned int lastVal = data[i];
			data[i] = data[0];
			data[0] = lastVal;
			
			siftdown(data, i, 0);
		}
		
		unsigned int lastVal = data[1];
		data[1] = data[0];
		data[0] = lastVal;
	}

private:
	void siftdown(unsigned int* data, size_t size, int i) {
		unsigned int iVal = data[i];
		
		unsigned int lInd = i*2 + 1;
		unsigned int rInd = lInd + 1;
		while (rInd < size) {
			unsigned int rVal = data[rInd];
			unsigned int lVal = data[lInd];
			
			if (rVal > iVal) {
				if (rVal > lVal) {
					data[i] = rVal;
					data[rInd] = iVal;
					
					i = rInd;
					lInd = i*2 + 1;
					rInd = i*2 + 2;
					
					continue;
				}
				else {
					data[i] = lVal;
					data[lInd] = iVal;
					
					i = lInd;
					lInd = i*2 + 1;
					rInd = i*2 + 2;
					
					continue;
				}
			}
			else if (lVal > iVal) {
				data[i] = lVal;
				data[lInd] = iVal;
				
				i = lInd;
				lInd = i*2 + 1;
				rInd = i*2 + 2;
				
				continue;
			}
			
			break;
		}
		
		if (lInd < size) {
			unsigned int lVal = data[lInd];
			
			if (lVal > iVal) {
				data[i] = lVal;
				data[lInd] = iVal;
			}
		}
	}
	
	int getLeftChild(unsigned int i) {
		return 2 * i + 1;
	}
	
	int getRightChild(unsigned int i) {
		return 2 * i + 2;
	}
	
	int getParent(unsigned int i) {
		return (i - 1) / 2;
	}
};