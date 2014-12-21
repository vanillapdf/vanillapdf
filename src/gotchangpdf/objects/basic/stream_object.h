#ifndef _STREAM_OBJECT_H
#define _STREAM_OBJECT_H

#include "fwd.h"
#include "constants.h"
#include "dictionary_object.h"
#include "deferred.h"
#include "buffer.h"

#include <memory>

namespace gotchangpdf
{
	class StreamObject : public Object
	{
	public:
		enum class Type : unsigned char
		{
			UNKNOWN = 0,
			CLASSIC,
			OBJECT_STREAM
		};

		explicit StreamObject(files::File * file);
		explicit StreamObject(const DictionaryObject& dictionary);

		Buffer GetData() const;

		virtual inline Object::Type GetType(void) const override { return Object::Type::StreamObject; }

		//friend Streams::Lexical::ReverseStream& operator>> (Streams::Lexical::ReverseStream& s, StreamObject& o);
		friend lexical::Parser& operator>> (lexical::Parser& s, StreamObject& o);

	public:
		DictionaryObjectPtr _dictionary;
		types::stream_offset _raw_data_offset = std::_BADOFF;

	private:
		mutable Buffer _data;
		Type _type = Type::UNKNOWN;

		files::File *_file;

		explicit StreamObject() = default;

		friend Deferred<StreamObject>;

		template <typename T>
		friend T* Allocate();
	};
}

#endif /* _STREAM_OBJECT_H */
