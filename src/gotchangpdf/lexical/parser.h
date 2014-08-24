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
			Parser(std::shared_ptr<files::File> file, std::shared_ptr<std::istream> stream);

			template<typename T>
			SmartPtr<T> readObjectWithType() { return readObject().GetAs<T>(); }

			SmartPtr<Object> readObjectWithType(Object::Type type);
			SmartPtr<Object> readObject();
			SmartPtr<Object> peekObject();

			std::shared_ptr<files::File> file(void) const;
			//void SetDeep(bool deep);
			//bool GetDeep(void) const;

			Parser(const Parser &);

		private:
			std::shared_ptr<files::File> _file;
			//bool _deep;
		};
	}
}

#endif /* _PARSER_H */
