#ifndef _C_NAME_CONSTANTS_H
#define _C_NAME_CONSTANTS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_name_constants.h
	* \brief This file contains useful predefined constants referenced by PDF specification
	*/

	// This command suppresses warning for undocumented members
	// Name constants does not have a context-less meaning
	//! \{

	/**
	* \addtogroup group_name_constants
	* @{
	*/

	VANILLAPDF_API extern const NameObjectHandle* NameConstant_AA;

	/**
	* \brief Represents ASCII85Decode filter, see 7.4.3.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ASCII85Decode;

	/**
	* \brief Represents ASCIIHexDecode filter, see 7.4.2.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ASCIIHexDecode;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_AcroForm;

	/**
	* \brief Represents \ref DeveloperExtensionHandle property.
	* \see \ref DeveloperExtensionHandle::DeveloperExtension_GetBaseVersion
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_BaseVersion;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_BitsPerComponent;

	/**
	* \brief Represents type for \ref CatalogHandle
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Catalog;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Collection;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Colors;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Columns;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Container;

	/**
	* \brief Represents page \ref ContentsHandle
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Contents;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Count;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_DecodeParms;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Dests;

	/**
	* \brief Represents document \ref DeveloperExtensionsHandle
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_DeveloperExtensions;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_EarlyChange;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Extends;

	/**
	* \brief Represents \ref DeveloperExtensionHandle property.
	* \see \ref DeveloperExtensionHandle::DeveloperExtension_GetExtensionLevel
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ExtensionLevel;

	/**
	* \brief Represents \ref CatalogHandle property.
	* \see \ref CatalogHandle::Catalog_GetExtensions
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Extensions;

	/**
	* \brief Name for expressing compression filters
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Filter;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_First;

	/**
	* \brief Represents FlateDecode filter, see 7.4.4.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_FlateDecode;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Index;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Kids;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Lang;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Legal;

	/**
	* \brief Usually represents length of a \ref StreamObjectHandle.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Length;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Limits;

	/**
	* \brief Represents \ref PageObjectHandle property.
	* \see \ref PageObjectHandle::PageObject_GetMediaBox
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_MediaBox;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Metadata;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_N;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Names;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_NeedsRendering;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Nums;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_OCProperties;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ObjStm;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_OpenAction;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Outlines;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_OutputIntents;

	/**
	* \brief Represents \ref PageLabelHandle property.
	* \see \ref PageLabelHandle::PageLabel_Prefix
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_P;

	/**
	* \brief Represents type for \ref PageObjectHandle.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Page;

	/**
	* \brief Represents type for \ref PageLabelHandle.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_PageLabel;

	/**
	* \brief Represents type for \ref PageLabelsHandle.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_PageLabels;

	/**
	* \brief Represents \ref CatalogHandle property.
	* \see \ref CatalogHandle::Catalog_GetPageLayout
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_PageLayout;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_PageMode;

	/**
	* \brief Represents catalog \ref PageTreeHandle.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Pages;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Parent;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Perms;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_PieceInfo;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Predictor;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Prev;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Requirements;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Resources;

	/**
	* \brief Represents \ref CatalogHandle entry in document's dictionary.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Root;

	/**
	* \brief Represents \ref PageLabelHandle property.
	* \see \ref PageLabelHandle::PageLabel_Style
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_S;

	/**
	* \brief Represents \ref PageLabelHandle property.
	* \see \ref PageLabelHandle::PageLabel_Start
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_St;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Size;

	/**
	* \brief Represents \ref PageLayout value.
	* \see \ref PageLayout_SinglePage
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_SinglePage;

	/**
	* \brief Represents \ref PageLayout value.
	* \see \ref PageLayout_OneColumn
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_OneColumn;

	/**
	* \brief Represents \ref PageLayout value.
	* \see \ref PageLayout_TwoColumnLeft
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_TwoColumnLeft;

	/**
	* \brief Represents \ref PageLayout value.
	* \see \ref PageLayout_TwoColumnRight
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_TwoColumnRight;

	/**
	* \brief Represents \ref PageLayout value.
	* \see \ref PageLayout_TwoPageLeft
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_TwoPageLeft;

	/**
	* \brief Represents \ref PageLayout value.
	* \see \ref PageLayout_TwoPageRight
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_TwoPageRight;

	VANILLAPDF_API extern const NameObjectHandle* NameConstant_SpiderInfo;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_StructTreeRoot;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Threads;

	/**
	* \brief Usually represents a dictionary type entry.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Type;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_URI;

	/**
	* \brief Represents a catalog's version property.
	* \see \ref CatalogHandle
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Version;

	/**
	* \brief Represents document \ref ViewerPreferencesHandle.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ViewerPreferences;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_W;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_XRefStm;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetHideToolbar
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_HideToolbar;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetHideMenubar
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_HideMenubar;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetHideWindowUI
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_HideWindowUI;

	/**
	* \brief Represents ViewerPreferencesHandle* property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetFitWindow
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_FitWindow;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetCenterWindow
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_CenterWindow;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetDisplayDocTitle
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_DisplayDocTitle;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetNonFullScreenPageMode
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_NonFullScreenPageMode;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetDirection
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Direction;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetViewArea
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ViewArea;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetViewClip
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ViewClip;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetPrintArea
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_PrintArea;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetPrintClip
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_PrintClip;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetPrintScaling
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_PrintScaling;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetDuplex
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Duplex;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetPickTrayByPDFSize
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_PickTrayByPDFSize;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetPrintPageRange
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_PrintPageRange;

	/**
	* \brief Represents \ref ViewerPreferencesHandle property.
	* \see \ref ViewerPreferencesHandle::ViewerPreferences_GetNumCopies
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_NumCopies;

	VANILLAPDF_API extern const NameObjectHandle* NameConstant_UseNone;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_UseOutlines;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_UseThumbs;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_UseOC;

	/**
	* \brief Represents \ref ReadingOrder value.
	* \see \ref ReadingOrder_LeftToRight
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_L2R;

	/**
	* \brief Represents \ref ReadingOrder value.
	* \see \ref ReadingOrder_RightToLeft
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_R2L;

	/**
	* \brief Represents \ref PrintScaling value.
	* \see \ref PrintScaling_AppDefault
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_AppDefault;

	/**
	* \brief Represents \ref PrintScaling value.
	* \see \ref PrintScaling_None
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_None;

	/**
	* \brief Represents \ref Duplex value.
	* \see \ref Duplex_Simplex
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Simplex;

	/**
	* \brief Represents \ref Duplex value.
	* \see \ref Duplex_DuplexFlipShortEdge
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_DuplexFlipShortEdge;

	/**
	* \brief Represents \ref Duplex value.
	* \see \ref Duplex_DuplexFlipLongEdge
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_DuplexFlipLongEdge;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_FullScreen;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_UseAttachments;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Last;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Next;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Dest;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_C;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_F;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_U;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_O;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_R;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_V;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ID;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Standard;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Title;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Encrypt;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_CF;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_StmF;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_StrF;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_EFF;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_CFM;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_V2;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_AESV2;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_AESV3;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_StdCF;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Crypt;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Identity;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Name;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_SubFilter;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_AdbePkcs7s3;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_AdbePkcs7s4;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_AdbePkcs7s5;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Recipients;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_DefaultCryptFilter;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Info;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Author;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Subject;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Keywords;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Creator;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Producer;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_CreationDate;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ModDate;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Trapped;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Unknown;

	/**
	* \brief Represents \ref DocumentTrappedType value.
	* \see \ref DocumentTrappedType_True
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_True;

	/**
	* \brief Represents \ref DocumentTrappedType value.
	* \see \ref DocumentTrappedType_False
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_False;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_NeedsRendering;

	/**
	* \brief Represents DCTDecode filter, see 7.4.8.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_DCTDecode;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_XYZ;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Fit;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_FitH;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_FitV;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_FitR;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_FitB;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_FitBH;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_FitBV;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Subtype;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Annot;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Annots;

	/**
	* \brief Represents type for \ref LinkAnnotationHandle.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Link;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_D;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Text;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_FreeText;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Line;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Square;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Circle;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Polygon;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_PolyLine;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Highlight;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Underline;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Squiggly;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_StrikeOut;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_RubberStamp;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Caret;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Ink;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Popup;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_FileAttachment;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Sound;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Movie;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Widget;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Screen;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_PrinterMark;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_TrapNetwork;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Watermark;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_TripleD;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Redact;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_XRef;

	/**
	* \brief Represents type for \ref FontHandle.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Font;

	/**
	* \brief Represents type for \ref CompositeFontHandle.
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Type0;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Type1;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Type3;

	/**
	* \brief Represents \ref CharacterMapHandle property.
	* \see \ref CharacterMapHandle::CharacterMap_ToUnicode
	*/
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ToUnicode;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_CIDSystemInfo;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_CMapName;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_CMapType;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_CMap;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Registry;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Ordering;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Supplement;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_FT;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Btn;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Ch;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Sig;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Fields;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ByteRange;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Reason;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Location;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_M;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ContactInfo;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Cert;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_BaseFont;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Width;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Height;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ColorSpace;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_RGB;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_G;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_CMYK;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_DeviceRGB;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_DeviceGray;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_DeviceCMYK;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_XObject;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Image;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_ProcSet;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_Tx;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_SigFlags;

	VANILLAPDF_API extern const NameObjectHandle* NameConstant_AdbePkcs7s3;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_AdbePkcs7s4;
	VANILLAPDF_API extern const NameObjectHandle* NameConstant_AdbePkcs7s5;

	/** @} */

	// This command ends suppress warning for undocumented members
	//! \}

#ifdef __cplusplus
};
#endif

#endif /* _C_NAME_CONSTANTS_H */
