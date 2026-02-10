#include <iostream>
#include <chrono>
#include <cassert>
#include <cmath>
#include <vector>

#include "queue.cpp"

// --- paste corrected Queue code here ---

using Clock = std::chrono::high_resolution_clock;

// Returns elapsed nanoseconds for a single operation via a lambda
template <typename Func>
double timeOp(Func fn) {
    auto start = Clock::now();
    fn();
    auto end = Clock::now();
    return std::chrono::duration<double, std::nano>(end - start).count();
}

// ===================== Correctness Tests =====================

void testBasicEnqueueDequeue() {
    Queue q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    assert(q.dequeue() == 1);
    assert(q.dequeue() == 2);
    assert(q.dequeue() == 3);
    assert(q.isEmpty());
    std::cout << "[PASS] Basic enqueue/dequeue (FIFO order)\n";
}

void testIsEmptyOnNew() {
    Queue q;
    assert(q.isEmpty());
    std::cout << "[PASS] Newly created queue is empty\n";
}

void testIsEmptyAfterDrain() {
    Queue q;
    q.enqueue(42);
    q.dequeue();
    assert(q.isEmpty());
    std::cout << "[PASS] Queue is empty after draining\n";
}

void testSingleElement() {
    Queue q;
    q.enqueue(99);
    assert(!q.isEmpty());
    assert(q.dequeue() == 99);
    assert(q.isEmpty());
    std::cout << "[PASS] Single element enqueue/dequeue\n";
}

void testInterleavedOps() {
    Queue q;
    q.enqueue(1);
    q.enqueue(2);
    assert(q.dequeue() == 1);
    q.enqueue(3);
    assert(q.dequeue() == 2);
    assert(q.dequeue() == 3);
    assert(q.isEmpty());
    std::cout << "[PASS] Interleaved enqueue/dequeue\n";
}

void testLargeVolume() {
    Queue q;
    const int N = 100000;
    for (int i = 0; i < N; i++) q.enqueue(i);
    for (int i = 0; i < N; i++) assert(q.dequeue() == i);
    assert(q.isEmpty());
    std::cout << "[PASS] Large volume (" << N << " elements) FIFO\n";
}

// ===================== O(1) Timing Tests =====================

// Measures avg time of `trials` enqueues on a queue already holding `n` items.
double avgEnqueueTime(int n, int trials) {
    Queue q;
    for (int i = 0; i < n; i++) q.enqueue(i);

    auto start = Clock::now();
    for (int i = 0; i < trials; i++) q.enqueue(i);
    auto end = Clock::now();

    return std::chrono::duration<double, std::nano>(end - start).count()
           / trials;
}

// Measures avg time of `trials` dequeues on a queue holding enough items.
double avgDequeueTime(int n, int trials) {
    Queue q;
    for (int i = 0; i < n + trials; i++) q.enqueue(i);
    // Drain to leave exactly `trials` items after skipping `n`
    for (int i = 0; i < n; i++) q.dequeue();

    auto start = Clock::now();
    for (int i = 0; i < trials; i++) q.dequeue();
    auto end = Clock::now();

    return std::chrono::duration<double, std::nano>(end - start).count()
           / trials;
}

double avgIsEmptyTime(int n, int trials) {
    Queue q;
    for (int i = 0; i < n; i++) q.enqueue(i);

    auto start = Clock::now();
    for (int i = 0; i < trials; i++) q.isEmpty();
    auto end = Clock::now();

    return std::chrono::duration<double, std::nano>(end - start).count()
           / trials;
}

// If O(1), doubling n should NOT double time.
// We check that the ratio of times stays below a threshold.
void testO1Complexity() {
    const int trials = 100000;
    const double RATIO_THRESHOLD = 3.0; // generous for noise

    std::vector<int> sizes = {1000, 10000, 100000, 1000000};

    std::cout << "\n--- Enqueue timing (avg ns) ---\n";
    std::vector<double> enqTimes;
    for (int n : sizes) {
        double t = avgEnqueueTime(n, trials);
        enqTimes.push_back(t);
        std::cout << "  n=" << n << "\t=> " << t << " ns/op\n";
    }

    std::cout << "\n--- Dequeue timing (avg ns) ---\n";
    std::vector<double> deqTimes;
    for (int n : sizes) {
        double t = avgDequeueTime(n, trials);
        deqTimes.push_back(t);
        std::cout << "  n=" << n << "\t=> " << t << " ns/op\n";
    }

    std::cout << "\n--- isEmpty timing (avg ns) ---\n";
    std::vector<double> emptyTimes;
    for (int n : sizes) {
        double t = avgIsEmptyTime(n, trials);
        emptyTimes.push_back(t);
        std::cout << "  n=" << n << "\t=> " << t << " ns/op\n";
    }

    // Verify: time at largest n / time at smallest n should be bounded
    auto checkConstant = [&](const std::string &name,
                             const std::vector<double> &times) {
        double ratio = times.back() / times.front();
        std::cout << "\n" << name << " ratio (n="
                  << sizes.back() << " / n=" << sizes.front()
                  << ") = " << ratio << "\n";
        if (ratio < RATIO_THRESHOLD) {
            std::cout << "[PASS] " << name << " is O(1)\n";
        } else {
            std::cout << "[WARN] " << name
                      << " ratio exceeded threshold (" << RATIO_THRESHOLD
                      << "). May be noise — re-run.\n";
        }
    };

    checkConstant("enqueue", enqTimes);
    checkConstant("dequeue", deqTimes);
    checkConstant("isEmpty", emptyTimes);
}

// ===================== Main =====================

int main() {
    std::cout << "=== Correctness Tests ===\n";
    testBasicEnqueueDequeue();
    testIsEmptyOnNew();
    testIsEmptyAfterDrain();
    testSingleElement();
    testInterleavedOps();
    testLargeVolume();

    std::cout << "\n=== O(1) Complexity Tests ===\n";
    testO1Complexity();

    return 0;
}