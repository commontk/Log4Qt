/******************************************************************************
 *
 * package:     Log4Qt
 * file:        logobject.h
 * created:     September 2007
 * author:      Martin Heinrich
 *
 * 
 * Copyright 2007 Martin Heinrich
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 ******************************************************************************/

#ifndef LOG4QT_LOGOBJECT_H
#define LOG4QT_LOGOBJECT_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include <QtCore/QObject>

#include <QtCore/QAtomic>
#include "log4qt/helpers/classlogger.h"


namespace Log4Qt
{

	class Logger;
	
	/*!
	 * \brief The class LogObject is the common base class for many classes 
	 *        in the package.
	 *
	 * The class inherits QObject to allow its subclass to be accessed using
	 * the Qt property system.
	 * 
	 * LogObject objects provide a reference counter. A reference to the 
	 * object is established by calling retain() and freed by calling 
	 * release(). The object will delete itself by calling 
	 * QObject::deleteLater() when the reference counter is decremented to 0
	 * and the object does not have a parent.
	 * 
	 * A class specific logger can be accessed over logger().
	 * 
	 * The class also implements generic streaming to QDebug. Streaming an 
	 * object to QDebug will invoke debug() to create class specific output.
	 * 
	 * \note All the functions declared in this class are thread-safe.
	 * 
	 * \sa \ref Ownership "Object ownership",
	 *     LOG4QT_DECLARE_QCLASS_LOGGER
	 */
	class LogObject : public QObject
	{
		Q_OBJECT
	
	public:	
		/*!
		 * Creates a LogObject which is a child of \a pObject.
		 */
		LogObject(QObject *pObject = 0);
		
		/*!
		 * Destroys the LogObject.
		 */
	    virtual ~LogObject();
	    
	private:
	    LogObject(const LogObject &rOther); // Not implemented
	    LogObject &operator=(const LogObject &rOther); // Not implemented
		
	public:
		/*!
		 * Returns the value of the reference counter.
		 */
		int referenceCount() const;
		
		/*! 
		 * Decrements the reference count of the object. If the reference count
		 * count reaches zero and the object does not have a parent the object
		 * is deleted.
		 */
		void release();
		
		/*! 
		 * Increments the reference count of the object. 
		 */
		void retain();
		
	protected:
	#ifndef QT_NO_DEBUG_STREAM
	    /*!
	     * Writes all object member variables to the given debug stream 
	     * \a rDebug and returns the stream.
	     *
	     * The member function is used by 
	     * QDebug operator<<(QDebug debug, const LogObject &rLogObject) to
	     * generate class specific output.
	     *
	     * \sa QDebug operator<<(QDebug debug, const LogObject &rLogObject)
	     */
	    virtual QDebug debug(QDebug &rDebug) const = 0;
	    
	    // Needs to be friend to access internal data
	    friend QDebug operator<<(QDebug debug, 
	                             const LogObject &rLogObject);
	#endif // QT_NO_DEBUG_STREAM
	
	    /*!
	     * Returns a pointer to a Logger named after of the object.
	     * 
	     * \sa Logger::logger(const char *pName)
	     */
	    Logger* logger() const;
	    
	private:
		volatile int mReferenceCount;
        mutable ClassLogger mLog4QtClassLogger;
	};
	
	
	/**************************************************************************
	 * Operators, Helper
	 **************************************************************************/
	
	#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * \relates LogObject
	 * 
	 * Writes all object member variables to the given debug stream \a debug 
	 * and returns the stream.
	 *
	 * To handle subclassing the function uses the virtual member function 
	 * debug(). This allows each class to generate its own output.
	 *
	 * \sa QDebug, debug()
	 */
	QDebug operator<<(QDebug debug, 
	                  const LogObject &rLogObject);
	#endif
	
	
	/**************************************************************************
	 * Inline
	 **************************************************************************/
	
	inline LogObject::LogObject(QObject *pParent) :
		QObject(),
		mReferenceCount(0)
	{}
	
	inline LogObject::~LogObject()
	{}
	
	inline int LogObject::referenceCount() const
	{	return mReferenceCount;	}
	
	inline void LogObject::release()
	{	if ((q_atomic_decrement(&mReferenceCount) == 0) && !parent())
				delete(this);	}
	
	inline void LogObject::retain() 
	{	q_atomic_increment(&mReferenceCount);	}

    inline Logger *LogObject::logger() const
    {   return mLog4QtClassLogger.logger(this);    }
	
} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::LogObject, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_LOGOBJECT_H
