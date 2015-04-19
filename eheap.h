/*
	CLASS: eheap
	AUTHOR: Daniel "FoxAcid" Ehmig
	DATE: Isabelle's Birthday
	PURPOSE: The purpose of this class is to implement the heap data
			 structure in a user-defined data type (class) in C++. The
			 heap can then be used by a programmer who wants to implement
			 heapsort in their program. The heap will be structured in a way
			 such that the MINIMUM-priority item will be at the top of the heap.
*/

#ifndef _EHEAP_H_
#define _EHEAP_H_
#include "stdafx.h"

#define less(a, b) a < b
#define INVALID_ITEM -1

namespace ehmig_heap
{
	typedef int Item;

	// Defines a node in the binary tree
	// representation of the heap
	typedef struct _heap_node
	{
		Item data;
		struct _heap_node * parent;
		struct _heap_node * left;
		struct _heap_node * right;
	} heap_node, *p_heap_node;

	class eheap
	{
	public:

		// Public Member Functions
		eheap();
		void insert(Item data);
		Item remove();
		void reset();
		void in_order(p_heap_node node);
		p_heap_node get_root() { return root; }
		bool isEmpty() { return (m_size == 0); }
		std::size_t size() { return m_size; }

	private:
		// Member Variables
		std::size_t m_size;
		p_heap_node root;
		p_heap_node previous;

		// Private Member Functions
		void min_heapify(p_heap_node node);
		void heapify_down(p_heap_node node);
		void decrease_key(p_heap_node node);
		void increase_key(p_heap_node node);
		p_heap_node new_node(Item ndata);
		p_heap_node far_right();
		p_heap_node far_left();
		void insert_farleft(p_heap_node node);
	};
}
#endif