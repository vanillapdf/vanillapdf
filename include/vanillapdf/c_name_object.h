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
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_GetValue(const struct NameObjectHandleTag* handle, BufferHandle* result);

	/**
	* \brief Set names new value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_SetValue(NameObjectHandle handle, BufferHandle value);

	/**
	* \copydoc Object_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_Release(NameObjectHandle handle);

	/** @} */

	// This command suppresses warning for undocumented members
	// Name constants does not have a context-less meaning
	//! \{

	/**
	* \addtogroup NameConstants
	* @{
	*/

	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_AA;

	/**
	* \brief Represents ASCII85Decode filter, see 7.4.3.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ASCII85Decode;

	/**
	* \brief Represents ASCIIHexDecode filter, see 7.4.2.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ASCIIHexDecode;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_AcroForm;

	/**
	* \brief Represents ::DeveloperExtensionHandle property.
	* \see DeveloperExtension_GetBaseVersion
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_BaseVersion;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_BitsPerComponent;

	/**
	* \brief Represents type for ::CatalogHandle
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Catalog;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Collection;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Colors;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Columns;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Container;

	/**
	* \brief Represents page ::ContentsHandle
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Contents;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Count;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_DecodeParms;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Dests;

	/**
	* \brief Represents document ::DeveloperExtensionsHandle
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_DeveloperExtensions;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_EarlyChange;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Extends;

	/**
	* \brief Represents ::DeveloperExtensionHandle property.
	* \see DeveloperExtension_GetExtensionLevel
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ExtensionLevel;

	/**
	* \brief Represents ::CatalogHandle property.
	* \see Catalog_GetExtensions
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Extensions;

	/**
	* \brief Name for expressing compression filters
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Filter;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_First;

	/**
	* \brief Represents FlateDecode filter, see 7.4.4.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_FlateDecode;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Index;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Kids;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Lang;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Legal;

	/**
	* \brief Usually represents length of a ::StreamObjectHandle.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Length;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Limits;

	/**
	* \brief Represents ::PageObjectHandle property.
	* \see PageObject_GetMediaBox
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_MediaBox;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Metadata;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_N;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Names;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_NeedsRendering;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Nums;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_OCProperties;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ObjStm;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_OpenAction;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Outlines;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_OutputIntents;

	/**
	* \brief Represents ::PageLabelHandle property.
	* \see PageLabel_Prefix
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_P;

	/**
	* \brief Represents type for ::PageObjectHandle.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Page;

	/**
	* \brief Represents type for ::PageLabelHandle.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_PageLabel;

	/**
	* \brief Represents type for ::PageLabelsHandle.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_PageLabels;

	/**
	* \brief Represents ::CatalogHandle property.
	* \see Catalog_GetPageLayout
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_PageLayout;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_PageMode;

	/**
	* \brief Represents catalog ::PageTreeHandle.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Pages;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Parent;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Perms;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_PieceInfo;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Predictor;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Prev;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Requirements;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Resources;

	/**
	* \brief Represents ::CatalogHandle entry in document's dictionary.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Root;

	/**
	* \brief Represents ::PageLabelHandle property.
	* \see PageLabel_Style
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_S;

	/**
	* \brief Represents ::PageLabelHandle property.
	* \see PageLabel_Start
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_St;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Size;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_SinglePage
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_SinglePage;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_OneColumn
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_OneColumn;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_TwoColumnLeft
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_TwoColumnLeft;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_TwoColumnRight
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_TwoColumnRight;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_TwoPageLeft
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_TwoPageLeft;

	/**
	* \brief Represents ::PageLayout value.
	* \see PageLayout_TwoPageRight
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_TwoPageRight;

	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_SpiderInfo;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_StructTreeRoot;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Threads;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Type;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_URI;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Version;

	/**
	* \brief Represents document ::ViewerPreferencesHandle.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ViewerPreferences;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_W;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_XRefStm;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetHideToolbar
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_HideToolbar;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetHideMenubar
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_HideMenubar;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetHideWindowUI
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_HideWindowUI;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetFitWindow
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_FitWindow;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetCenterWindow
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_CenterWindow;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetDisplayDocTitle
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_DisplayDocTitle;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetNonFullScreenPageMode
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_NonFullScreenPageMode;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetDirection
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Direction;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetViewArea
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ViewArea;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetViewClip
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ViewClip;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPrintArea
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_PrintArea;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPrintClip
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_PrintClip;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPrintScaling
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_PrintScaling;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetDuplex
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Duplex;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPickTrayByPDFSize
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_PickTrayByPDFSize;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetPrintPageRange
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_PrintPageRange;

	/**
	* \brief Represents ::ViewerPreferencesHandle property.
	* \see ViewerPreferences_GetNumCopies
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_NumCopies;

	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_UseNone;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_UseOutlines;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_UseThumbs;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_UseOC;

	/**
	* \brief Represents ::ReadingOrder value.
	* \see ReadingOrder_LeftToRight
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_L2R;

	/**
	* \brief Represents ::ReadingOrder value.
	* \see ReadingOrder_RightToLeft
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_R2L;

	/**
	* \brief Represents ::PrintScaling value.
	* \see PrintScaling_AppDefault
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_AppDefault;

	/**
	* \brief Represents ::PrintScaling value.
	* \see PrintScaling_None
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_None;

	/**
	* \brief Represents ::Duplex value.
	* \see Duplex_Simplex
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Simplex;

	/**
	* \brief Represents ::Duplex value.
	* \see Duplex_DuplexFlipShortEdge
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_DuplexFlipShortEdge;

	/**
	* \brief Represents ::Duplex value.
	* \see Duplex_DuplexFlipLongEdge
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_DuplexFlipLongEdge;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_FullScreen;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_UseAttachments;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Last;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Next;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Dest;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_C;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_F;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_U;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_O;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_R;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_V;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ID;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Standard;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Title;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Encrypt;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_CF;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_StmF;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_StrF;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_EFF;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_CFM;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_V2;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_AESV2;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_AESV3;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_StdCF;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Crypt;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Identity;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Name;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_SubFilter;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_AdbePkcs7s3;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_AdbePkcs7s4;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_AdbePkcs7s5;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Recipients;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_DefaultCryptFilter;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Info;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Author;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Subject;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Keywords;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Creator;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Producer;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_CreationDate;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ModDate;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Trapped;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Unknown;

	/**
	* \brief Represents ::DocumentTrappedType value.
	* \see DocumentTrappedType_True
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_True;

	/**
	* \brief Represents ::DocumentTrappedType value.
	* \see DocumentTrappedType_False
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_False;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_NeedsRendering;

	/**
	* \brief Represents DCTDecode filter, see 7.4.8.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_DCTDecode;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_XYZ;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Fit;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_FitH;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_FitV;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_FitR;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_FitB;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_FitBH;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_FitBV;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Subtype;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Annot;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Annots;

	/**
	* \brief Represents type for ::LinkAnnotationHandle.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Link;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_D;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Text;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_FreeText;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Line;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Square;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Circle;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Polygon;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_PolyLine;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Highlight;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Underline;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Squiggly;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_StrikeOut;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_RubberStamp;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Caret;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Ink;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Popup;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_FileAttachment;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Sound;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Movie;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Widget;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Screen;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_PrinterMark;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_TrapNetwork;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Watermark;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_TripleD;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Redact;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_XRef;

	/**
	* \brief Represents type for ::FontHandle.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Font;

	/**
	* \brief Represents type for ::CompositeFontHandle.
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Type0;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Type1;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Type3;

	/**
	* \brief Represents ::CharacterMapHandle property.
	* \see CharacterMap_ToUnicode
	*/
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ToUnicode;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_CIDSystemInfo;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_CMapName;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_CMapType;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_CMap;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Registry;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Ordering;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Supplement;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_FT;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Btn;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Ch;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Sig;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Fields;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ByteRange;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Reason;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Location;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_M;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ContactInfo;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Cert;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_BaseFont;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Width;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Height;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ColorSpace;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_RGB;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_G;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_CMYK;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_DeviceRGB;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_DeviceGray;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_DeviceCMYK;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_XObject;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_Image;
	VANILLAPDF_API extern const struct NameObjectHandleTag* NameConstant_ProcSet;

	/** @} */

	// This command ends suppress warning for undocumented members
	//! \}

#ifdef __cplusplus
};
#endif

#endif /* _C_NAME_OBJECT_H */
