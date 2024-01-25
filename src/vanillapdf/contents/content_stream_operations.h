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
	OperationBeginText() = default;
	explicit OperationBeginText(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::BeginText; }
	virtual std::string ToPdf() const override;
};

class OperationEndText : public OperationBase {
public:
	OperationEndText() = default;
	explicit OperationEndText(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::EndText; }
	virtual std::string ToPdf() const override;
};

class OperationSaveGraphicsState : public OperationBase {
public:
	OperationSaveGraphicsState() = default;
	explicit OperationSaveGraphicsState(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::SaveGraphicsState; }
	virtual std::string ToPdf() const override;
};

class OperationRestoreGraphicsState : public OperationBase {
public:
	OperationRestoreGraphicsState() = default;
	explicit OperationRestoreGraphicsState(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::RestoreGraphicsState; }
	virtual std::string ToPdf() const override;
};

class OperationSetStrokingColorSpaceRGB : public OperationBase, public IModifyObserver {
public:
	OperationSetStrokingColorSpaceRGB() = default;
	explicit OperationSetStrokingColorSpaceRGB(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::SetStrokingColorSpaceRGB; }
	virtual std::string ToPdf() const override;
	syntax::RealObjectPtr GetRed() const { return m_red; }
	syntax::RealObjectPtr GetGreen() const { return m_green; }
	syntax::RealObjectPtr GetBlue() const { return m_blue; }

	void SetRed(syntax::RealObjectPtr value) {
		m_red->Unsubscribe(this);
		m_red = value;
		m_red->Subscribe(this);
		OnChanged();
	}

	void SetGreen(syntax::RealObjectPtr value) {
		m_green->Unsubscribe(this);
		m_green = value;
		m_green->Subscribe(this);
		OnChanged();
	}

	void SetBlue(syntax::RealObjectPtr value) {
		m_blue->Unsubscribe(this);
		m_blue = value;
		m_blue->Subscribe(this);
		OnChanged();
	}

	virtual void ObserveeChanged(const IModifyObservable*) override {
		OnChanged();
	}

	~OperationSetStrokingColorSpaceRGB();

private:
	syntax::RealObjectPtr m_red;
	syntax::RealObjectPtr m_green;
	syntax::RealObjectPtr m_blue;
};

class OperationSetNonstrokingColorSpaceRGB : public OperationBase, public IModifyObserver {
public:
	OperationSetNonstrokingColorSpaceRGB() = default;
	explicit OperationSetNonstrokingColorSpaceRGB(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::SetNonstrokingColorSpaceRGB; }
	virtual std::string ToPdf() const override;
	syntax::RealObjectPtr GetRed() const { return m_red; }
	syntax::RealObjectPtr GetGreen() const { return m_green; }
	syntax::RealObjectPtr GetBlue() const { return m_blue; }

	void SetRed(syntax::RealObjectPtr value) {
		m_red->Unsubscribe(this);
		m_red = value;
		m_red->Subscribe(this);
		OnChanged();
	}

	void SetGreen(syntax::RealObjectPtr value) {
		m_green->Unsubscribe(this);
		m_green = value;
		m_green->Subscribe(this);
		OnChanged();
	}

	void SetBlue(syntax::RealObjectPtr value) {
		m_blue->Unsubscribe(this);
		m_blue = value;
		m_blue->Subscribe(this);
		OnChanged();
	}

	virtual void ObserveeChanged(const IModifyObservable*) override { OnChanged(); }

	~OperationSetNonstrokingColorSpaceRGB();

private:
	syntax::RealObjectPtr m_red;
	syntax::RealObjectPtr m_green;
	syntax::RealObjectPtr m_blue;
};

class OperationTextFont : public OperationBase, public IModifyObserver {
public:
	OperationTextFont() = default;
	explicit OperationTextFont(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::TextFont; }
	virtual std::string ToPdf() const override;
	syntax::NameObjectPtr GetName() const { return m_font; }
	syntax::RealObjectPtr GetScale() const { return m_scale; }

	void SetName(syntax::NameObjectPtr value) {
		m_font->Unsubscribe(this);
		m_font = value;
		m_font->Subscribe(this);
		OnChanged();
	}

	void SetScale(syntax::RealObjectPtr value) {
		m_scale->Unsubscribe(this);
		m_scale = value;
		m_scale->Subscribe(this);
		OnChanged();
	}

	virtual void ObserveeChanged(const IModifyObservable*) override {
		OnChanged();
	}

	~OperationTextFont();

private:
	syntax::NameObjectPtr m_font;
	syntax::RealObjectPtr m_scale;
};

class OperationTextTranslate : public OperationBase, public IModifyObserver {
public:
	OperationTextTranslate() = default;
	explicit OperationTextTranslate(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::TextTranslate; }
	virtual std::string ToPdf() const override;
	syntax::IntegerObjectPtr GetX() const { return m_x; }
	syntax::IntegerObjectPtr GetY() const { return m_y; }

