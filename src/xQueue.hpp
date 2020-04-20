#pragma once

#include <deque>
// DUMMY - xQueue impl
namespace wthermo {
    using size_t = unsigned int;

template <size_t CAPACITY, typename T>
class Queue {
    private:
    std::deque<T> mData;
    size_t mItemCount{0};

    public:
    Queue() {
        //mData.reserve(SIZE);
    }

    bool emplace_back(T item)
    {
        if (mItemCount >= CAPACITY)
            return false; 
        
        mData.emplace_back(std::move(item));
        mItemCount++;
        return true;
    }
    bool push_back(const T& item)
    {
        if (mItemCount >= CAPACITY)
            return false; 
        
        mData.push_back(item);
        mItemCount++;
        return true;
    }
    bool consume_front(T* buffer)
    {
        if (mItemCount<1)
            return false;
        mItemCount--;
        *buffer = mData.front();
        mData.pop_front();
        return true;
    }

};

// helpers
template <size_t CAPACITY, typename T>
Queue<CAPACITY, T> xQueueCreate()
{
    Queue<CAPACITY, T> result;
    return result;
}

}
// #define xQueueCreate(xQueueCapacity, pvItemSize) DONT DO THIS - we'll lose type information!

#define xQueueSendToBack(xQueuePtr, pvItemToQueue, xTicksToWait) /* ignore xTicksToWait? */ xQueuePtr->push_back(pvItemToQueue);
#define xQueueGenericSend(xQueuePtr, pvItemToQueue, xTicksToWait) xQueueSendToBack(xQueuePtr, pvItemToQueue, xTicksToWait)
#define xQueueSend(xQueuePtr, pvItemToQueue, xTicksToWait) xQueueGenericSend(xQueuePtr, pvItemToQueue, xTicksToWait)

#define xQueueGenericReceive(xQueuePtr, pvBuffer, xTicksToWait) xQueuePtr->consume_front(pvBuffer)
#define xQueueReceive(xQueuePtr, pvBuffer, xTicksToWait) xQueueGenericReceive(xQueuePtr, pvBuffer, xTicksToWait)
#define pdFALSE false