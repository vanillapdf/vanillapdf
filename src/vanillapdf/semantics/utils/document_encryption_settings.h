#ifndef _DOCUMENT_ENCRYPTION_SETTINGS_H
#define _DOCUMENT_ENCRYPTION_SETTINGS_H

#include "semantics/utils/semantics_fwd.h"

#include "syntax/utils/encryption.h"
#include "syntax/utils/output_pointer.h"

#include "utils/unknown_interface.h"

namespace vanillapdf {
namespace semantics {

class DocumentEncryptionSettings : public IUnknown {
public:
	enum UserAccessPermissionFlags : int32_t
	{
		None = 0,
		PrintDegraded = 4,
		ModifyContents = 8,
		CopyAndExtract = 16,
		AddAnnotations = 32,
		FillForms = 256,
		ExtractText = 512,
		AssembleDocument = 1024,
		PrintFaithful = 2048
	};

public:
	DocumentEncryptionSettings();

	syntax::EncryptionAlgorithm GetEncryptionAlgorithm() const noexcept {
		return m_algorithm;
	}

	void SetEncryptionAlgorithm(syntax::EncryptionAlgorithm algorithm) noexcept {
		m_algorithm = algorithm;
	}

	int32_t GetKeyLength() const noexcept {
		return m_key_length;
	}

	void SetKeyLength(int32_t value) noexcept {
		m_key_length = value;
	}

	UserAccessPermissionFlags GetUserPermissions() const noexcept {
		return m_permissions;
	}

	void SetUserPermissions(UserAccessPermissionFlags value) noexcept {
		m_permissions = value;
	}

	BufferPtr GetUserPassword() const {
		return m_user_password;
	}

	void SetUserPassword(BufferPtr value) {
		m_user_password = value;
	}

	BufferPtr GetOwnerPassword() const {
		return m_owner_password;
	}

	void SetOwnerPassword(BufferPtr value) {
		m_owner_password = value;
	}

	static int32_t GetPermissionInteger(UserAccessPermissionFlags value);

private:
	syntax::EncryptionAlgorithm m_algorithm;
	int32_t m_key_length;
	UserAccessPermissionFlags m_permissions;

	BufferPtr m_user_password;
	BufferPtr m_owner_password;
};

} // semantics
} // vanillapdf

#endif /* _DOCUMENT_ENCRYPTION_SETTINGS_H */
