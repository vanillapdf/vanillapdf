#ifndef _PARSER_H
#define _PARSER_H

#include "fwd.h"
#include "lexical_stream.h"
#include "deferred.h"

namespace gotchangpdf
{
	namespace lexical
	{
		class Parser : public lexical::Stream
		{
		public:
			Parser(files::File * file, CharacterSource & stream);
			Parser(const Parser & other);

			template<typename T>
			Deferred<T> readObjectWithType() { return DirectObjectGetAs<T>(readObject()); }

			//DirectObject readObjectWithType(Object::Type type);
			DirectObject readObject();
			DirectObject peekObject();

			files::File * file(void) const;
			//void SetDeep(bool deep);
			//bool GetDeep(void) const;

		private:
			files::File * _file;
			//bool _deep;
		};

		inline files::File * Parser::file(void) const { return _file; }
	}
}

#endif /* _PARSER_H */
