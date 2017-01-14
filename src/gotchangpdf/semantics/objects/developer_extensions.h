#ifndef _DEVELOPER_EXTENSIONS_H
#define _DEVELOPER_EXTENSIONS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"
#include "utils/pdf_version.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class DeveloperExtension : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			DeveloperExtension(syntax::DictionaryObjectPtr root);
			Version BaseVersion(void) const;
			syntax::IntegerObjectPtr ExtensionLevel(void) const;
		};

		class DeveloperExtensions : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			class Iterator : public IUnknown
			{
			public:
				typedef syntax::DictionaryObject::const_iterator IteratorT;

				typedef IteratorT::value_type value_type;
				typedef IteratorT::difference_type difference_type;
				typedef IteratorT::pointer pointer;
				typedef IteratorT::reference reference;
				typedef IteratorT::iterator_category iterator_category;

			public:
				Iterator() = default;
				Iterator(IteratorT it) : _it(it) {}

				const Iterator& operator++()
				{
					++_it;
					return *this;
				}

				const Iterator operator++(int)
				{
					Iterator temp(_it);
					++_it;
					return temp;
				}

				syntax::NameObjectPtr First() const { return _it->first; }
				DeveloperExtensionPtr Second() const;
				IteratorT Value() const { return _it; }

				bool operator==(const Iterator& other) const
				{
					return _it == other._it;
				}

			private:
				IteratorT _it;
			};

			using IteratorPtr = DeferredIterator<Iterator>;

		public:
			explicit DeveloperExtensions(syntax::DictionaryObjectPtr root);

			bool TryFind(const syntax::NameObjectPtr& name, OutputDeveloperExtensionPtr& result) const;
			bool Contains(const syntax::NameObjectPtr& name) const;
			void Insert(const syntax::NameObjectPtr& name, const DeveloperExtension& value);
			bool Remove(const syntax::NameObjectPtr& name);
			IteratorPtr Begin(void) const;
			IteratorPtr End(void) const;
		};
	}
}

#endif /* _DEVELOPER_EXTENSIONS_H */
