#ifndef _PARSER_H
#define _PARSER_H

#include "fwd.h"
#include "lexical_stream.h"
#include "smart_ptr.h"

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
			SmartPtr<T> readObjectWithType() { return readObject().GetAs<T>(); }

			SmartPtr<Object> readObjectWithType(Object::Type type);
			SmartPtr<Object> readObject();
			SmartPtr<Object> peekObject();

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
