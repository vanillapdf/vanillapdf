#include "precompiled.h"
#include "name_object.h"
#include "c_name_object.h"

namespace gotchangpdf
{
	namespace syntax
	{
		std::string NameObject::ToString(void) const
		{
			std::stringstream ss;
			auto size = _value->size();
			for (decltype(size) i = 0; i < size; ++i) {
				auto current = _value[i];
				if (current < '!' || current > '~') {
					/* Regular characters that are outside the range
					EXCLAMATION MARK(21h) (!) to TILDE (7Eh) (~)
					should be written using the hexadecimal notation */
					int converted = static_cast<int>(current);
					ss << '#';
					ss << std::hex << converted;
					continue;
				}

				ss << current;
			}

			return ss.str();
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

DECLARE_CONST_NAME(AA);
DECLARE_CONST_NAME(ASCII85Decode);
DECLARE_CONST_NAME(ASCIIHexDecode);
DECLARE_CONST_NAME(AcroForm);
DECLARE_CONST_NAME(BaseVersion);
DECLARE_CONST_NAME(BitsPerComponent);
DECLARE_CONST_NAME(Catalog);
DECLARE_CONST_NAME(Collection);
DECLARE_CONST_NAME(Colors);
DECLARE_CONST_NAME(Columns);
DECLARE_CONST_NAME(Container);
DECLARE_CONST_NAME(Contents);
DECLARE_CONST_NAME(Count);
DECLARE_CONST_NAME(DecodeParms);
DECLARE_CONST_NAME(Dests);
DECLARE_CONST_NAME(DeveloperExtensions);
DECLARE_CONST_NAME(EarlyChange);
DECLARE_CONST_NAME(Extends);
DECLARE_CONST_NAME(ExtensionLevel);
DECLARE_CONST_NAME(Extensions);
DECLARE_CONST_NAME(Filter);
DECLARE_CONST_NAME(First);
DECLARE_CONST_NAME(FlateDecode);
DECLARE_CONST_NAME(Index);
DECLARE_CONST_NAME(Kids);
DECLARE_CONST_NAME(Lang);
DECLARE_CONST_NAME(Legal);
DECLARE_CONST_NAME(Length);
DECLARE_CONST_NAME(Limits);
DECLARE_CONST_NAME(MediaBox);
DECLARE_CONST_NAME(Metadata);
DECLARE_CONST_NAME(N);
DECLARE_CONST_NAME(Names);
DECLARE_CONST_NAME(NeedsRendering);
DECLARE_CONST_NAME(Nums);
DECLARE_CONST_NAME(OCProperties);
DECLARE_CONST_NAME(ObjStm);
DECLARE_CONST_NAME(OpenAction);
DECLARE_CONST_NAME(Outlines);
DECLARE_CONST_NAME(OutputIntents);
DECLARE_CONST_NAME(P);
DECLARE_CONST_NAME(Page);
DECLARE_CONST_NAME(PageLabel);
DECLARE_CONST_NAME(PageLabels);
DECLARE_CONST_NAME(PageLayout);
DECLARE_CONST_NAME(PageMode);
DECLARE_CONST_NAME(Pages);
DECLARE_CONST_NAME(Parent);
DECLARE_CONST_NAME(Perms);
DECLARE_CONST_NAME(PieceInfo);
DECLARE_CONST_NAME(Predictor);
DECLARE_CONST_NAME(Prev);
DECLARE_CONST_NAME(Requirements);
DECLARE_CONST_NAME(Resources);
DECLARE_CONST_NAME(Root);
DECLARE_CONST_NAME(S);
DECLARE_CONST_NAME(St);
DECLARE_CONST_NAME(Size);
DECLARE_CONST_NAME(SinglePage);
DECLARE_CONST_NAME(OneColumn);
DECLARE_CONST_NAME(TwoColumnLeft);
DECLARE_CONST_NAME(TwoColumnRight);
DECLARE_CONST_NAME(TwoPageLeft);
DECLARE_CONST_NAME(TwoPageRight);
DECLARE_CONST_NAME(SpiderInfo);
DECLARE_CONST_NAME(StructTreeRoot);
DECLARE_CONST_NAME(Threads);
DECLARE_CONST_NAME(Type);
DECLARE_CONST_NAME(URI);
DECLARE_CONST_NAME(Version);
DECLARE_CONST_NAME(ViewerPreferences);
DECLARE_CONST_NAME(W);
DECLARE_CONST_NAME(XRefStm);
DECLARE_CONST_NAME(HideToolbar);
DECLARE_CONST_NAME(HideMenubar);
DECLARE_CONST_NAME(HideWindowUI);
DECLARE_CONST_NAME(FitWindow);
DECLARE_CONST_NAME(CenterWindow);
DECLARE_CONST_NAME(DisplayDocTitle);
DECLARE_CONST_NAME(NonFullScreenPageMode);
DECLARE_CONST_NAME(Direction);
DECLARE_CONST_NAME(ViewArea);
DECLARE_CONST_NAME(ViewClip);
DECLARE_CONST_NAME(PrintArea);
DECLARE_CONST_NAME(PrintClip);
DECLARE_CONST_NAME(PrintScaling);
DECLARE_CONST_NAME(Duplex);
DECLARE_CONST_NAME(PickTrayByPDFSize);
DECLARE_CONST_NAME(PrintPageRange);
DECLARE_CONST_NAME(NumCopies);
DECLARE_CONST_NAME(UseNone);
DECLARE_CONST_NAME(UseOutlines);
DECLARE_CONST_NAME(UseThumbs);
DECLARE_CONST_NAME(UseOC);
DECLARE_CONST_NAME(L2R);
DECLARE_CONST_NAME(R2L);
DECLARE_CONST_NAME(AppDefault);
DECLARE_CONST_NAME(None);
DECLARE_CONST_NAME(Simplex);
DECLARE_CONST_NAME(DuplexFlipShortEdge);
DECLARE_CONST_NAME(DuplexFlipLongEdge);
DECLARE_CONST_NAME(FullScreen);
DECLARE_CONST_NAME(UseAttachments);
DECLARE_CONST_NAME(Last);
DECLARE_CONST_NAME(Next);
DECLARE_CONST_NAME(Dest);
DECLARE_CONST_NAME(C);
DECLARE_CONST_NAME(F);
DECLARE_CONST_NAME(Title);