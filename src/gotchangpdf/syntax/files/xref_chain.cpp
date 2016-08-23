#include "precompiled.h"
#include "xref_chain.h"

namespace gotchangpdf
{
	namespace syntax
	{
		XrefEntryBasePtr XrefChain::GetXrefEntry(types::big_uint objNumber,
			types::ushort genNumber)
		{
			for (auto it = _list.begin(); it != _list.end(); it++) {
				auto xref = (*it);

				if (!xref->Contains(objNumber))
					continue;

				auto item = xref->Find(objNumber);
				if (item->GetGenerationNumber() != genNumber)
					continue;

				assert(item->GetObjectNumber() == objNumber && item->GetGenerationNumber() == genNumber);
				return item;
			}

			throw ObjectMissingException(objNumber, genNumber);
		}

		bool XrefChain::Contains(types::big_uint objNumber,
			types::ushort genNumber)
		{
			for (auto it = _list.begin(); it != _list.end(); it++) {
				auto xref = (*it);
				if (!xref->Contains(objNumber)) {
					continue;
				}

				auto item = xref->Find(objNumber);
				if (item->GetGenerationNumber() != genNumber)
					continue;

				assert(item->GetObjectNumber() == objNumber && item->GetGenerationNumber() == genNumber);
				return true;
			}

			return false;
		}
	}
}
