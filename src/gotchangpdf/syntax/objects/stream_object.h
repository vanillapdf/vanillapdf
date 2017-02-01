#ifndef _STREAM_OBJECT_H
#define _STREAM_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/object.h"
#include "syntax/objects/dictionary_object.h"

#include "utils/buffer.h"

namespace gotchangpdf {
namespace syntax {

class StreamObject : public Object, public IModifyObserver {
public:
	StreamObject();
	StreamObject(DictionaryObjectPtr header, types::stream_offset offset);
	virtual Object::Type GetType(void) const noexcept override;
	virtual std::string ToString(void) const override;
	virtual std::string ToPdf(void) const override;

	virtual void ObserveeChanged(IModifyObservable* observee) override;

	DictionaryObjectPtr GetHeader() const;
	void SetHeader(DictionaryObjectPtr header);

	types::stream_offset GetDataOffset() const;
	void SetDataOffset(types::stream_offset offset);

	BufferPtr GetBodyRaw() const;
	BufferPtr GetBody() const;
	BufferPtr GetBodyEncoded() const;
	void SetBody(BufferPtr value);

	virtual StreamObject* Clone(void) const override;
	virtual bool Equals(ObjectPtr other) const override;

	virtual void SetFile(WeakReference<File> file) noexcept override;
	virtual void SetInitialized(bool initialized = true) noexcept override;

	virtual ~StreamObject();

private:
	DictionaryObjectPtr _header;
	types::stream_offset _raw_data_offset = constant::BAD_OFFSET;

	mutable BufferPtr _body;
	mutable BufferPtr _body_decoded;
};

} // syntax
} // gotchangpdf

#endif /* _STREAM_OBJECT_H */
