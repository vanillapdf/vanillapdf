#ifndef _NULL_OBJECT_H
#define _NULL_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

namespace vanillapdf {
namespace syntax {

class NullObject : public ContainableObject {
public:
	virtual Object::Type GetObjectType(void) const noexcept override { return Object::Type::Null; }
	virtual void ToPdfStreamInternal(IOutputStreamPtr output) const override;

	static NullObjectPtr GetInstance(void);

	virtual size_t Hash() const override;
	bool Equals(const NullObject& other) const;
	virtual bool Equals(ObjectPtr other) const override;

	virtual NullObject* Clone(void) const override;

	bool operator==(const NullObject& other) const { return Equals(other); }
	bool operator!=(const NullObject& other) const { return !Equals(other); }
	bool operator<(const NullObject&) const { return false; }

	// IObservable<IModifyObserver> overrides

	virtual void Subscribe(const WeakReference<IModifyObserver>&) override {
		// Do nothing - Null object never changes
	}

	virtual bool Unsubscribe(const WeakReference<IModifyObserver>&) override {
		return true;
	}

private:
	NullObject() = default;
};

} // syntax
} // vanillapdf

#endif /* _NULL_OBJECT_H */
