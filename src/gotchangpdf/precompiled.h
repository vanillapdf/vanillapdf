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

// Solution
#include "deferred.h"
#include "objects.h"
#include "exception.h"
#include "constants.h"

// snprintf in boost
#pragma warning (disable: 4996)
#include "log.h"

#endif /* _PRECOMPILED_H */
