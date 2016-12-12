// Every file has its own include guard

// Do not reorder the includes spontaneously

// base
#include "object.h"

// self-sufficient
#include "null_object.h"
#include "name_object.h"
#include "boolean_object.h"
#include "integer_object.h"
#include "real_object.h"
#include "string_object.h"
#include "indirect_object_reference.h"
#include "mixed_array_object.h"

// recursive
#include "array_object.h"
#include "dictionary_object.h"
#include "stream_object.h"
