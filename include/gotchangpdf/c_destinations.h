#ifndef _C_DESTINATIONS_H
#define _C_DESTINATIONS_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#include "c_version.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		DestinationType_XYZ = 0,
		DestinationType_Fit,
		DestinationType_FitHorizontal,
		DestinationType_FitVertical,
		DestinationType_FitRectangle,
		DestinationType_FitBoundingBox,
		DestinationType_FitBoundingBoxHorizontal,
		DestinationType_FitBoundingBoxVertical,
	} DestinationType, *PDestinationType;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Destination_GetPageNumber(DestinationHandle handle, PObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Destination_Release(DestinationHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION NamedDestinations_Contains(NamedDestinationsHandle handle, NameHandle name, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NamedDestinations_Find(NamedDestinationsHandle handle, NameHandle name, PDestinationHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NamedDestinations_Release(NamedDestinationsHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_DESTINATIONS_H */
