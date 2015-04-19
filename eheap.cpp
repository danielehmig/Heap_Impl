#include "stdafx.h"
#include "eheap.h"

using namespace std;

namespace ehmig_heap
{
	/*
	=========================================
	eheap

	Constructor that constructs an empty heap
	=========================================
	*/
	eheap::eheap()
	{
		m_size = 0;
		root = NULL;
		previous = NULL;
	}

	/*
	=============================================
	insert

	Insert a new item into the heap
	=============================================
	*/
	void eheap::insert(Item data)
	{
		p_heap_node node = new_node(data);
		++m_size;
		// CASE 1: We are inserting the first item
		if(root == NULL)
		{
			root = node;
			previous = root;
		}
		// CASE 2: We are inserting at the second level,
		// and the left child is the new position.
		else if(root->left == NULL)
		{
			root->left = node;
			node->parent = root;
			previous = node;
			min_heapify(node);
		}
		// CASE 3: We are inserting at the second level,
		// and the right child is the new position.
		else if(root->right == NULL)
		{
			root->right = node;
			node->parent = root;
			previous = node;
			min_heapify(node);
		}
		// We are inserting past the second level
		else
		{
			// CASE 4: The previously inserted node was 
			// the left child of its parent.
			if(previous == previous->parent->left)
			{
				previous->parent->right = node;
				node->parent = previous->parent;
				previous = node;
				min_heapify(node);
			}
			// CASE 5: The previously inserted node was
			// the right child of its parent.
			else
			{
				// SUB-CASE A: The current level is not complete.
				// TODO: Find a way around the lg(n) far_right() call.
				if(previous != far_right())
				{
					p_heap_node p = ((previous->parent)->parent)->right;
					p->left = node;
					previous = node; // THERE MIGHT BE DRAGONS HERE......?
					node->parent = p;
					min_heapify(node);
				}
				// SUB-CASE B: The current level is complete, and
				// we need a new level.
				else
				{
					// We have to insert on the far-left on a new level
					insert_farleft(node);
				}

			}
		}
	}

	/*
	================================================
	insert_farleft

	When we've hit case 5b for insertion, we need to
	add a new level to the tree and insert there.
	================================================
	*/
	void eheap::insert_farleft(p_heap_node node)
	{
		// By some twist of logic, the tree could be empty.
		if(root == NULL)
		{
			return;
		}

		p_heap_node prev = root;
		while(prev->left != NULL)
		{
			prev = prev->left;
		}
		prev->left = node;
		node->parent = prev;
		previous = node;
		min_heapify(node);
	}
	
	/*
	============================================
	far_right

	Follows every right child until it finds the
	last non-null right child.
	============================================
	*/
	p_heap_node eheap::far_right()
	{
		// The tree is empty.
		if(root == NULL)
		{
			return NULL;
		}
		
		// The heap contains at least one node.
		p_heap_node prev = root;
		while(prev->right != NULL)
		{
			prev = prev->right;
		}
		return prev;
	}

	/*
	===========================================
	far_left

	Follows every left child until it finds the
	last non-null left child.
	===========================================
	*/
	p_heap_node eheap::far_left()
	{
		// The tree is empty.
		if(root == NULL)
		{
			return NULL;
		}

		p_heap_node prev = root;
		while(prev->left != NULL)
		{
			prev = prev->left;
		}
		return prev;
	}

	/*
	=================================================
	min_heapify

	Helper function that restores the heap properties
	=================================================
	*/
	void eheap::min_heapify(p_heap_node node)
	{
		// CASE 1: We have an empty heap.
		if(root == NULL)
		{
			return;
		}
		// CASE 2: The function was invoked with a NULL argument.
		if(node == NULL)
		{
			return;
		}
		// CASE 3: The function was invoked with the root; 
		// we hit the base case.
		if(node == root)
		{
			return;
		}
		p_heap_node current = node;
		// CASE 4: We are working our way up from some node 
		// other than the root to the root.
		if(less(current->data, current->parent->data))
		{
			Item temp = current->data;
			current->data = current->parent->data;
			current->parent->data = temp;
			min_heapify(current->parent);
		}
	}


