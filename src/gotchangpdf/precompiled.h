#ifndef _PRECOMPILED_H
#define _PRECOMPILED_H

// decorated name length exceeded, name was truncated
#pragma warning (disable: 4503)

// forceinline not inlined
#pragma warning (disable: 4714)

// Do while false
#pragma warning (disable: 4127)

// Solution
#include "deferred.h"
#include "smart_ptr.h"
#include "objects.h"
#include "object_visitors.h"
#include "exception.h"
#include "constants.h"

#pragma warning (push, 3)
// snprintf in boost
#pragma warning (disable: 4996)
#include "log.h"
#pragma warning (pop)

#pragma warning (push, 3)

// C++ standard
#include <map>
#include <string>
#include <memory>
#include <vector>

#include <exception>
#include <cstdint>
#include <algorithm>
#include <functional>
#include <sstream>
#include <cstddef>
#include <locale>

#include <iosfwd>
#include <istream>
#include <ostream>
#include <iostream>
#include <fstream>

#include <cassert>

// OpenMP
#include <omp.h>

// Boost
#include <boost/static_assert.hpp>
#include <boost/variant.hpp>
#include <boost/scope_exit.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>

// IO streams
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/gzip.hpp>

// Fusion
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/adapted/adt/adapt_adt.hpp>
#include <boost/fusion/include/std_pair.hpp>

// Spirit

// conversion from 'unsigned int' to 'float', possible loss of data in real_impl
#pragma warning (disable: 4244)

// signed/unsigned mismatch in real_impl
#pragma warning (disable: 4018)
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/repository/include/qi_advance.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

#pragma warning (pop)

#endif /* _PRECOMPILED_H */
