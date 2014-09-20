//
//  Queue.h
//  GEDebug
//
//  Created by Andrew Shidel on 8/25/14.
//  Copyright (c) 2014 user. All rights reserved.
//

#ifndef GEDebug_Queue_h
#define GEDebug_Queue_h

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class Queue
{
public:
    Queue(const Queue& rhs) {}
    Queue(){
        _size = 0;
    }
    
    T pop()
    {
        _size--;
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        auto item = queue_.front();
        queue_.pop();
        return item;
    }
    
    void pop(T& item)
    {
        _size--;
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        item = queue_.front();
        queue_.pop();
    }
    T back(){
        auto item = queue_.back();
        return item;
    }

    void push(const T& item)
    {
        _size++;
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(item);
        mlock.unlock();
        cond_.notify_one();
    }
    
    void push(T&& item)
    {
        _size++;
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(std::move(item));
        mlock.unlock();
        cond_.notify_one();
    }
    int size(){
        return _size;
    }
    void remove(T item){
        _size--;
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        queue_.remove(item);
    }
    
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
    int _size;
};

#endif
