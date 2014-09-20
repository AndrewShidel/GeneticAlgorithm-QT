//
//  Map.h
//  GEDebug
//
//  Created by Andrew Shidel on 8/26/14.
//  Copyright (c) 2014 user. All rights reserved.
//

#ifndef GEDebug_Map_h
#define GEDebug_Map_h

#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T, typename E>
class Map
{
public:
    Map(const Map& rhs) {}
    Map(){
        size_=0;
    }
    E get(T key){
        std::unique_lock<std::mutex> mlock(mutex_);
        auto item = map_[key];
        mlock.unlock();
        return item;
    }
    void erase(T key){
        size_--;
        std::unique_lock<std::mutex> mlock(mutex_);
        map_.erase(key);
        mlock.unlock();
    }
    void set(T key, E val){
        std::unique_lock<std::mutex> mlock(mutex_);
        size_++;
        map_[key] = val;
        mlock.unlock();
    }
    int size(){
        return size_;
    }
    
private:
    std::mutex mutex_;
    std::condition_variable cond_;
    std::map<T,E> map_;
    int size_;
};

#endif
