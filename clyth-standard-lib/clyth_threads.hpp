#ifndef clyth_threads_hpp
#define clyth_threads_hpp

#include "clyth_common.hpp"

/**
 * The current statuses that a thread could have.
 */
enum THREAD_STATUS {
    CLEAN_EXIT = 0, FAILED = 1, ERROR = 2, HALTED = 3, NOT_SCHEDULED = 4
};

/**
 * Thread exit status containing exit code, and allowing an additional return value for custom data structures.
 */
template<typename T>
class ThreadExitStatus {
public:
    ThreadExitStatus();
    ~ThreadExitStatus();

    THREAD_STATUS exitCode();
    virtual T threadReturnValue();
private:
    THREAD_STATUS thread_status;
};

/**
 * Thread base class, allowing for various virtual thread actions and features such as defining priority with 
 * the scale to obey (low-to-high meaning lowest value is highest priority), run, join, restart, and additional
 * actions that may be added in the future to extend existing functionality is possible.
 */
class Thread {
public:
    Thread();
    ~Thread();

    bool setThreadPriority(int64_t priority_value, bool low_to_high_scale);
    bool run();
    bool join();
    bool restart();
private:
    int64_t thread_priority = -1;
};

#endif
