#include "Trailer.h"
#include "integer_object.h"
#include "token.h"
#include "lexical_reverse_stream.h"
#include "exception.h"

#include <cassert>

namespace gotchangpdf
{
	namespace files
	{
		using namespace exceptions;
		using namespace lexical;
		using namespace std;

		ReverseStream& operator>>(ReverseStream& s, Trailer& o)
		{
			s.ReadTokenWithType(Token::Type::EOL);
			s.ReadTokenWithType(Token::Type::END_OF_FILE);
			s.ReadTokenWithType(Token::Type::EOL);

			IntegerObject offset;
			s >> offset;

			if (0 == offset)
				throw Exception("Invalid xref offset");

			o._xref_offset = offset;

			s.ReadTokenWithType(Token::Type::EOL);
			s.ReadTokenWithType(Token::Type::START_XREF);

			/* TODO read dictionary */

			return s;
		}

		types::stream_offset Trailer::xref_offset() const { return _xref_offset; }

		DictionaryObjectPtr Trailer::dictionary() const { return _dictionary; }
	}
}
