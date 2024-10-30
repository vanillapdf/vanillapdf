#!/usr/bin/python

import ntpath
import os
import io
import subprocess
import sys
import json
import unicodedata

USER_PASSWORD_KEY = "user_password"
OWNER_PASSWORD_KEY = "owner_password"
CERTIFICATE_KEY = "certificate"

LICENSE_OPTION = "-l"
PASSWORD_OPTION = "-p"
CERTIFICATE_OPTION = "-k"
MERGE_OPTION = "-m"
QUIET_OPTION = "-q"
SKIP_PROCESS_OPTION = "-sp"
SKIP_SAVE_OPTION = "-ss"
SKIP_EDIT_OPTION = "-se"
SKIP_INCREMENTAL_SAVE_OPTION = "-si"
SIGNING_CERTIFICATE_OPTION = "-sc"

def normalize_string( str ):
	normalized = unicodedata.normalize('NFC', str)
	encoded = normalized.encode('utf8')
	return encoded.decode('latin-1')

if (len(sys.argv) < 6):
	print ("Incorrect number of arguments!")
	print ("Usage: executable_path test_file_path encryption_config license_file_path source_root_path")
	sys.exit(1)

# Initialize return value
rv = -1

# Parse command line arguments
executable_path = sys.argv[1]
test_file_path = sys.argv[2]
encryption_config_path = sys.argv[3]
license_file_path = sys.argv[4]
source_root_path = sys.argv[5]

encryption_config_dir = os.path.dirname(encryption_config_path)
test_filename = ntpath.basename(test_file_path)

# Open the settings for encrypted files
config_data = ""
encryption_data = ""
with io.open(encryption_config_path, encoding='utf8') as test_config:
	config_data = json.load(test_config)
	encryption_data = config_data["Encryption"]

# Create devnull for output of test case
FNULL = open(os.devnull, 'w')

# Create list of base parameters
base_parameters = [executable_path, test_file_path, LICENSE_OPTION, license_file_path, QUIET_OPTION]

if ("Merge" in config_data):
	base_parameters.append(MERGE_OPTION)
	base_parameters.append(os.path.join(source_root_path, config_data["Merge"]))

if ("SkipProcess" in config_data and test_filename in config_data["SkipProcess"]):
	base_parameters.append(SKIP_PROCESS_OPTION)

if ("SkipSave" in config_data and test_filename in config_data["SkipSave"]):
	base_parameters.append(SKIP_SAVE_OPTION)

if ("SkipSave" in config_data and test_filename in config_data["SkipEdit"]):
	base_parameters.append(SKIP_EDIT_OPTION)

if ("SkipIncrementalSave" in config_data and test_filename in config_data["SkipIncrementalSave"]):
	base_parameters.append(SKIP_INCREMENTAL_SAVE_OPTION)

if ("SigningCertificate" in config_data):
	signing_certificate_path = os.path.join(source_root_path, config_data["SigningCertificate"])

	base_parameters.append(SIGNING_CERTIFICATE_OPTION)
	base_parameters.append(signing_certificate_path)

# Determine if the file is encrypted
is_encrypted = test_filename in encryption_data

# Check if filename is in our encrypted configuration file
if (is_encrypted):
	# Authentication using user or owner password
	if (USER_PASSWORD_KEY in encryption_data[test_filename] or OWNER_PASSWORD_KEY in encryption_data[test_filename]):
		# User password
		if (USER_PASSWORD_KEY in encryption_data[test_filename]):
			raw_password = encryption_data[test_filename][USER_PASSWORD_KEY]
			user_password = normalize_string(raw_password)
			user_specific_parameters = base_parameters
			user_specific_parameters.append(PASSWORD_OPTION)
			user_specific_parameters.append(user_password)
			
			rv = subprocess.call(user_specific_parameters)
			if (rv != 0):
				sys.exit(rv)
			
		# Owner password
		if (OWNER_PASSWORD_KEY in encryption_data[test_filename]):
			raw_password = encryption_data[test_filename][OWNER_PASSWORD_KEY]
			owner_password = normalize_string(raw_password)
			owner_specific_parameters = base_parameters
			owner_specific_parameters.append(PASSWORD_OPTION)
			owner_specific_parameters.append(owner_password)

			rv = subprocess.call(owner_specific_parameters)
			if (rv != 0):
				sys.exit(rv)
			
		sys.exit(rv)

	# Authentication using certificate
	if (CERTIFICATE_KEY in encryption_data[test_filename]):
		key = encryption_data[test_filename][CERTIFICATE_KEY]
		
		# Key may address a file local to the encryption config
		full_key_path = os.path.join(encryption_config_dir, key)

		specific_parameters = base_parameters
		specific_parameters.append(CERTIFICATE_OPTION)
		specific_parameters.append(full_key_path)

		rv = subprocess.call(specific_parameters)
		sys.exit(rv)

	# Configuration error
	sys.exit(1)

# Run test with default behavior
rv = subprocess.call(base_parameters)
sys.exit(rv)
