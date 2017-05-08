#ifndef _STRING_OBJECT_H
#define _STRING_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"
#include "utils/buffer.h"

namespace gotchangpdf {
namespace syntax {

class StringObjectBase : public ContainableObject {
public:
	enum class StringType {
		Literal = 0,
		Hexadecimal
	};

	virtual BufferPtr GetValue() const = 0;
	virtual void SetValue(BufferPtr value) = 0;

	virtual StringType GetStringType(void) const noexcept = 0;
	virtual Object::Type GetType(void) const noexcept override { return Object::Type::String; }

	bool Equals(const StringObjectBase& other) const { return GetValue() == other.GetValue(); }
	bool operator==(const StringObjectBase& other) const { return Equals(other); }
	bool operator<(const StringObjectBase& other) const { return GetValue() < other.GetValue(); }

	virtual bool Equals(ObjectPtr other) const override;
	virtual size_t Hash() const override;
};

class HexadecimalStringObject : public StringObjectBase, public IModifyObserver {
public:
	HexadecimalStringObject();
	explicit HexadecimalStringObject(BufferPtr value);
	explicit HexadecimalStringObject(const char * chars);
	explicit HexadecimalStringObject(std::string value);

	virtual void ObserveeChanged(IModifyObservable*) override { OnChanged(); }

	virtual StringObjectBase::StringType GetStringType(void) const noexcept override { return StringObjectBase::StringType::Hexadecimal; }

	virtual BufferPtr GetValue() const override;
	virtual void SetValue(BufferPtr value) override { _value->assign(value.begin(), value.end()); }
	virtual std::string ToPdf(void) const override;

	virtual HexadecimalStringObject* Clone(void) const override;

	virtual ~HexadecimalStringObject();

private:
	BufferPtr _raw_value;
	mutable BufferPtr _value;
};

class LiteralStringObject : public StringObjectBase, public IModifyObserver {
public:
	LiteralStringObject();
	explicit LiteralStringObject(BufferPtr value);
	explicit LiteralStringObject(const char * chars);
	explicit LiteralStringObject(std::string value);

	virtual void ObserveeChanged(IModifyObservable*) override { OnChanged(); }

	virtual StringObjectBase::StringType GetStringType(void) const noexcept override { return StringObjectBase::StringType::Literal; }
	virtual BufferPtr GetValue() const override;
	virtual void SetValue(BufferPtr value) override { _value->assign(value.begin(), value.end()); }
	virtual std::string ToPdf(void) const override;

	virtual LiteralStringObject* Clone(void) const override;

	virtual ~LiteralStringObject();

private:
	BufferPtr _raw_value;
	mutable BufferPtr _value;
};

class StringObjectPtr : public Deferred<StringObjectBase> {
public:
	using Deferred<StringObjectBase>::Deferred;

public:
	StringObjectPtr();
};

} // syntax

inline bool operator==(const Deferred<syntax::StringObjectBase>& left, const Deferred<syntax::StringObjectBase>& right) {
	return *left == *right;
}

inline bool operator<(const Deferred<syntax::StringObjectBase>& left, const Deferred<syntax::StringObjectBase>& right) {
	return *left < *right;
}

} // gotchangpdf

#endif /* _STRING_OBJECT_H */
