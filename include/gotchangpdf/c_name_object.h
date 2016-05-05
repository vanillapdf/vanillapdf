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

	GOTCHANG_PDF_API extern const NameHandle NameConstant_AA;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_ASCII85Decode;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_ASCIIHexDecode;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_AcroForm;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_BaseVersion;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_BitsPerComponent;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Catalog;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Collection;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Colors;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Columns;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Container;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Contents;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Count;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_DecodeParms;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Dests;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_DeveloperExtensions;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_EarlyChange;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Extends;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_ExtensionLevel;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Extensions;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Filter;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_First;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_FlateDecode;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Index;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Kids;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Lang;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Legal;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Length;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Limits;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_MediaBox;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Metadata;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_N;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Names;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_NeedsRendering;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Nums;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_OCProperties;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_ObjStm;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_OpenAction;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Outlines;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_OutputIntents;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_P;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Page;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_PageLabel;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_PageLabels;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_PageLayout;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_PageMode;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Pages;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Parent;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Perms;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_PieceInfo;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Predictor;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Prev;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Requirements;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Resources;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Root;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_S;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_St;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Size;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_SinglePage;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_OneColumn;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_TwoColumnLeft;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_TwoColumnRight;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_TwoPageLeft;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_TwoPageRight;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_SpiderInfo;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_StructTreeRoot;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Threads;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Type;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_URI;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Version;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_ViewerPreferences;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_W;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_XRefStm;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_HideToolbar;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_HideMenubar;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_HideWindowUI;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_FitWindow;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_CenterWindow;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_DisplayDocTitle;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_NonFullScreenPageMode;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Direction;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_ViewArea;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_ViewClip;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_PrintArea;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_PrintClip;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_PrintScaling;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Duplex;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_PickTrayByPDFSize;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_PrintPageRange;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_NumCopies;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_UseNone;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_UseOutlines;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_UseThumbs;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_UseOC;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_L2R;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_R2L;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_AppDefault;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_None;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Simplex;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_DuplexFlipShortEdge;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_DuplexFlipLongEdge;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_FullScreen;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_UseAttachments;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Last;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Next;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Dest;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_C;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_F;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_U;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_O;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_R;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_V;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_ID;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Standard;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Title;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Encrypt;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_CF;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_StmF;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_StrF;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_EFF;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_CFM;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_V2;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_AESV2;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_AESV3;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_StdCF;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Crypt;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Identity;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Name;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_SubFilter;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_AdbePkcs7s3;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_AdbePkcs7s4;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_AdbePkcs7s5;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_Recipients;
	GOTCHANG_PDF_API extern const NameHandle NameConstant_DefaultCryptFilter;

#ifdef __cplusplus
};
#endif

#endif /* _C_NAME_OBJECT_H */
