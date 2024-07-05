#pragma once

struct ListNode {
	int Val;
	ListNode* Next;
	ListNode() : Val(0), Next(nullptr) {}
	ListNode(int x) : Val(x), Next(nullptr) {}
	ListNode(int x, ListNode* next) : Val(x), Next(next) {}
};