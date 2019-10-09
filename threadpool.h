#pragma once
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace std
{
#define  MAX_THREAD_NUM 256

	class threadpool {
		using Task = std::function<void()>;
		std::vector<std::thread> pool;
		std::queue<Task> tasks;
		std::mutex m_lock;
		std::condition_variable cv_task;
		std::atomic<bool> stoped;
		std::atomic<int>  idlThrNum;

	public:
		inline threadpool(unsigned short size = 4) :stoped{ false } {
			idlThrNum = size < 1 ? 1 : size;
			for (size = 0; size < idlThrNum; ++size) {
				pool.emplace_back(
					[this]
				{
					// working thread
					while (!this->stoped) {
						std::function<void()> task;
						{ 
							std::unique_lock<std::mutex> lock{ this->m_lock };
							this->cv_task.wait(lock,
								[this] {
								return this->stoped.load() || !this->tasks.empty();
							}
							); // wait until there is task
							if (this->stoped && this->tasks.empty())
								return;
							task = std::move(this->tasks.front()); // select a task
							this->tasks.pop();
						}
						idlThrNum--;
						task();
						idlThrNum++;
					}
				}
				);
			}
		}
		inline ~threadpool() {
			stoped.store(true);
			cv_task.notify_all(); // invoke all tasks
			for (std::thread& thread : pool) {
				//thread.detach(); 
				if (thread.joinable())
					thread.join(); // wait for thread to finish
			}
		}

	public:
		// commit a task
		// invoke get to wait until current thread finish
		// Invoke class member
		// Option1: - bind： .commit(std::bind(&Dog::sayHello, &dog));
		// Option2: - mem_fn： .commit(std::mem_fn(&Dog::sayHello), &dog)
		template<class F, class... Args>
		auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))> {
			if (stoped.load())    // stop == true ??
				throw std::runtime_error("commit on ThreadPool is stopped.");

			using RetType = decltype(f(args...)); // typename std::result_of<F(Args...)>::type
			auto task = std::make_shared<std::packaged_task<RetType()> >(
				std::bind(std::forward<F>(f), std::forward<Args>(args)...)
				);    // wtf !
			std::future<RetType> future = task->get_future();
			{  
				std::lock_guard<std::mutex> lock{ m_lock };
				tasks.emplace(
					[task]()
				{ // push(Task{...})
					(*task)();
				}
				);
			}
			cv_task.notify_one();

			return future;
		}

		int idlCount() { return idlThrNum; }

	};

}

#endif