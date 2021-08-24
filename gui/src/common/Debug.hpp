/**
 ** rckam: a Qt remote control for digital cameras
 **
 ** Copyright (C) <year>  <name of author>
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Affero General Public License as
 ** published by the Free Software Foundation, either version 3 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Affero General Public License for more details.
 **
 ** You should have received a copy of the GNU Affero General Public License
 ** along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **/

#ifndef COMMON_DEBUG_HPP
#define COMMON_DEBUG_HPP

#include <atomic>
#include <memory>
#include <typeinfo>

#include <boost/algorithm/string.hpp>
#include <boost/date_time.hpp>
#include <boost/io/ios_state.hpp>
#include <boost/thread.hpp>

#include "common/SystemCompatibility.hpp"

namespace rckam
{
namespace common
{

#define PROFILING_NOINLINE
//#define PROFILING_NOINLINE __attribute__((noinline))


static std::ostream nostream(0);
//TODO: check why simple CerrLocker(std::cerr) << ... << is not good enough
/**
 * \brief helper macro to simplify the thread-guarded logging. All elements on a single << line are serialized
 * under one CerrLocker
 */
#define RCKAM_THREAD_CERR \
    if(const ::rckam::common::detail::CerrLocker &rckam_cerr_lock = ::rckam::common::detail::CerrLocker()); \
    else (rckam_cerr_lock.cerrBlocked() ? ::rckam::common::nostream : std::cerr) << rckam::common::detail::ThreadTimestamp()

#define RCKAM_ASSERT_CERR \
    if(::rckam::common::detail::CerrLocker rckam_cerr_lock = ::rckam::common::detail::CerrLocker()); \
    else std::cerr << rckam::common::detail::ThreadTimestamp()


#define RCKAM_SCOPE_BLOCK_CERR if (const ::rckam::common::detail::CerrBlocker blocker = ::rckam::common::detail::CerrBlocker()); else

/**
 * \brief Evaluates expression always (even if NDEBUG is set and so on). Also uses ostream serialization which,
 *        unlike the standard assert, has shown not to allocate the dynamic memory at the time when you least
 *        expect this to happen.
 */


#define RCKAM_VERIFY_MSG(expr, msg) {if (expr) {} else \
{ RCKAM_ASSERT_CERR << "ERROR: ***** Internal Program Error - assertion (" << #expr << ") failed in " \
    << (BOOST_CURRENT_FUNCTION) << ":" << __FILE__ << '(' << __LINE__ << "): " << msg << std::endl; \
    ::rckam::common::terminateWithCoreDump();}}

#if (RCKAM_BUILD_TYPE == RCKAM_BUILD_Release)
#define RCKAM_ASSERT_MSG(expr, msg)
#else
#define RCKAM_ASSERT_MSG(expr, msg) {if (expr) {} else \
{ RCKAM_ASSERT_CERR << "ERROR: ***** Internal Program Error - assertion (" << #expr << ") failed in " \
    << (BOOST_CURRENT_FUNCTION) << ":" << __FILE__ << '(' << __LINE__ << "): " << msg << std::endl; \
    ::rckam::common::terminateWithCoreDump();}}
#endif

inline std::string parseStat(const std::string &stat)
{
    std::vector<std::string> statFields;
    boost::algorithm::split(statFields, stat,  boost::algorithm::is_any_of(" "));
    return std::string(statFields.at(22) + "vm " + statFields.at(23) + "res");
}

class ScopedMallocBlock : boost::noncopyable
{
public:
    enum Mode
    {
        Invalid = 0,
        Off,
        Warning,
        Strict
    };

    ScopedMallocBlock(const Mode mode);
    ~ScopedMallocBlock();
private:
    const Mode mode_;

    friend class ScopedMallocBlockUnblock;
    void block();
    void unblock();
};

class ScopedMallocBlockUnblock : boost::noncopyable
{
    ScopedMallocBlock &block_;
public:
    ScopedMallocBlockUnblock(ScopedMallocBlock & block);
    ~ScopedMallocBlockUnblock();
};

namespace detail
{
class ThreadTimestamp
{
public:
};

struct IndentBase
{
protected:
    static THREAD_LOCAL unsigned width;
public:
    static unsigned getWidth() {return width;}
};

template <int i>
struct IndentT: public IndentBase
{
    IndentT() {width += i;}
    ~IndentT() {width -= i;}
};

class IndentStorer
{
public:
     IndentStorer(const IndentBase& indent) : indent(indent) {}
     const IndentBase & indent;
};

inline std::ostream & operator << (std::ostream &os, const IndentStorer & indenter) {
    os << std::setfill(' ') << std::setw(indenter.indent.getWidth()) << "";
    return os;
}

/**
 * \brief formats time stamp and thread id to simplify threaded logging
 */
inline std::ostream & operator << (std::ostream &os, const ThreadTimestamp &) {

    // IMPORTANT: this is the way to serialize date without causing any dynamic memory operations to occur
    ::std::time_t t;
    ::std::time(&t);
    ::std::tm curr, *curr_ptr;
    curr_ptr = boost::date_time::c_time::localtime(&t, &curr);

    os << (curr_ptr->tm_year + 1900) << '-' <<
        std::setfill('0') << std::setw(2) << (curr_ptr->tm_mon + 1) << '-'  <<
        std::setfill('0') << std::setw(2) << curr_ptr->tm_mday << ' '  <<

        std::setfill('0') << std::setw(2) << curr_ptr->tm_hour << ':' <<
        std::setfill('0') << std::setw(2) << curr_ptr->tm_min << ':' <<
        std::setfill('0') << std::setw(2) << curr_ptr->tm_sec << ' ' <<
        "\t[" << boost::this_thread::get_id() << "]\t";
    return os;
}

/**
 * \brief Blocks RCKAM_THREAD_CERR messages. Use for unit tests
 */
class CerrBlocker
{
    static std::atomic_int cerrBlocked_;
public:
    CerrBlocker();