	/*
	=========================================================
	heapify_down

	Restore the heap properties after a removal has occurred.
	=========================================================
	*/
	void eheap::heapify_down(p_heap_node node)
	{
		// CASE 1: This is our (retarded) base case; 
		// we don't need to go down further.
		if(node == NULL)
		{
			return;
		}
		// CASE 2: This our real base case; the left child is 
		// NULL. Thus, we can stop.
		if(node->left == NULL)
		{
			return;
		}
		// CASE 3: This is our recursive case. We compare the node
		// to its two children to see which one to swap with.
		if(node->right != NULL)
		{
			if(less(node->left->data, node->data) || 
				less(node->right->data, node->data))
			{
				if(less(node->left->data, node->right->data))
				{
					Item temp = node->left->data;
					node->left->data = node->data;
					node->data = temp;
					heapify_down(node->left);
				}
				else
				{
					Item temp = node->right->data;
					node->right->data = node->data;
					node->data = temp;
					heapify_down(node->right);
				}
			}
		}
		// CASE 4: We only have a left child. This is
		// the last level of the recursion.
		else
		{
			if(less(node->left->data, node->data))
			{
				Item temp = node->left->data;
				node->left->data = node->data;
				node->data = temp;
				// TODO: Do we need to actually make this recursive call?
				heapify_down(node->left);
			}
		}

	}

	/*
	========================================
	remove

	Remove the topmost item on the heap, and 
	restore the heap properties.
	========================================
	*/
	Item eheap::remove()
	{
		// ********************* HERE THERE BE DRAGONS *********************

		// CASE 1: The heap is empty. We have to return
		// something, so return an invalid item.
		if(isEmpty())
		{
			return INVALID_ITEM;
		}
		Item ret_val = root->data;
		// CASE 2: Node to replace root is the root itself.
		// Just remove the root and be done with it.
		if(m_size ==  1)
		{
			delete root;
			root = NULL;
			previous = NULL;
		}
		// CASE 3: The node to replace the root is the right
		// child of the root.
		else if(previous == root->right)
		{
			root->right = NULL;
			previous->left = root->left;
			delete root;
			root = previous;
			root->parent = NULL;
			previous = root->left;
			// We might need to heapify.
			if(less(root->left->data, root->data))
			{
				Item temp = root->left->data;
				root->left->data = root->data;
				root->data = temp;
			}
		}
		// CASE 4: The node to replace the root is the left
		// child of the root.
		else if(previous == root->left)
		{
			root->left = NULL;
			delete root;
			root = previous;
			root->parent = NULL;
		}
		// CASE 5: The node to replace the root is the right child
		// of its parent.
		else if(previous == previous->parent->right)
		{
			root->data = previous->data;
			p_heap_node new_prev = previous->parent->left;
			previous->parent->right = NULL;
			delete previous;
			previous = new_prev;
			heapify_down(root);
		}
		// CASE 6: The node to replace the root is the left child
		// of its parent.
		else
		{
			// SUB-CASE A: The node to replace the root is the
			// farthest one left on its level.
			if(previous == far_left())
			{
				root->data = previous->data;
				p_heap_node temp = previous;
				previous = far_right();
				temp->parent->left = NULL;
				delete temp;
				heapify_down(root);
			}
			else
			{
				root->data = previous->data;
				p_heap_node temp = previous;
				previous = (((previous->parent)->parent)->left)->right;
				temp->parent->left = NULL;
				delete temp;
				heapify_down(root);
			}
		}
		--m_size;
		return ret_val;

	}

	/*
	=========================================
	in_order

	Perform an in order traversal of the heap
	=========================================
	*/
	void eheap::in_order(p_heap_node node)
	{
		if(node != NULL)
		{
			in_order(node->left);
			std::cout << node->data << "\n";
			in_order(node->right);
		}
	}

	void eheap::reset()
	{

	}

	void eheap::decrease_key(p_heap_node node)
	{

	}

	void eheap::increase_key(p_heap_node node)
	{

	}

	/*
	====================================================
	new_node

	Allocates and returns a new node with the given data
	====================================================
	*/
	p_heap_node eheap::new_node(Item ndata)
	{
		p_heap_node node = new heap_node;
		node->data = ndata;
		node->left = NULL;
		node->right = NULL;
		node->parent = NULL;
		return node;
	}


}