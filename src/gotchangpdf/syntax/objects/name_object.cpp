#include "precompiled.h"
#include "name_object.h"
#include "c_name_object.h"

#include <cassert>

namespace gotchangpdf
{
	namespace syntax
	{
		std::ostream& operator<<(std::ostream& os, const syntax::NameObject& name)
		{
			os << name.Value()->ToString();
			return os;
		}
	}

	namespace constant
	{
		namespace Name
		{
#			define DECLARE_CONST_NAME(name)	const syntax::NameObjectPtr name(BufferPtr(#name, sizeof(#name)))

			DECLARE_CONST_NAME(ASCII85Decode);
			DECLARE_CONST_NAME(BitsPerComponent);
			DECLARE_CONST_NAME(Colors);
			DECLARE_CONST_NAME(Columns);
			DECLARE_CONST_NAME(Container);
			DECLARE_CONST_NAME(Contents);
			DECLARE_CONST_NAME(DecodeParms);
			DECLARE_CONST_NAME(EarlyChange);
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
			DECLARE_CONST_NAME(W);
			DECLARE_CONST_NAME(XRefStm);
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

using namespace gotchangpdf;

NameHandle Length = reinterpret_cast<NameHandle>(constant::Name::Length.Content.get());
NameHandle Type = reinterpret_cast<NameHandle>(constant::Name::Type.Content.get());
NameHandle ObjStm = reinterpret_cast<NameHandle>(constant::Name::ObjStm.Content.get());
NameHandle N = reinterpret_cast<NameHandle>(constant::Name::N.Content.get());
NameHandle First = reinterpret_cast<NameHandle>(constant::Name::First.Content.get());
NameHandle Extends = reinterpret_cast<NameHandle>(constant::Name::Extends.Content.get());
NameHandle Filter = reinterpret_cast<NameHandle>(constant::Name::Filter.Content.get());
NameHandle FlateDecode = reinterpret_cast<NameHandle>(constant::Name::FlateDecode.Content.get());
NameHandle Catalog = reinterpret_cast<NameHandle>(constant::Name::Catalog.Content.get());
NameHandle Pages = reinterpret_cast<NameHandle>(constant::Name::Pages.Content.get());
NameHandle Page = reinterpret_cast<NameHandle>(constant::Name::Page.Content.get());
NameHandle Count = reinterpret_cast<NameHandle>(constant::Name::Count.Content.get());
NameHandle Kids = reinterpret_cast<NameHandle>(constant::Name::Kids.Content.get());
NameHandle Container = reinterpret_cast<NameHandle>(constant::Name::Container.Content.get());
NameHandle MediaBox = reinterpret_cast<NameHandle>(constant::Name::MediaBox.Content.get());
NameHandle Resources = reinterpret_cast<NameHandle>(constant::Name::Resources.Content.get());
NameHandle Parent = reinterpret_cast<NameHandle>(constant::Name::Parent.Content.get());
NameHandle Root = reinterpret_cast<NameHandle>(constant::Name::Root.Content.get());
NameHandle W = reinterpret_cast<NameHandle>(constant::Name::W.Content.get());
