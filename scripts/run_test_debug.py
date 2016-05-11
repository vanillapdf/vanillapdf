#!/usr/bin/python

import platform
import datetime
import os
import subprocess
import sys
import json

PASSWORD_KEY = "password"
PASSWORD_OPTION = "-p"
CERTIFICATE_KEY = "certificate"
CERTIFICATE_OPTION = "-k"

print ('Running test script')

path = ""
if len(sys.argv) < 2:
	print ("No mode was specified (debug/release)")
	sys.exit(1)
	
if sys.argv[1] == "debug":
	path = '..\\build\\gotchangpdf.test\\debug\\gotchangpdf.test.exe'
elif sys.argv[1] == "release":
	path = '..\\build\\gotchangpdf.test\\release\\gotchangpdf.test.exe'
else:
	print ("Incorrect mode was specified (debug/release)")
	sys.exit(1)

testdir = '..\\test\\'

system = platform.system()
now = datetime.datetime.now()
logpath = '..\\log\\{}-{}-{}_{}-{}-{}_{}.log'.format(now.day, now.month, now.year, now.hour, now.minute, now.second, system)

encryption_data = ""
with open('encryption.cfg') as encryption_config:    
	encryption_data = json.load(encryption_config)

# Open log file
logfile = open(logpath, 'w')
FNULL = open(os.devnull, 'w')
failed_count = 0;

for root, dirs, files in os.walk(testdir):
	for file in files:
		if not file.endswith(".pdf"):
			continue
			
		print ("Found test case named {}...  ".format(file))
		logfile.write("Found test case named {}...  ".format(file))
		
		file_path = os.path.join(root, file)
		is_encrypted = file in encryption_data
		rv = -1
		if (is_encrypted):
			if (PASSWORD_KEY in encryption_data[file]):
				password = encryption_data[file][PASSWORD_KEY]
				rv = subprocess.call([path, file_path, PASSWORD_OPTION, password], stdout=FNULL)
			elif (CERTIFICATE_KEY in encryption_data[file]):
				key = encryption_data[file][CERTIFICATE_KEY]
				rv = subprocess.call([path, file_path, CERTIFICATE_OPTION, key], stdout=FNULL)
			else:
				print ("Configuration error for file {}".format(file))
				logfile.write("Configuration error for file {}".format(file))
				continue
		else:
			rv = subprocess.call([path, file_path], stdout=FNULL)
			
		if (rv == 0):
			print ("Passed!")
			logfile.write("Passed!\n")
		else:
			print ("Failed!")
			logfile.write("Failed!\n")
			failed_count += 1
		
if (failed_count == 0):
	print ("All tests have finished successfully");
	logfile.write("All tests have finished successfully")
else:
	print ("{} test scenarios failed".format(failed_count));
	logfile.write("{} test scenarios failed".format(failed_count))
			
logfile.close()
sys.exit(0)