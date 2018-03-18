#!/usr/bin/python

# Imports
import os
import jinja2

# Constants
source_folder = 'templates'
destination_folder = 'build'
pages_folder = os.path.join(destination_folder, 'page')

context = {
	'current_year': ' '
}

files = {
	'index.html': destination_folder,
	'about.html': pages_folder,
	'contact.html': pages_folder,
	'download.html': pages_folder,
	'faq.html': pages_folder
}

# Functions
def write_file(path, content):
	directory = os.path.dirname(path)
	if not os.path.exists(directory):
		os.makedirs(directory)
		
	with open(path, 'wb+') as file:
		file.write(content)

def read_file(path):
	contents = ""
	with open(path) as file:
		for line in file.readlines():
			contents += line

	return contents

def render_template(template_path, context):
	path, filename = os.path.split(template_path)
	environment = jinja2.Environment(loader = jinja2.FileSystemLoader(path))
	template = environment.get_template(filename)
	return template.render(context)

# Actual script
for (filename, destination_folder) in files.items():
	source_path = os.path.join(source_folder, filename)
	destination_path = os.path.join(destination_folder, filename)
	
	content = render_template(source_path, context)
	content_encoded = content.encode('utf-8')
	write_file(destination_path, content_encoded)
