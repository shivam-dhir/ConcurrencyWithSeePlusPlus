#include <iostream>
#include <thread>

void print() {
	std::cout << "Hello from thread" << std::endl;
}

int main() {

	// This creates a temporary thread which is destroyed at the end of the expression (;)
	// This should never be done
	// std::thread(print);

	// print here decays into a function pointer -> void (*) ()
	std::thread thread1(print);
	thread1.join();

	std::thread myThread([] {
		printf("lamda thread\n");
		});
	myThread.join();
		
	std::cout << "Main thread" << std::endl;
}