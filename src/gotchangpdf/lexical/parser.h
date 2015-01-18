#ifndef _PARSER_H
#define _PARSER_H

#include "fwd.h"
#include "lexical_stream.h"

namespace gotchangpdf
{
	namespace lexical
	{
		class Parser : public lexical::Stream
		{
		public:
			Parser(files::File * file, CharacterSource & stream) : lexical::Stream(stream), _file(file) {}
			Parser(const Parser & other) : lexical::Stream(other) { _file = other.file(); }

			files::File * file(void) const;

		private:
			files::File * _file;
		};

		inline files::File * Parser::file(void) const { return _file; }
	}
}

#endif /* _PARSER_H */
