#include<bits/stdc++.h>
#include<mutex>
#include<thread>
#include<chrono>
#include<algorithm>
#include<list>
using namespace std;
using namespace std::chrono;
vector<bool> check(10000,false);
std::mutex mu;
class HT
{
    private:
    list<int> *table;
    int elements;
    int count;
    int getHash(int x)
    {
        return x % elements;
    }
    public:
    HT()
    {

    }
    void Hash_Init(int numOfBuckets)
    {
        mu.lock();
        elements = numOfBuckets;
        table = new list<int>[elements];
        count=0;
        mu.unlock();
    }
    int Hash_Insert(int x)
    {
        mu.lock();
        if(count==elements)
        {
            mu.unlock();
            return -1;
        } 
        if(!check[x])
        {
            check[x]=true;
            table[getHash(x)].push_back(x);
            count++;
            mu.unlock();
            return 0;
        }
        mu.unlock();
        return -1;
    }
    int Hash_Remove(int x)
    {
        mu.lock();
        int val = getHash(x);
        if(!check[x])
        {
            mu.unlock();
            return -1;
        }
        list<int>:: iterator i;
        for(i = table[val].begin(); i!=table[val].end(); i++)
        {
            if(*i==x)
                break;
        }
        if(i!=table[val].end())
        {
            table[x].erase(i);
            count--;
            check[x]=false;
            mu.unlock();
            return 0;
        }
        mu.unlock();
        return -1;
    }
    int Hash_CountElements()
    {
        return count;
    }
    int Hash_CountBucketElements(int bucketNumber)
    {
        return table[bucketNumber].size();
    }
};
int main()
{
    HT hasht;
    std::thread t1(&HT::Hash_Init,&hasht,10);
    t1.join();
    // hasht.Hash_Init(10);
    vector<std::thread> g;
    for(int i=0; i<12; i++)
    {
        std::thread t2(&HT::Hash_Insert,&hasht,i);
        g.push_back(move(t2));
        g[i].join();
        // hasht.Hash_Insert(i);
    }
    cout<<hasht.Hash_CountElements()<<endl;
    cout<<hasht.Hash_CountBucketElements(5)<<endl;
    std::thread t3(&HT::Hash_Remove,&hasht,9);
    t3.join();
    std::thread t4(&HT::Hash_Remove,&hasht,6);
    t4.join();
    // cout<<hasht.Hash_Remove(9)<<endl;
    cout<<hasht.Hash_CountElements()<<endl;
    cout<<hasht.Hash_CountBucketElements(6)<<endl;
}