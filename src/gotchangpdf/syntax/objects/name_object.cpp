#include "precompiled.h"
#include "name_object.h"
#include "c_name_object.h"

namespace gotchangpdf
{
	namespace syntax
	{
		std::ostream& operator<<(std::ostream& os, const syntax::NameObject& name)
		{
			os << name.ToString();
			return os;
		}
	}
}

namespace std
{
	size_t hash<gotchangpdf::syntax::NameObject>::operator()(const gotchangpdf::syntax::NameObject& name) const
	{
		size_t result = 0;
		for (auto & val : *name.Value())
		{
			std::hash<char> hash_fn;
			result ^= hash_fn(val);
		}

		return result;
	}
}

#define DECLARE_CONST_NAME(name) \
	namespace gotchangpdf { namespace constant { namespace Name { \
	const syntax::NameObjectPtr name(BufferPtr(#name, sizeof(#name))); \
	} /* Name */ } /* constant */ } /* gotchangpdf */ \
	const NameHandle NameConstant_##name = reinterpret_cast<NameHandle>(gotchangpdf::constant::Name::##name##.Content.get());

DECLARE_CONST_NAME(ASCII85Decode);
DECLARE_CONST_NAME(BaseVersion);
DECLARE_CONST_NAME(BitsPerComponent);
DECLARE_CONST_NAME(Colors);
DECLARE_CONST_NAME(Columns);
DECLARE_CONST_NAME(Container);
DECLARE_CONST_NAME(Contents);
DECLARE_CONST_NAME(DecodeParms);
DECLARE_CONST_NAME(DeveloperExtensions);
DECLARE_CONST_NAME(EarlyChange);
DECLARE_CONST_NAME(ExtensionLevel);
DECLARE_CONST_NAME(Extensions);
DECLARE_CONST_NAME(Length);
DECLARE_CONST_NAME(Type);
DECLARE_CONST_NAME(ObjStm);
DECLARE_CONST_NAME(N);
DECLARE_CONST_NAME(First);
DECLARE_CONST_NAME(Extends);
DECLARE_CONST_NAME(Filter);
DECLARE_CONST_NAME(FlateDecode);
DECLARE_CONST_NAME(Index);
DECLARE_CONST_NAME(Catalog);
DECLARE_CONST_NAME(Pages);
DECLARE_CONST_NAME(Page);
DECLARE_CONST_NAME(Count);
DECLARE_CONST_NAME(Kids);
DECLARE_CONST_NAME(MediaBox);
DECLARE_CONST_NAME(Resources);
DECLARE_CONST_NAME(Parent);
DECLARE_CONST_NAME(Predictor);
DECLARE_CONST_NAME(Prev);
DECLARE_CONST_NAME(Root);
DECLARE_CONST_NAME(Size);
DECLARE_CONST_NAME(Version);
DECLARE_CONST_NAME(W);
DECLARE_CONST_NAME(XRefStm);