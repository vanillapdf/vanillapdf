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
	'versions_folder': '/versions/',
	'assets_folder': '../assets/',
	'contact_page': 'contact.php',
	'about_page': 'about.php',
	'download_page': 'download.html',
	'faq_page': 'faq.html',
	'order_page': 'order.php',
	'checkout_page': 'checkout.php',
	'licenses_page': 'licenses.html',
	
	'LATEST_VERSION': '1.0.0',
	'PREVIOUS_VERSION': '0.0.0',
	
	'CURRENCY': '&euro;',
	'TAX_PERCENTAGE': 19,
	
	'PERSONAL_LICENSE_DESCRIPTION':				'Vanilla.PDF Personal license',
	'COMMERCIAL_LICENSE_DESCRIPTION':			'Vanilla.PDF Commercial OEM license',
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
	
	'INDEX_TITLE': 'Vanilla.PDF - SDK for creating and modifying PDF documents',
	'ABOUT_TITLE': 'Vanilla.PDF - About',
	'CONTACT_TITLE': 'Vanilla.PDF - Contact information',
	'DOWNLOAD_TITLE': 'Vanilla.PDF - Download',
	'FAQ_TITLE': 'Vanilla.PDF - Frequently asked questions',
	'ORDER_TITLE': 'Vanilla.PDF - Order',
	'CHECKOUT_TITLE': 'Vanilla.PDF - Checkout',
	'LICENSES_TITLE': 'Vanilla.PDF - Licenses',
	'404_TITLE': 'Vanilla.PDF - Page not found',
	
	'INDEX_DESCRIPTION': 'Integration is very easy with pre-packaged binaries for multiple operating systems such as Windows, Linux and Mac.',
	'ABOUT_DESCRIPTION': 'Brief bio about company history, project aim and future expansion.',
	'CONTACT_DESCRIPTION': 'Here are the ways you can contact us with any questions you have.',
	'DOWNLOAD_DESCRIPTION': 'Download the latest version.',
	'FAQ_DESCRIPTION': "Got a question? We've got answers. If you have some other questions, contact us using email.",
	'ORDER_DESCRIPTION': 'Choose your favorite product and fill the billing details details.',
	'CHECKOUT_DESCRIPTION': 'Review your billing address before submitting the request.',
	'LICENSES_DESCRIPTION': 'Terms and conditions are a set of rules and guidelines, that a user must agree to in order to use the website.',
	'404_DESCRIPTION': "Seems you're looking for something that doesn't exist.",
}

index_file = 'index.html'

page_files = [
	'about.php',
	'contact.php',
	'download.html',
	'faq.html',
	'order.php',
	'checkout.php',
	'licenses.html',
	'404.html'
]

js_files = [
	'price_constants.js',
	'custom_mailer.js',
	'checkout.js',
	'contact.js'
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
	
	file_basename = os.path.splitext(filename)[0]
	title_key = file_basename.upper() + '_TITLE'
	title_value = generic_context[title_key]
	
	description_key = file_basename.upper() + '_DESCRIPTION'
	description_value = generic_context[description_key]
	
	local_context = dict(generic_context)
	local_context['TITLE'] = title_value
	local_context['DESCRIPTION'] = description_value
	
	generate_template(source_path, destination_path, local_context)
	
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

index_context['TITLE'] = generic_context['INDEX_TITLE']
index_context['DESCRIPTION'] = generic_context['INDEX_DESCRIPTION']

# Generate the index template
generate_template(index_source_path, index_destination_path, index_context)

# Print success message
print('Templates were generated successfully')
