#ifndef _OBJECTS_STREAM_H
#define _OBJECTS_STREAM_H

#include "fwd.h"
#include "lexical_stream.h"
#include "object_reference_wrapper.h"

namespace gotchangpdf
{
	namespace lexical
	{
		class Parser : public lexical::Stream
		{
		public:
			Parser(std::shared_ptr<files::File> file, std::shared_ptr<std::istream> stream);

			template<typename T>
			ObjectReferenceWrapper<T> readObjectWithType() { return readObject().GetAs<T>(); }

			ObjectReferenceWrapper<Object> readObjectWithType(Object::Type type);
			ObjectReferenceWrapper<Object> readObject();
			ObjectReferenceWrapper<Object> peekObject();

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

#endif /* _OBJECTS_STREAM_H */
