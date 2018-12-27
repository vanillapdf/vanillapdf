<?php
/**
 * Template Name: Licenses
 
 * @package WordPress
 * @subpackage Vanilla.PDF
 * @since Vanilla.PDF 1.0
 */
?>

<!DOCTYPE html>
<html lang="en">
  <head>
  <?php
		const PAGE_TITLE = 'Vanilla.PDF - Licenses';
		const PAGE_DESCRIPTION = "Terms and conditions are a set of rules and guidelines, that a user must agree to in order to use the website.";

		// Wordpress head
		wp_head();
	?>
  </head>

  <body>

    <?php do_action('vanillapdf_body_begin'); ?>

    <!-- Navbar -->
    <nav class="navbar navbar-expand-lg navbar-dark" data-navbar="fixed">
      <div class="container">

        <?php get_template_part('inc/navbar-left'); ?>

        <section class="navbar-mobile">
          <nav class="nav nav-navbar ml-auto">
            <a class="nav-link" href="#home">Home</a>
            <a class="nav-link" href="#section-eula">EULA</a>
			<a class="nav-link" href="#section-terms">EULA</a>
            <a class="nav-link" href="#footer">References</a>
          </nav>
        </section>

      </div>
    </nav><!-- /.navbar -->


    <!-- Header -->
    <header id="home" class="header">
      <div class="container text-center">

        <div class="row">
          <div class="col-md-8 mx-auto">

            <h1>Licenses</h1>
            <p class="lead-2 opacity-90 mt-6">Last update: April 30, 2018</p>

          </div>
        </div>

      </div>
    </header><!-- /.header -->


    <!-- Main Content -->
    <main class="main-content">

      <section id="section-eula" class="section h-fullscreen">
        <div class="container">
		  <header class="section-header">
            <h2>End User License Agreement</h2>
          </header>
		  
          <div class="row h-90">
            <div class="col-md-10 mx-auto">
			
			  <object data="<?php echo get_assets_folder(); ?>/data/EULA.pdf" type="application/pdf" width="100%" height="100%">
			    <p><b>Example fallback content</b>: This browser does not support PDFs. Please download the PDF to view it: <a href="<?php echo get_assets_folder(); ?>/data/EULA.pdf">Download PDF</a>.</p>
			  </object>
            </div>
          </div>
        </div>
      </section>
	  
	  <section id="section-terms" class="section h-fullscreen">
        <div class="container">
		  <header class="section-header">
            <h2>Terms of use</h2>
          </header>
          <div class="row h-90">
            <div class="col-md-10 mx-auto">
			
			  <object data="<?php echo get_assets_folder(); ?>/data/terms_of_use.pdf" type="application/pdf" width="100%" height="100%">
			    <p><b>Example fallback content</b>: This browser does not support PDFs. Please download the PDF to view it: <a href="<?php echo get_assets_folder(); ?>/data/terms_of_use.pdf">Download PDF</a>.</p>
			  </object>
            </div>
          </div>
        </div>
      </section>

    </main><!-- /.main-content -->


	<!-- Footer -->
	<footer id="footer" class="footer py-7">
		<?php get_template_part( 'inc/footer' ); ?>
	</footer><!-- /.footer -->


    <!-- Scripts -->
	<?php wp_footer(); ?>

  </body>
</html>
