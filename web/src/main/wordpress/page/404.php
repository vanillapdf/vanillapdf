<!DOCTYPE html>
<html lang="en">
  <head>
  <?php
		const PAGE_TITLE = 'Vanilla.PDF - Page not found';
		const PAGE_DESCRIPTION = "Seems you're looking for something that doesn't exist.";

		// Wordpress head
		wp_head();
	?>
  </head>

  <body class="layout-centered bg-gray">


    <!-- Main Content -->
    <main class="main-content text-center pb-lg-8">
      <div class="container">

        <h1 class="display-1 text-muted mb-7">Page Not Found</h1>
        <p class="lead">Seems you're looking for something that doesn't exist.</p>
        <br>
        <button class="btn btn-secondary w-150 mr-2" type="button" onclick="window.history.back();">Go back</button>
        <a class="btn btn-secondary w-150" href="<?php echo get_home_url(); ?>">Return Home</a>

      </div>
    </main><!-- /.main-content -->

	<!-- Scripts -->
	<?php wp_footer(); ?>

  </body>
</html>
