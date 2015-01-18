#include "xref.h"

#include "integer_object.h"
#include "parser.h"
#include "exception.h"
#include "character.h"

#include <cassert>
#include <sstream>

namespace gotchangpdf
{
	namespace files
	{
		using namespace std;
		using namespace lexical;
		using namespace exceptions;
		using namespace character;

		XrefEntryPtr ReadEntry(lexical::Parser& s, types::integer objNumber)
		{
			char sp1, sp2, key, eol1, eol2;
			Token offset, number;
			s >> offset >> sp1 >> number >> sp2 >> key >> eol1 >> eol2;

			if (!(IsSpace(eol1) && Equals(eol2, WhiteSpace::CARRIAGE_RETURN)) &&
				!(IsSpace(eol1) && IsNewline(eol2)) &&
				!(Equals(eol1, WhiteSpace::CARRIAGE_RETURN) && IsNewline(eol2)))
			{
				throw Exception("End of line marker was not found in xref table entry");
			}

			static const char IN_USE = 'n';
			static const char NOT_IN_USE = 'f';

			bool usage = false;
			if (key == IN_USE)
				usage = true;
			else if (key == NOT_IN_USE)
				usage = false;
			else
			{
				stringstream buffer;
				buffer << "Key in XRef table is either of " << IN_USE << " or " << NOT_IN_USE;

				throw Exception(buffer.str());
			}

			XrefEntry result;
			result.SetUsage(usage);
			result.SetOffset(IntegerObject(offset));
			result.SetObjectNumber(objNumber);
			result.SetGenerationNumber(IntegerObject(number));
			result.SetInitialized(false);
			return result;
		}

		Parser& operator>>(Parser& s, Xref& o)
		{
			// XRef stream
			if (s.PeekTokenType() == Token::Type::INTEGER_OBJECT)
			{
				//auto obj = s.readObject();

				// TODO process

				return s;
			}

			s.ReadTokenWithType(Token::Type::XREF_MARKER);
			s.ReadTokenWithType(Token::Type::EOL);

			while (s.PeekTokenType() != Token::Type::TRAILER)
			{
				IntegerObject revision, numberOfObjects;
				s >> revision >> numberOfObjects;

				s.ReadTokenWithType(Token::Type::EOL);

				for (types::integer i = 0; i < numberOfObjects; ++i)
				{
					// check for overflow
					assert(revision + i >= revision);

					auto entry = ReadEntry(s, static_cast<types::integer>(revision + i));
					o.push_back(entry);
				}
			}

			return s;
		}
	}
}
