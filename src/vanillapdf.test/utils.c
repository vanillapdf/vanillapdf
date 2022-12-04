#include "test.h"

#include <stdarg.h>

const int VANILLAPDF_TEST_ERROR_SUCCESS = 0;
const int VANILLAPDF_TEST_ERROR_INVALID_PASSWORD = 1;
const int VANILLAPDF_TEST_ERROR_INVALID_PARAMETERS = 2;
const int VANILLAPDF_TEST_ERROR_LOGGING_ENABLED = 3;
const int VANILLAPDF_TEST_ERROR_FAILURE = 255;

static boolean_type quiet_mode = 0;

boolean_type is_quiet_mode() {
	return quiet_mode;
}

error_type set_quiet_mode(boolean_type value) {
	quiet_mode = value;
	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

void print_spaces(int nested) {
	if (is_quiet_mode() == VANILLAPDF_RV_TRUE) {
		return;
	}

	int i;
	for (i = 0; i < nested; ++i) {
		print_text("  ");
	}
}

void print_text(const char * const format, ...) {
	if (is_quiet_mode() == VANILLAPDF_RV_TRUE) {
		return;
	}

	va_list argument_list;
	va_start(argument_list, format);
	vprintf(format, argument_list);
	va_end(argument_list);
}

error_type process_utils_conversions() {
	BufferHandle* buffer_handle = NULL;
	IUnknownHandle *unknown_buffer_handle = NULL;

	BufferArrayHandle* buffer_array_handle = NULL;
	IUnknownHandle *unknown_buffer_array_handle = NULL;

	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Create(&buffer_handle));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_ToUnknown(buffer_handle, &unknown_buffer_handle));
	RETURN_ERROR_IF_NOT_SUCCESS(IUnknown_Release(unknown_buffer_handle));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(buffer_handle));

	RETURN_ERROR_IF_NOT_SUCCESS(BufferArray_Create(&buffer_array_handle));
	RETURN_ERROR_IF_NOT_SUCCESS(BufferArray_ToUnknown(buffer_array_handle, &unknown_buffer_array_handle));
	RETURN_ERROR_IF_NOT_SUCCESS(IUnknown_Release(unknown_buffer_array_handle));
	RETURN_ERROR_IF_NOT_SUCCESS(BufferArray_Release(buffer_array_handle));

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

