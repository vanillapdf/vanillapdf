#ifndef _HEADER_H
#define _HEADER_H

#include "syntax/utils/syntax_fwd.h"
#include "utils/unknown_interface.h"
#include "utils/pdf_version.h"

namespace vanillapdf {
namespace syntax {

class Header : public virtual IUnknown, public IWeakReferenceable<Header> {
public:
	Version GetVersion(void) const noexcept { return _version; }
	void SetVersion(Version version) { _version = version; }

private:
	Version _version;
};

} // syntax
} // vanillapdf

#endif /* _HEADER_H */
