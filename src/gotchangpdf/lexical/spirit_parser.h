#ifndef _SPIRIT_PARSER_H
#define _SPIRIT_PARSER_H

#include "fwd.h"
#include "lexical_stream.h"
#include "deferred.h"

namespace gotchangpdf
{
	namespace lexical
	{
		class SpiritParser : public lexical::Stream
		{
		public:
			SpiritParser(files::File * file, CharacterSource & stream);
			SpiritParser(const Parser & other);

			//template<typename T>
			//Deferred<T> readObjectWithType() { return DirectObjectGetAs<T>(readObject()); }

			//DirectObject readObjectWithType(Object::Type type);
			IndirectObjectPtr readObject();
			IndirectObjectPtr peekObject();

			files::File * file(void) const;
			//void SetDeep(bool deep);
			//bool GetDeep(void) const;

		private:
			files::File * _file;
			//bool _deep;
		};

		inline files::File * SpiritParser::file(void) const { return _file; }
	}
}

#endif /* _SPIRIT_PARSER_H */
