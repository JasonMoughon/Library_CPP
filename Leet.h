#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "ListNode.h"

class Leet {
public:
    std::vector<int> FindTargetSum(std::vector<int>& nums, int target) {
        std::unordered_map<int, int> hashMap;

        for (int i = 0; i < nums.size(); i++)
        {
            int result = target - nums[i];
            //if (hashMap.contains(result)) return { hashMap[result], i };

            hashMap[nums[i]] = i;
        }
        return {};
    }

    ListNode* AddTwoListNodes(ListNode* l1, ListNode* l2)
    {
        ListNode* dummyHead = new ListNode(0);
        ListNode* tail = dummyHead;
        int carry = 0;

        while (l1 != nullptr || l2 != nullptr || carry != 0) {
            int digit1 = (l1 != nullptr) ? l1->Val : 0;
            int digit2 = (l2 != nullptr) ? l2->Val : 0;

            int sum = digit1 + digit2 + carry;
            int digit = sum % 10;
            carry = sum / 10;

            ListNode* newNode = new ListNode(digit);
            tail->Next = newNode;
            tail = tail->Next;

            l1 = (l1 != nullptr) ? l1->Next : nullptr;
            l2 = (l2 != nullptr) ? l2->Next : nullptr;
        }

        ListNode* result = dummyHead->Next;
        delete dummyHead, tail;
        return result;
    }

    std::string AddBinaryStrings(std::string a, std::string b) {
        //Check Edge Cases
        if (a.size() == 0)
        {
            if (b.size() == 0) return "";
            return b;
        }
        if (b.size() == 0) return a;

        std::string result = "";
        int indexA = a.size() - 1;
        int indexB = b.size() - 1;
        char aChar = '0';
        char bChar = '0';
        char carry = '0';

        while (indexA >= 0 || indexB >= 0)
        {
            aChar = (indexA >= 0) ? a[indexA] : '0';
            bChar = (indexB >= 0) ? b[indexB] : '0';
            char newChar = (((aChar - '0') + (bChar - '0') + (carry - '0')) + '0');

            if (newChar >= '2') newChar -= 2;

            result = newChar + result;
            carry = ((aChar - '0') + (bChar - '0') + (carry - '0') >= 2) ? '1' : '0';

            indexA--;
            indexB--;
        }

        if (carry == '1') result = carry + result;
        return result;
    }
};