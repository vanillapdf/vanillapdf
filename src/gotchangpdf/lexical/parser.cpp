#include "parser.h"
#include "exception.h"
#include "file.h"

#include "stream_object.h"
#include "boolean_object.h"
#include "null_object.h"
#include "function_object.h"
#include "indirect_object.h"
#include "dictionary_object.h"
#include "array_object.h"
#include "integer_object.h"
#include "stream_object.h"
#include "real_object.h"
#include "string_object.h"
#include "indirect_object_reference.h"

namespace gotchangpdf
{
	namespace lexical
	{
		using namespace lexical;
		using namespace exceptions;

		Parser::Parser(files::File * file, CharacterSource & stream)
			: lexical::Stream(stream), _file(file) {}

		Parser::Parser(const gotchangpdf::lexical::Parser &other)
			: lexical::Stream(other) { _file = other.file(); }
	}
}
