#include <iostream>
#include <mutex>
#include <condition_variable>

std::mutex gLock;
std::condition_variable gConditionVariable;

int main() {

	int result = 0;
	bool notified = false;

	std::jthread worker([&] {
		//acquire global lock
		std::unique_lock<std::mutex> lock(gLock);
		result = 23 * 96;
		notified = true;
		std::cout << "worker has completed work \n";
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		// notify one of the threads (only one other in this case), while holding the lock
		gConditionVariable.notify_one();
	});
	// when worker thread completes its work and the scope ends, unique_lock is destroyed and gLock is releaseed

	std::jthread reporter([&] {
		// acquire global lock
		std::unique_lock<std::mutex> lock(gLock);
		// if worker thread has not completed its work, wait for it. wait() automatically releases the lock and blocks it until a notification arrives
		if (!notified)
			gConditionVariable.wait(lock);
		std::cout << "Reporter. Result: " << result << std::endl;
	});
}