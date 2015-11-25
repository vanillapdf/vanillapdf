#ifndef _CONTENTS_H
#define _CONTENTS_H

#include "fwd.h"
#include "content_stream.h"
#include "direct_object.h"

#include <vector>

namespace gotchangpdf
{
	namespace documents
	{
		class BeginTextOperation : public lexical::ContentStreamOperation
		{

		};

		class EndTextOperation : public lexical::ContentStreamOperation
		{

		};

		typedef Deferred<BeginTextOperation> BeginTextOperationPtr;
		typedef Deferred<EndTextOperation> EndTextOperationPtr;

		typedef boost::variant<
			lexical::ContentStreamOperationPtr,
			BeginTextOperationPtr,
			EndTextOperationPtr
		> ContentOperationPtr;

		class ContentObjectBase : public lexical::ContentInstructionBase
		{
		public:
			enum class Type
			{
				TextObject
			};

			virtual Type GetType(void) const _NOEXCEPT = 0;
			inline virtual ContentInstructionBase::Type GetInstructionType(void) const _NOEXCEPT override { return ContentInstructionBase::Type::Object; }
		};

		class TextObject : public ContentObjectBase
		{
		public:
			TextObject(lexical::ContentStreamOperationCollection ops) : _operations(ops) {}

			inline virtual Type GetType(void) const _NOEXCEPT override { return Type::TextObject; }

		private:
			lexical::ContentStreamOperationCollection _operations;
		};

		typedef Deferred<TextObject> TextObjectPtr;

		typedef boost::variant<
			TextObjectPtr
		> ContentObjectPtr;

		typedef boost::variant<
			ContentObjectPtr,
			ContentOperationPtr
		> ContentInstructionPtr;

		class ContentStreamInstructionBaseVisitor : public boost::static_visitor<lexical::ContentInstructionBase*>
		{
		public:
			inline lexical::ContentInstructionBase* operator()(ContentObjectPtr obj) const { return obj.apply_visitor(*this); }
			inline lexical::ContentInstructionBase* operator()(ContentOperationPtr obj) const { return obj.apply_visitor(*this); }

			template <typename T>
			inline lexical::ContentInstructionBase* operator()(Deferred<T>& obj) const { return obj.Content.get(); }
		};

		typedef std::vector<ContentInstructionPtr> ContentInstructionCollection;

		class Contents : public IUnknown
		{
		public:
			Contents() = default;
			explicit Contents(StreamObjectPtr obj);
			explicit Contents(ArrayObjectPtr<IndirectObjectReferencePtr> obj);

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
