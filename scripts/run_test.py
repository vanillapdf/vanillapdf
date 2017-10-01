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

def normalize_string( str ):
	normalized = unicodedata.normalize('NFC', str)
	encoded = normalized.encode('utf8')
	return encoded.decode('latin-1')

if (len(sys.argv) < 5):
	print ("Incorrect number of arguments!")
	print ("Usage: executable_path encryption_config test_file_path")
	sys.exit(1)

# Initialize return value
rv = -1

# Parse command line arguments
executable_path = sys.argv[1]
test_file_path = sys.argv[2]
encryption_config_path = sys.argv[3]
license_file_path = sys.argv[4]

encryption_config_dir = os.path.dirname(encryption_config_path)
test_filename = ntpath.basename(test_file_path)

# Open the settings for encrypted files
encryption_data = ""
with io.open(encryption_config_path, encoding='utf8') as encryption_config:
	encryption_data = json.load(encryption_config)

# Create devnull for output of test case
FNULL = open(os.devnull, 'w')

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
			rv = subprocess.call([executable_path, test_file_path, PASSWORD_OPTION, user_password, LICENSE_OPTION, license_file_path], stdout=FNULL)
			if (rv != 0):
				sys.exit(rv)
			
		# Owner password
		if (OWNER_PASSWORD_KEY in encryption_data[test_filename]):
			raw_password = encryption_data[test_filename][OWNER_PASSWORD_KEY]
			owner_password = normalize_string(raw_password)
			rv = subprocess.call([executable_path, test_file_path, PASSWORD_OPTION, owner_password, LICENSE_OPTION, license_file_path], stdout=FNULL)
			if (rv != 0):
				sys.exit(rv)
			
		sys.exit(rv)

	# Authentication using certificate
	if (CERTIFICATE_KEY in encryption_data[test_filename]):
		key = encryption_data[test_filename][CERTIFICATE_KEY]
		
		# Key may address a file local to the encryption config
		full_key_path = os.path.join(encryption_config_dir, key)
		rv = subprocess.call([executable_path, test_file_path, CERTIFICATE_OPTION, full_key_path, LICENSE_OPTION, license_file_path], stdout=FNULL)
		sys.exit(rv)

	# Configuration error
	sys.exit(1)

# Run test with default behavior
rv = subprocess.call([executable_path, test_file_path, LICENSE_OPTION, license_file_path], stdout=FNULL)
sys.exit(rv)
