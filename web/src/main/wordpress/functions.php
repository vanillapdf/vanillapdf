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

const PRICE_CURRENCY_SYMBOL = '&euro;';

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
	$title = get_the_title();
	
	echo '<!-- Meta tags -->' . "\n";
	echo '<meta charset="utf-8">' . "\n";
	echo '<meta name="author" content="Vanilla.PDF Labs">' . "\n";
	echo '<meta name="robots" content="index,follow">' . "\n";
	echo '<meta name="description" content="{{ DESCRIPTION }}">' . "\n";
	echo '<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">' . "\n";

	echo '<title>' . $title . '</title>' . "\n";

	echo '<meta property="og:title" content="{{ TITLE }}">' . "\n";
	echo '<meta property="og:description" content="{{ DESCRIPTION }}">' . "\n";
	echo '<meta property="og:type" content="website">' . "\n";
	echo '<meta property="og:site_name" content="{{ TITLE }}">' . "\n";

	echo '<meta name="apple-mobile-web-app-title" content="{{ TITLE }}">' . "\n";
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
