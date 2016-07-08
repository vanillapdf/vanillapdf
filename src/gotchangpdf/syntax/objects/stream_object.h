#ifndef _STREAM_OBJECT_H
#define _STREAM_OBJECT_H

#include "syntax_fwd.h"
#include "object.h"
#include "buffer.h"
#include "dictionary_object.h"

#include <iosfwd>

namespace gotchangpdf
{
	namespace syntax
	{
		class StreamObject : public Object, public IModifyObserver
		{
		public:
			StreamObject();
			StreamObject(DictionaryObjectPtr header, types::stream_offset offset);
			virtual Object::Type GetType(void) const noexcept override { return Object::Type::Stream; }
			virtual std::string ToString(void) const override;
			virtual std::string ToPdf(void) const override;

			virtual void ObserveeChanged(IModifyObservable*) override { OnChanged(); }

			DictionaryObjectPtr GetHeader() const { return _header; }
			void SetHeader(DictionaryObjectPtr header) { _header->Unsubscribe(this); header->Subscribe(this); _header = header; OnChanged(); }

			types::stream_offset GetDataOffset() const { return _raw_data_offset; }
			void SetDataOffset(types::stream_offset offset) { _raw_data_offset = offset; OnChanged(); }

			BufferPtr GetBodyRaw() const;
			BufferPtr GetBody() const;
			void SetBody(BufferPtr value) { _body_decoded->assign(value.begin(), value.end()); }

			virtual StreamObject* Clone(void) const override;

			virtual ~StreamObject()
			{
				_body->Unsubscribe(this);
				_body_decoded->Unsubscribe(this);
			}

		private:
			DictionaryObjectPtr _header;
			types::stream_offset _raw_data_offset = std::_BADOFF;

			mutable BufferPtr _body;
			mutable BufferPtr _body_decoded;
		};
	}
}

#endif /* _STREAM_OBJECT_H */
