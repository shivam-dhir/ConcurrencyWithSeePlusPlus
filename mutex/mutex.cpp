#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

// Mutex is also known as binary semaphore

std::mutex gLock;

static int count = 0;

void count_increment_lock_guard() {

	// std::lock_guard is an RAII based locking mechanism, which returns the lock itself
	std::lock_guard<std::mutex> lockGuard(gLock);
	count++;

	// when the scope ends and the lockGuard object is destroyed, its destructor will itseld reuturn the lock
	// which avoids the possibility of a deadlock occurring.
}

void deadlock() {
	gLock.lock();
	count++;

	// If we forget to unlock the mutex, the first thread that gets the lock will never return it adn all the other threads will wait 
	// to acquire the lock, and the program becomes a deadlock and goes on forever.
	// This is dangerous. To prevent this, we use lock_guard(), which is a RAII based lock
	//gLock.unlock();

	// This is another of handling a deadlock. If an exception occurs and the function doesn't return the lock, it results in a deadlock.
	// we then have to handle the exception, return the lock, and again write unlock() if exception is not thrown
	try {
		count++;
		throw "dangerous";
	}
	catch (...) {
		gLock.unlock();
		std::cout << "exception\n";
		return;
	}

	gLock.unlock();

}

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