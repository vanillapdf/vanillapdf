#include "Trailer.h"
#include "IntegerObject.h"
#include "Token.h"
#include "lexical/LexicalReverseStream.h"
#include "Exception.h"

#include <cassert>

namespace Pdf
{
	using namespace Lexical;
	using namespace Streams::Lexical;
	using namespace std;

	Trailer::Trailer() : _xref_offset(0), _dictionary(boost::intrusive_ptr<DictionaryObject>(new DictionaryObject())) {}

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

	streamOffsetValueType Trailer::xref_offset() const { return _xref_offset; }

	boost::intrusive_ptr<DictionaryObject> Trailer::dictionary() const { return _dictionary; }
}
namespace boost
{
	void intrusive_ptr_add_ref(Pdf::Trailer* obj)
	{
		++obj->_intrusive_ref_count;
	}

	void intrusive_ptr_release(Pdf::Trailer* obj)
	{
		if (--obj->_intrusive_ref_count == 0)
			delete obj;
	}
}
