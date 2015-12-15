#ifndef _C_NAME_OBJECT_H
#define _C_NAME_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_Value(NameHandle handle, PBufferHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_Release(NameHandle handle);

	// Constant names

	GOTCHANG_PDF_API extern const NameHandle AA;
	GOTCHANG_PDF_API extern const NameHandle ASCII85Decode;
	GOTCHANG_PDF_API extern const NameHandle ASCIIHexDecode;
	GOTCHANG_PDF_API extern const NameHandle AcroForm;
	GOTCHANG_PDF_API extern const NameHandle BaseVersion;
	GOTCHANG_PDF_API extern const NameHandle BitsPerComponent;
	GOTCHANG_PDF_API extern const NameHandle Catalog;
	GOTCHANG_PDF_API extern const NameHandle Collection;
	GOTCHANG_PDF_API extern const NameHandle Colors;
	GOTCHANG_PDF_API extern const NameHandle Columns;
	GOTCHANG_PDF_API extern const NameHandle Container;
	GOTCHANG_PDF_API extern const NameHandle Contents;
	GOTCHANG_PDF_API extern const NameHandle Count;
	GOTCHANG_PDF_API extern const NameHandle DecodeParms;
	GOTCHANG_PDF_API extern const NameHandle Dests;
	GOTCHANG_PDF_API extern const NameHandle DeveloperExtensions;
	GOTCHANG_PDF_API extern const NameHandle EarlyChange;
	GOTCHANG_PDF_API extern const NameHandle Extends;
	GOTCHANG_PDF_API extern const NameHandle ExtensionLevel;
	GOTCHANG_PDF_API extern const NameHandle Extensions;
	GOTCHANG_PDF_API extern const NameHandle Filter;
	GOTCHANG_PDF_API extern const NameHandle First;
	GOTCHANG_PDF_API extern const NameHandle FlateDecode;
	GOTCHANG_PDF_API extern const NameHandle Index;
	GOTCHANG_PDF_API extern const NameHandle Kids;
	GOTCHANG_PDF_API extern const NameHandle Lang;
	GOTCHANG_PDF_API extern const NameHandle Legal;
	GOTCHANG_PDF_API extern const NameHandle Length;
	GOTCHANG_PDF_API extern const NameHandle Limits;
	GOTCHANG_PDF_API extern const NameHandle MediaBox;
	GOTCHANG_PDF_API extern const NameHandle Metadata;
	GOTCHANG_PDF_API extern const NameHandle N;
	GOTCHANG_PDF_API extern const NameHandle Names;
	GOTCHANG_PDF_API extern const NameHandle NeedsRendering;
	GOTCHANG_PDF_API extern const NameHandle Nums;
	GOTCHANG_PDF_API extern const NameHandle OCProperties;
	GOTCHANG_PDF_API extern const NameHandle ObjStm;
	GOTCHANG_PDF_API extern const NameHandle OpenAction;
	GOTCHANG_PDF_API extern const NameHandle Outlines;
	GOTCHANG_PDF_API extern const NameHandle OutputIntents;
	GOTCHANG_PDF_API extern const NameHandle P;
	GOTCHANG_PDF_API extern const NameHandle Page;
	GOTCHANG_PDF_API extern const NameHandle PageLabel;
	GOTCHANG_PDF_API extern const NameHandle PageLabels;
	GOTCHANG_PDF_API extern const NameHandle PageLayout;
	GOTCHANG_PDF_API extern const NameHandle PageMode;
	GOTCHANG_PDF_API extern const NameHandle Pages;
	GOTCHANG_PDF_API extern const NameHandle Parent;
	GOTCHANG_PDF_API extern const NameHandle Perms;
	GOTCHANG_PDF_API extern const NameHandle PieceInfo;
	GOTCHANG_PDF_API extern const NameHandle Predictor;
	GOTCHANG_PDF_API extern const NameHandle Prev;
	GOTCHANG_PDF_API extern const NameHandle Requirements;
	GOTCHANG_PDF_API extern const NameHandle Resources;
	GOTCHANG_PDF_API extern const NameHandle Root;
	GOTCHANG_PDF_API extern const NameHandle S;
	GOTCHANG_PDF_API extern const NameHandle St;
	GOTCHANG_PDF_API extern const NameHandle Size;
	GOTCHANG_PDF_API extern const NameHandle SpiderInfo;
	GOTCHANG_PDF_API extern const NameHandle StructTreeRoot;
	GOTCHANG_PDF_API extern const NameHandle Threads;
	GOTCHANG_PDF_API extern const NameHandle Type;
	GOTCHANG_PDF_API extern const NameHandle URI;
	GOTCHANG_PDF_API extern const NameHandle Version;
	GOTCHANG_PDF_API extern const NameHandle ViewerPreferences;
	GOTCHANG_PDF_API extern const NameHandle W;
	GOTCHANG_PDF_API extern const NameHandle XRefStm;

#ifdef __cplusplus
};
#endif

#endif /* _C_NAME_OBJECT_H */
