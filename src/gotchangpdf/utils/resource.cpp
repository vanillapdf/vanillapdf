#include "precompiled.h"

#include "utils/resource.h"

namespace gotchangpdf {

static const uint8_t MASTER_CERTIFICATE[] = R"(-----BEGIN CERTIFICATE-----
MIICvDCCAaSgAwIBAgIBATANBgkqhkiG9w0BAQUFADASMRAwDgYDVQQDEwdOZXdS
b290MB4XDTE3MDIwMTE3MjUwMFoXDTI3MDIwMTE3MjUwMFowEjEQMA4GA1UEAxMH
TmV3Um9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOM5TcEUi9cn
HGawTR5dAmX58jQVSPH89H4g0yWiB3HGDgPHdZxBg5MH2PCHWoil4h/KQWUI8XVt
HWEpH5zjkPe/lQAxaNUWsUf20wIzBm3G7T68oLffS7BVuyh1Sfd27UqoXcmtiwZm
Kj1AWS2YCcw2d5qq+771kau7WzPVOLOcBX/vwOLjuuRPBOZV23Ei7hpHrvrkKubV
UIKUruDbNIXrDpVuz4ikRYmuSx/gpE5X46x4uJ/Q40cORSnKL96nsMkqiFo1ydeh
WxlG1l//maQoJy7HK1u+WPMg9ppoGb6zSguqUF69bSmBrJcbyVtW2hTZtt6kE0kU
ixsd8EhC4tsCAwEAAaMdMBswDAYDVR0TBAUwAwEB/zALBgNVHQ8EBAMCAf4wDQYJ
KoZIhvcNAQEFBQADggEBAMy0Q0THRUbS4E4o/cVIMWAZNgwAOEm3RwGvvMvZaivN
+wmY8jQi9fjOAlpCyfAN1C899kQS0/e2iWJx66rvKnEtPjVjaCCzcPMaBy8MwEBn
B1PIE96djzRktHV9SzesrrLKZEgDa5YrB5CjfkY4A1Ibg5FvgyI8V5fbPMCF7AfI
QyMx0X6oPIhIoVKr4ze/olJRBIJyYhMGH4N1OGsyd1opQX+CVBcSpSHDOSZfuRVI
qKwGgc4cAtlwstkYjKi2hQRMrFebpVoJjeI7BkaiF2tm+OrPkTuRm3Z+2IQFnR3v
NnU41+txqBdrP5y5T+SyKJXOGqgMg5WBBxisU4NY4tE=
-----END CERTIFICATE-----)";

static const uint8_t WATERMARK_IMAGE[] = R"(TODO)";

static const uint8_t SERIAL_BLACKLIST[] = R"([])";

BufferPtr Resource::Load(ResourceID id) {
	switch (id) {
		case ResourceID::MASTER_CERTIFICATE:
			return make_deferred<Buffer>(MASTER_CERTIFICATE, sizeof(MASTER_CERTIFICATE));
		case ResourceID::WATERMARK_IMAGE:
			return make_deferred<Buffer>(WATERMARK_IMAGE, sizeof(WATERMARK_IMAGE));
		case ResourceID::SERIAL_BLACKLIST:
			return make_deferred<Buffer>(SERIAL_BLACKLIST, sizeof(SERIAL_BLACKLIST));
	}

	throw GeneralException("Resource was not found");
}

} // gotchangpdf
