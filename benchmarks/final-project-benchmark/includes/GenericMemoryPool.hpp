#ifndef GENERIC_MEMORY_POOL_H_
# define GENERIC_MEMORY_POOL_H_

#include <mutex>

#include <map>
#include <unordered_set>

#include <stdlib.h>

template<typename T>
class GenericMemoryPool
{
public:
    typedef T value_type;
    static unsigned int const TYPE_SIZE = sizeof(value_type);
    static unsigned int const NB_ALLOCATION = 100000;
private:
    GenericMemoryPool()
    {
        AllocNewChunk();
    }
    
    ~GenericMemoryPool()
    {
        assert(_retainMem.size() == 0);
    }
    
public:
    static GenericMemoryPool<T> * GetInstance() { return _instance; }
    static void DeleteInstance() { delete _instance; }
    static void CreateInstance() { _instance = new GenericMemoryPool<T>(); }
    
    void * Retain()
    {
        std::lock_guard<std::mutex> _(_mutex);
        
        void * memory = 0;
        
        if (_freeMem.size() == 0)
            AllocNewChunk();
        
        memory = *_freeMem.begin();
        _freeMem.erase(memory);
        _retainMem.insert(memory);
        //_memoryMap[memory] = false;
        
        return memory;
    }
    
    void Release(void * ptr)
    {
        std::lock_guard<std::mutex> _(_mutex);
        
        //assert(_memoryMap.find(ptr) != _memoryMap.end());
        _retainMem.erase(ptr);
        //_memoryMap[ptr] = true;
        _freeMem.insert(ptr);
    }
    
protected:
    void AllocNewChunk()
    {   
        void * memory = 0;
        
        for (unsigned int i = 0; i < NB_ALLOCATION; ++i)
        {
            memory = malloc(TYPE_SIZE);
            //_memoryMap[memory] = true;
            _freeMem.insert(memory);
        }
    }
private:
    
    std::map<void*, bool>  _memoryMap;
    std::unordered_set<void *>      _freeMem;
    std::unordered_set<void *>      _retainMem;
    
    std::mutex             _mutex;
    static GenericMemoryPool<T> * _instance;
};

template<typename T>
GenericMemoryPool<T> * GenericMemoryPool<T>::_instance = 0;

#endif