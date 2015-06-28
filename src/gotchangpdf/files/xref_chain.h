#ifndef _XREF_CHAIN_H
#define _XREF_CHAIN_H

#include "fwd.h"
#include "deferred.h"
#include "smart_ptr.h"
#include "xref_with_metadata.h"

#include <list>

namespace gotchangpdf
{
	namespace files
	{
		class XrefChain : public IUnknown
		{
		public:
			using list_type = std::list<XrefWithMetadataPtr>;

		public:
			class Iterator : public IUnknown
			{
			public:
				Iterator() = default;
				Iterator(list_type::const_iterator it) : _it(it) {}

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

				XrefWithMetadataPtr Value() const { return *_it; }

				bool operator==(const Iterator& other) const { return _it == other._it; }
				bool operator!=(const Iterator& other) const { return _it != other._it; }

			private:
				list_type::const_iterator _it;
			};

			using IteratorPtr = SmartPtr < Iterator > ;

		public:
			IteratorPtr Begin() const { return new Iterator(_list.begin()); }
			IteratorPtr End(void) const { return new Iterator(_list.end()); }
			void Append(XrefWithMetadataPtr item) { _list.push_back(item); }

		private:
			list_type _list;
		};
	}
}

#endif /* _XREF_CHAIN_H */
