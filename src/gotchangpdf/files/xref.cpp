#include "xref.h"

#include "spirit_parser.h"
#include "integer_object.h"
#include "exception.h"
#include "character.h"
#include "array_object.h"

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

		XrefEntryPtr ReadEntry(lexical::SpiritParser& s, types::integer objNumber)
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

		SpiritParser& operator>>(SpiritParser& s, Xref& o)
		{
			// XRef stream
			if (s.PeekTokenType() == Token::Type::INTEGER_OBJECT)
			{
				// Get stream object data
				auto xref = s.ReadDirectObjectWithType<StreamObjectPtr>();
				auto header = xref->GetHeader();

				auto fields = header->FindAs<ArrayObjectPtr<IntegerObjectPtr>>(constant::Name::W);

				auto field_size = fields->Size();
				assert(field_size == 3);

				auto body = xref->GetBodyDecoded();

				// Iterate over entries
				auto it = body.begin();
				while (it != body.end())
				{
					int field1 = 0;
					auto field1_size = fields->At(0);
					assert(*field1_size == 1);
					for (int i = 0; i < *field1_size; ++i)
					{
						field1 = (field1 << 8) + (*it)++;
					}

					int field2 = 0;
					auto field2_size = fields->At(1);
					assert(*field2_size == 2);
					for (int i = 0; i < *field2_size; ++i)
					{
						field2 = (field2 << 8) + (*it)++;
					}

					int field3 = 0;
					auto field3_size = fields->At(2);
					assert(*field3_size == 1);
					for (int i = 0; i < *field3_size; ++i)
					{
						field3 = (field3 << 8) + (*it)++;
					}

					XrefEntry entry;
					switch (*it)
					{
					case '\0':
					case '\1':
					case '\2':
					default:
						throw exceptions::Exception("Unknown field in cross reference stream");
					}

					//o.push_back(entry);
					++it;
				}

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
