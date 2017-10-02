#ifndef _CONTENT_STREAM_OPERATIONS_H
#define _CONTENT_STREAM_OPERATIONS_H

#include "contents/content_fwd.h"
#include "contents/content_stream_operation_base.h"

#include "syntax/objects/object.h"
#include "syntax/objects/string_object.h"
#include "syntax/objects/name_object.h"
#include "syntax/objects/integer_object.h"

#include <vector>

namespace vanillapdf {
namespace contents {

class OperationBeginText : public OperationBase {
public:
	explicit OperationBeginText(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::BeginText; }
	virtual std::string ToPdf() const override;
};

class OperationEndText : public OperationBase {
public:
	explicit OperationEndText(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::EndText; }
	virtual std::string ToPdf() const override;
};

class OperationTextFont : public OperationBase, public IModifyObserver {
public:
	explicit OperationTextFont(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::TextFont; }
	virtual std::string ToPdf() const override;
	syntax::NameObjectPtr GetName() const { return m_font; }
	syntax::IntegerObjectPtr GetScale() const { return m_scale; }

	void SetName(syntax::NameObjectPtr value) {
		m_font->Unsubscribe(this);
		m_font = value;
		m_font->Subscribe(this);
		OnChanged();
	}

	void SetScale(syntax::IntegerObjectPtr value) {
		m_scale->Unsubscribe(this);
		m_scale = value;
		m_scale->Subscribe(this);
		OnChanged();
	}

	virtual void ObserveeChanged(IModifyObservable*) override { OnChanged(); }

	~OperationTextFont();

private:
	syntax::NameObjectPtr m_font;
	syntax::IntegerObjectPtr m_scale;
};

class OperationTextShow : public OperationBase, public IModifyObserver {
public:
	explicit OperationTextShow(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::TextShow; }
	virtual std::string ToPdf() const override;
	syntax::StringObjectPtr GetValue() const { return _str; }
	void SetValue(syntax::StringObjectPtr value) {
		_str->Unsubscribe(this);
		_str = value;
		_str->Subscribe(this);
		OnChanged();
	}

	virtual void ObserveeChanged(IModifyObservable*) override { OnChanged(); }

	~OperationTextShow();

private:
	syntax::StringObjectPtr _str;
};

class OperationTextShowArray : public OperationBase, public IModifyObserver {
public:
	explicit OperationTextShowArray(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::TextShowArray; }
	virtual std::string ToPdf() const override;
	syntax::MixedArrayObjectPtr GetValue() const { return m_items; }
	void SetValue(syntax::MixedArrayObjectPtr value) {
		m_items->Unsubscribe(this);
		m_items = value;
		m_items->Subscribe(this);
		OnChanged();
	}

	virtual void ObserveeChanged(IModifyObservable*) override { OnChanged(); }

	~OperationTextShowArray();

private:
	syntax::MixedArrayObjectPtr m_items;
};

class OperationBeginInlineImageObject : public OperationBase {
public:
	explicit OperationBeginInlineImageObject(const std::vector<syntax::ObjectPtr>& operands);
	virtual Type GetOperationType(void) const noexcept override { return Type::BeginInlineImageObject; }
	virtual std::string ToPdf() const override;
};

class OperationBeginInlineImageData : public OperationBase {
public:
	explicit OperationBeginInlineImageData(const std::vector<syntax::ObjectPtr>& operands);
	virtual Type GetOperationType(void) const noexcept override { return Type::BeginInlineImageData; }
	virtual std::string ToPdf() const override;
};

class OperationEndInlineImageObject : public OperationBase {
public:
	explicit OperationEndInlineImageObject(const std::vector<syntax::ObjectPtr>& operands);
	virtual Type GetOperationType(void) const noexcept override { return Type::EndInlineImageObject; }
	virtual std::string ToPdf() const override;
};

} // contents
} // vanillapdf

#endif /* _CONTENT_STREAM_OPERATIONS_H */
