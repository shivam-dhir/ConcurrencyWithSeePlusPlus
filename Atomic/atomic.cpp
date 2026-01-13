#include <iostream>
#include <atomic>

static std::atomic<int> counter;

void increment_counter() {
	// oeprator++ is overloaded for std::atomic, operator+ isn't, so be careful 
	counter++;

	// atomics help to remove the complexity of mutex and lock_guard etc.
}

int main() {
	std::cout << counter << std::endl;
}