	void SetX(syntax::IntegerObjectPtr value) {
		m_x->Unsubscribe(this);
		m_x = value;
		m_x->Subscribe(this);
		OnChanged();
	}

	void SetY(syntax::IntegerObjectPtr value) {
		m_y->Unsubscribe(this);
		m_y = value;
		m_y->Subscribe(this);
		OnChanged();
	}

	virtual void ObserveeChanged(const IModifyObservable*) override {
		OnChanged();
	}

	~OperationTextTranslate();

private:
	syntax::IntegerObjectPtr m_x;
	syntax::IntegerObjectPtr m_y;
};

class OperationTextShow : public OperationBase, public IModifyObserver {
public:
	OperationTextShow() = default;
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

	virtual void ObserveeChanged(const IModifyObservable*) override {
		OnChanged();
	}

	~OperationTextShow();

private:
	syntax::StringObjectPtr _str;
};

class OperationTextShowArray : public OperationBase, public IModifyObserver {
public:
	OperationTextShowArray() = default;
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

	virtual void ObserveeChanged(const IModifyObservable*) override {
		OnChanged();
	}

	~OperationTextShowArray();

private:
	syntax::MixedArrayObjectPtr m_items;
};

class OperationBeginInlineImageObject : public OperationBase {
public:
	OperationBeginInlineImageObject() = default;
	explicit OperationBeginInlineImageObject(const std::vector<syntax::ObjectPtr>& operands);
	virtual Type GetOperationType(void) const noexcept override { return Type::BeginInlineImageObject; }
	virtual std::string ToPdf() const override;
};

class OperationBeginInlineImageData : public OperationBase {
public:
	OperationBeginInlineImageData() = default;
	explicit OperationBeginInlineImageData(const std::vector<syntax::ObjectPtr>& operands);
	virtual Type GetOperationType(void) const noexcept override { return Type::BeginInlineImageData; }
	virtual std::string ToPdf() const override;
};

class OperationEndInlineImageObject : public OperationBase {
public:
	OperationEndInlineImageObject() = default;
	explicit OperationEndInlineImageObject(const std::vector<syntax::ObjectPtr>& operands);
	virtual Type GetOperationType(void) const noexcept override { return Type::EndInlineImageObject; }
	virtual std::string ToPdf() const override;
};

class OperationTransformationMatrix : public OperationBase, public IModifyObserver {
public:
	OperationTransformationMatrix() = default;
	explicit OperationTransformationMatrix(const std::vector<syntax::ObjectPtr>& operands);
	virtual Type GetOperationType(void) const noexcept override { return Type::TransformationMatrix; }
	virtual std::string ToPdf() const override;

	virtual void ObserveeChanged(const IModifyObservable*) override {
		OnChanged();
	}

	~OperationTransformationMatrix();

	syntax::IntegerObjectPtr GetA() const { return m_a; }
	syntax::IntegerObjectPtr GetB() const { return m_b; }
	syntax::IntegerObjectPtr GetC() const { return m_c; }
	syntax::IntegerObjectPtr GetD() const { return m_d; }
	syntax::IntegerObjectPtr GetE() const { return m_e; }
	syntax::IntegerObjectPtr GetF() const { return m_f; }

	void SetA(syntax::IntegerObjectPtr value) {
		m_a->Unsubscribe(this);
		m_a = value;
		m_a->Subscribe(this);
		OnChanged();
	}

	void SetB(syntax::IntegerObjectPtr value) {
		m_b->Unsubscribe(this);
		m_b = value;
		m_b->Subscribe(this);
		OnChanged();
	}

	void SetC(syntax::IntegerObjectPtr value) {
		m_c->Unsubscribe(this);
		m_c = value;
		m_c->Subscribe(this);
		OnChanged();
	}

	void SetD(syntax::IntegerObjectPtr value) {
		m_d->Unsubscribe(this);
		m_d = value;
		m_d->Subscribe(this);
		OnChanged();
	}

	void SetE(syntax::IntegerObjectPtr value) {
		m_e->Unsubscribe(this);
		m_e = value;
		m_e->Subscribe(this);
		OnChanged();
	}

	void SetF(syntax::IntegerObjectPtr value) {
		m_f->Unsubscribe(this);
		m_f = value;
		m_f->Subscribe(this);
		OnChanged();
	}

private:
	syntax::IntegerObjectPtr m_a;
	syntax::IntegerObjectPtr m_b;
	syntax::IntegerObjectPtr m_c;
	syntax::IntegerObjectPtr m_d;
	syntax::IntegerObjectPtr m_e;
	syntax::IntegerObjectPtr m_f;
};

} // contents
} // vanillapdf

#endif /* _CONTENT_STREAM_OPERATIONS_H */
