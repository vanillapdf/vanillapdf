#ifndef _CONTENT_STREAM_OBJECTS_H
#define _CONTENT_STREAM_OBJECTS_H

#include "syntax_fwd.h"
#include "constants.h"
#include "content_stream_operation_base.h"

#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		namespace contents
		{
			class ContentObjectBase : public syntax::contents::InstructionBase
			{
			public:
				enum class Type
				{
					TextObject,
					InlineImageObject
				};

				virtual Type GetType(void) const noexcept = 0;
				virtual syntax::contents::InstructionBase::Type GetInstructionType(void) const noexcept override { return InstructionBase::Type::Object; }
			};

			class TextObject : public ContentObjectBase
			{
			public:
				TextObject(contents::BaseOperationCollection ops) : _operations(ops) {}

				virtual Type GetType(void) const noexcept override { return Type::TextObject; }
				virtual std::string ToPdf() const override;

				types::uinteger GetOperationsSize(void) const { return _operations.size(); }
				OperationBasePtr GetOperationAt(types::uinteger at) const { return _operations.at(at); }

			private:
				BaseOperationCollection _operations;
			};

			class InlineImageObject : public ContentObjectBase
			{
			public:
				InlineImageObject(DictionaryObjectPtr dictionary, BufferPtr data)
					: m_dictionary(dictionary), m_data(data) {}

				virtual Type GetType(void) const noexcept override { return Type::InlineImageObject; }
				virtual std::string ToPdf() const override;

				DictionaryObjectPtr GetDictionary() const { return m_dictionary; }
				BufferPtr GetData() const { return m_data; }

			private:
				DictionaryObjectPtr m_dictionary;
				BufferPtr m_data;
			};
		}
	}
}

#endif /* _CONTENT_STREAM_OBJECTS_H */
