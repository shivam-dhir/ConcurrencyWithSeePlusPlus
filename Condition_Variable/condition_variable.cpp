#include <iostream>
#include <mutex>
#include <condition_variable>

std::mutex gLock;
std::condition_variable gConditionVariable;

int main() {

	int result = 0;
	bool notified = false;

	std::jthread worker([&] {
		std::unique_lock<std::mutex> lock(gLock);
		result = 23 * 96;
		notified = true;
		std::cout << "worker has completed work \n";
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		gConditionVariable.notify_one();
	});

	std::jthread reporter([&] {
		std::unique_lock<std::mutex> lock(gLock);
		if (!notified)
			gConditionVariable.wait(lock);
		std::cout << "Reporter. Result: " << result << std::endl;
	});
}