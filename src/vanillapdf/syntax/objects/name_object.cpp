#include "precompiled.h"

#include "utils/character.h"

#include "syntax/objects/name_object.h"

#include "vanillapdf/syntax/c_name_object.h"

#include <sstream>

namespace vanillapdf {
namespace syntax {

NameObject::NameObject() {
	_value->Subscribe(this);
}

NameObject::NameObject(const char * value)
	: NameObject(make_deferred<Buffer>(value)) {
}

NameObject::NameObject(const std::string& chars)
	: NameObject(make_deferred<Buffer>(chars)) {

}

NameObject::NameObject(BufferPtr name) : _value(name) {
	_value->Subscribe(this);
	_value->SetInitialized();
}

size_t NameObject::Hash() const {
	return _value->Hash();
}

void NameObject::ObserveeChanged(IModifyObservable*) {
	OnChanged();
}

BufferPtr NameObject::GetValue() const {
	return _value;
}

void NameObject::SetValue(BufferPtr value) {
	_value->assign(value.begin(), value.end());
}

bool NameObject::Equals(const NameObject& other) const {
	return _value->Equals(other._value);
}

bool NameObject::Equals(ObjectPtr other) const {
	if (!ObjectUtils::IsType<NameObjectPtr>(other)) {
		return false;
	}

	auto other_obj = ObjectUtils::ConvertTo<NameObjectPtr>(other);
	return Equals(*other_obj);
}

Object::Type NameObject::GetType(void) const noexcept {
	return Object::Type::Name;
}

std::string NameObject::ToPdf(void) const {
	return "/" + ToString();
}

NameObject* NameObject::Clone(void) const {
	NameObjectPtr result(pdf_new NameObject(), false);

	result->SetValue(_value->Clone());
	result->SetFile(m_file);

	return result.detach();
}

NameObject::~NameObject() {
	_value->Unsubscribe(this);
}

std::string NameObject::GetHexadecimalNotation(char ch) const {
	std::stringstream ss;
	int converted = static_cast<int>(ch);
	ss << '#';
	ss << std::hex << converted;

	return ss.str();
}

std::string NameObject::ToString(void) const {
	std::stringstream ss;
	auto size = _value->size();
	for (decltype(size) i = 0; i < size; ++i) {
		auto current = _value[i];

		if ('#' == current) {
			ss << GetHexadecimalNotation(current);
			continue;
		}

		if (!IsRegular(current)) {
			ss << GetHexadecimalNotation(current);
			continue;
		}

		if (current < '!' || current > '~') {
			/* Regular characters that are outside the range
			EXCLAMATION MARK(21h) (!) to TILDE (7Eh) (~)
			should be written using the hexadecimal notation */

			ss << GetHexadecimalNotation(current);
			continue;
		}

		ss << current;
	}

	return ss.str();
}

} // syntax

bool operator==(const syntax::NameObject& left, const syntax::NameObject& right) {
	return (left.GetValue() == right.GetValue());
}

bool operator!=(const syntax::NameObject& left, const syntax::NameObject& right) {
	return (left.GetValue() != right.GetValue());
}

bool operator<(const syntax::NameObject& left, const syntax::NameObject& right) {
	return (left.GetValue() < right.GetValue());
}

bool operator==(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right) {
	return (*left == *right);
}

bool operator!=(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right) {
	return (*left != *right);
}

bool operator<(const Deferred<syntax::NameObject>& left, const Deferred<syntax::NameObject>& right) {
	return (*left < *right);
}

} // vanillapdf

namespace std {
size_t hash<vanillapdf::syntax::NameObject>::operator()(const vanillapdf::syntax::NameObject& name) const {
	size_t result = 0;
	for (auto & val : name.GetValue()) {
		std::hash<char> hash_fn;
		result ^= hash_fn(val);
	}

	return result;
}

} // std

namespace vanillapdf {
namespace constant {
namespace Name {

const syntax::NameObject AdbePkcs7s3(make_deferred<Buffer>("adbe.pkcs7.s3", sizeof("adbe.pkcs7.s3") - 1));
const syntax::NameObject AdbePkcs7s4(make_deferred<Buffer>("adbe.pkcs7.s4", sizeof("adbe.pkcs7.s4") - 1));
const syntax::NameObject AdbePkcs7s5(make_deferred<Buffer>("adbe.pkcs7.s5", sizeof("adbe.pkcs7.s5") - 1));

} // Name
} // constant
} // vanillapdf

const NameObjectHandleTag* NameConstant_AdbePkcs7s3 = reinterpret_cast<const NameObjectHandleTag*>(&vanillapdf::constant::Name::AdbePkcs7s3);
const NameObjectHandleTag* NameConstant_AdbePkcs7s4 = reinterpret_cast<const NameObjectHandleTag*>(&vanillapdf::constant::Name::AdbePkcs7s4);
const NameObjectHandleTag* NameConstant_AdbePkcs7s5 = reinterpret_cast<const NameObjectHandleTag*>(&vanillapdf::constant::Name::AdbePkcs7s5);

