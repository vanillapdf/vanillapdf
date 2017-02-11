#ifndef _C_NAME_OBJECT_H
#define _C_NAME_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_name_object.h
	* This file contains class definitions for NameObjectHandle
	*/

	/**
	* \class NameObjectHandle
	* \extends ObjectHandle
	* \ingroup Objects
	* \brief Represents unique name references in document.
	* \see NameConstants
	*/

	/**
	* \memberof NameObjectHandle
	* @{
	*/

	/**
	* \brief Get names binary representation
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_GetValue(const struct NameObjectHandleTag* handle, BufferHandle* result);

	/**
	* \brief Set names new value
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_SetValue(NameObjectHandle handle, BufferHandle value);

	/**
	* \copydoc Object_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_Release(NameObjectHandle handle);

	/** @} */

	/**
	* \addtogroup NameConstants
	* @{
	*/

	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_AA;

	/**
	* \brief Represents ASCII85Decode filter, see 7.4.3.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_ASCII85Decode;

	/**
	* \brief Represents ASCIIHexDecode filter, see 7.4.2.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_ASCIIHexDecode;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_AcroForm;

	/**
	* \brief Represents ::DeveloperExtensionHandle property.
	* \see DeveloperExtension_GetBaseVersion
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_BaseVersion;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_BitsPerComponent;

	/**
	* \brief Represents type for ::CatalogHandle
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Catalog;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Collection;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Colors;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Columns;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Container;

	/**
	* \brief Represents page ::ContentsHandle
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Contents;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Count;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_DecodeParms;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Dests;

	/**
	* \brief Represents document ::DeveloperExtensionsHandle
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_DeveloperExtensions;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_EarlyChange;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Extends;

	/**
	* \brief Represents ::DeveloperExtensionHandle property.
	* \see DeveloperExtension_GetExtensionLevel
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_ExtensionLevel;

	/**
	* \brief Represents ::CatalogHandle property.
	* \see Catalog_GetExtensions
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Extensions;

	/**
	* \brief Name for expressing compression filters
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Filter;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_First;

	/**
	* \brief Represents FlateDecode filter, see 7.4.4.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_FlateDecode;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Index;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Kids;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Lang;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Legal;

	/**
	* \brief Usually represents length of a ::StreamObjectHandle.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Length;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Limits;

	/**
	* \brief Represents ::PageObjectHandle property.
	* \see PageObject_GetMediaBox
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_MediaBox;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Metadata;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_N;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Names;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_NeedsRendering;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Nums;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_OCProperties;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_ObjStm;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_OpenAction;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Outlines;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_OutputIntents;

	/**
	* \brief Represents ::PageLabelHandle property.
	* \see PageLabel_Prefix
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_P;

	/**
	* \brief Represents type for ::PageObjectHandle.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Page;

	/**
	* \brief Represents type for ::PageLabelHandle.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_PageLabel;

	/**
	* \brief Represents type for ::PageLabelsHandle.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_PageLabels;

	/**
	* \brief Represents ::CatalogHandle property.
	* \see Catalog_GetPageLayout
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_PageLayout;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_PageMode;

	/**
	* \brief Represents catalog ::PageTreeHandle.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Pages;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Parent;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Perms;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_PieceInfo;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Predictor;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Prev;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Requirements;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Resources;

	/**
	* \brief Represents ::CatalogHandle entry in document's dictionary.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Root;

	/**
	* \brief Represents ::PageLabelHandle property.
	* \see PageLabel_Style
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_S;

	/**
	* \brief Represents ::PageLabelHandle property.
	* \see PageLabel_Start
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_St;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Size;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_SinglePage
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_SinglePage;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_OneColumn
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_OneColumn;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_TwoColumnLeft
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_TwoColumnLeft;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_TwoColumnRight
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_TwoColumnRight;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_TwoPageLeft
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_TwoPageLeft;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_TwoPageRight
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_TwoPageRight;

	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_SpiderInfo;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_StructTreeRoot;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Threads;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Type;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_URI;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Version;

	/**
	* \brief Represents document ::ViewerPreferencesHandle.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_ViewerPreferences;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_W;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_XRefStm;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetHideToolbar
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_HideToolbar;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetHideMenubar
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_HideMenubar;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetHideWindowUI
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_HideWindowUI;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetFitWindow
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_FitWindow;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetCenterWindow
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_CenterWindow;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetDisplayDocTitle
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_DisplayDocTitle;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetNonFullScreenPageMode
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_NonFullScreenPageMode;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetDirection
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Direction;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetViewArea
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_ViewArea;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetViewClip
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_ViewClip;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPrintArea
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_PrintArea;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPrintClip
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_PrintClip;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPrintScaling
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_PrintScaling;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetDuplex
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Duplex;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPickTrayByPDFSize
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_PickTrayByPDFSize;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPrintPageRange
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_PrintPageRange;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetNumCopies
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_NumCopies;

	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_UseNone;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_UseOutlines;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_UseThumbs;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_UseOC;

	/**
	* \brief Represents ::ReadingOrder value.
	* \see ReadingOrder_LeftToRight
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_L2R;

	/**
	* \brief Represents ::ReadingOrder value.
	* \see ReadingOrder_RightToLeft
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_R2L;

	/**
	* \brief Represents ::PrintScaling value.
	* \see PrintScaling_AppDefault
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_AppDefault;

	/**
	* \brief Represents ::PrintScaling value.
	* \see PrintScaling_None
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_None;

	/**
	* \brief Represents ::Duplex value.
	* \see Duplex_Simplex
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Simplex;

	/**
	* \brief Represents ::Duplex value.
	* \see Duplex_DuplexFlipShortEdge
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_DuplexFlipShortEdge;

	/**
	* \brief Represents ::Duplex value.
	* \see Duplex_DuplexFlipLongEdge
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_DuplexFlipLongEdge;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_FullScreen;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_UseAttachments;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Last;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Next;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Dest;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_C;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_F;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_U;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_O;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_R;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_V;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_ID;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Standard;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Title;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Encrypt;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_CF;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_StmF;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_StrF;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_EFF;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_CFM;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_V2;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_AESV2;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_AESV3;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_StdCF;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Crypt;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Identity;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Name;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_SubFilter;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_AdbePkcs7s3;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_AdbePkcs7s4;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_AdbePkcs7s5;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Recipients;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_DefaultCryptFilter;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Info;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Author;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Subject;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Keywords;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Creator;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Producer;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_CreationDate;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_ModDate;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Trapped;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Unknown;

	/**
	* \brief Represents ::DocumentTrappedType value.
	* \see DocumentTrappedType_True
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_True;

	/**
	* \brief Represents ::DocumentTrappedType value.
	* \see DocumentTrappedType_False
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_False;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_NeedsRendering;

	/**
	* \brief Represents DCTDecode filter, see 7.4.8.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_DCTDecode;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_XYZ;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Fit;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_FitH;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_FitV;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_FitR;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_FitB;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_FitBH;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_FitBV;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Subtype;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Annot;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Annots;

	/**
	* \brief Represents type for ::LinkAnnotationHandle.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Link;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_D;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Text;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_FreeText;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Line;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Square;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Circle;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Polygon;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_PolyLine;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Highlight;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Underline;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Squiggly;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_StrikeOut;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_RubberStamp;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Caret;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Ink;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Popup;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_FileAttachment;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Sound;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Movie;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Widget;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Screen;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_PrinterMark;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_TrapNetwork;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Watermark;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_TripleD;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Redact;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_XRef;

	/**
	* \brief Represents type for ::FontHandle.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Font;

	/**
	* \brief Represents type for ::CompositeFontHandle.
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Type0;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Type1;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Type3;

	/**
	* \brief Represents ::CharacterMapHandle property.
	* \see CharacterMap_ToUnicode
	*/
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_ToUnicode;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_CIDSystemInfo;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_CMapName;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_CMapType;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_CMap;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Registry;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Ordering;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Supplement;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_FT;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Btn;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Ch;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Sig;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Fields;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_ByteRange;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Reason;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Location;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_M;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_ContactInfo;
	GOTCHANG_PDF_API extern const struct NameObjectHandleTag* NameConstant_Cert;

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_NAME_OBJECT_H */
