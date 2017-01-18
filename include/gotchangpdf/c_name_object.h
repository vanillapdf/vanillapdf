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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_GetValue(NameObjectHandle handle, BufferHandle* result);

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

	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_AA;

	/**
	* \brief Represents ASCII85Decode filter, see 7.4.3.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ASCII85Decode;

	/**
	* \brief Represents ASCIIHexDecode filter, see 7.4.2.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ASCIIHexDecode;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_AcroForm;

	/**
	* \brief Represents ::DeveloperExtensionHandle property.
	* \see DeveloperExtension_GetBaseVersion
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_BaseVersion;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_BitsPerComponent;

	/**
	* \brief Represents type for ::CatalogHandle
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Catalog;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Collection;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Colors;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Columns;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Container;

	/**
	* \brief Represents page ::ContentsHandle
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Contents;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Count;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_DecodeParms;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Dests;

	/**
	* \brief Represents document ::DeveloperExtensionsHandle
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_DeveloperExtensions;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_EarlyChange;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Extends;

	/**
	* \brief Represents ::DeveloperExtensionHandle property.
	* \see DeveloperExtension_GetExtensionLevel
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ExtensionLevel;

	/**
	* \brief Represents ::CatalogHandle property.
	* \see Catalog_GetExtensions
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Extensions;

	/**
	* \brief Name for expressing compression filters
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Filter;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_First;

	/**
	* \brief Represents FlateDecode filter, see 7.4.4.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FlateDecode;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Index;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Kids;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Lang;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Legal;

	/**
	* \brief Usually represents length of a ::StreamObjectHandle.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Length;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Limits;

	/**
	* \brief Represents ::PageObjectHandle property.
	* \see PageObject_GetMediaBox
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_MediaBox;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Metadata;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_N;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Names;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_NeedsRendering;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Nums;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_OCProperties;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ObjStm;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_OpenAction;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Outlines;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_OutputIntents;

	/**
	* \brief Represents ::PageLabelHandle property.
	* \see PageLabel_Prefix
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_P;

	/**
	* \brief Represents type for ::PageObjectHandle.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Page;

	/**
	* \brief Represents type for ::PageLabelHandle.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PageLabel;

	/**
	* \brief Represents type for ::PageLabelsHandle.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PageLabels;

	/**
	* \brief Represents ::CatalogHandle property.
	* \see Catalog_GetPageLayout
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PageLayout;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PageMode;

	/**
	* \brief Represents catalog ::PageTreeHandle.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Pages;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Parent;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Perms;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PieceInfo;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Predictor;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Prev;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Requirements;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Resources;

	/**
	* \brief Represents ::CatalogHandle entry in document's dictionary.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Root;

	/**
	* \brief Represents ::PageLabelHandle property.
	* \see PageLabel_Style
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_S;

	/**
	* \brief Represents ::PageLabelHandle property.
	* \see PageLabel_Start
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_St;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Size;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_SinglePage
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_SinglePage;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_OneColumn
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_OneColumn;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_TwoColumnLeft
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_TwoColumnLeft;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_TwoColumnRight
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_TwoColumnRight;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_TwoPageLeft
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_TwoPageLeft;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_TwoPageRight
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_TwoPageRight;

	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_SpiderInfo;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_StructTreeRoot;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Threads;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Type;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_URI;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Version;

	/**
	* \brief Represents document ::ViewerPreferencesHandle.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ViewerPreferences;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_W;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_XRefStm;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetHideToolbar
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_HideToolbar;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetHideMenubar
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_HideMenubar;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetHideWindowUI
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_HideWindowUI;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetFitWindow
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FitWindow;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetCenterWindow
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_CenterWindow;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetDisplayDocTitle
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_DisplayDocTitle;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetNonFullScreenPageMode
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_NonFullScreenPageMode;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetDirection
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Direction;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetViewArea
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ViewArea;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetViewClip
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ViewClip;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPrintArea
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PrintArea;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPrintClip
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PrintClip;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPrintScaling
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PrintScaling;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetDuplex
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Duplex;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPickTrayByPDFSize
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PickTrayByPDFSize;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPrintPageRange
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PrintPageRange;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetNumCopies
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_NumCopies;

	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_UseNone;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_UseOutlines;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_UseThumbs;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_UseOC;

	/**
	* \brief Represents ::ReadingOrder value.
	* \see ReadingOrder_LeftToRight
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_L2R;

	/**
	* \brief Represents ::ReadingOrder value.
	* \see ReadingOrder_RightToLeft
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_R2L;

	/**
	* \brief Represents ::PrintScaling value.
	* \see PrintScaling_AppDefault
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_AppDefault;

	/**
	* \brief Represents ::PrintScaling value.
	* \see PrintScaling_None
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_None;

	/**
	* \brief Represents ::Duplex value.
	* \see Duplex_Simplex
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Simplex;

	/**
	* \brief Represents ::Duplex value.
	* \see Duplex_DuplexFlipShortEdge
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_DuplexFlipShortEdge;

	/**
	* \brief Represents ::Duplex value.
	* \see Duplex_DuplexFlipLongEdge
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_DuplexFlipLongEdge;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FullScreen;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_UseAttachments;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Last;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Next;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Dest;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_C;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_F;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_U;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_O;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_R;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_V;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ID;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Standard;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Title;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Encrypt;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_CF;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_StmF;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_StrF;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_EFF;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_CFM;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_V2;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_AESV2;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_AESV3;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_StdCF;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Crypt;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Identity;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Name;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_SubFilter;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_AdbePkcs7s3;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_AdbePkcs7s4;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_AdbePkcs7s5;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Recipients;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_DefaultCryptFilter;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Info;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Author;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Subject;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Keywords;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Creator;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Producer;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_CreationDate;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ModDate;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Trapped;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Unknown;

	/**
	* \brief Represents ::DocumentTrappedType value.
	* \see DocumentTrappedType_True
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_True;

	/**
	* \brief Represents ::DocumentTrappedType value.
	* \see DocumentTrappedType_False
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_False;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_NeedsRendering;

	/**
	* \brief Represents DCTDecode filter, see 7.4.8.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_DCTDecode;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_XYZ;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Fit;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FitH;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FitV;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FitR;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FitB;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FitBH;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FitBV;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Subtype;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Annot;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Annots;

	/**
	* \brief Represents type for ::LinkAnnotationHandle.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Link;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_D;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Text;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FreeText;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Line;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Square;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Circle;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Polygon;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PolyLine;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Highlight;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Underline;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Squiggly;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_StrikeOut;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_RubberStamp;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Caret;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Ink;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Popup;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FileAttachment;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Sound;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Movie;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Widget;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Screen;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PrinterMark;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_TrapNetwork;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Watermark;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_TripleD;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Redact;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_XRef;

	/**
	* \brief Represents type for ::FontHandle.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Font;

	/**
	* \brief Represents type for ::CompositeFontHandle.
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Type0;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Type1;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Type3;

	/**
	* \brief Represents ::CharacterMapHandle property.
	* \see CharacterMap_ToUnicode
	*/
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ToUnicode;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_CIDSystemInfo;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_CMapName;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_CMapType;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_CMap;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Registry;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Ordering;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Supplement;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FT;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Btn;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Ch;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Sig;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Fields;

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_NAME_OBJECT_H */
