#ifndef _CONTENTS_H
#define _CONTENTS_H

#include "semantics_fwd.h"
#include "content_stream.h"
#include "direct_object.h"

#include <vector>

namespace gotchangpdf
{
	namespace semantics
	{
		class BeginTextOperation : public syntax::contents::Operation
		{

		};

		class EndTextOperation : public syntax::contents::Operation
		{

		};

		typedef Deferred<BeginTextOperation> BeginTextOperationPtr;
		typedef Deferred<EndTextOperation> EndTextOperationPtr;

		typedef boost::variant<
			syntax::contents::OperationPtr,
			BeginTextOperationPtr,
			EndTextOperationPtr
		> ContentOperationPtr;

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
			TextObject(syntax::contents::OperationCollection ops) : _operations(ops) {}

			inline virtual Type GetType(void) const _NOEXCEPT override { return Type::TextObject; }

		private:
			syntax::contents::OperationCollection _operations;
		};

		typedef Deferred<TextObject> TextObjectPtr;

		typedef boost::variant<
			TextObjectPtr
		> ContentObjectPtr;

		typedef boost::variant<
			ContentObjectPtr,
			ContentOperationPtr
		> ContentInstructionPtr;

		class ContentStreamInstructionBaseVisitor : public boost::static_visitor<syntax::contents::InstructionBase*>
		{
		public:
			inline syntax::contents::InstructionBase* operator()(ContentObjectPtr obj) const { return obj.apply_visitor(*this); }
			inline syntax::contents::InstructionBase* operator()(ContentOperationPtr obj) const { return obj.apply_visitor(*this); }

			template <typename T>
			inline syntax::contents::InstructionBase* operator()(Deferred<T>& obj) const { return obj.Content.get(); }
		};

		typedef std::vector<ContentInstructionPtr> ContentInstructionCollection;

		class Contents : public IUnknown
		{
		public:
			Contents() = default;
			explicit Contents(syntax::StreamObjectPtr obj);
			explicit Contents(syntax::ArrayObjectPtr<syntax::IndirectObjectReferencePtr> obj);

			ContentInstructionCollection Instructions(void) const;
			types::uinteger GetInstructionsSize(void) const;
			ContentInstructionPtr GetInstructionAt(types::uinteger at) const;

			inline types::uinteger GetContentStreamSize(void) const { return _contents.size(); }
			inline ContentStreamPtr GetContentStreamAt(types::uinteger at) const { return _contents.at(at); }

		private:
			std::vector<ContentStreamPtr> _contents;
			mutable ContentInstructionCollection _instructions;
		};
	}
}

#endif /* _CONTENTS_H */
