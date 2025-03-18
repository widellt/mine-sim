#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

/**
 * A thread pool for parallelizing truck operations in the lunar mining simulation.
 * Uses a fixed number of worker threads to process tasks from a shared queue.
 */
class ThreadPool {
public:
    /**
     * Constructor initializes the thread pool with the specified number of worker threads.
     * 
     * @param numThreads Number of worker threads to create (defaults to hardware concurrency)
     */
    ThreadPool(size_t numThreads = std::thread::hardware_concurrency()) 
        : m_stop(false) {
        // Ensure at least one thread
        numThreads = numThreads > 0 ? numThreads : 1;
        
        // Create the worker threads
        for (size_t i = 0; i < numThreads; ++i) {
            m_workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        // Wait for a task or stop signal
                        std::unique_lock<std::mutex> lock(m_queueMutex);
                        m_condition.wait(lock, [this] { 
                            return m_stop || !m_tasks.empty(); 
                        });
                        
                        // Exit if stopping and no tasks
                        if (m_stop && m_tasks.empty()) {
                            return;
                        }
                        
                        // Get the next task
                        task = std::move(m_tasks.front());
                        m_tasks.pop();
                    }
                    
                    // Execute the task
                    task();
                }
            });
        }
    }
    
    /**
     * Destructor stops all threads and cleans up the thread pool.
     */
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_stop = true;
        }
        
        // Wake up all threads so they can exit
        m_condition.notify_all();
        
        // Wait for all threads to finish
        for (std::thread &worker : m_workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
    
    /**
     * Enqueues a new task to be executed by the thread pool.
     * Returns a future that will eventually hold the result of the task.
     * 
     * @param f The function to execute
     * @param args The arguments to pass to the function
     * @return A future that will contain the result of the function call
     */
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type> {
        
        // Create the function type based on the return type of F(Args...)
        using return_type = typename std::result_of<F(Args...)>::type;
        
        // Create a packaged task that will execute the function with its arguments
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        // Get the future result before pushing the task
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            
            // Don't allow enqueueing after stopping the pool
            if (m_stop) {
                throw std::runtime_error("Cannot enqueue on a stopped ThreadPool");
            }
            
            // Wrap the packaged task in a void function for the queue
            m_tasks.emplace([task]() { (*task)(); });
        }
        
        // Notify one worker thread that a task is available
        m_condition.notify_one();
        
        return res;
    }
    
    /**
     * Returns the number of worker threads in the pool.
     */
    size_t size() const {
        return m_workers.size();
    }

private:
    // Worker threads
    std::vector<std::thread> m_workers;
    
    // Task queue
    std::queue<std::function<void()>> m_tasks;
    
    // Synchronization
    std::mutex m_queueMutex;
    std::condition_variable m_condition;
    bool m_stop;
};