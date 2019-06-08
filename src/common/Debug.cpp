/**
 **
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
 **
 **/

#include "common/Debug.hh"
#include "common/SystemCompatibility.hh"

namespace rckam
{
namespace common
{


namespace detail
{

// global variable that allows turning off cerr output for things such as unit tests.
std::atomic_int CerrBlocker::cerrBlocked_(0);
boost::recursive_mutex CerrLocker::cerrMutex_;
THREAD_LOCAL unsigned IndentBase::width = 0;

#if 0

static bool malloc_warning_hook(size_t size, const void *caller)
{
    RCKAM_THREAD_CERR << "WARNING: blocked allocation of " << size << " bytes requested.\n";
    return true;
}

static bool malloc_strict_hook(size_t size, const void *caller)
{
    RCKAM_THREAD_CERR << "ERROR: blocked allocation of " << size << " bytes requested.\n";
    return false;
}

//static bool malloc_ignore_hook(size_t size, const void *caller)
//{
//    return true;
//}

#endif // #if 0

} // namespace detail

#if 0

ScopedMallocBlock::ScopedMallocBlock(const ScopedMallocBlock::Mode mode) :
    mode_(mode)
{
    block();
}

void ScopedMallocBlock::block()
{
    switch(mode_)
    {
    case Off:
//        hookMalloc(detail::malloc_ignore_hook);
        break;
    case Warning:
        hookMalloc(detail::malloc_warning_hook);
        break;
    case Strict:
        hookMalloc(detail::malloc_strict_hook);
        break;
    default:
        RCKAM_ASSERT_MSG(false, "invalid malloc block mode specified");
        break;
    }
}

ScopedMallocBlock::~ScopedMallocBlock()
{
    unblock();
}

void ScopedMallocBlock::unblock()
{
    switch(mode_)
    {
    case Off:
//        unhookMalloc(detail::malloc_ignore_hook);
        break;
    case Warning:
        unhookMalloc(detail::malloc_warning_hook);
        break;
    case Strict:
        unhookMalloc(detail::malloc_strict_hook);
        break;
    default:
        RCKAM_ASSERT_MSG(false, "invalid malloc block mode specified");
        break;
    }
}

ScopedMallocBlockUnblock::ScopedMallocBlockUnblock(ScopedMallocBlock &block) :
        block_(block)
{
    block_.unblock();
}

ScopedMallocBlockUnblock::~ScopedMallocBlockUnblock()
{
    block_.block();
}

#endif // #if 0

} // namespace common
} // namespace rckam
