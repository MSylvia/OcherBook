#ifndef SINGLE_THREADED

#include "util/Lock.h"
#include "util/Stopwatch.h"
#include "util/Thread.h"

#include <assert.h>
#include <errno.h>
#include <new>
#include <stdlib.h>
#include <string.h>


Lock::Lock()
{
#ifdef USE_LIBTASK
    memset(&m_lock, 0, sizeof(m_lock));
#elif defined(__BEOS__) || defined(__HAIKU__)
    // nothing
#else
    if (pthread_mutex_init(&m_lock, NULL))
        throw std::bad_alloc();
#endif
}


Lock::~Lock()
{
#ifdef USE_LIBTASK
    assert(canqlock(&m_lock));
#elif defined(__BEOS__) || defined(__HAIKU__)
    assert(!m_lock.IsLocked());
#else
    int r = pthread_mutex_destroy(&m_lock);
    assert(r == 0);
    (void)r;
#endif
}

bool Lock::lockWithTimeout(unsigned int usec)
{
#ifdef USE_LIBTASK
    Stopwatch stopwatch;
    do {
        if (canqlock(&m_lock)) {
            qlock(&m_lock);
            return true;
        }
        Thread::yieldCurrent();
    } while (stopwatch.elapsedUSec() < usec);
    return false;
#elif defined(__BEOS__) || defined(__HAIKU__)
    return m_lock.LockWithTimeout(usec) == B_OK;
#elif defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >= 200112L)
    if (usec) {
        struct timespec absTime;
        absTime = Clock::futureUsec(usec);
        int r = pthread_mutex_timedlock(&m_lock, &absTime);
        if (r) {
            assert(r == ETIMEDOUT);
            return false;
        }
        return true;
    } else {
        return pthread_mutex_trylock(&m_lock) == 0;
    }
#else
    if (usec) {
        // An alternative to polling is for unlock to signal on a cond var, but the overhead may
        // be excessive.
        unsigned int sleepUsec = usec / 10;
        if (sleepUsec < 1)
            sleepUsec = 1;
        else if (sleepUsec > 1000)
            sleepUsec = 1000;
        Stopwatch stopwatch;
        while (1) {
            int r = pthread_mutex_trylock(&m_lock);
            if (r == 0)
                return true;
            if (stopwatch.elapsedUSec() > usec)
                break;
            Thread::_sleepUSec(sleepUsec);
        }
        return false;
    } else {
        return pthread_mutex_trylock(&m_lock) == 0;
    }
#endif
}

#endif
