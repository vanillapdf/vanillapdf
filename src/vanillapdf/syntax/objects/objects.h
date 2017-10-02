// Every file has its own include guard

// Do not reorder the includes spontaneously

// base
#include "syntax/objects/object.h"

// self-sufficient
#include "syntax/objects/null_object.h"
#include "syntax/objects/name_object.h"
#include "syntax/objects/boolean_object.h"
#include "syntax/objects/integer_object.h"
#include "syntax/objects/real_object.h"
#include "syntax/objects/string_object.h"
#include "syntax/objects/indirect_object_reference.h"
#include "syntax/objects/mixed_array_object.h"

// recursive
#include "syntax/objects/array_object.h"
#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/stream_object.h"
