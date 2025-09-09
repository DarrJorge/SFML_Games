#include <iostream>
#include <vector>
#include <optional>
#include <chrono>
#include <list>
#include <map>

#include "src/ZombieArenaGame.h"

struct CustomNode
{
    CustomNode(int val) : value(val), prev(nullptr), next(nullptr){}
    CustomNode *prev;
    CustomNode *next;
    int value;
};

template<class T>
class allocator
{
public:
    using value_type = T;

    T* allocate(size_t n) // count elements
    {
        std::cout << "Items: " << n << " allocate memory: " << n * sizeof(T) << " bytes" << std::endl;
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* ptr, size_t n)
    {
        std::cout << "Deallocate memory: " << n * sizeof(T) << " bytes" << std::endl;
        ::operator delete(ptr);
    }
};

template<class T>
class ArenaAllocator
{
    std::size_t size;

    char* start;
    char* current;

    int* counter;

public:

    using value_type = T;

    explicit ArenaAllocator(size_t bytes = 1024 * 1024)
        : size(bytes), start(static_cast<char*>(operator new(bytes))),
        current(start), counter(new int(1))
        {}

    ArenaAllocator(const ArenaAllocator& other)
    {
        std::cout << "ArenaAlloc! Constructor copy" << std::endl;
        this->start = other.start;
        this->current = other.current;
        this->size = other.size;
        this->counter = other.counter;
        (*counter)++;

    }

    void deleter()
    {
        (*counter)--;
        if (*counter == 0)
        {
            operator delete(start);
            delete counter;
        }
    }

    ~ArenaAllocator()
    {
        deleter();
    }

    T* allocate(std::size_t n)
    {
        std::size_t bytes = n * sizeof(T);
        auto return_memory = current;
        current += bytes;
        return reinterpret_cast<T*>(return_memory);
    }

    void deallocate(T*, std::size_t ) noexcept {}

    ArenaAllocator select_on_container_copy_construction() const
    {
        std::cout << "select_on_container_copy_construction" << std::endl;
        return ArenaAllocator();
    }

    template <class U>
    bool operator==(ArenaAllocator<U> const& other) noexcept
    {
        return this->start == other.start;
    }

    ArenaAllocator& operator=(const ArenaAllocator& other)
    {
        std::cout << "operator=" << std::endl;

        if (this == &other) return *this;

        deleter();

        this->start = other.start;
        this->current = other.current;
        this->size = other.size;
        this->counter = other.counter;
        (*counter)++;

        return *this;
    }

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::false_type;
};

int main()
{
    ZombieArenaGame game;
    game.run();

    return 0;
}