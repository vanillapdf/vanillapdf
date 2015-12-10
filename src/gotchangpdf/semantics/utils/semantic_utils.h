#ifndef _SEMANTIC_UTILS_H
#define _SEMANTIC_UTILS_H

#include "version.h"
#include "name_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class SemanticUtils
		{
		public:
			static Version GetVersionFromName(const syntax::NameObjectPtr& name);
		};
	}
}

#endif /* _SEMANTIC_UTILS_H */
