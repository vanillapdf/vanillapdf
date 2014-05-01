#ifndef _INDIRECT_OBJECT_H
#define _INDIRECT_OBJECT_H

#include "Fwd.h"
#include "BaseObjects/Object.h"
#include "Interface/IIndirectObject.h"

#include "boost/intrusive_ptr.hpp"

#include <memory>

namespace Pdf
{
	class IndirectObject : public Object, public IIndirectObject
	{
	public:
		IndirectObject(std::shared_ptr<Files::File> file, int objNumber, int genNumber, std::streamoff offset = std::_BADOFF);
		IndirectObject(std::shared_ptr<Files::File> file, const Buffer& value);
		IndirectObject(const IndirectObject& other);

		void SetObject(boost::intrusive_ptr<Object> ref);
		boost::intrusive_ptr<Object> GetObject() const;
		void SetOffset(streamOffsetValueType offset);

		/* IIndirectObject */
		virtual IObject* GetIObject() const override;
		virtual streamOffsetValueType GetOffset() const override;

	private:
		std::shared_ptr<Files::File> _file;
		int _objNumber;
		int _genNumber;
		streamOffsetValueType _offset;

		mutable boost::intrusive_ptr<Object> _reference;

		friend void ::boost::intrusive_ptr_add_ref(IndirectObject*);
		friend void ::boost::intrusive_ptr_release(IndirectObject*);
	};
}

#endif /* _INDIRECT_OBJECT_H */
