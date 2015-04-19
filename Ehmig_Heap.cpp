// Ehmig_Heap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "eheap.h"
using namespace ehmig_heap;
#define NUM_ITEMS 100

int _tmain(int argc, _TCHAR* argv[])
{
	// Generate 100 random numbers, and insert them into the heap
	eheap test_heap;
	std::size_t i = 0;
	for(i = 0; i < NUM_ITEMS; ++i)
	{
		int new_val = (int)(rand() % 100);
		test_heap.insert(new_val);
	}

	std::cout << "Heap Size: " << test_heap.size() << "\n\n";
	// Test the heap sorting functionality
	//test_heap.in_order(test_heap.get_root());
	
	for(i = 0; i < NUM_ITEMS; ++i)
	{
		std::cout << test_heap.remove() << " ";
		if(i % 10 == 0)
			std::cout << "\n";
	}
	
	std::cout << "\n";
	return EXIT_SUCCESS;
}

