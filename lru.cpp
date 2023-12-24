#include<bits/stdc++.h>
#include<mutex>
#include<thread>
#include<chrono>
#include<algorithm>
#include<list>
#include<unordered_map>
using namespace std;
using namespace std::chrono;
std::mutex mu;
void dosome(int seconds)
{
    std::this_thread::sleep_for(chrono::seconds(seconds));
}
class LRUCache
{
    private:
    list<int> dq;
    unordered_map<int,list<int>::iterator> map;
    int size;
    int count;
    public:
    LRUCache()
    {}
    void LRU_Init(int size)
    {
        mu.lock();
        this->size = size;
        count=0;
        mu.unlock();
    }
    int LRU_Insert(int element)
    {
        mu.lock();
        if(element<=0)
        {
            mu.unlock();
            return -1;
        }
        if(map.find(element) != map.end())
        {
            dq.erase(map[element]);
            mu.unlock();
            return -1;
        }
        if(map.find(element) == map.end())
        {
            if(dq.size()==size)
            {
                int last = dq.back();
                dq.pop_back();
                map.erase(last);
            }
        }
        dq.push_front(element);
        map[element] = dq.begin();
        if(count!= size)
            count++;
        mu.unlock();
        return 0;
    }
    int LRU_Access(int element)
    {
        mu.lock();
        if(map.find(element) == map.end())
        {
            mu.unlock();
            return -1;
        }
        if(map.find(element) != map.end())
        {
            dq.erase(map[element]);
        }
        dq.push_front(element);
        map[element] = dq.begin();
        mu.unlock();
        return 0;
    }
    int LRU_Remove(int element)
    {
        mu.lock();
        if(map.find(element) == map.end())
        {
            mu.unlock();
            return -1;
        }
        dq.erase(map[element]);
        count--;
        mu.unlock();
        return 0;
    }
    int LRU_Size()
    {
        return count;
    }
};
int main()
{
    LRUCache l1;
    std::thread t1(&LRUCache::LRU_Init,&l1,10);
    t1.join();
    // l1.LRU_Init(10);
    vector<std::thread> g;
    for(int i=0; i<12; i++)
    {
        std::thread t2(&LRUCache::LRU_Insert,&l1,i);
        g.push_back(move(t2));
        g[i].join();
        // l1.LRU_Insert(i);
    }
    std::thread t3(&LRUCache::LRU_Access,&l1,6);
    t3.join();
    // cout<<l1.LRU_Access(6)<<endl;
    std::thread t4(&LRUCache::LRU_Remove,&l1,3);
    t4.join();
    // cout<<l1.LRU_Remove(3)<<endl;
    cout<<l1.LRU_Size();
    return 0;
}
