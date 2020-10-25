#!/usr/bin/python

import tarfile
import argparse
import os.path

def extract_file(file, destination):
    if file.endswith("tar.gz"):
        tar = tarfile.open(file, "r:gz")
        tar.extractall(destination)
        tar.close()
    elif file.endswith("tar"):
        tar = tarfile.open(file, "r:")
        tar.extractall(destination)
        tar.close()
    else:
        print("Unknown compression filter")
        sys.exit(-1)


# Parse input arguments
parser = argparse.ArgumentParser(description='Extract compressed build packages into build folder according to target architecture.')
parser.add_argument('build_version', help='Software version of the current release')
parser.add_argument('build_folder', help='Folder containing the extracted build files')
parser.add_argument('archives_folder', help='Folder containing the compressed archive files')

args = parser.parse_args()

# Check for files in list in archives

archive_prefix = "vanillapdf_"

archives_suffix = [
    "_i386.tar.gz",
    "_amd64.tar.gz"
]

for suffix in archives_suffix:
    filename = archive_prefix + args.build_version + suffix
    print ("Checking file: " + filename)

    archive_path = os.path.join(args.archives_folder, filename)
    if not os.path.exists(archive_path):
        sys.exit(-1)

    print ("File exists, extracting")
    extract_file(archive_path, args.build_folder)