//! [Print buffer]
error_type process_buffer(BufferHandle* buffer, int nested) {
	string_type data = NULL;
	char* local_string = NULL;
	size_type size = 0;
	size_type print_size = 0;
	unsigned long long size_converted = 0;

	print_spaces(nested);
	print_text("Buffer begin\n");

	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(buffer, &data, &size));

	if (size >= SIZE_MAX) {
		unsigned long long size_long_long = size;
		print_text("Buffer size is too big: %llu bytes\n", size_long_long);
		return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	print_size = size > 20 ? 10 : size;
	size_converted = size;

	local_string = (char*) calloc(sizeof(char), print_size + 1);
	if (NULL == local_string) {
		unsigned long long print_size_converted = print_size;
		print_text("Could not allocate memory: %llu bytes\n", print_size_converted + 1);
		return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	memcpy(local_string, data, print_size);

	print_spaces(nested + 1);
	print_text("Size: %llu\n", size_converted);
	print_spaces(nested + 1);
	print_text("Data: %s\n", local_string);

	free(local_string);

	print_spaces(nested);
	print_text("Buffer end\n");

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}
//! [Print buffer]

error_type process_version(PDFVersion version, int nested) {
	print_spaces(nested);
	print_text("PDF Version: 1.%d \n", version);

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

//! [Test logging]
error_type process_logging() {
	boolean_type logging_enabled = VANILLAPDF_RV_TRUE;
	LoggingSeverity logging_severity;

	RETURN_ERROR_IF_NOT_SUCCESS(Logging_IsEnabled(&logging_enabled));

	if (logging_enabled != VANILLAPDF_RV_FALSE) {
		return VANILLAPDF_TEST_ERROR_LOGGING_ENABLED;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Logging_Enable());
	RETURN_ERROR_IF_NOT_SUCCESS(Logging_IsEnabled(&logging_enabled));

	if (logging_enabled != VANILLAPDF_RV_TRUE) {
		return VANILLAPDF_TEST_ERROR_LOGGING_ENABLED;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Logging_Disable());
	RETURN_ERROR_IF_NOT_SUCCESS(Logging_IsEnabled(&logging_enabled));

	if (logging_enabled != VANILLAPDF_RV_FALSE) {
		return VANILLAPDF_TEST_ERROR_LOGGING_ENABLED;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Logging_Enable());
	RETURN_ERROR_IF_NOT_SUCCESS(Logging_IsEnabled(&logging_enabled));

	if (logging_enabled != VANILLAPDF_RV_TRUE) {
		return VANILLAPDF_TEST_ERROR_LOGGING_ENABLED;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Logging_SetSeverity(LoggingSeverity_Info));

	RETURN_ERROR_IF_NOT_SUCCESS(Logging_GetSeverity(&logging_severity));
	if (logging_severity != LoggingSeverity_Info) {
		return VANILLAPDF_TEST_ERROR_LOGGING_ENABLED;
	}

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}
//! [Test logging]

error_type process_constants() {
	boolean_type dummy_boolean;
	error_type dummy_error;
	const NameObjectHandle* dummy_name = NULL;

	// They might seem unused, but they test symbol visibility
	UNUSED(dummy_boolean);
	UNUSED(dummy_error);
	UNUSED(dummy_name);

	// return values

	dummy_boolean = VANILLAPDF_RV_FALSE;
	dummy_boolean = VANILLAPDF_RV_TRUE;

	// global error states

	dummy_error = VANILLAPDF_ERROR_SUCCESS;
	dummy_error = VANILLAPDF_ERROR_PARAMETER_VALUE;
	dummy_error = VANILLAPDF_ERROR_NOT_SUPPORTED;
	dummy_error = VANILLAPDF_ERROR_USER_CANCELLED;
	dummy_error = VANILLAPDF_ERROR_ZLIB_DATA;
	dummy_error = VANILLAPDF_ERROR_INVALID_LICENSE;
	dummy_error = VANILLAPDF_ERROR_GENERAL;

	// Syntax errors

	dummy_error = VANILLAPDF_ERROR_CONVERSION;
	dummy_error = VANILLAPDF_ERROR_FILE_DISPOSED;
	dummy_error = VANILLAPDF_ERROR_FILE_NOT_INITIALIZED;
	dummy_error = VANILLAPDF_ERROR_OBJECT_MISSING;
	dummy_error = VANILLAPDF_ERROR_PARSE_EXCEPTION;
	dummy_error = VANILLAPDF_ERROR_INVALID_PASSWORD;
	dummy_error = VANILLAPDF_ERROR_DUPLICATE_KEY;

	// Name constants

	dummy_name = NameConstant_AA;
	dummy_name = NameConstant_ASCII85Decode;
	dummy_name = NameConstant_ASCIIHexDecode;
	dummy_name = NameConstant_AcroForm;
	dummy_name = NameConstant_BaseVersion;
	dummy_name = NameConstant_BitsPerComponent;
	dummy_name = NameConstant_Catalog;
	dummy_name = NameConstant_Collection;
	dummy_name = NameConstant_Colors;
	dummy_name = NameConstant_Columns;
	dummy_name = NameConstant_Container;
	dummy_name = NameConstant_Contents;
	dummy_name = NameConstant_Count;
	dummy_name = NameConstant_DecodeParms;
	dummy_name = NameConstant_Dests;
	dummy_name = NameConstant_DeveloperExtensions;
	dummy_name = NameConstant_EarlyChange;
	dummy_name = NameConstant_Extends;
	dummy_name = NameConstant_ExtensionLevel;
	dummy_name = NameConstant_Extensions;
	dummy_name = NameConstant_Filter;
	dummy_name = NameConstant_First;
	dummy_name = NameConstant_FlateDecode;
	dummy_name = NameConstant_Index;
	dummy_name = NameConstant_Kids;
	dummy_name = NameConstant_Lang;
	dummy_name = NameConstant_Legal;
	dummy_name = NameConstant_Length;
	dummy_name = NameConstant_Limits;
	dummy_name = NameConstant_MediaBox;
	dummy_name = NameConstant_Metadata;
	dummy_name = NameConstant_N;
	dummy_name = NameConstant_Names;
	dummy_name = NameConstant_NeedsRendering;
	dummy_name = NameConstant_Nums;
	dummy_name = NameConstant_OCProperties;
	dummy_name = NameConstant_ObjStm;
	dummy_name = NameConstant_OpenAction;
	dummy_name = NameConstant_Outlines;
	dummy_name = NameConstant_OutputIntents;
	dummy_name = NameConstant_P;
	dummy_name = NameConstant_Page;
	dummy_name = NameConstant_PageLabel;
	dummy_name = NameConstant_PageLabels;
	dummy_name = NameConstant_PageLayout;
	dummy_name = NameConstant_PageMode;
	dummy_name = NameConstant_Pages;
	dummy_name = NameConstant_Parent;
	dummy_name = NameConstant_Perms;
	dummy_name = NameConstant_PieceInfo;
	dummy_name = NameConstant_Predictor;
	dummy_name = NameConstant_Prev;
	dummy_name = NameConstant_Requirements;
	dummy_name = NameConstant_Resources;
	dummy_name = NameConstant_Root;
	dummy_name = NameConstant_S;
	dummy_name = NameConstant_St;
	dummy_name = NameConstant_Size;
	dummy_name = NameConstant_SinglePage;
	dummy_name = NameConstant_OneColumn;
	dummy_name = NameConstant_TwoColumnLeft;
	dummy_name = NameConstant_TwoColumnRight;
	dummy_name = NameConstant_TwoPageLeft;
	dummy_name = NameConstant_TwoPageRight;
	dummy_name = NameConstant_SpiderInfo;
	dummy_name = NameConstant_StructTreeRoot;
	dummy_name = NameConstant_Threads;
	dummy_name = NameConstant_Type;
	dummy_name = NameConstant_URI;
	dummy_name = NameConstant_Version;
	dummy_name = NameConstant_ViewerPreferences;
	dummy_name = NameConstant_W;
	dummy_name = NameConstant_XRefStm;
	dummy_name = NameConstant_HideToolbar;
	dummy_name = NameConstant_HideMenubar;
	dummy_name = NameConstant_HideWindowUI;
	dummy_name = NameConstant_FitWindow;
	dummy_name = NameConstant_CenterWindow;
	dummy_name = NameConstant_DisplayDocTitle;
	dummy_name = NameConstant_NonFullScreenPageMode;
	dummy_name = NameConstant_Direction;
	dummy_name = NameConstant_ViewArea;
	dummy_name = NameConstant_ViewClip;
	dummy_name = NameConstant_PrintArea;
	dummy_name = NameConstant_PrintClip;
	dummy_name = NameConstant_PrintScaling;
	dummy_name = NameConstant_Duplex;
	dummy_name = NameConstant_PickTrayByPDFSize;
	dummy_name = NameConstant_PrintPageRange;
	dummy_name = NameConstant_NumCopies;
	dummy_name = NameConstant_UseNone;
	dummy_name = NameConstant_UseOutlines;
	dummy_name = NameConstant_UseThumbs;
	dummy_name = NameConstant_UseOC;
	dummy_name = NameConstant_L2R;
	dummy_name = NameConstant_R2L;
	dummy_name = NameConstant_AppDefault;
	dummy_name = NameConstant_None;
	dummy_name = NameConstant_Simplex;
	dummy_name = NameConstant_DuplexFlipShortEdge;
	dummy_name = NameConstant_DuplexFlipLongEdge;
	dummy_name = NameConstant_FullScreen;
	dummy_name = NameConstant_UseAttachments;
	dummy_name = NameConstant_Last;
	dummy_name = NameConstant_Next;
	dummy_name = NameConstant_Dest;
	dummy_name = NameConstant_C;
	dummy_name = NameConstant_F;
	dummy_name = NameConstant_U;
	dummy_name = NameConstant_O;
	dummy_name = NameConstant_R;
	dummy_name = NameConstant_V;
	dummy_name = NameConstant_ID;
	dummy_name = NameConstant_Standard;
	dummy_name = NameConstant_Title;
	dummy_name = NameConstant_Encrypt;
	dummy_name = NameConstant_CF;
	dummy_name = NameConstant_StmF;
	dummy_name = NameConstant_StrF;
	dummy_name = NameConstant_EFF;
	dummy_name = NameConstant_CFM;
	dummy_name = NameConstant_V2;
	dummy_name = NameConstant_AESV2;
	dummy_name = NameConstant_AESV3;
	dummy_name = NameConstant_StdCF;
	dummy_name = NameConstant_Crypt;
	dummy_name = NameConstant_Identity;
	dummy_name = NameConstant_Name;
	dummy_name = NameConstant_SubFilter;
	dummy_name = NameConstant_AdbePkcs7s3;
	dummy_name = NameConstant_AdbePkcs7s4;
	dummy_name = NameConstant_AdbePkcs7s5;
	dummy_name = NameConstant_Recipients;
	dummy_name = NameConstant_DefaultCryptFilter;
	dummy_name = NameConstant_Info;
	dummy_name = NameConstant_Author;
	dummy_name = NameConstant_Subject;
	dummy_name = NameConstant_Keywords;
	dummy_name = NameConstant_Creator;
	dummy_name = NameConstant_Producer;
	dummy_name = NameConstant_CreationDate;
	dummy_name = NameConstant_ModDate;
	dummy_name = NameConstant_Trapped;
	dummy_name = NameConstant_Unknown;
	dummy_name = NameConstant_True;
	dummy_name = NameConstant_False;
	dummy_name = NameConstant_NeedsRendering;
	dummy_name = NameConstant_DCTDecode;
	dummy_name = NameConstant_XYZ;
	dummy_name = NameConstant_Fit;
	dummy_name = NameConstant_FitH;
	dummy_name = NameConstant_FitV;
	dummy_name = NameConstant_FitR;
	dummy_name = NameConstant_FitB;
	dummy_name = NameConstant_FitBH;
	dummy_name = NameConstant_FitBV;
	dummy_name = NameConstant_Subtype;
	dummy_name = NameConstant_Annot;
	dummy_name = NameConstant_Annots;
	dummy_name = NameConstant_Link;
	dummy_name = NameConstant_D;
	dummy_name = NameConstant_Text;
	dummy_name = NameConstant_FreeText;
	dummy_name = NameConstant_Line;
	dummy_name = NameConstant_Square;
	dummy_name = NameConstant_Circle;
	dummy_name = NameConstant_Polygon;
	dummy_name = NameConstant_PolyLine;
	dummy_name = NameConstant_Highlight;
	dummy_name = NameConstant_Underline;
	dummy_name = NameConstant_Squiggly;
	dummy_name = NameConstant_StrikeOut;
	dummy_name = NameConstant_RubberStamp;
	dummy_name = NameConstant_Caret;
	dummy_name = NameConstant_Ink;
	dummy_name = NameConstant_Popup;
	dummy_name = NameConstant_FileAttachment;
	dummy_name = NameConstant_Sound;
	dummy_name = NameConstant_Movie;
	dummy_name = NameConstant_Widget;
	dummy_name = NameConstant_Screen;
	dummy_name = NameConstant_PrinterMark;
	dummy_name = NameConstant_TrapNetwork;
	dummy_name = NameConstant_Watermark;
	dummy_name = NameConstant_TripleD;
	dummy_name = NameConstant_Redact;
	dummy_name = NameConstant_XRef;
	dummy_name = NameConstant_Font;
	dummy_name = NameConstant_Type0;
	dummy_name = NameConstant_Type1;
	dummy_name = NameConstant_Type3;
	dummy_name = NameConstant_ToUnicode;
	dummy_name = NameConstant_CIDSystemInfo;
	dummy_name = NameConstant_CMapName;
	dummy_name = NameConstant_CMapType;
	dummy_name = NameConstant_CMap;
	dummy_name = NameConstant_Registry;
	dummy_name = NameConstant_Ordering;
	dummy_name = NameConstant_Supplement;
	dummy_name = NameConstant_FT;
	dummy_name = NameConstant_Btn;
	dummy_name = NameConstant_Ch;
	dummy_name = NameConstant_Sig;
	dummy_name = NameConstant_Fields;
	dummy_name = NameConstant_ByteRange;
	dummy_name = NameConstant_Reason;
	dummy_name = NameConstant_Location;
	dummy_name = NameConstant_M;
	dummy_name = NameConstant_ContactInfo;
	dummy_name = NameConstant_Cert;
	dummy_name = NameConstant_BaseFont;
	dummy_name = NameConstant_Width;
	dummy_name = NameConstant_Height;
	dummy_name = NameConstant_ColorSpace;
	dummy_name = NameConstant_RGB;
	dummy_name = NameConstant_G;
	dummy_name = NameConstant_CMYK;
	dummy_name = NameConstant_DeviceRGB;
	dummy_name = NameConstant_DeviceGray;
	dummy_name = NameConstant_DeviceCMYK;
	dummy_name = NameConstant_XObject;
	dummy_name = NameConstant_Image;
	dummy_name = NameConstant_ProcSet;
	dummy_name = NameConstant_Tx;
	dummy_name = NameConstant_SigFlags;

	dummy_name = NameConstant_AdbePkcs7s3;
	dummy_name = NameConstant_AdbePkcs7s4;
	dummy_name = NameConstant_AdbePkcs7s5;

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}

//! [Set license file]
error_type process_license_info(string_type license_file) {
	BufferHandle* license_buffer = NULL;
	InputStreamHandle* input_stream = NULL;
	boolean_type is_valid = VANILLAPDF_RV_FALSE;

	RETURN_ERROR_IF_NOT_SUCCESS(LicenseInfo_IsValid(&is_valid));
	if (is_valid != VANILLAPDF_RV_FALSE) {
		print_text("License is enabled by default\n");
		return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(InputStream_CreateFromFile(license_file, &input_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(InputStream_ToBuffer(input_stream, &license_buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(LicenseInfo_SetLicenseBuffer(license_buffer));
	RETURN_ERROR_IF_NOT_SUCCESS(LicenseInfo_IsValid(&is_valid));

	if (is_valid != VANILLAPDF_RV_TRUE) {
		print_text("Could not enable license\n");
		return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(InputStream_Release(input_stream));
	RETURN_ERROR_IF_NOT_SUCCESS(Buffer_Release(license_buffer));

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}
//! [Set license file]

//! [Print library info]
error_type process_library_info() {

	// Misc
	string_type library_author = NULL;

	// Version info
	integer_type library_version_major = 0;
	integer_type library_version_minor = 0;
	integer_type library_version_patch = 0;
	integer_type library_version_build = 0;

	// Build time information
	integer_type library_build_day = 0;
	integer_type library_build_month = 0;
	integer_type library_build_year = 0;

	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetVersionMajor(&library_version_major));
	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetVersionMinor(&library_version_minor));
	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetVersionPatch(&library_version_patch));
	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetVersionBuild(&library_version_build));
	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetAuthor(&library_author));

	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetBuildDay(&library_build_day));
	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetBuildMonth(&library_build_month));
	RETURN_ERROR_IF_NOT_SUCCESS(LibraryInfo_GetBuildYear(&library_build_year));

	print_text("Library vanillapdf %d.%d.%d.%d by %s\n",
		library_version_major,
		library_version_minor,
		library_version_patch,
		library_version_build,
		library_author
	);

	print_text("Built on %d.%d.%d\n",
		library_build_day,
		library_build_month,
		library_build_year
	);

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}
//! [Print library info]

//! [Print last error]
error_type print_last_error() {
	error_type error = 0;
	char* error_message = NULL;
	char* error_code_name = NULL;
	size_type error_code_name_length = 0;
	size_type error_message_length = 0;

	RETURN_ERROR_IF_NOT_SUCCESS(Errors_GetLastError(&error));

	// Last error message
	RETURN_ERROR_IF_NOT_SUCCESS(Errors_GetLastErrorMessageLength(&error_message_length));

	if (error_message_length >= SIZE_MAX) {
		unsigned long long length_converted = error_message_length;
		print_text("Buffer size is too big: %llu bytes\n", length_converted);
		return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	error_message = (char*) calloc(sizeof(char), error_message_length);
	if (NULL == error_message) {
		unsigned long long length_converted = error_message_length;
		print_text("Could not allocate memory: %llu bytes\n", length_converted);
		return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Errors_GetLastErrorMessage(error_message, error_message_length));

	// error code name
	RETURN_ERROR_IF_NOT_SUCCESS(Errors_GetPrintableErrorTextLength(error, &error_code_name_length));

	if (error_code_name_length >= SIZE_MAX) {
		unsigned long long length_converted = error_code_name_length;
		print_text("Buffer size is too big: %llu bytes\n", length_converted);
		return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	error_code_name = (char*) calloc(error_code_name_length, sizeof(char));
	if (NULL == error_code_name) {
		unsigned long long length_converted = error_code_name_length;
		print_text("Could not allocate memory: %llu bytes\n", length_converted);
		return VANILLAPDF_TEST_ERROR_FAILURE;
	}

	RETURN_ERROR_IF_NOT_SUCCESS(Errors_GetPrintableErrorText(error, error_code_name, error_code_name_length));

	if (error_message_length == 0) {
		print_text("Error %u (%s)\n", error, error_code_name);
	} else {
		print_text("Error %u (%s): %s\n", error, error_code_name, error_message);
	}

	free(error_message);
	free(error_code_name);

	return VANILLAPDF_TEST_ERROR_SUCCESS;
}
//! [Print last error]
