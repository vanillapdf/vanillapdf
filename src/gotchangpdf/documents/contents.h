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
		class ContentOperationBase : public IUnknown
		{

		};

		class BeginTextOperation : public ContentOperationBase
		{

		};

		class EndTextOperation : public ContentOperationBase
		{

		};

		typedef Deferred<BeginTextOperation> BeginTextOperationPtr;
		typedef Deferred<EndTextOperation> EndTextOperationPtr;

		typedef boost::variant<
			lexical::ContentStreamOperation,
			BeginTextOperationPtr,
			EndTextOperationPtr
		> ContentOperationPtr;

		class ContentObjectBase : public IUnknown
		{

		};

		class TextObject : public ContentObjectBase
		{
		public:
			TextObject(lexical::ContentStreamOperationCollection ops) : _operations(ops) {}

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

		typedef std::vector<ContentInstructionPtr> ContentInstructionCollection;

		class Contents : public IUnknown
		{
		public:
			Contents() = default;
			explicit Contents(StreamObjectPtr obj);
			explicit Contents(ArrayObjectPtr<IndirectObjectReferencePtr> obj);

			ContentInstructionCollection Operations(void) const;
			types::uinteger GetOperationsSize(void) const;
			ContentInstructionPtr GetOperationAt(types::uinteger at) const;

			inline types::uinteger GetContentStreamSize(void) const { return _contents.size(); }
			inline ContentStreamPtr GetContentStreamAt(types::uinteger at) const { return _contents.at(at); }

		private:
			std::vector<ContentStreamPtr> _contents;
		};
	}
}

#endif /* _CONTENTS_H */
