<?php
/**
 */

const LATEST_VERSION = '0.0.1-alpha';

const PAGE_DOWNLOAD = 'download.html';
const PAGE_CONTACT = 'contact.php';
const PAGE_ABOUT = 'about.php';
const PAGE_LICENSES = 'licenses.php';
const PAGE_FAQ = 'faq.php';
const PAGE_ORDER = 'order.php';
const PAGE_INDEX = 'index.php';

const TAX_PERCENTAGE = 19;
const PRICE_CURRENCY_SYMBOL = '&euro;';

const PERSONAL_LICENSE_DESCRIPTION =		'Vanilla.PDF Personal license';
const COMMERCIAL_LICENSE_DESCRIPTION =		'Vanilla.PDF Commercial OEM license';
const PERSONAL_SUPPORT_DESCRIPTION =		'Vanilla.PDF Extended personal support';
const COMMERCIAL_SUPPORT_DESCRIPTION =		'Vanilla.PDF Extended commercial support';

const PERSONAL_LICENSE_PRICE =				10 - 1;
const PERSONAL_ONE_YEAR_SUPPORT_PRICE =		40 - 1;
const PERSONAL_TWO_YEAR_SUPPORT_PRICE =		2 * (24 - 1);
const PERSONAL_THREE_YEAR_SUPPORT_PRICE =	3 * (19 - 1);
const COMMERCIAL_LICENSE_PRICE =			110 - 1;
const COMMERCIAL_ONE_YEAR_SUPPORT_PRICE =	300 - 1;
const COMMERCIAL_TWO_YEAR_SUPPORT_PRICE =	2 * (180 - 1);
const COMMERCIAL_THREE_YEAR_SUPPORT_PRICE =	3 * (140 - 1);

function get_assets_folder() {
	return get_template_directory_uri() . '/assets';
}

function get_pages_folder() {
	return get_template_directory_uri() . '/page';
}

function get_versions_folder() {
	return get_template_directory_uri() . '/versions';
}

function get_stylesheet_folder() {
	return get_assets_folder() . '/css';
}

function vanillapdf_setup() {
	/*
	 * Let WordPress manage the document title.
	 * By adding theme support, we declare that this theme does not use a
	 * hard-coded <title> tag in the document head, and expect WordPress to
	 * provide it for us.
	 */
	// add_theme_support( 'title-tag' );

}

add_action( 'after_setup_theme', 'vanillapdf_setup' );

function vanillapdf_scripts() {

	// Theme stylesheet.
	wp_enqueue_style( 'template-meta', get_stylesheet_uri() );
	
	// Add Genericons, used in the main stylesheet.
	wp_enqueue_style( 'template-page', get_stylesheet_folder() . '/page.min.css');
	wp_enqueue_style( 'template-style', get_stylesheet_folder() . '/style.min.css');
	wp_enqueue_style( 'template-custom', get_stylesheet_folder() . '/custom.css');
	
	wp_enqueue_script('js-page', get_assets_folder() . '/js/page.min.js');
	wp_enqueue_script('js-script', get_assets_folder() . '/js/script.js');

}
add_action( 'wp_enqueue_scripts', 'vanillapdf_scripts' );

function vanillapdf_favicons() {
	echo '<!-- Favicons -->' . "\n";
	echo '<link rel="apple-touch-icon" sizes="180x180" href="' . get_assets_folder() . '/img/apple-touch-icon.png">' . "\n";
	echo '<link rel="icon" type="image/png" sizes="16x16" href="' . get_assets_folder() . '/img/favicon-16x16.png">' . "\n";
	echo '<link rel="icon" type="image/png" sizes="32x32" href="' . get_assets_folder() . '/img/favicon-32x32.png">' . "\n";
	echo '<link rel="shortcut icon" href="' . get_assets_folder() . '/img/favicon.ico">' . "\n";
}

add_action('wp_head', 'vanillapdf_favicons');

function vanillapdf_meta() {
	$title = PAGE_TITLE;
	$description = PAGE_DESCRIPTION;
	
	echo '<!-- Meta tags -->' . "\n";
	echo '<meta charset="utf-8">' . "\n";
	echo '<meta name="author" content="Vanilla.PDF Labs">' . "\n";
	echo '<meta name="robots" content="index,follow">' . "\n";
	echo '<meta name="description" content="' . $description . '">' . "\n";
	echo '<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">' . "\n";

	echo '<title>' . $title . '</title>' . "\n";

	echo '<meta property="og:title" content="' . $title . '">' . "\n";
	echo '<meta property="og:description" content="' . $description . '">' . "\n";
	echo '<meta property="og:type" content="website">' . "\n";
	echo '<meta property="og:site_name" content="' . $title . '">' . "\n";

	echo '<meta name="apple-mobile-web-app-title" content="' . $title . '">' . "\n";
	echo '<meta name="apple-mobile-web-app-capable" content="yes">' . "\n";
	echo '<meta name="mobile-web-app-capable" content="yes">' . "\n";

	//echo '<link rel="canonical" href="{{ BASE_URL }}{{ FILENAME }}" />' . "\n";

}

add_action('wp_head', 'vanillapdf_meta');

function vanillapdf_google_analytics() {
	echo '<!-- Global site tag (gtag.js) - Google Analytics -->' . "\n";
	echo '<script async src="https://www.googletagmanager.com/gtag/js?id=UA-106797397-1"></script>' . "\n";
	echo '<script>' . "\n";
	echo 'window.dataLayer = window.dataLayer || [];' . "\n";
	echo 'function gtag(){dataLayer.push(arguments);}' . "\n";
	echo "gtag('js', new Date());" . "\n";
	echo "gtag('config', 'UA-106797397-1');" . "\n";
	echo '</script>' . "\n";

}

add_action('wp_head', 'vanillapdf_google_analytics');
