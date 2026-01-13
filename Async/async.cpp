// std::async is a feature that helps us do a task asynchronously without blocking the current thread
// it starts execution on the callable (function) in a separate thread and uses std::future to store the results
// std::future can be checked if the result is ready, or will be in a certain amount of time.
// get() method on std::future block the current thread and waits for std::async to finish execution and return a result
// it is something similar to a join() function on a thread

#include <iostream>
#include <future>

static bool bufferedFileLoader() {

	for (int i = 0; i < 10; i++)
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return true;
}

int main() {

	// starts executing bufferedFileLoader() in a separate thread
	std::future<bool> backgroundThread = std::async(std::launch::async, bufferedFileLoader);
	std::future_status status = std::future_status::deferred;
	
	// status just checks if the function has completed its work and has returned something
	// when it does, its future_status becomes 'ready'
	while (status != std::future_status::ready) {
		std::cout << "Waiting for data ... \n";
		status = backgroundThread.wait_for(std::chrono::milliseconds(1));
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	std::cout << " --- Data ready for use --- \n";
}

