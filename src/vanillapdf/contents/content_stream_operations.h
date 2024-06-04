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

class OperationSetStrokingColorSpaceRGB : public OperationBase {
public:
	OperationSetStrokingColorSpaceRGB() = default;
	explicit OperationSetStrokingColorSpaceRGB(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::SetStrokingColorSpaceRGB; }
	virtual std::string ToPdf() const override;
	syntax::RealObjectPtr GetRed() const { return m_red; }
	syntax::RealObjectPtr GetGreen() const { return m_green; }
	syntax::RealObjectPtr GetBlue() const { return m_blue; }

	void SetRed(syntax::RealObjectPtr value) {
		m_red = value;
	}

	void SetGreen(syntax::RealObjectPtr value) {
		m_green = value;
	}

	void SetBlue(syntax::RealObjectPtr value) {
		m_blue = value;
	}

private:
	syntax::RealObjectPtr m_red;
	syntax::RealObjectPtr m_green;
	syntax::RealObjectPtr m_blue;
};

class OperationSetNonstrokingColorSpaceRGB : public OperationBase {
public:
	OperationSetNonstrokingColorSpaceRGB() = default;
	explicit OperationSetNonstrokingColorSpaceRGB(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::SetNonstrokingColorSpaceRGB; }
	virtual std::string ToPdf() const override;
	syntax::RealObjectPtr GetRed() const { return m_red; }
	syntax::RealObjectPtr GetGreen() const { return m_green; }
	syntax::RealObjectPtr GetBlue() const { return m_blue; }

	void SetRed(syntax::RealObjectPtr value) {
		m_red = value;
	}

	void SetGreen(syntax::RealObjectPtr value) {
		m_green = value;
	}

	void SetBlue(syntax::RealObjectPtr value) {
		m_blue = value;
	}

private:
	syntax::RealObjectPtr m_red;
	syntax::RealObjectPtr m_green;
	syntax::RealObjectPtr m_blue;
};

class OperationTextFont : public OperationBase {
public:
	OperationTextFont() = default;
	explicit OperationTextFont(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::TextFont; }
	virtual std::string ToPdf() const override;
	syntax::NameObjectPtr GetName() const { return m_font; }
	syntax::RealObjectPtr GetScale() const { return m_scale; }

	void SetName(syntax::NameObjectPtr value) {
		m_font = value;
	}

	void SetScale(syntax::RealObjectPtr value) {
		m_scale = value;
	}

private:
	syntax::NameObjectPtr m_font;
	syntax::RealObjectPtr m_scale;
};

class OperationTextTranslate : public OperationBase {
public:
	OperationTextTranslate() = default;
	explicit OperationTextTranslate(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::TextTranslate; }
	virtual std::string ToPdf() const override;
	syntax::IntegerObjectPtr GetX() const { return m_x; }
	syntax::IntegerObjectPtr GetY() const { return m_y; }

	void SetX(syntax::IntegerObjectPtr value) {
		m_x = value;
	}

	void SetY(syntax::IntegerObjectPtr value) {
		m_y = value;
	}

private:
	syntax::IntegerObjectPtr m_x;
	syntax::IntegerObjectPtr m_y;
};

class OperationTextShow : public OperationBase {
public:
	OperationTextShow() = default;
	explicit OperationTextShow(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::TextShow; }
	virtual std::string ToPdf() const override;
	syntax::StringObjectPtr GetValue() const { return _str; }
	void SetValue(syntax::StringObjectPtr value) {
		_str = value;
	}

private:
	syntax::StringObjectPtr _str;
};

class OperationTextShowArray : public OperationBase {
public:
	OperationTextShowArray() = default;
	explicit OperationTextShowArray(const std::vector<syntax::ObjectPtr>& operands);

	virtual Type GetOperationType(void) const noexcept override { return Type::TextShowArray; }
	virtual std::string ToPdf() const override;
	syntax::MixedArrayObjectPtr GetValue() const { return m_items; }
	void SetValue(syntax::MixedArrayObjectPtr value) {
		m_items = value;
	}

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

class OperationTransformationMatrix : public OperationBase {
public:
	OperationTransformationMatrix() = default;
	explicit OperationTransformationMatrix(const std::vector<syntax::ObjectPtr>& operands);
	virtual Type GetOperationType(void) const noexcept override { return Type::TransformationMatrix; }
	virtual std::string ToPdf() const override;

	syntax::IntegerObjectPtr GetA() const { return m_a; }
	syntax::IntegerObjectPtr GetB() const { return m_b; }
	syntax::IntegerObjectPtr GetC() const { return m_c; }
	syntax::IntegerObjectPtr GetD() const { return m_d; }
	syntax::IntegerObjectPtr GetE() const { return m_e; }
	syntax::IntegerObjectPtr GetF() const { return m_f; }

	void SetA(syntax::IntegerObjectPtr value) {
		m_a = value;
	}

	void SetB(syntax::IntegerObjectPtr value) {
		m_b = value;
	}

	void SetC(syntax::IntegerObjectPtr value) {
		m_c = value;
	}

	void SetD(syntax::IntegerObjectPtr value) {
		m_d = value;
	}

	void SetE(syntax::IntegerObjectPtr value) {
		m_e = value;
	}

	void SetF(syntax::IntegerObjectPtr value) {
		m_f = value;
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
