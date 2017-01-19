#ifndef _FIELDS_H
#define _FIELDS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"
#include "semantics/objects/digital_signature.h"

namespace gotchangpdf {
namespace semantics {

class Field : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	enum Type {
		Button,
		Text,
		Choice,
		Signature
	};

public:
	explicit Field(syntax::DictionaryObjectPtr root);
	static std::unique_ptr<Field> Create(syntax::DictionaryObjectPtr root);

	virtual Field::Type GetType() const noexcept = 0;
};

class ButtonField : public Field {
public:
	explicit ButtonField(syntax::DictionaryObjectPtr root);
	virtual Field::Type GetType() const noexcept { return Field::Type::Button; }
};

class TextField : public Field {
public:
	explicit TextField(syntax::DictionaryObjectPtr root);
	virtual Field::Type GetType() const noexcept { return Field::Type::Text; }
};

class ChoiceField : public Field {
public:
	explicit ChoiceField(syntax::DictionaryObjectPtr root);
	virtual Field::Type GetType() const noexcept { return Field::Type::Choice; }
};

class SignatureField : public Field {
public:
	explicit SignatureField(syntax::DictionaryObjectPtr root);
	virtual Field::Type GetType() const noexcept { return Field::Type::Signature; }

	bool Value(OuputDigitalSignaturePtr result) const;
};

} // semantics
} // gotchangpdf

#endif /* _FIELDS_H */