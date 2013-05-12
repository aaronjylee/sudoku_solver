#include "cell.h"
using namespace std;
	
Cell::Cell () : count(9) {
	for (int num = 0 ; num < 9 ; num++)
		candidates[num] = true;
}
	
Cell::Cell (int value) : count(1) {
	for (int num = 0 ; num < 9 ; num++) {
		if (num == value - 1)
			candidates[num] = true;
		else
			candidates[num] = false;
	}
}
	
bool Cell::at (int num) const {
	return candidates[num - 1];
}
	
int Cell::first () const {
	for (int num = 0 ; num < 9 ; num++)
		if (candidates[num])
			return num + 1;
	return -1;
}
	
int Cell::possibilities () const {
	return count;
}
	
bool Cell::removeCandidate (int pos) {
	if (candidates[pos - 1]) {
		candidates[pos - 1] = false;
		count--;
	}
	if (count == 0)
		return false;
	return true;
}

