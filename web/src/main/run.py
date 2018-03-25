#!/usr/bin/python

# Imports
import os
import jinja2

# Constants
source_folder = 'templates'
include_folder = 'include'
build_folder = 'build'
pages_folder = os.path.join(build_folder, 'page')
assets_folder = os.path.join(build_folder, 'assets')
js_folder = os.path.join(assets_folder, 'js')

PERSONAL_LICENSE_PRICE =				300 - 1;
PERSONAL_ONE_YEAR_SUPPORT_PRICE =		200 - 1;
PERSONAL_TWO_YEAR_SUPPORT_PRICE =		2 * (175 - 1);
PERSONAL_THREE_YEAR_SUPPORT_PRICE =		3 * (150 - 1);
COMMERCIAL_LICENSE_PRICE =				1500 - 1;
COMMERCIAL_ONE_YEAR_SUPPORT_PRICE =		500 - 1;
COMMERCIAL_TWO_YEAR_SUPPORT_PRICE =		2 * (450 - 1);
COMMERCIAL_THREE_YEAR_SUPPORT_PRICE =	3 * (400 - 1);

generic_context = {
	'index_file': '../index.html',
	'root_folder': '../',
	'pages_folder': '',
	'doc_folder': '../doc/',
	'assets_folder': '../assets/',
	'contact_page': 'contact.html',
	'about_page': 'about.html',
	'download_page': 'download.html',
	'faq_page': 'faq.html',
	'order_page': 'order.php',
	'checkout_page': 'checkout.php',
	
	'CURRENCY': '&euro;',
	'TAX_PERCENTAGE': 19,
	
	'PERSONAL_LICENSE_DESCRIPTION':				'Vanilla.PDF Personal license',
	'COMMERCIAL_LICENSE_DESCRIPTION':			'Vanilla.PDF Commercial license',
	'PERSONAL_SUPPORT_DESCRIPTION':				'Vanilla.PDF Extended personal support',
	'COMMERCIAL_SUPPORT_DESCRIPTION':			'Vanilla.PDF Extended commercial support',
	
	'PERSONAL_LICENSE_PRICE':					PERSONAL_LICENSE_PRICE,
	'PERSONAL_ONE_YEAR_SUPPORT_PRICE':			PERSONAL_ONE_YEAR_SUPPORT_PRICE,
	'PERSONAL_TWO_YEAR_SUPPORT_PRICE':			PERSONAL_TWO_YEAR_SUPPORT_PRICE,
	'PERSONAL_THREE_YEAR_SUPPORT_PRICE':		PERSONAL_THREE_YEAR_SUPPORT_PRICE,
	'COMMERCIAL_LICENSE_PRICE':					COMMERCIAL_LICENSE_PRICE,
	'COMMERCIAL_ONE_YEAR_SUPPORT_PRICE':		COMMERCIAL_ONE_YEAR_SUPPORT_PRICE,
	'COMMERCIAL_TWO_YEAR_SUPPORT_PRICE':		COMMERCIAL_TWO_YEAR_SUPPORT_PRICE,
	'COMMERCIAL_THREE_YEAR_SUPPORT_PRICE':		COMMERCIAL_THREE_YEAR_SUPPORT_PRICE,
	'PERSONAL_LICENSE_WITH_SUPPORT_PRICE':		PERSONAL_LICENSE_PRICE + PERSONAL_ONE_YEAR_SUPPORT_PRICE,
	'COMMERCIAL_LICENSE_WITH_SUPPORT_PRICE':	COMMERCIAL_LICENSE_PRICE + COMMERCIAL_ONE_YEAR_SUPPORT_PRICE,
}

index_file = 'index.html'

page_files = [
	'about.html',
	'contact.html',
	'download.html',
	'faq.html',
	'order.php',
	'checkout.php'
]

js_files = [
	'price_constants.js',
	'pricing.js',
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

#################### Actual script ####################

# Process pages
for filename in page_files:
	source_path = os.path.join(source_folder, filename)
	destination_path = os.path.join(pages_folder, filename)
	
	generate_template(source_path, destination_path, generic_context)
	
# Process js
for filename in js_files:
	source_path = os.path.join(source_folder, filename)
	destination_path = os.path.join(js_folder, filename)
	
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

# Print success message
print('Templates were generated successfully')
