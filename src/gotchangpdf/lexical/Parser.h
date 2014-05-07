#ifndef _OBJECTS_STREAM_H
#define _OBJECTS_STREAM_H

#include "Fwd.h"
#include "LexicalStream.h"

#include "boost/intrusive_ptr.hpp"

namespace Pdf
{
	namespace Lexical
	{
		class Parser : public Streams::Lexical::Stream
		{
		public:
			Parser(std::shared_ptr<Files::File> file, std::shared_ptr<std::istream> stream);

			template<typename T>
			boost::intrusive_ptr<T> readObjectWithType();

			boost::intrusive_ptr<Pdf::Object> readObjectWithType(Pdf::IObject::Type type);
			boost::intrusive_ptr<Pdf::Object> readObject();
			boost::intrusive_ptr<Pdf::Object> peekObject();

			std::shared_ptr<Files::File> file(void) const;
			//void SetDeep(bool deep);
			//bool GetDeep(void) const;

			Pdf::Lexical::Parser::Parser(const Pdf::Lexical::Parser &);

		private:
			std::shared_ptr<Files::File> _file;
			//bool _deep;
		};

		template<typename T>
		boost::intrusive_ptr<T>
			Pdf::Lexical::Parser::readObjectWithType()
		{
			return boost::dynamic_pointer_cast<T>(readObject());
		}
	}
}

#endif /* _OBJECTS_STREAM_H */
