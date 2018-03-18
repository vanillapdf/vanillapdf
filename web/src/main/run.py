#!/usr/bin/python

# Imports
import os
import jinja2

# Constants
source_folder = 'templates'
include_folder = 'include'
build_folder = 'build'
pages_folder = os.path.join(build_folder, 'page')

generic_context = {
	'index_file': '../index.html',
	'root_folder': '../',
	'pages_folder': '',
	'doc_folder': '../doc/',
	'assets_folder': '../assets/'
}

index_file = 'index.html'

files = [
	'about.html',
	'contact.html',
	'download.html',
	'faq.html'
]

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
	environment = jinja2.Environment(
		loader = jinja2.FileSystemLoader([source_folder, include_folder]),
		keep_trailing_newline = True
	)
		
	template = environment.get_template(filename)
	return template.render(context)

def generate_template(source_path, destination_path, context):
	content = render_template(source_path, context)
	content_encoded = content.encode('utf-8')
	write_file(destination_path, content_encoded)

# Actual script
for filename in files:
	source_path = os.path.join(source_folder, filename)
	destination_path = os.path.join(pages_folder, filename)
	
	generate_template(source_path, destination_path, generic_context)

# Generate the index file
index_source_path = os.path.join(source_folder, index_file)
index_destination_path = os.path.join(build_folder, index_file)

# Index file has special context
index_context = dict(generic_context)
index_context['index_file'] = ''
index_context['root_folder'] = ''
index_context['assets_folder'] = 'assets/'
index_context['pages_folder'] = 'page/'
index_context['doc_folder'] = 'doc/'

# Generate the index template
generate_template(index_source_path, index_destination_path, index_context)
