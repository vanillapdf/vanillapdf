#include "precompiled.h"

#include "semantics/utils/document_encryption_settings.h"

namespace vanillapdf {
namespace semantics {

	using namespace syntax;

	DocumentEncryptionSettings::DocumentEncryptionSettings() {
		m_algorithm = EncryptionAlgorithm::RC4;
		m_key_length = 40;
		m_permissions = UserAccessPermissionFlags::None;
	}

	int32_t DocumentEncryptionSettings::GetPermissionInteger(UserAccessPermissionFlags value) {
		int32_t permission_value = -1;

		if (value & UserAccessPermissionFlags::PrintDegraded) {
			permission_value &= ~UserAccessPermissionFlags::PrintDegraded;
		}

		if (value & UserAccessPermissionFlags::ModifyContents) {
			permission_value &= ~UserAccessPermissionFlags::ModifyContents;
		}

		if (value & UserAccessPermissionFlags::CopyAndExtract) {
			permission_value &= ~UserAccessPermissionFlags::CopyAndExtract;
		}

		if (value & UserAccessPermissionFlags::AddAnnotations) {
			permission_value &= ~UserAccessPermissionFlags::AddAnnotations;
		}

		if (value & UserAccessPermissionFlags::FillForms) {
			permission_value &= ~UserAccessPermissionFlags::FillForms;
		}

		if (value & UserAccessPermissionFlags::ExtractText) {
			permission_value &= ~UserAccessPermissionFlags::ExtractText;
		}

		if (value & UserAccessPermissionFlags::AssembleDocument) {
			permission_value &= ~UserAccessPermissionFlags::AssembleDocument;
		}

		if (value & UserAccessPermissionFlags::PrintFaithful) {
			permission_value &= ~UserAccessPermissionFlags::PrintFaithful;
		}

		return permission_value;
	}

} // semantics
} // vanillapdf