    ~CerrBlocker();

    operator bool () const {
        return false;
    }

    static bool blocked() {return cerrBlocked_;}
};

struct CerrStreamBlocker : public std::ostream
{
    const bool block_;

	CerrStreamBlocker(const bool block) : std::ostream(0), block_(block) {}

    template <typename AnyT>
    const CerrStreamBlocker& operator << (const AnyT value) const
    {
        if (!block_)
        {
            std::cerr << value;
        }

        return *this;
    }

//    template <typename AnyT>
//    friend const CerrStreamBlocker& operator << (const CerrStreamBlocker& sb, const AnyT &value)
//    {
//        if (!sb.block_)
//        {
//            std::cerr << value;
//        }
//
//        return sb;
//    }


};
/**
 * \brief Guards std::cerr for the duration of CerrLocker existance
 *        Restores any changes made to ios::base
 */
class CerrLocker
{
    // some people allocate memory from under their trace code. For example by using boost::format.
    // if memory control is on, we don't want them to be dead-locked on their own thread cerrMutex_.
    static boost::recursive_mutex cerrMutex_;
    boost::lock_guard<boost::recursive_mutex> lock_;
    boost::io::ios_base_all_saver ias_;

public:

    CerrLocker(const CerrLocker &) : lock_(cerrMutex_), ias_(std::cerr){
    }
    CerrLocker() : lock_(cerrMutex_), ias_(std::cerr) {
    }
    operator bool () const {
        return false;
    }

    bool cerrBlocked() const {return CerrBlocker::blocked();}
};

inline CerrBlocker::CerrBlocker()
{
//    RCKAM_ASSERT_CERR << "cerr blocked" << std::endl;
    ++cerrBlocked_;
}

inline CerrBlocker::~CerrBlocker()
{
    RCKAM_ASSERT_MSG(cerrBlocked_, "Attempt to unblock more times than blocked. something is really wrong");
    --cerrBlocked_;
//    RCKAM_ASSERT_CERR << "cerr unblocked" << std::endl;
}


inline void assertion_failed_msg(char const * expr, char const * msg, char const * function,
                                 char const * file, int64_t line)
{
    RCKAM_ASSERT_CERR
    << "ERROR: ***** Internal Program Error - assertion (" << expr << ") failed in "
    << function << ":" << file << '(' << line << "): " << msg << std::endl;

    common::terminateWithCoreDump();
}

} // namespace detail

inline std::time_t time()
{
    std::time_t ret;
    RCKAM_VERIFY_MSG(-1 != ::std::time(&ret), "std::time failed, errno: " << errno << strerror(errno));
    return ret;
}

} // namespace common

//rckam namespace RckamDebugTraceIndent gets shadowed by non-incrementing type inside the first trace. This way the indent is
//consistent within the function
typedef rckam::common::detail::IndentT<1> RckamDebugTraceIndent;


/**
 ** \brief Provide a mechanism for detailed level of debugging
 **/
#ifdef RCKAM_THREAD_CERR_DEV_TRACE_ENABLED
    #define RCKAM_THREAD_CERR_DEV_TRACE(trace) {RCKAM_THREAD_CERR << trace << std::endl;}
    #define RCKAM_DEV_TRACE_BLOCK(block) block
    #define RCKAM_THREAD_CERR_DEV_TRACE_CLUSTER_ID(clusterId, trace) RCKAM_THREAD_CERR_DEV_TRACE(trace)
#else
    #define RCKAM_THREAD_CERR_DEV_TRACE(blah)
    #ifdef RCKAM_THREAD_CERR_DEV_TRACE_ENABLED_CLUSTER_ID
        #define RCKAM_THREAD_CERR_DEV_TRACE_CLUSTER_ID_INDENTING(clusterId, trace, line) const RckamDebugTraceIndent indent ## line; typedef rckam::common::detail::IndentBase RckamDebugTraceIndent; rckam::common::detail::IndentStorer indentStorer ## line(indent ## line); if(RCKAM_THREAD_CERR_DEV_TRACE_ENABLED_CLUSTER_ID == (clusterId)) { RCKAM_THREAD_CERR << indentStorer ## line << trace << std::endl; }
        #define RCKAM_THREAD_CERR_DEV_TRACE_CLUSTER_ID_PROXY(clusterId, trace, line) RCKAM_THREAD_CERR_DEV_TRACE_CLUSTER_ID_INDENTING(clusterId, trace, line)
        #define RCKAM_THREAD_CERR_DEV_TRACE_CLUSTER_ID(clusterId, trace) RCKAM_THREAD_CERR_DEV_TRACE_CLUSTER_ID_PROXY(clusterId, trace, __LINE__)
        #define RCKAM_DEV_TRACE_BLOCK(block) block
    #else
        #define RCKAM_THREAD_CERR_DEV_TRACE_CLUSTER_ID(clusterId, blah)
        #define RCKAM_DEV_TRACE_BLOCK(block)
    #endif
#endif

} // namespace rckam



#endif // #ifndef COMMON_DEBUG_HPP
