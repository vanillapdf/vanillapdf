#ifndef _OBJECTS_STREAM_H
#define _OBJECTS_STREAM_H

#include "fwd.h"
#include "lexical_stream.h"

#include "boost/intrusive_ptr.hpp"

namespace gotchangpdf
{
	namespace lexical
	{
		class Parser : public lexical::Stream
		{
		public:
			Parser(std::shared_ptr<files::File> file, std::shared_ptr<std::istream> stream);

			template<typename T>
			boost::intrusive_ptr<T> readObjectWithType();

			boost::intrusive_ptr<gotchangpdf::Object> readObjectWithType(gotchangpdf::IObject::Type type);
			boost::intrusive_ptr<gotchangpdf::Object> readObject();
			boost::intrusive_ptr<gotchangpdf::Object> peekObject();

			std::shared_ptr<files::File> file(void) const;
			//void SetDeep(bool deep);
			//bool GetDeep(void) const;

			gotchangpdf::lexical::Parser::Parser(const gotchangpdf::lexical::Parser &);

		private:
			std::shared_ptr<files::File> _file;
			//bool _deep;
		};

		template<typename T>
		boost::intrusive_ptr<T>
			gotchangpdf::lexical::Parser::readObjectWithType()
		{
			return boost::dynamic_pointer_cast<T>(readObject());
		}
	}
}

#endif /* _OBJECTS_STREAM_H */
