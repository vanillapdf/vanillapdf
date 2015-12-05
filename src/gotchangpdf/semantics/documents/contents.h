#ifndef _CONTENTS_H
#define _CONTENTS_H

#include "semantics_fwd.h"
#include "content_stream.h"

#include <vector>

namespace gotchangpdf
{
	namespace semantics
	{
		namespace contents
		{
			class OperationBeginText : public syntax::contents::OperationBase
			{
			public:
				inline virtual Type GetOperationType(void) const _NOEXCEPT override { return Type::BeginText; }
			};

			class OperationEndText : public syntax::contents::OperationBase
			{
			public:
				inline virtual Type GetOperationType(void) const _NOEXCEPT override { return Type::EndText; }
			};

			class OperationTextShow : public syntax::contents::OperationBase
			{
			public:
				explicit OperationTextShow(syntax::StringObjectPtr str) : _str(str) {}

				inline virtual Type GetOperationType(void) const _NOEXCEPT override { return Type::TextShow; }

				syntax::StringObjectPtr Value() const { return _str; }

			private:
				syntax::StringObjectPtr _str;
			};

			typedef std::vector<syntax::contents::OperationBasePtr> OperationCollection;

			class ContentObjectBase : public syntax::contents::InstructionBase
			{
			public:
				enum class Type
				{
					TextObject
				};

				virtual Type GetType(void) const _NOEXCEPT = 0;
				inline virtual syntax::contents::InstructionBase::Type GetInstructionType(void) const _NOEXCEPT override { return InstructionBase::Type::Object; }
			};

			class TextObject : public ContentObjectBase
			{
			public:
				TextObject(OperationCollection ops) : _operations(ops) {}

				inline virtual Type GetType(void) const _NOEXCEPT override { return Type::TextObject; }

				types::uinteger GetOperationsSize(void) const { return _operations.size(); }
				syntax::contents::OperationBasePtr GetOperationAt(types::uinteger at) const { return _operations.at(at); }

			private:
				OperationCollection _operations;
			};

			typedef std::vector<syntax::contents::InstructionBasePtr> InstructionCollection;
		}

		class Contents : public IUnknown
		{
		public:
			Contents() = default;
			explicit Contents(syntax::StreamObjectPtr obj);
			explicit Contents(syntax::ArrayObjectPtr<syntax::IndirectObjectReferencePtr> obj);

			contents::InstructionCollection Instructions(void) const;
			types::uinteger GetInstructionsSize(void) const;
			syntax::contents::InstructionBasePtr GetInstructionAt(types::uinteger at) const;

			inline types::uinteger GetContentStreamSize(void) const { return _contents.size(); }
			inline ContentStreamPtr GetContentStreamAt(types::uinteger at) const { return _contents.at(at); }

		private:
			std::vector<ContentStreamPtr> _contents;
			mutable contents::InstructionCollection _instructions;
		};
	}
}

#endif /* _CONTENTS_H */
