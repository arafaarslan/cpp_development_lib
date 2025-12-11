#include <cassert>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include "ThreadSafeStack.h"

static void testTryPopOnEmpty() {
    ThreadSafeStack<int> q;
    int out = -1;
    bool ok = q.try_pop(out);
    assert(!ok);
}

static void testPushAndSize() {
    ThreadSafeStack<int> q;
    q.push(1);
    q.push(2);
    assert(q.size() == 2);
    assert(!q.empty());
}

static void testPushThenPop() {
    ThreadSafeStack<int> q;
    q.push(42);
    int out = 0;
    bool ok = q.try_pop(out);
    assert(ok);
    assert(out == 42);
    assert(q.empty());
}

static void testConcurrentProducersConsumers() {
    ThreadSafeStack<int> q;

    const int producers = 4;
    const int consumers = 4;
    const int per_producer = 1000;
    const int total = producers * per_producer;

    std::atomic<int> produced{0};
    std::atomic<int> consumed{0};

    std::vector<std::thread> ps;
    ps.reserve(producers);
    for (int p = 0; p < producers; ++p) {
        ps.emplace_back([&]() {
            for (int i = 0; i < per_producer; ++i) {
                q.push(i);
                produced.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    std::vector<std::thread> cs;
    cs.reserve(consumers);
    for (int c = 0; c < consumers; ++c) {
        cs.emplace_back([&]() {
            int value;
            while (consumed.load(std::memory_order_relaxed) < total) {
                if (q.try_pop(value)) {
                    consumed.fetch_add(1, std::memory_order_relaxed);
                } else {
                    std::this_thread::sleep_for(std::chrono::microseconds(50));
                }
            }
        });
    }

    for (auto &t : ps) t.join();
    for (auto &t : cs) t.join();

    assert(produced.load() == total);
    assert(consumed.load() == total);
    assert(q.empty());
}

int main() {
    testTryPopOnEmpty();
    testPushAndSize();
    testPushThenPop();
    testConcurrentProducersConsumers();

    std::cout << "All ThreadSafeQueue tests passed.\n";
    return 0;
}


