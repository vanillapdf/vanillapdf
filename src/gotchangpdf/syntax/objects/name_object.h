#ifndef _NAME_OBJECT_H
#define _NAME_OBJECT_H

#include "syntax_fwd.h"
#include "containable.h"
#include "buffer.h"
#include "deferred.h"

#include <cstddef>

namespace gotchangpdf
{
	namespace syntax
	{
		class NameObject : public ContainableObject
		{
		public:
			typedef BufferPtr value_type;

		public:
			NameObject() = default;
			explicit NameObject(value_type name) : _value(name) {}

			value_type Value() const _NOEXCEPT { return _value; }

			bool operator==(const NameObject& other) const { return Equals(other); }
			bool operator!=(const NameObject& other) const { return !Equals(other); }
			bool operator<(const NameObject& other) const { return *_value < *other._value; }

			bool Equals(const NameObject& other) const { return _value->Equals(*other._value); }

			virtual Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Name; }
			virtual std::string ToPdf(void) const override { return "/" + ToString(); }
			virtual std::string ToString(void) const override;

			void SetName(value_type name) { _value = name; }

		public:
			value_type _value;
		};
	}

	inline bool operator==(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right)
	{
		return *left == *right;
	}

	inline bool operator!=(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right)
	{
		return *left != *right;
	}

	inline bool operator<(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right)
	{
		return *left < *right;
	}

	namespace constant
	{
		namespace Name
		{
			extern const syntax::NameObjectPtr AA;
			extern const syntax::NameObjectPtr ASCII85Decode;
			extern const syntax::NameObjectPtr ASCIIHexDecode;
			extern const syntax::NameObjectPtr AcroForm;
			extern const syntax::NameObjectPtr BaseVersion;
			extern const syntax::NameObjectPtr BitsPerComponent;
			extern const syntax::NameObjectPtr Catalog;
			extern const syntax::NameObjectPtr Collection;
			extern const syntax::NameObjectPtr Colors;
			extern const syntax::NameObjectPtr Columns;
			extern const syntax::NameObjectPtr Container;
			extern const syntax::NameObjectPtr Contents;
			extern const syntax::NameObjectPtr Count;
			extern const syntax::NameObjectPtr DecodeParms;
			extern const syntax::NameObjectPtr Dests;
			extern const syntax::NameObjectPtr DeveloperExtensions;
			extern const syntax::NameObjectPtr EarlyChange;
			extern const syntax::NameObjectPtr Extends;
			extern const syntax::NameObjectPtr ExtensionLevel;
			extern const syntax::NameObjectPtr Extensions;
			extern const syntax::NameObjectPtr Filter;
			extern const syntax::NameObjectPtr First;
			extern const syntax::NameObjectPtr FlateDecode;
			extern const syntax::NameObjectPtr Index;
			extern const syntax::NameObjectPtr Kids;
			extern const syntax::NameObjectPtr Lang;
			extern const syntax::NameObjectPtr Legal;
			extern const syntax::NameObjectPtr Length;
			extern const syntax::NameObjectPtr Limits;
			extern const syntax::NameObjectPtr MediaBox;
			extern const syntax::NameObjectPtr Metadata;
			extern const syntax::NameObjectPtr N;
			extern const syntax::NameObjectPtr Names;
			extern const syntax::NameObjectPtr NeedsRendering;
			extern const syntax::NameObjectPtr Nums;
			extern const syntax::NameObjectPtr OCProperties;
			extern const syntax::NameObjectPtr ObjStm;
			extern const syntax::NameObjectPtr OpenAction;
			extern const syntax::NameObjectPtr Outlines;
			extern const syntax::NameObjectPtr OutputIntents;
			extern const syntax::NameObjectPtr P;
			extern const syntax::NameObjectPtr Page;
			extern const syntax::NameObjectPtr PageLabel;
			extern const syntax::NameObjectPtr PageLabels;
			extern const syntax::NameObjectPtr PageLayout;
			extern const syntax::NameObjectPtr PageMode;
			extern const syntax::NameObjectPtr Pages;
			extern const syntax::NameObjectPtr Parent;
			extern const syntax::NameObjectPtr Perms;
			extern const syntax::NameObjectPtr PieceInfo;
			extern const syntax::NameObjectPtr Predictor;
			extern const syntax::NameObjectPtr Prev;
			extern const syntax::NameObjectPtr Requirements;
			extern const syntax::NameObjectPtr Resources;
			extern const syntax::NameObjectPtr Root;
			extern const syntax::NameObjectPtr S;
			extern const syntax::NameObjectPtr St;
			extern const syntax::NameObjectPtr Size;
			extern const syntax::NameObjectPtr SinglePage;
			extern const syntax::NameObjectPtr OneColumn;
			extern const syntax::NameObjectPtr TwoColumnLeft;
			extern const syntax::NameObjectPtr TwoColumnRight;
			extern const syntax::NameObjectPtr TwoPageLeft;
			extern const syntax::NameObjectPtr TwoPageRight;
			extern const syntax::NameObjectPtr SpiderInfo;
			extern const syntax::NameObjectPtr StructTreeRoot;
			extern const syntax::NameObjectPtr Threads;
			extern const syntax::NameObjectPtr Type;
			extern const syntax::NameObjectPtr URI;
			extern const syntax::NameObjectPtr Version;
			extern const syntax::NameObjectPtr ViewerPreferences;
			extern const syntax::NameObjectPtr W;
			extern const syntax::NameObjectPtr XRefStm;
			extern const syntax::NameObjectPtr HideToolbar;
			extern const syntax::NameObjectPtr HideMenubar;
			extern const syntax::NameObjectPtr HideWindowUI;
			extern const syntax::NameObjectPtr FitWindow;
			extern const syntax::NameObjectPtr CenterWindow;
			extern const syntax::NameObjectPtr DisplayDocTitle;
			extern const syntax::NameObjectPtr NonFullScreenPageMode;
			extern const syntax::NameObjectPtr Direction;
			extern const syntax::NameObjectPtr ViewArea;
			extern const syntax::NameObjectPtr ViewClip;
			extern const syntax::NameObjectPtr PrintArea;
			extern const syntax::NameObjectPtr PrintClip;
			extern const syntax::NameObjectPtr PrintScaling;
			extern const syntax::NameObjectPtr Duplex;
			extern const syntax::NameObjectPtr PickTrayByPDFSize;
			extern const syntax::NameObjectPtr PrintPageRange;
			extern const syntax::NameObjectPtr NumCopies;
			extern const syntax::NameObjectPtr UseNone;
			extern const syntax::NameObjectPtr UseOutlines;
			extern const syntax::NameObjectPtr UseThumbs;
			extern const syntax::NameObjectPtr UseOC;
			extern const syntax::NameObjectPtr L2R;
			extern const syntax::NameObjectPtr R2L;
			extern const syntax::NameObjectPtr AppDefault;
			extern const syntax::NameObjectPtr None;
			extern const syntax::NameObjectPtr Simplex;
			extern const syntax::NameObjectPtr DuplexFlipShortEdge;
			extern const syntax::NameObjectPtr DuplexFlipLongEdge;
			extern const syntax::NameObjectPtr FullScreen;
			extern const syntax::NameObjectPtr UseAttachments;
			extern const syntax::NameObjectPtr Last;
			extern const syntax::NameObjectPtr Next;
			extern const syntax::NameObjectPtr Dest;
			extern const syntax::NameObjectPtr C;
			extern const syntax::NameObjectPtr F;
			extern const syntax::NameObjectPtr U;
			extern const syntax::NameObjectPtr O;
			extern const syntax::NameObjectPtr R;
			extern const syntax::NameObjectPtr V;
			extern const syntax::NameObjectPtr ID;
			extern const syntax::NameObjectPtr Standard;
			extern const syntax::NameObjectPtr Title;
			extern const syntax::NameObjectPtr Encrypt;
			extern const syntax::NameObjectPtr CF;
			extern const syntax::NameObjectPtr StmF;
			extern const syntax::NameObjectPtr StrF;
			extern const syntax::NameObjectPtr EFF;
			extern const syntax::NameObjectPtr CFM;
			extern const syntax::NameObjectPtr V2;
			extern const syntax::NameObjectPtr AESV2;
			extern const syntax::NameObjectPtr AESV3;
			extern const syntax::NameObjectPtr StdCF;
			extern const syntax::NameObjectPtr Crypt;
			extern const syntax::NameObjectPtr Identity;
			extern const syntax::NameObjectPtr Name;
			extern const syntax::NameObjectPtr SubFilter;
			extern const syntax::NameObjectPtr AdbePkcs7s3;
			extern const syntax::NameObjectPtr AdbePkcs7s4;
			extern const syntax::NameObjectPtr AdbePkcs7s5;
			extern const syntax::NameObjectPtr Recipients;
			extern const syntax::NameObjectPtr DefaultCryptFilter;
		}
	}
}

namespace std
{
	template <> struct hash<gotchangpdf::syntax::NameObject>
	{
		size_t operator()(const gotchangpdf::syntax::NameObject& name) const;
	};
}

#endif /* _NAME_OBJECT_H */
