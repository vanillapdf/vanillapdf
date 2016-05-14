#!/usr/bin/python

import platform
import datetime
import os
import subprocess
import sys
import json

USER_PASSWORD_KEY = "user_password"
OWNER_PASSWORD_KEY = "owner_password"
PASSWORD_OPTION = "-p"
CERTIFICATE_KEY = "certificate"
CERTIFICATE_OPTION = "-k"

failed_count = 0;
def process_rv( rv ):
	global failed_count
	
	if (rv == 0):
		print ("Passed!")
		logfile.write("Passed!\n")
	else:
		print ("Failed!")
		logfile.write("Failed!\n")
		failed_count += 1

#Inform the user that process has started
print ('Running test script')

# Use default release configuration
path = ""
if (len(sys.argv) < 2 or sys.argv[1] == "release"):
	path = '..\\build\\gotchangpdf.test\\release\\gotchangpdf.test.exe'
elif sys.argv[1] == "debug":
	path = '..\\build\\gotchangpdf.test\\debug\\gotchangpdf.test.exe'
else:
	print ("Incorrect mode was specified (debug/release)")
	sys.exit(1)

# All of the test files shall be in the test folder
testdir = '..\\test\\'

system = platform.system()
now = datetime.datetime.now()
logpath = '..\\log\\{}-{}-{}_{}-{}-{}_{}.log'.format(now.day, now.month, now.year, now.hour, now.minute, now.second, system)

# Open the settings for encrypted files
encryption_data = ""
with open('encryption.cfg') as encryption_config:    
	encryption_data = json.load(encryption_config)

# Open log file
logfile = open(logpath, 'w')
FNULL = open(os.devnull, 'w')

# Enumerate all test files
for root, dirs, files in os.walk(testdir):
	for file in files:
		# Skip files that does not end with .pdf
		if not file.endswith(".pdf"):
			continue
			
		print ("Found test case named {}...  ".format(file))
		logfile.write("Found test case named {}...  ".format(file))
		
		# Calculate full file path
		file_path = os.path.join(root, file)
		is_encrypted = file in encryption_data
		rv = -1
		
		# Check if filename is in our encrypted configuration file
		if (is_encrypted):
			# Authentication using user or owner password
			if (USER_PASSWORD_KEY in encryption_data[file] or OWNER_PASSWORD_KEY in encryption_data[file]):
				both_keys = USER_PASSWORD_KEY in encryption_data[file] and OWNER_PASSWORD_KEY in encryption_data[file]
				
				# User password
				if (USER_PASSWORD_KEY in encryption_data[file]):
					logfile.write(os.linesep)
					logfile.write("Using user password...  ")
					print ("Using user password...  ")
					user_password = encryption_data[file][USER_PASSWORD_KEY]
					rv = subprocess.call([path, file_path, PASSWORD_OPTION, user_password], stdout=FNULL)
					process_rv(rv)
					
				# Owner password
				if (OWNER_PASSWORD_KEY in encryption_data[file]):
					if not (both_keys):
						logfile.write(os.linesep)
						
					logfile.write("Using owner password...  ")
					print ("Using owner password...  ")
					owner_password = encryption_data[file][OWNER_PASSWORD_KEY]
					rv = subprocess.call([path, file_path, PASSWORD_OPTION, owner_password], stdout=FNULL)
					process_rv(rv)
					
				continue
					
			# Authentication using certificate
			if (CERTIFICATE_KEY in encryption_data[file]):
				logfile.write(os.linesep)
				logfile.write("Using certificate key...  ")
				print ("Using certificate key...  ")
				key = encryption_data[file][CERTIFICATE_KEY]
				rv = subprocess.call([path, file_path, CERTIFICATE_OPTION, key], stdout=FNULL)
				process_rv(rv)
				continue;
				
			print ("Configuration error for file {}".format(file))
			logfile.write("Configuration error for file {}".format(file))
			continue
			
		# Run test with default behavior
		rv = subprocess.call([path, file_path], stdout=FNULL)
		process_rv(rv)
	
# Log if everything has finished successfully
if (failed_count == 0):
	print ("All tests have finished successfully");
	logfile.write("All tests have finished successfully")
else:
	print ("{} test scenarios failed".format(failed_count));
	logfile.write("{} test scenarios failed".format(failed_count))
			
# Cleanup
logfile.close()

# Return success
sys.exit(0)