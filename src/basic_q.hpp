
#ifndef BASIC_Q_HPP
#define BASIC_Q_HPP

#include <atomic>
#include <memory>

/**
 * @brief A lock-free multi-producer multi-consumer (MPMC) queue.
 * 
 * Implementation based on the Michael & Scott (1996) algorithm.
 * Uses a singly linked list with a dummy node. 
 * Thread-safe for concurrent enqueue and dequeue.
 * 
 * NOTE: For production, consider using a custom memory reclamation
 * strategy (hazard pointers, epoch-based GC) instead of shared_ptr,
 * since shared_ptr can be relatively slow under contention.
 */
template <typename T>
class BasicQueue {
private:
    struct Node {
        std::shared_ptr<T> data;
        std::atomic<Node*> next;

        Node() : next(nullptr) {}
        Node(T value) 
            : data(std::make_shared<T>(std::move(value))), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;

public:
    BasicQueue() {
        Node* dummy = new Node(); // initial dummy node
        head.store(dummy);
        tail.store(dummy);
    }

    ~BasicQueue() {
        // Clean up all remaining nodes
        while (Node* old_head = head.load()) {
            head.store(old_head->next);
            delete old_head;
        }
    }

    /**
     * @brief Enqueue a new value into the queue.
     * 
     * @param value Value to insert (moved).
     */
    void enqueue(T value) {
        Node* new_node = new Node(std::move(value));
        Node* old_tail;

        while (true) {
            old_tail = tail.load();
            Node* next = old_tail->next.load();

            if (old_tail == tail.load()) {
                if (next == nullptr) {
                    // Try to link new_node at the end
                    if (old_tail->next.compare_exchange_weak(next, new_node)) {
                        // Swing tail to the new node
                        tail.compare_exchange_weak(old_tail, new_node);
                        return;
                    }
                } else {
                    // Tail is behind, advance it
                    tail.compare_exchange_weak(old_tail, next);
                }
            }
        }
    }

    /**
     * @brief Dequeue a value from the queue.
     * 
     * @return std::shared_ptr<T> containing the dequeued value,
     * or nullptr if the queue was empty.
     */
    std::shared_ptr<T> dequeue() {
        Node* old_head;

        while (true) {
            old_head = head.load();
            Node* old_tail = tail.load();
            Node* next = old_head->next.load();

            if (old_head == head.load()) {
                if (old_head == old_tail) {
                    if (next == nullptr) {
                        return nullptr; // queue empty
                    }
                    // Tail is behind, advance it
                    tail.compare_exchange_weak(old_tail, next);
                } else {
                    // Try to swing head forward
                    if (head.compare_exchange_weak(old_head, next)) {
                        std::shared_ptr<T> res = next->data;
                        next->data.reset(); // release ownership
                        delete old_head;
                        return res;
                    }
                }
            }
        }
    }

    // Non-copyable, non-movable (safe for concurrent usage)
    BasicQueue(const BasicQueue&) = delete;
    BasicQueue& operator=(const BasicQueue&) = delete;
    BasicQueue(BasicQueue&&) = delete;
    BasicQueue& operator=(BasicQueue&&) = delete;
};

#endif // BASIC_Q_HPP
