#ifndef _PRECOMPILED_H
#define _PRECOMPILED_H

/*--------------Global warning disables--------------*/

// declaration of 'end' hides class member
#pragma warning (disable: 4458)

// declaration of 'attr' hides global declaration
#pragma warning (disable: 4459)

// decorated name length exceeded, name was truncated
#pragma warning (disable: 4503)

// forceinline not inlined
#pragma warning (disable: 4714)

// Do while false
#pragma warning (disable: 4127)

/*---------------------------------------------------*/

// Boost variant size if by default only 20
// Content stream grammar requires around 70
#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_LIST_SIZE 80

// Solution
#include "deferred.h"
#include "objects.h"
#include "exception.h"
#include "constants.h"

// snprintf in boost
#pragma warning (disable: 4996)
#include "log.h"

#endif /* _PRECOMPILED_H */
