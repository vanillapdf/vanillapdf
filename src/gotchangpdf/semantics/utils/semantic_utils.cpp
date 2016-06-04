#include "precompiled.h"

#include "semantic_utils.h"

#include <regex>

namespace gotchangpdf
{
	namespace semantics
	{
		Version SemanticUtils::GetVersionFromName(const syntax::NameObjectPtr& name)
		{
			auto ver_str = name->GetValue()->ToString();

			std::smatch sm;
			std::regex header_regex("([0-9])\\.([0-9])");
			if (!std::regex_match(ver_str, sm, header_regex))
				throw NotSupportedException("Unsupported pdf version: " + ver_str);

			assert(sm.size() == 3);
			if (1 != stoi(sm[1]))
				throw NotSupportedException("Unsupported pdf version: " + ver_str);

			switch (stoi(sm[2])) {
			case 0: return Version::PDF10;
			case 1: return Version::PDF11;
			case 2: return Version::PDF12;
			case 3: return Version::PDF13;
			case 4: return Version::PDF14;
			case 5: return Version::PDF15;
			case 6: return Version::PDF16;
			case 7: return Version::PDF17;
			default: throw NotSupportedException("Unsupported pdf version: " + ver_str);
			}
		}
	}
}
