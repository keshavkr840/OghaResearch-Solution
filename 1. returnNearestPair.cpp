#include<bits/stdc++.h>
using namespace std;

//assumed that no elements are duplicate
vector<pair<int,int>> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> map;
    vector<pair<int, pair<int,int>>> v;
    
    for (int i = 0;i<nums.size(); i++) {
        auto it = map.find(target - nums[i]);
        
        //if number is found then add them to vector v
        if (it != map.end()) 
            v.push_back({i-it->second-1,{it->first,nums[i]}});
        
        map[nums[i]] = i;
    }

    sort(v.begin(),v.end());
    
    int i= v.front().first;
    vector<pair<int,int>> answer;
    while(v.front().first==i){
        answer.push_back(v.front().second);
        if (v.size() > 0) 
            v.erase(v.begin());
    }
    
    return answer;
    
}

int main() {
    vector<int> nums = {1,5,3,6,4,2};
    int x = 7;
    twoSum(nums,x);
    return 0;
}
