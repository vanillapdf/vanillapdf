#ifndef _PARSER_H
#define _PARSER_H

#include "fwd.h"
#include "lexical_stream.h"
#include "direct_object.h"
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

			files::File * file(void) const;

		private:
			files::File * _file;
		};

		inline files::File * Parser::file(void) const { return _file; }
	}
}

#endif /* _PARSER_H */
