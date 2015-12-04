#ifndef _CONTENTS_H
#define _CONTENTS_H

#include "semantics_fwd.h"
#include "content_stream.h"

#include <vector>

namespace gotchangpdf
{
	namespace semantics
	{
		class BeginTextOperation : public syntax::contents::OperationBase
		{
		public:
			inline virtual Type GetOperationType(void) const _NOEXCEPT override { return Type::BeginText; }
		};

		class EndTextOperation : public syntax::contents::OperationBase
		{
		public:
			inline virtual Type GetOperationType(void) const _NOEXCEPT override { return Type::EndText; }
		};

		class TextShowOperation : public syntax::contents::OperationBase
		{
		public:
			explicit TextShowOperation(syntax::StringObjectPtr str);

			inline virtual Type GetOperationType(void) const _NOEXCEPT override { return Type::TextShow; }

		private:
			syntax::StringObjectPtr _str;
		};

		typedef Deferred<BeginTextOperation> BeginTextOperationPtr;
		typedef Deferred<EndTextOperation> EndTextOperationPtr;
		typedef Deferred<TextShowOperation> TextShowOperationPtr;

		typedef std::vector<syntax::contents::OperationBasePtr> ContentOperationCollection;

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
			TextObject(ContentOperationCollection ops) : _operations(ops) {}

			inline virtual Type GetType(void) const _NOEXCEPT override { return Type::TextObject; }

			types::uinteger GetOperationsSize(void) const { return _operations.size(); }
			syntax::contents::OperationBasePtr GetOperationAt(types::uinteger at) const { return _operations.at(at); }

		private:
			ContentOperationCollection _operations;
		};

		typedef Deferred<TextObject> TextObjectPtr;

		typedef std::vector<syntax::contents::InstructionBasePtr> ContentInstructionCollection;

		class Contents : public IUnknown
		{
		public:
			Contents() = default;
			explicit Contents(syntax::StreamObjectPtr obj);
			explicit Contents(syntax::ArrayObjectPtr<syntax::IndirectObjectReferencePtr> obj);

			ContentInstructionCollection Instructions(void) const;
			types::uinteger GetInstructionsSize(void) const;
			syntax::contents::InstructionBasePtr GetInstructionAt(types::uinteger at) const;

			inline types::uinteger GetContentStreamSize(void) const { return _contents.size(); }
			inline ContentStreamPtr GetContentStreamAt(types::uinteger at) const { return _contents.at(at); }

		private:
			std::vector<ContentStreamPtr> _contents;
			mutable ContentInstructionCollection _instructions;
		};
	}
}

#endif /* _CONTENTS_H */
