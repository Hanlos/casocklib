/*
 * casocklib - An asynchronous communication library for C++
 * ---------------------------------------------------------
 * Copyright (C) 2010 Leandro Costa
 *
 * This file is part of casocklib.
 *
 * casocklib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * casocklib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with casocklib. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * \file casock/util/Lockable.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include "casock/util/Lockable.h"

#include <unistd.h>
#include <sys/time.h>

//#ifdef USE_MEM_SIZE
#include "casock/util/Logger.h"
//#endif

namespace casock {
  namespace util {
    Lockable::Lockable (const int& type)
      : m_mutex_attr (), m_mutex (), m_cond ()
    {
      pthread_mutexattr_init (&m_mutex_attr);
      pthread_mutexattr_settype (&m_mutex_attr, type);
      pthread_mutex_init (&m_mutex, &m_mutex_attr);
      pthread_cond_init (&m_cond, NULL);
    }

    Lockable::~Lockable ()
    {
      while (pthread_mutex_destroy (&m_mutex) != 0)
        sleep (1);

      pthread_mutexattr_destroy (&m_mutex_attr);
      pthread_cond_destroy (&m_cond);
    }

    void Lockable::lock () const
    {
      LOGMSG (DISABLE, "Lockable::%s () - p [%p]\n", __FUNCTION__, this);
      pthread_mutex_lock (&m_mutex);
    }

    void Lockable::unlock () const
    {
      LOGMSG (DISABLE, "Lockable::%s () - p [%p]\n", __FUNCTION__, this);
      pthread_mutex_unlock (&m_mutex);
    }

    bool Lockable::tryLock () const
    {
      return (pthread_mutex_trylock (&m_mutex) == 0);
    }

    int Lockable::cond_wait (const uint32& timeout) const
    {
      int ret = 0;

      if (timeout == 0)
        ret = pthread_cond_wait (&m_cond, &m_mutex);
      else
      {
        struct timeval tv_now;
        struct timespec ts_timeout;

        gettimeofday (&tv_now, NULL);

        ts_timeout.tv_sec = tv_now.tv_sec + timeout;
        ts_timeout.tv_nsec = tv_now.tv_usec * 1000;

        ret = pthread_cond_timedwait (&m_cond, &m_mutex, &ts_timeout);
      }

      return ret;
    }

    void Lockable::cond_broadcast () const
    {
      pthread_cond_broadcast (&m_cond);
    }

    void Lockable::cond_signal () const
    {
      pthread_cond_signal (&m_cond);
    }

#ifdef USE_MEM_SIZE
    const uint64 Lockable::getMemSize () const
    {
      LOGMSG (DISABLE, "Lockable::%s () - [%p]\n", __FUNCTION__, this);

      uint64 size = 0;

      size += sizeof (pthread_mutexattr_t);
      size += sizeof (pthread_mutex_t);
      size += sizeof (pthread_cond_t);

      LOGMSG (DISABLE, "Lockable::%s () - [%u]\n", __FUNCTION__, size);

      return size;
    }
#endif
  }
}
