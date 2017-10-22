#ifndef _NAME_OBJECT_H
#define _NAME_OBJECT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/objects/containable.h"

#include "utils/buffer.h"
#include "utils/deferred.h"

#include <cstddef>

namespace vanillapdf {
namespace syntax {

class NameObject : public ContainableObject, public IModifyObserver {
public:
	NameObject();
	explicit NameObject(BufferPtr name);
	explicit NameObject(const char * chars);
	explicit NameObject(const std::string& chars);
	virtual ~NameObject();

	virtual void ObserveeChanged(IModifyObservable*) override;

	BufferPtr GetValue() const;
	void SetValue(BufferPtr value);

	bool Equals(const NameObject& other) const;
	virtual bool Equals(ObjectPtr other) const override;

	virtual Object::Type GetType(void) const noexcept override;
	virtual std::string ToPdf(void) const override;
	virtual std::string ToString(void) const override;

	virtual size_t Hash() const override;
	virtual NameObject* Clone(void) const override;

private:
	BufferPtr _value;

	std::string GetHexadecimalNotation(char ch) const;
};

} // syntax

bool operator==(const syntax::NameObject& left, const syntax::NameObject& right);
bool operator!=(const syntax::NameObject& left, const syntax::NameObject& right);
bool operator<(const syntax::NameObject& left, const syntax::NameObject& right);

bool operator==(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right);
bool operator!=(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right);
bool operator<(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right);

namespace constant {
namespace Name {

extern const syntax::NameObject AA;
extern const syntax::NameObject ASCII85Decode;
extern const syntax::NameObject ASCIIHexDecode;
extern const syntax::NameObject AcroForm;
extern const syntax::NameObject BaseVersion;
extern const syntax::NameObject BitsPerComponent;
extern const syntax::NameObject Catalog;
extern const syntax::NameObject Collection;
extern const syntax::NameObject Colors;
extern const syntax::NameObject Columns;
extern const syntax::NameObject Container;
extern const syntax::NameObject Contents;
extern const syntax::NameObject Count;
extern const syntax::NameObject DecodeParms;
extern const syntax::NameObject Dests;
extern const syntax::NameObject DeveloperExtensions;
extern const syntax::NameObject EarlyChange;
extern const syntax::NameObject Extends;
extern const syntax::NameObject ExtensionLevel;
extern const syntax::NameObject Extensions;
extern const syntax::NameObject Filter;
extern const syntax::NameObject First;
extern const syntax::NameObject FlateDecode;
extern const syntax::NameObject Index;
extern const syntax::NameObject Kids;
extern const syntax::NameObject Lang;
extern const syntax::NameObject Legal;
extern const syntax::NameObject Length;
extern const syntax::NameObject Limits;
extern const syntax::NameObject MediaBox;
extern const syntax::NameObject Metadata;
extern const syntax::NameObject N;
extern const syntax::NameObject Names;
extern const syntax::NameObject NeedsRendering;
extern const syntax::NameObject Nums;
extern const syntax::NameObject OCProperties;
extern const syntax::NameObject ObjStm;
extern const syntax::NameObject OpenAction;
extern const syntax::NameObject Outlines;
extern const syntax::NameObject OutputIntents;
extern const syntax::NameObject P;
extern const syntax::NameObject Page;
extern const syntax::NameObject PageLabel;
extern const syntax::NameObject PageLabels;
extern const syntax::NameObject PageLayout;
extern const syntax::NameObject PageMode;
extern const syntax::NameObject Pages;
extern const syntax::NameObject Parent;
extern const syntax::NameObject Perms;
extern const syntax::NameObject PieceInfo;
extern const syntax::NameObject Predictor;
extern const syntax::NameObject Prev;
extern const syntax::NameObject Requirements;
extern const syntax::NameObject Resources;
extern const syntax::NameObject Root;
extern const syntax::NameObject S;
extern const syntax::NameObject St;
extern const syntax::NameObject Size;
extern const syntax::NameObject SinglePage;
extern const syntax::NameObject OneColumn;
extern const syntax::NameObject TwoColumnLeft;
extern const syntax::NameObject TwoColumnRight;
extern const syntax::NameObject TwoPageLeft;
extern const syntax::NameObject TwoPageRight;
extern const syntax::NameObject SpiderInfo;
extern const syntax::NameObject StructTreeRoot;
extern const syntax::NameObject Threads;
extern const syntax::NameObject Type;
extern const syntax::NameObject URI;
extern const syntax::NameObject Version;
extern const syntax::NameObject ViewerPreferences;
extern const syntax::NameObject W;
extern const syntax::NameObject XRefStm;
extern const syntax::NameObject HideToolbar;
extern const syntax::NameObject HideMenubar;
extern const syntax::NameObject HideWindowUI;
extern const syntax::NameObject FitWindow;
extern const syntax::NameObject CenterWindow;
extern const syntax::NameObject DisplayDocTitle;
extern const syntax::NameObject NonFullScreenPageMode;
extern const syntax::NameObject Direction;
extern const syntax::NameObject ViewArea;
extern const syntax::NameObject ViewClip;
extern const syntax::NameObject PrintArea;
extern const syntax::NameObject PrintClip;
extern const syntax::NameObject PrintScaling;
extern const syntax::NameObject Duplex;
extern const syntax::NameObject PickTrayByPDFSize;
extern const syntax::NameObject PrintPageRange;
extern const syntax::NameObject NumCopies;
extern const syntax::NameObject UseNone;
extern const syntax::NameObject UseOutlines;
extern const syntax::NameObject UseThumbs;
extern const syntax::NameObject UseOC;
extern const syntax::NameObject L2R;
extern const syntax::NameObject R2L;
extern const syntax::NameObject AppDefault;
extern const syntax::NameObject None;
extern const syntax::NameObject Simplex;
extern const syntax::NameObject DuplexFlipShortEdge;
extern const syntax::NameObject DuplexFlipLongEdge;
extern const syntax::NameObject FullScreen;
extern const syntax::NameObject UseAttachments;
extern const syntax::NameObject Last;
extern const syntax::NameObject Next;
extern const syntax::NameObject Dest;
extern const syntax::NameObject C;
extern const syntax::NameObject F;
extern const syntax::NameObject U;
extern const syntax::NameObject O;
extern const syntax::NameObject R;
extern const syntax::NameObject V;
extern const syntax::NameObject ID;
extern const syntax::NameObject Standard;
extern const syntax::NameObject Title;
extern const syntax::NameObject Encrypt;
extern const syntax::NameObject CF;
extern const syntax::NameObject StmF;
extern const syntax::NameObject StrF;
extern const syntax::NameObject EFF;
extern const syntax::NameObject CFM;
extern const syntax::NameObject V2;
extern const syntax::NameObject AESV2;
extern const syntax::NameObject AESV3;
extern const syntax::NameObject StdCF;
extern const syntax::NameObject Crypt;
extern const syntax::NameObject Identity;
extern const syntax::NameObject Name;
extern const syntax::NameObject SubFilter;
extern const syntax::NameObject AdbePkcs7s3;
extern const syntax::NameObject AdbePkcs7s4;
extern const syntax::NameObject AdbePkcs7s5;
extern const syntax::NameObject Recipients;
extern const syntax::NameObject DefaultCryptFilter;
extern const syntax::NameObject Info;
extern const syntax::NameObject Author;
extern const syntax::NameObject Subject;
extern const syntax::NameObject Keywords;
extern const syntax::NameObject Creator;
extern const syntax::NameObject Producer;
extern const syntax::NameObject CreationDate;
extern const syntax::NameObject ModDate;
extern const syntax::NameObject Trapped;
extern const syntax::NameObject Unknown;
extern const syntax::NameObject True;
extern const syntax::NameObject False;
extern const syntax::NameObject NeedsRendering;
extern const syntax::NameObject DCTDecode;
extern const syntax::NameObject XYZ;
extern const syntax::NameObject Fit;
extern const syntax::NameObject FitH;
extern const syntax::NameObject FitV;
extern const syntax::NameObject FitR;
extern const syntax::NameObject FitB;
extern const syntax::NameObject FitBH;
extern const syntax::NameObject FitBV;
extern const syntax::NameObject Subtype;
extern const syntax::NameObject Annot;
extern const syntax::NameObject Annots;
extern const syntax::NameObject Link;
extern const syntax::NameObject D;
extern const syntax::NameObject Text;
extern const syntax::NameObject FreeText;
extern const syntax::NameObject Line;
extern const syntax::NameObject Square;
extern const syntax::NameObject Circle;
extern const syntax::NameObject Polygon;
extern const syntax::NameObject PolyLine;
extern const syntax::NameObject Highlight;
extern const syntax::NameObject Underline;
extern const syntax::NameObject Squiggly;
extern const syntax::NameObject StrikeOut;
extern const syntax::NameObject RubberStamp;
extern const syntax::NameObject Caret;
extern const syntax::NameObject Ink;
extern const syntax::NameObject Popup;
extern const syntax::NameObject FileAttachment;
extern const syntax::NameObject Sound;
extern const syntax::NameObject Movie;
extern const syntax::NameObject Widget;
extern const syntax::NameObject Screen;
extern const syntax::NameObject PrinterMark;
extern const syntax::NameObject TrapNetwork;
extern const syntax::NameObject Watermark;
extern const syntax::NameObject TripleD;
extern const syntax::NameObject Redact;
extern const syntax::NameObject XRef;
extern const syntax::NameObject Font;
extern const syntax::NameObject Type0;
extern const syntax::NameObject Type1;
extern const syntax::NameObject Type3;
extern const syntax::NameObject ToUnicode;
extern const syntax::NameObject CIDSystemInfo;
extern const syntax::NameObject CMapName;
extern const syntax::NameObject CMapType;
extern const syntax::NameObject CMap;
extern const syntax::NameObject Registry;
extern const syntax::NameObject Ordering;
extern const syntax::NameObject Supplement;
extern const syntax::NameObject FT;
extern const syntax::NameObject Btn;
extern const syntax::NameObject Tx;
extern const syntax::NameObject Ch;
extern const syntax::NameObject Sig;
extern const syntax::NameObject Fields;
extern const syntax::NameObject ByteRange;
extern const syntax::NameObject Reason;
extern const syntax::NameObject Location;
extern const syntax::NameObject M;
extern const syntax::NameObject ContactInfo;
extern const syntax::NameObject Cert;
extern const syntax::NameObject BaseFont;

} // Constant
} // Name
} // vanillapdf

namespace std {

template <> struct hash<vanillapdf::syntax::NameObject> {
	size_t operator()(const vanillapdf::syntax::NameObject& name) const;
};

} // std

#endif /* _NAME_OBJECT_H */
