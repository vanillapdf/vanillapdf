<?php
/**
 */
 
function get_assets_folder() {
	return get_template_directory_uri() . '/assets';
}

function get_stylesheet_folder() {
	return get_assets_folder() . '/css';
}

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
	echo '<link rel="apple-touch-icon" sizes="180x180" href="' . get_assets_folder() . '/img/apple-touch-icon.png">' . "\n";
	echo '<link rel="icon" type="image/png" sizes="16x16" href="' . get_assets_folder() . '/img/favicon-16x16.png">' . "\n";
	echo '<link rel="icon" type="image/png" sizes="32x32" href="' . get_assets_folder() . '/img/favicon-32x32.png">' . "\n";
	echo '<link rel="shortcut icon" href="' . get_assets_folder() . '/img/favicon.ico">' . "\n";
}

add_action('wp_head', 'vanillapdf_favicons');