#define DECLARE_CONST_NAME(name) \
	namespace vanillapdf { namespace constant { namespace Name { \
	const syntax::NameObject name(make_deferred<Buffer>(#name, sizeof(#name) - 1)); \
	} /* Name */ } /* constant */ } /* vanillapdf */ \
	const NameObjectHandleTag* NameConstant_##name = reinterpret_cast<const NameObjectHandleTag*>(&vanillapdf::constant::Name::name);

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
DECLARE_CONST_NAME(U);
DECLARE_CONST_NAME(O);
DECLARE_CONST_NAME(R);
DECLARE_CONST_NAME(V);
DECLARE_CONST_NAME(ID);
DECLARE_CONST_NAME(Standard);
DECLARE_CONST_NAME(Title);
DECLARE_CONST_NAME(Encrypt);
DECLARE_CONST_NAME(CF);
DECLARE_CONST_NAME(StmF);
DECLARE_CONST_NAME(StrF);
DECLARE_CONST_NAME(EFF);
DECLARE_CONST_NAME(CFM);
DECLARE_CONST_NAME(V2);
DECLARE_CONST_NAME(AESV2);
DECLARE_CONST_NAME(AESV3);
DECLARE_CONST_NAME(StdCF);
DECLARE_CONST_NAME(Crypt);
DECLARE_CONST_NAME(Identity);
DECLARE_CONST_NAME(Name);
DECLARE_CONST_NAME(SubFilter);
DECLARE_CONST_NAME(Recipients);
DECLARE_CONST_NAME(DefaultCryptFilter);
DECLARE_CONST_NAME(Info);
DECLARE_CONST_NAME(Author);
DECLARE_CONST_NAME(Subject);
DECLARE_CONST_NAME(Keywords);
DECLARE_CONST_NAME(Creator);
DECLARE_CONST_NAME(Producer);
DECLARE_CONST_NAME(CreationDate);
DECLARE_CONST_NAME(ModDate);
DECLARE_CONST_NAME(Trapped);
DECLARE_CONST_NAME(Unknown);
DECLARE_CONST_NAME(True);
DECLARE_CONST_NAME(False);
DECLARE_CONST_NAME(DCTDecode);
DECLARE_CONST_NAME(XYZ);
DECLARE_CONST_NAME(Fit);
DECLARE_CONST_NAME(FitH);
DECLARE_CONST_NAME(FitV);
DECLARE_CONST_NAME(FitR);
DECLARE_CONST_NAME(FitB);
DECLARE_CONST_NAME(FitBH);
DECLARE_CONST_NAME(FitBV);
DECLARE_CONST_NAME(Subtype);
DECLARE_CONST_NAME(Annot);
DECLARE_CONST_NAME(Annots);
DECLARE_CONST_NAME(Link);
DECLARE_CONST_NAME(D);
DECLARE_CONST_NAME(Text)
DECLARE_CONST_NAME(FreeText);
DECLARE_CONST_NAME(Line);
DECLARE_CONST_NAME(Square);
DECLARE_CONST_NAME(Circle);
DECLARE_CONST_NAME(Polygon);
DECLARE_CONST_NAME(PolyLine);
DECLARE_CONST_NAME(Highlight);
DECLARE_CONST_NAME(Underline);
DECLARE_CONST_NAME(Squiggly);
DECLARE_CONST_NAME(StrikeOut);
DECLARE_CONST_NAME(RubberStamp);
DECLARE_CONST_NAME(Caret);
DECLARE_CONST_NAME(Ink);
DECLARE_CONST_NAME(Popup);
DECLARE_CONST_NAME(FileAttachment);
DECLARE_CONST_NAME(Sound);
DECLARE_CONST_NAME(Movie);
DECLARE_CONST_NAME(Widget);
DECLARE_CONST_NAME(Screen);
DECLARE_CONST_NAME(PrinterMark);
DECLARE_CONST_NAME(TrapNetwork);
DECLARE_CONST_NAME(Watermark);
DECLARE_CONST_NAME(TripleD);
DECLARE_CONST_NAME(Redact);
DECLARE_CONST_NAME(XRef);
DECLARE_CONST_NAME(Font);
DECLARE_CONST_NAME(Type0);
DECLARE_CONST_NAME(Type1);
DECLARE_CONST_NAME(Type3);
DECLARE_CONST_NAME(ToUnicode);
DECLARE_CONST_NAME(CIDSystemInfo);
DECLARE_CONST_NAME(CMapName);
DECLARE_CONST_NAME(CMapType);
DECLARE_CONST_NAME(CMap);
DECLARE_CONST_NAME(Registry);
DECLARE_CONST_NAME(Ordering);
DECLARE_CONST_NAME(Supplement);
DECLARE_CONST_NAME(FT);
DECLARE_CONST_NAME(Btn);
DECLARE_CONST_NAME(Tx);
DECLARE_CONST_NAME(Ch);
DECLARE_CONST_NAME(Sig);
DECLARE_CONST_NAME(Fields);
DECLARE_CONST_NAME(ByteRange);
DECLARE_CONST_NAME(Reason);
DECLARE_CONST_NAME(Location);
DECLARE_CONST_NAME(M);
DECLARE_CONST_NAME(ContactInfo);
DECLARE_CONST_NAME(Cert);
DECLARE_CONST_NAME(BaseFont);
DECLARE_CONST_NAME(Width);
DECLARE_CONST_NAME(Height);
DECLARE_CONST_NAME(ColorSpace);
DECLARE_CONST_NAME(RGB);
DECLARE_CONST_NAME(G);
DECLARE_CONST_NAME(CMYK);
DECLARE_CONST_NAME(DeviceRGB);
DECLARE_CONST_NAME(DeviceGray);
DECLARE_CONST_NAME(DeviceCMYK);
DECLARE_CONST_NAME(XObject);
DECLARE_CONST_NAME(Image);
DECLARE_CONST_NAME(ProcSet);
