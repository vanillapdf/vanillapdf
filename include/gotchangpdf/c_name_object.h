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

	extern NameHandle ASCII85Decode;
	extern NameHandle BitsPerComponent;
	extern NameHandle Colors;
	extern NameHandle Columns;
	extern NameHandle Container;
	extern NameHandle Contents;
	extern NameHandle DecodeParms;
	extern NameHandle EarlyChange;
	extern NameHandle Length;
	extern NameHandle Type;
	extern NameHandle ObjStm;
	extern NameHandle N;
	extern NameHandle First;
	extern NameHandle Extends;
	extern NameHandle Filter;
	extern NameHandle FlateDecode;
	extern NameHandle Index;
	extern NameHandle Catalog;
	extern NameHandle Pages;
	extern NameHandle Page;
	extern NameHandle Count;
	extern NameHandle Kids;
	extern NameHandle MediaBox;
	extern NameHandle Resources;
	extern NameHandle Parent;
	extern NameHandle Predictor;
	extern NameHandle Prev;
	extern NameHandle Root;
	extern NameHandle Size;
	extern NameHandle Version;
	extern NameHandle W;
	extern NameHandle XRefStm;

#ifdef __cplusplus
};
#endif

#endif /* _C_NAME_OBJECT_H */
