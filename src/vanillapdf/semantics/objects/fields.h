#ifndef _FIELDS_H
#define _FIELDS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"
#include "semantics/objects/digital_signature.h"

namespace vanillapdf {
namespace semantics {

class FieldCollection : public HighLevelObject<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>> {
public:
    explicit FieldCollection(syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr> root) : HighLevelObject(root) {}

    types::size_type GetSize() const;
    FieldPtr At(types::size_type index) const;
};

// A view over one field dictionary (12.7.3.1). The hierarchy it belongs to
// is the concern of FieldTree: the root /Fields array is the top of the
// hierarchy and the tree enumerates that level, a field enumerates its own
// /Kids below it. A field knows its ancestors through /Parent only.
class Field : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    enum Type {
        Undefined = 0,
        NonTerminal,
        Button,
        Text,
        Choice,
        Signature
    };

    // Form of quadding (justification) of variable text fields (Table 222)
    enum class Quadding {
        LeftJustified = 0,
        Centered = 1,
        RightJustified = 2
    };

public:
    explicit Field(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

    // The typed view over a dictionary, chosen by /FT resolved through the
    // /Parent chain - it is inheritable, and a field merged with its widget
    // annotation usually carries it on the parent only. The type says
    // nothing about the position in the hierarchy: a group under a typed
    // ancestor is a TextField / ButtonField / ... too, and IsTerminal is
    // the question to ask about the position.
    static FieldPtr Create(syntax::DictionaryObjectPtr root);

    virtual Field::Type GetFieldType() const noexcept = 0;

    // A /Kids entry is a child field when it carries /T, /Kids or /FT;
    // otherwise it is a widget annotation. A merged field carries both /T
    // and /Subtype /Widget and is a child field. Widgets never have /T; a
    // nameless intermediate node is legal (12.7.3.2) and recognized by its
    // /Kids, the same way QPDF classifies the entries.
    static bool IsFieldDictionary(const syntax::DictionaryObjectPtr& dictionary);

    // The kinds of entry a /Fields or /Kids array holds. Both arrays shall
    // hold indirect references (Table 218, Table 220): a reference to a
    // field dictionary is a child field, a reference to any other
    // dictionary is a widget annotation. Anything else - a direct
    // dictionary, a reference to something other than a dictionary - is
    // malformed. It does occur in existing files and is skipped with a
    // warning rather than failing the enumeration, the same policy every
    // hierarchy walker applies to a cyclic /Kids link.
    enum class ChildEntryType {
        Undefined = 0,
        Field,
        Widget,
        Malformed
    };

    // Classifies one entry of a /Fields or /Kids array, resolving the
    // dictionary for a field or a widget. The tree and the field's own
    // child walk share this one classification, so the flat view stays a
    // projection of the structural one.
    static ChildEntryType ClassifyChildEntry(const syntax::ObjectPtr& entry, syntax::OutputDictionaryObjectPtr& dictionary);

    // A terminal field has no children other than widget annotations (12.7.3.2).
    static bool IsTerminalDictionary(const syntax::DictionaryObjectPtr& dictionary);
    bool IsTerminal() const;

    // Child fields in /Kids order - widget annotations are not children,
    // malformed entries are skipped as ClassifyChildEntry describes. Zero
    // for a terminal field. The level above the root-level fields is the
    // /Fields array, enumerated by FieldTree::GetRootChild.
    types::size_type GetChildCount() const;
    FieldPtr GetChild(types::size_type index) const;

    // The parent field through /Parent. A root-level field has none -
    // Table 220 requires /Parent for kids only - and reports false.
    bool GetParent(OuputFieldPtr& result) const;

    // Fully qualified field name: the /T partial names joined with '.' from
    // the root of the hierarchy down to this field (12.7.3.2). Levels without
    // a /T entry do not contribute a segment. Each /T is a text string
    // (7.9.2.2) normalized to UTF-8, so the result is a UTF-8 buffer
    // regardless of how the partial names are encoded in the document.
    BufferPtr GetQualifiedName() const;

    bool GetName(syntax::OutputStringObjectPtr& result) const;

    // Sets the partial field name (/T). Because the PERIOD separates the
    // segments of fully qualified names, a partial name shall not contain
    // one (12.7.3.2) - such a value is an InvalidParameterException
    void SetName(syntax::StringObjectPtr value);

    bool GetAlternateName(syntax::OutputStringObjectPtr& result) const;
    void SetAlternateName(syntax::StringObjectPtr value);

    bool GetFieldFlags(types::big_int& result) const;
    void SetFieldFlags(types::big_int value);

    // The field value (/V) and default value (/DV) as raw objects, resolved
    // through the /Parent chain (Table 220). Their type depends on the
    // field type - a name for buttons, a text string for text fields, a
    // text string or an array for choice fields, a dictionary for signature
    // fields - which the typed subclasses expose directly.
    bool GetValueObject(syntax::OutputObjectPtr& result) const;
    bool GetDefaultValueObject(syntax::OutputObjectPtr& result) const;

    // Default appearance string (/DA) and quadding (/Q) for variable text
    // fields, resolved through the /Parent chain - this field and its
    // ancestors, nothing else. The document-wide default lives in the
    // AcroForm dictionary (12.7.3.3), which owns the last step of the
    // lookup: InteractiveForm::ResolveDefaultAppearance / ResolveQuadding.
    // The setters write this field's own dictionary, overriding any
    // inherited value.
    bool GetDefaultAppearance(syntax::OutputStringObjectPtr& result) const;
    void SetDefaultAppearance(syntax::StringObjectPtr value);
    bool GetQuadding(Quadding& result) const;
    void SetQuadding(Quadding value);

    // Maps a raw /Q value to the enumerated form; an unknown value read from
    // the document is a ParseException
    static Quadding ConvertQuadding(const syntax::IntegerObjectPtr& value);

    // Maps the enumerated form back to its raw /Q value; an unknown
    // enumeration value is a ConversionException
    static types::big_int ConvertQuadding(Quadding value);

protected:
    // Resolves an inheritable field attribute - /FT, /Ff, /V and /DV per Table 220,
    // /DA and /Q per 12.7.3.3 - by searching the field dictionary and then walking
    // the /Parent chain. Returns false when the key is present in none of them.
    // Each /Parent shall be an indirect reference (Table 220); a chain hop
    // that is not one ends the search. Visited parent references are tracked
    // by object identity, so a cyclic chain terminates instead of looping.
    static bool FindInheritedEntry(
        const syntax::DictionaryObjectPtr& dictionary,
        const syntax::NameObject& key,
        syntax::OutputObjectPtr& result);

    bool GetInheritedEntry(const syntax::NameObject& key, syntax::OutputObjectPtr& result) const;
};

class NonTerminalField : public Field {
public:
    explicit NonTerminalField(syntax::DictionaryObjectPtr root) : Field(root) {}
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::NonTerminal; }
};

class ButtonField : public Field {
public:
    explicit ButtonField(syntax::DictionaryObjectPtr root) : Field(root) {}
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::Button; }

