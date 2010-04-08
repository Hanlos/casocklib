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
 * \file casock/util/Logger.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include "Logger.h"

#include <stdarg.h>
#include <unistd.h>
#include <time.h>

//#ifdef HAVE_SYS_SYSCALL_H
#include <sys/syscall.h>
//#endif

#ifdef USE_THREADS
#include "Thread.h"
#endif

#include "types.h"


Logger*	Logger::mspInstance = NULL;


Logger* Logger::getInstance ()
{
	if (mspInstance == NULL)
		mspInstance = new Logger ();

	return mspInstance;
}

void Logger::finalize ()
{
#ifdef USE_LOGGER
	LOGMSG (LOW_LEVEL, "Logger::finalize () - [%p]\n", mspInstance);
#endif

	if (mspInstance)
		delete mspInstance;

	mspInstance = NULL;
}

#ifdef USE_THREADS
Logger::Logger (const e_debug &debug, const string &log_file, FILE *pFP) : Lockable ()
#else
Logger::Logger (const e_debug &debug, const string &log_file, FILE *pFP)
#endif
{
	m_debug_level	= debug;
	m_log_file	= log_file;
	mpFP		= pFP;
}

Logger::~Logger ()
{
//	LOGMSG (NO_DEBUG, "Logger::~Logger () - [%p]\n", this);

	closeLogFile ();
}

void Logger::setDebugLevel (e_debug debug)
{
	m_debug_level = debug;
}

void Logger::setLogFile (const string &file)
{
	m_log_file = file;

	openLogFile ();
}

void Logger::openLogFile ()
{
	mpFP = fopen (m_log_file.c_str (), "w");
}

void Logger::closeLogFile ()
{
	if (mpFP)
		fclose (mpFP);

	mpFP = NULL;
}

void Logger::print (e_debug debug, char *msg, ...)
{
	if (m_debug_level >= debug)
	{
#ifdef USE_THREADS
		int oldstate = Thread::disableCancel ();
#endif

		char buffer[Logger::MSG_BUFF_SIZE];
		va_list ap;
		
		va_start (ap, msg);
		vsnprintf (buffer, Logger::MSG_BUFF_SIZE, msg, ap);

		time_t str_time;
		struct tm str_tm;
		char log_date[Logger::DATE_BUFF_SIZE];
		time (&str_time);
		localtime_r (&str_time, &str_tm);
		strftime (log_date, Logger::DATE_BUFF_SIZE, "%d/%m/%Y-%H:%M:%S", &str_tm);

		if (mpFP)
		{
#ifdef USE_THREADS
			Lock ();
#endif

//#ifdef HAVE_SYS_SYSCALL_H
			fprintf	(mpFP, "%s-%ld: %s", log_date, syscall (__NR_gettid), buffer);
//#else
//			fprintf	(mpFP, "%s-%ld: %s", log_date, syscall (Logger::GETTID_SYSCALL_ID), buffer);
//#endif

			fflush	(mpFP);
#ifdef USE_THREADS
			Unlock ();
#endif
		}
		else
		{
#ifdef USE_THREADS
			Lock ();
#endif

//#ifdef HAVE_SYS_SYSCALL_H
			fprintf	(stderr, "%s-%ld: %s", log_date, syscall (__NR_gettid), buffer);
//#else
//			fprintf	(stderr, "%s-%ld: %s", log_date, syscall (Logger::GETTID_SYSCALL_ID), buffer);
//#endif

			fflush	(stderr);
#ifdef USE_THREADS
			Unlock ();
#endif
		}

		va_end (ap);

#ifdef USE_THREADS
		Thread::setCancelState (oldstate);
#endif
	}
}

#ifdef USE_MEM_SIZE
const uint64 Logger::getMemSize () const
{
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "Logger::getMemSize () - [%p]\n", this);
#endif

	uint64 size = 0;

	size += sizeof (e_debug);
	size += sizeof (string);
	size += sizeof (FILE *);
	size += m_log_file.size () + 1;
#ifdef USE_THREADS
	size += Lockable::getMemSize ();
#endif

	return size;
}
#endif