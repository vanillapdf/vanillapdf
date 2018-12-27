<?php
/**
 * Template Name: Download
 
 * @package WordPress
 * @subpackage Vanilla.PDF
 * @since Vanilla.PDF 1.0
 */
?>

<!DOCTYPE html>
<html lang="en">
  <head>
  <?php
		const PAGE_TITLE = 'Vanilla.PDF - Download';
		const PAGE_DESCRIPTION = "Download the latest version.";

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
            <a class="nav-link" href="#section-download">Download</a>
            <a class="nav-link" href="#footer">References</a>
          </nav>
        </section>

      </div>
    </nav><!-- /.navbar -->


    <!-- Header -->
    <header id="home" class="header pt-10 pb-0">
      <div class="container text-center">
        <h1 class="display-4">Downloads</h1>
        <p class="lead-2 mt-6">Choose build for your platform</p>
      </div>
    </header><!-- /.header -->


    <!-- Main Content -->
    <main class="main-content">

      <!--
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      | Download
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      !-->
      <section id="section-download" class="section">
        <div class="container">
		
		  <header class="section-header">
			<h3 style="font-family: inherit;">Latest release (<?php echo LATEST_VERSION; ?>)</h3>
		  </header>
		
		  <div class="row">
		  
		  <?php get_template_part('inc/notes/' . LATEST_VERSION); ?>
		  
		  <table class="table table-bordered table-hover">
			<thead>
			  <tr class="bg-gray">
				<th>Platform</th>
				<th>Filename</th>
				<th>Hash</th>
			  </tr>
			</thead>
			<tbody>
			  <tr>
				<td>Windows win64-x64 Installer</td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_win64-x64.msi">vanillapdf_<?php echo LATEST_VERSION; ?>_win64-x64.msi</a></td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_win64-x64.msi.sha256">vanillapdf_<?php echo LATEST_VERSION; ?>_win64-x64.msi.sha256</a></td>
			  </tr>
			  <tr>
				<td>Windows win64-x64 ZIP</td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_win64-x64.zip">vanillapdf_<?php echo LATEST_VERSION; ?>_win64-x64.zip</a></td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_win64-x64.zip.sha256">vanillapdf_<?php echo LATEST_VERSION; ?>_win64-x64.zip.sha256</a></td>
			  </tr>
			  <tr>
				<td>Windows win32-x86 Installer</td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_win32-x86.msi">vanillapdf_<?php echo LATEST_VERSION; ?>_win32-x86.msi</a></td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_win32-x86.msi.sha256">vanillapdf_<?php echo LATEST_VERSION; ?>_win32-x86.msi.sha256</a></td>
			  </tr>
			  <tr>
				<td>Windows win32-x86 ZIP</td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_win32-x86.zip">vanillapdf_<?php echo LATEST_VERSION; ?>_win32-x86.zip</a></td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_win32-x86.zip.sha256">vanillapdf_<?php echo LATEST_VERSION; ?>_win32-x86.zip.sha256</a></td>
			  </tr>
			  <tr>
				<td>Linux amd64 debian</td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_amd64.deb">vanillapdf_<?php echo LATEST_VERSION; ?>_amd64.deb</a></td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_amd64.deb.sha256">vanillapdf_<?php echo LATEST_VERSION; ?>_amd64.deb.sha256</a></td>
			  </tr>
			  <tr>
				<td>Linux amd64 tarball</td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_amd64.tar.gz">vanillapdf_<?php echo LATEST_VERSION; ?>_amd64.tar.gz</a></td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_amd64.tar.gz.sha256">vanillapdf_<?php echo LATEST_VERSION; ?>_amd64.tar.gz.sha256</a></td>
			  </tr>
			  <tr>
				<td>Linux i386 debian</td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_i386.deb">vanillapdf_<?php echo LATEST_VERSION; ?>_i386.deb</a></td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_i386.deb.sha256">vanillapdf_<?php echo LATEST_VERSION; ?>_i386.deb.sha256</a></td>
			  </tr>
			  <tr>
				<td>Linux i386 tarball</td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_i386.tar.gz">vanillapdf_<?php echo LATEST_VERSION; ?>_i386.tar.gz</a></td>
				<td><a href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/build/vanillapdf_<?php echo LATEST_VERSION; ?>_i386.tar.gz.sha256">vanillapdf_<?php echo LATEST_VERSION; ?>_i386.tar.gz.sha256</a></td>
			  </tr>
			</tbody>
		  </table>
		  
		  <p>For more information visit full <a id="latest-doc" href="<?php echo get_versions_folder() . '/' . LATEST_VERSION; ?>/doc/index.html">API documentation</a>.</p>
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
