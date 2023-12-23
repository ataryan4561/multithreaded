#include<bits/stdc++.h>
#include<mutex>
#include<thread>
#include<chrono>
#include<algorithm>
using namespace std;
using namespace std::chrono;
std::mutex mu;
int counter;
void Counter_Init()
{
    mu.lock();
    counter =0;
    mu.unlock();
}
int Counter_GetValue()
{
    return counter;
}
void Counter_Increment()
{
    mu.lock();
    counter++;
    mu.unlock();
}
void Counter_Decrement()
{
    mu.lock();
    counter--;
    mu.unlock();
}
int main()
{
    std:: thread t1(Counter_Init);
    t1.join();
    vector<std::thread> g;
    vector<std::thread> g1;
    for(int i=0; i<50; i++)
    {
        std::thread t2(Counter_Increment);
        g.push_back(move(t2));
        g[i].join();
    }
    for(int i=0; i<25; i++)
    {
        std::thread t3(Counter_Decrement);
        g1.push_back(move(t3));
        g1[i].join();
    }
    cout<<Counter_GetValue()<<endl;
}