    bool GetValue(syntax::OutputNameObjectPtr& result) const;
    void SetValue(syntax::NameObjectPtr value);
};

class TextField : public Field {
public:
    explicit TextField(syntax::DictionaryObjectPtr root) : Field(root) {}
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::Text; }

    bool GetValue(syntax::OutputStringObjectPtr& result) const;
    void SetValue(syntax::StringObjectPtr value);
    bool GetDefaultValue(syntax::OutputStringObjectPtr& result) const;
    bool GetMaxLength(syntax::OutputIntegerObjectPtr& result) const;
};

class ChoiceField : public Field {
public:
    explicit ChoiceField(syntax::DictionaryObjectPtr root) : Field(root) {}
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::Choice; }

    bool GetValue(syntax::OutputStringObjectPtr& result) const;
    void SetValue(syntax::StringObjectPtr value);
    bool GetOptionCount(types::size_type& result) const;
    bool GetOptionAt(types::size_type index, syntax::OutputContainableObjectPtr& result) const;
};

class SignatureField : public Field {
public:
    explicit SignatureField(syntax::DictionaryObjectPtr root) : Field(root) {}
    virtual Field::Type GetFieldType() const noexcept override { return Field::Type::Signature; }

    bool Value(OuputDigitalSignaturePtr& result) const;
};

} // semantics
} // vanillapdf

#endif /* _FIELDS_H */
