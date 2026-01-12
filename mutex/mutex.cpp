#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

// Mutex is also known as binary semaphore

std::mutex gLock;

static int count = 0;

void count_increment() {
	// the code between the lock() and unlock() is a critical section and can't be accessed by multiple
	// threads at the same time. This makes the count++ an atomic section
	gLock.lock();
		count++;
	gLock.unlock();
}

int main() {
	std::vector<std::thread> threads;
	// multiple threads may read the count value at the same time and increment a stale value
	for (int i = 0; i < 10000; i++)
		threads.push_back(std::thread(count_increment));

	for (int i = 0; i < 10000; i++)
		threads[i].join();

	std::cout << "count: " << count << std::endl;
}