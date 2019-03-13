/*
 * demo.cpp
 *
 *  Created on: Mar 13, 2019
 *      Author: QUAN
 */



#include <iostream>
#include <string.h>
#include <vector>

struct node {
	int val;
	struct node * next;
};
typedef struct node node_type;

class Solution {
public:
	~Solution();
	void show_list(node_type * list);
	bool list_is_empty(node_type * list);
	node_type * list_x_node(int length);
	node_type * list_init(int length);
	void list_insert_head(node_type * list_head, node_type * new_node);
	void list_insert_tail(node_type * list_head, node_type * new_node);

	void set_node_number(int node_number);
	int read_node_number(void);

private:
	node_type* list_head_init(int val);
	int node_length;
	int head_node_value = -1;
};


int main(void)
{
	using namespace std;

	Solution solution;

	node_type * list = solution.list_init(12);
	if(nullptr == list) {
		cout << "----- init error" << endl;
	}
	solution.show_list(list);
	return 0;
}

Solution::~Solution(void)
{
	std::cout << "------------------ start exit protocol ------------------" << std::endl;

}

bool Solution::list_is_empty(node_type * list)
{
	if(list)	// if list is empty
		return false;
	else
		return true;
}

void Solution::show_list(node_type * list)
{
	bool status = Solution::list_is_empty(list);
	if(status)
		std::cout <<"------> list is empty" << std::endl;
	else {
		std::cout << "List:" << std::endl;
		std::cout << "\t";
		for(node_type *node = list; node != nullptr; node = node->next) {
			std::cout << "--->";
			std::cout << node->val;

		}
		std::cout << std::endl;
	}
}

node_type * Solution::list_init(int length)
{
	std::cout << "------------------ start init list ------------------" << std::endl;


	node_type * head = Solution::list_head_init(Solution::head_node_value);

	for(int index = 0; index < length; index++) {
		node_type * new_node = new (node_type);
		if(!new_node) {
			std::cout << "error : malloc new_node" << std::endl;
			return nullptr;
		}
		new_node->val = index + 1;
		Solution::list_insert_tail(head, new_node);
	}
	Solution::set_node_number(length);
	return head;
}

void Solution::set_node_number(int list_node_number)
{
	Solution::node_length = list_node_number;
}

void Solution::list_insert_head(node_type * list_head, node_type * new_node)
{
	new_node->next = list_head->next;
	list_head->next = new_node;
}

void Solution::list_insert_tail(node_type * list_head, node_type * new_node)
{
	while(nullptr != list_head->next)
	{
		list_head = list_head->next;
	}
	new_node->next = list_head->next;
	list_head->next = new_node;
}

node_type* Solution::list_head_init(int val)
{
	node_type * list_head = new (node_type);
	if(!list_head) {
		std::cout << "error : malloc list" << std::endl;
		return nullptr;
	}
	list_head->val = -1;
	list_head->next = nullptr;

	return list_head;
}

int Solution::read_node_number(void)
{
	return Solution::node_length;
}
