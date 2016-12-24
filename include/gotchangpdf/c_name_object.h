#ifndef _C_NAME_OBJECT_H
#define _C_NAME_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/** \file c_name_object.h
	* This file contains class definitions for NameObjectHandle
	*/

	/**
	* \brief Get names binary representation
	* \memberof NameObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_GetValue(NameObjectHandle handle, PBufferHandle result);

	/**
	* \brief Set names new value
	* \memberof NameObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_SetValue(NameObjectHandle handle, BufferHandle value);

	/**
	* \memberof NameObjectHandle
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_Release(NameObjectHandle handle);

	/**
	* \defgroup NameConstants Name constants
	* \ingroup Objects
	* \brief Often used name constant definitions
	* @{
	*/

	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_AA;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ASCII85Decode;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ASCIIHexDecode;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_AcroForm;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_BaseVersion;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_BitsPerComponent;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Catalog;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Collection;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Colors;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Columns;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Container;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Contents;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Count;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_DecodeParms;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Dests;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_DeveloperExtensions;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_EarlyChange;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Extends;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ExtensionLevel;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Extensions;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Filter;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_First;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FlateDecode;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Index;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Kids;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Lang;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Legal;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Length;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Limits;
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
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_P;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Page;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PageLabel;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PageLabels;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PageLayout;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PageMode;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Pages;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Parent;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Perms;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PieceInfo;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Predictor;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Prev;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Requirements;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Resources;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Root;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_S;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_St;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Size;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_SinglePage;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_OneColumn;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_TwoColumnLeft;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_TwoColumnRight;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_TwoPageLeft;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_TwoPageRight;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_SpiderInfo;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_StructTreeRoot;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Threads;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Type;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_URI;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Version;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ViewerPreferences;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_W;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_XRefStm;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_HideToolbar;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_HideMenubar;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_HideWindowUI;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_FitWindow;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_CenterWindow;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_DisplayDocTitle;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_NonFullScreenPageMode;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Direction;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ViewArea;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ViewClip;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PrintArea;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PrintClip;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PrintScaling;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Duplex;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PickTrayByPDFSize;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_PrintPageRange;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_NumCopies;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_UseNone;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_UseOutlines;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_UseThumbs;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_UseOC;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_L2R;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_R2L;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_AppDefault;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_None;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Simplex;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_DuplexFlipShortEdge;
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
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_True;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_False;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_NeedsRendering;
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
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Font;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Type0;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Type1;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Type3;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_ToUnicode;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_CIDSystemInfo;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_CMapName;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_CMapType;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_CMap;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Registry;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Ordering;
	GOTCHANG_PDF_API extern const NameObjectHandle NameConstant_Supplement;

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_NAME_OBJECT_H */
