#include <iostream>
#include <thread>
#include <vector>

void print() {
	std::cout << "Hello from thread" << std::endl;
}

int main() {

	// This creates a temporary thread which is destroyed at the end of the expression (;)
	// This should never be done
	// std::thread(print);

	// print here decays into a function pointer -> void (*) ()
	/*std::thread thread1(print);
	thread1.join();

	std::thread myThread([] {
		printf("lamda thread\n");
		});
	myThread.join();*/

	auto lambda = [&](int i) {
		// '::' is a scope resolution operator. It is used when accessing something that belongs to a scope, not an object. Eg: things like static, global members, variables. Mostly used with namespaces (because namespaces are scopes)
		// '.' is used when an object is accessing a class member/variable
		// There is a diff b/w get_id and get_id(), get_id refers to the function itself & get_id() will return the result of running the function
		std::cout << "Hello from thread" << i << ": " << std::this_thread::get_id() << std::endl;
		};

	std::vector<std::thread> threads;

	/*for (int i = 0; i < 10; i++) {
		threads.push_back(std::thread(lambda, i));
	}

	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}*/
	
	// jthreads follow the RAII principle. These do not have to be joined explicitly. The destructor for them is called at the end of the scope they are called in. (RAII)
	// we can still explicitly call the join method earlier if we want to and if such is our use case.
	std::vector<std::jthread> jThreads;
	// jthreads usually have a overhead. They need to enter the lambda function, get lock for iostream and a couple other things
	// printing "Main thread" is usually faster, that's why it always prints first
	for (int i = 0; i < 5; i++) {
		jThreads.push_back(std::jthread(lambda, i));
	}


	{
		// if i instantiate the jthreads in a scope like this, they will be destroyed after the scope ends.
		// if we do this, "Main thread" will always print last, since join will called for each thread at the end of this scope
		for (int i = 0; i < 5; i++) {
			jThreads.push_back(std::jthread(lambda, i));
		}
	}

		
	std::cout << "Main thread" << std::endl;
}