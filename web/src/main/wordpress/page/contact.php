<!DOCTYPE html>
<html lang="en">
  <head>
  <?php
		const PAGE_TITLE = 'Vanilla.PDF - Contact information';
		const PAGE_DESCRIPTION = "Here are the ways you can contact us with any questions you have.";
		
				// Extra scripts
		add_action('wp_enqueue_scripts', function() {
			wp_enqueue_script('js-contact', get_assets_folder() . '/js/contact.js');
			wp_enqueue_script('js-mailer', get_assets_folder() . '/js/custom_mailer.js');
		});

		// Wordpress head
		wp_head();
	?>
  </head>

  <body>


    <!-- Navbar -->
    <nav class="navbar navbar-expand-lg navbar-dark" data-navbar="fixed">
      <div class="container">

        <?php get_template_part('inc/navbar-left'); ?>

        <section class="navbar-mobile">
          <nav class="nav nav-navbar ml-auto">
            <a class="nav-link" href="#home">Home</a>
			<a class="nav-link" href="#section-options">Options</a>
            <a class="nav-link" href="#footer">References</a>
          </nav>
        </section>

      </div>
    </nav><!-- /.navbar -->


    <!-- Header -->
    <header id="home" class="header pt-10 pb-0">
      <div class="container text-center">
        <h1 class="display-4">Let's Get In Touch</h1>
        <p class="lead-2 mt-6">Help us separate the issues, so we can prioritize the most critical problems</p>
      </div>
    </header><!-- /.header -->


    <!-- Main Content -->
    <main class="main-content">
	
	  <!--
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      | Options
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      !-->
      <section id="section-options" class="section text-center bg-vanilla-light">
        <div class="container">
		  <div class="row gap-y">

            <div class="col-lg-3 d-flex">
              <a class="card card-body border text-center hover-shadow-6 text-default" href="#" onclick="GeneralQuestionClick()">
                <p class="my-5"><i class="fa fa-question lead-8 text-lighter"></i></p>
                <h5>General question</h5>
              </a>
            </div>

			<div class="col-lg-3 d-flex">
              <a class="card card-body border text-center hover-shadow-6 text-default" href="#" onclick="TemporaryLicenseClick()">
                <p class="my-5"><i class="fa fa-clock-o lead-8 text-lighter"></i></p>
                <h5>Temporary license</h5>
              </a>
            </div>


            <div class="col-lg-3 d-flex">
              <a class="card card-body border text-center hover-shadow-6 text-default" href="#" onclick="ReportBugClick()">
                <p class="my-5"><i class="fa fa-bug lead-8 text-lighter"></i></p>
                <h5>Report a bug</h5>
              </a>
            </div>


            <div class="col-lg-3 d-flex">
              <a class="card card-body border text-center hover-shadow-6 text-default" href="#" onclick="DiscountClick()">
                <p class="my-5"><i class="fa fa-percent lead-8 text-lighter"></i></p>
                <h5>Request a discount</h5>
              </a>
            </div>
          </div>
      </section>


	  <!--
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  | Contact form
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  !-->
	  <section id="section-contact" class="section">
		<div class="container">
		
		  <header class="section-header">
            <h2>Generic question</h2>
            <hr>
			<p class="lead">Let us know in case of any questions. We are here to help</p>
          </header>

		  <div class="row gap-y">
			<div class="col-md-6">

			  <form action="<?php echo get_assets_folder(); ?>/php/sendmail.php" method="POST" data-form="custom-mailer">
				<div class="alert alert-success d-on-success">We received your message and will contact you back soon.</div>

				<div class="form-group">
				  <label for="contact-name">Your name:</label>
				  <input class="form-control form-control-lg" id="contact-name" type="text" name="name" placeholder="Your name" required>
				</div>

				<div class="form-group">
				  <label for="contact-email">Your email address:</label>
				  <input class="form-control form-control-lg" id="contact-email" type="email" name="email" placeholder="Your email address" required>
				</div>

				<div class="form-group">
				  <label for="contact-message">Your message:</label>
				  <textarea class="form-control form-control-lg" id="contact-message" name="message" rows="4" placeholder="Your message" required></textarea>
				</div>
				
				<div class="text-center w-75 d-block mx-auto p-5" data-provide="recaptcha" data-callback="EnableContact">
				</div>

				<input type="hidden" name="subject" value="Generic question">
				
				<div class="text-center">
					<button id="btn-contact" class="btn btn-primary" type="submit" disabled>Send Enquiry</button>
				</div>
			  </form>

			</div>


			<!-- Contact address -->
			<?php get_template_part('inc/contact_address'); ?>
		  </div>


		</div>
	  </section>
	  
	  <!--
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  | Discount form
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  !-->
	  <section id="section-discount" class="section">
		<div class="container">
		
		  <header class="section-header">
            <h2>Request a discount</h2>
            <hr>
			<p class="lead">Our prices might be costly, but we might create a special offer tailored for you!</p>
          </header>

		  <div class="row gap-y">
			<div class="col-md-6">

			  <form action="<?php echo get_assets_folder(); ?>/php/sendmail.php" method="POST" data-form="custom-mailer">
				<div class="alert alert-success d-on-success">We received your message and will contact you back soon.</div>

				<div class="form-group">
				  <label for="discount-name">Your name:</label>
				  <input class="form-control form-control-lg" id="discount-name" type="text" name="name" placeholder="Your name (required)" required>
				</div>
				
				<div class="form-group">
				  <label for="discount-company">Your company name:</label>
				  <input class="form-control form-control-lg" id="discount-company" type="text" name="company" placeholder="Your company name">
				</div>

				<div class="form-group">
				  <label for="discount-email">Your email address:</label>
				  <input class="form-control form-control-lg" id="discount-email" type="email" name="email" placeholder="Your email address (required)" required>
				</div>
				
				<div class="form-group">
				  <label for="discount-country">Your country:</label>
				  <input class="form-control form-control-lg" id="discount-country" type="text" name="country" placeholder="Your country">
				</div>
				
				<div class="form-group">
				  <label for="discount-website">Your product website:</label>
				  <input class="form-control form-control-lg" id="discount-website" type="url" name="website" placeholder="Your product website">
				</div>
				
				<div class="form-group">
				  <label for="discount-team">What is your team size?</label>
				  <input class="form-control form-control-lg" id="discount-team" type="text" name="team" placeholder="Team size">
				</div>
				
				<div class="form-group">
				  <label for="discount-users">How many people use your product?</label>
				  <input class="form-control form-control-lg" id="discount-users" type="text" name="users" placeholder="Current users">
				</div>
				
				<div class="form-group custom-control custom-checkbox">
                  <input id="discount-banner" type="checkbox" class="custom-control-input">
                  <label for="discount-banner" class="custom-control-label">Are you willing to place our banner at your website?</label>
                </div>

				<div class="form-group">
				  <label for="discount-message">Your Message:</label>
				  <textarea class="form-control form-control-lg" id="discount-message" name="message" rows="4" placeholder="Your Message"></textarea>
				</div>
				
				<div class="text-center w-75 d-block mx-auto p-5" data-provide="recaptcha" data-callback="EnableDiscount">
				</div>

				<input type="hidden" name="subject" value="Discount request">
				
				<div class="text-center">
					<button id="btn-discount" class="btn btn-primary" type="submit" disabled>Request a discount</button>
				</div>
			  </form>

			</div>
			
			<!-- Contact address -->
			<?php get_template_part('inc/contact_address'); ?>
		  </div>


		</div>
	  </section>
	  
	  <!--
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  | Temporary license
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  !-->
	  <section id="section-license" class="section">
		<div class="container">
		
		  <header class="section-header">
            <h2>Temporary license</h2>
            <hr>
			<p class="lead">For extended evaluation purposes we can provide you a temporary license, that grants full access to the product for a limited period of time</p>
          </header>

		  <div class="row gap-y">
			<div class="col-md-6">

			  <form action="<?php echo get_assets_folder(); ?>/php/sendmail.php" method="POST" data-form="custom-mailer">
				<div class="alert alert-success d-on-success">We received your message and will contact you back soon.</div>

				<div class="form-group">
				  <label for="license-name">Your name:</label>
				  <input class="form-control form-control-lg" id="license-name" type="text" name="name" placeholder="Your name (required)" required>
				</div>
				
				<div class="form-group">
				  <label for="license-company">Your company name:</label>
				  <input class="form-control form-control-lg" id="license-company" type="text" name="company" placeholder="Your company name">
				</div>

				<div class="form-group">
				  <label for="license-email">Your email address:</label>
				  <input class="form-control form-control-lg" id="license-email" type="email" name="email" placeholder="Your email address (required)" required>
				</div>
				
				<div class="form-group">
				  <label for="license-country">Your country:</label>
				  <input class="form-control form-control-lg" id="license-country" type="text" name="country" placeholder="Your country">
				</div>

				<div class="form-group">
				  <label for="license-reason">Reason:</label>
				  <textarea class="form-control form-control-lg" id="license-reason" name="reason" rows="4" placeholder="Reason (required)" required></textarea>
				</div>
				
				<div class="text-center w-75 d-block mx-auto p-5" data-provide="recaptcha" data-callback="EnableLicense">
				</div>

				<input type="hidden" name="subject" value="Temporary license">
				
				<div class="text-center">
					<button id="btn-license" class="btn btn-primary" type="submit" disabled>Request a temporary license</button>
				</div>
			  </form>

			</div>
			
			<!-- Contact address -->
			<?php get_template_part('inc/contact_address'); ?>
		  </div>


		</div>
	  </section>
	  
	  <!--
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  | Bug report
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  !-->
	  <section id="section-report" class="section">
		<div class="container">
		
		  <header class="section-header">
            <h2>Report a bug</h2>
            <hr>
			<p class="lead">When an information is missing, obsolete or simply anything does not behave as expected, please let us know</p>
          </header>

		  <div class="row gap-y">
			<div class="col-md-6">

			  <form action="<?php echo get_assets_folder(); ?>/php/sendmail.php" method="POST" data-form="custom-mailer" enctype="multipart/form-data">
				<div class="alert alert-success d-on-success">We received your message and will contact you back soon.</div>
				
				<div class="form-group">
				  <label for="report-name">Your name:</label>
				  <input class="form-control form-control-lg" id="report-name" type="text" name="name" placeholder="Your name (required)" required>
				</div>
				
				<div class="form-group">
				  <label for="report-company">Your company name:</label>
				  <input class="form-control form-control-lg" id="report-company" type="text" name="company" placeholder="Your company name">
				</div>

				<div class="form-group">
				  <label for="report-email">Your email address:</label>
				  <input class="form-control form-control-lg" id="report-email" type="email" name="email" placeholder="Your email address (required)" required>
				</div>
				
				<div class="form-group">
				  <label for="report-version">Vanilla.PDF version:</label>
				  <input class="form-control form-control-lg" id="report-version" type="text" name="version" placeholder="Vanilla.PDF version (required)" required>
				</div>
				
				<div class="form-group">
				  <label for="report-os">Operating system:</label>
				  <input class="form-control form-control-lg" id="report-os" type="text" name="os" placeholder="Operating system (required)" required>
				</div>
				
				<div class="form-group">
				  <label for="report-lang">Programming language:</label>
				  <input class="form-control form-control-lg" id="report-lang" type="text" name="lang" placeholder="Programming language (required)" required>
				</div>
				
				<!-- Attachment -->
				
				<div class="form-group">
				  <label for="report-file">Source file:</label>
				  <input class="form-control form-control-lg" id="report-file" type="file" name="input-file" accept=".pdf">
				</div>
				
				<div class="form-group">
				  <label for="report-code">Source code to reproduce the issue:</label>
				  <textarea class="form-control form-control-lg" id="report-source" name="source" rows="4" placeholder="Source code"></textarea>
				</div>

				<div class="form-group">
				  <label for="report-message">Message:</label>
				  <textarea class="form-control form-control-lg" id="report-message" name="message" rows="4" placeholder="Your Message"></textarea>
				</div>

				<div class="text-center w-75 d-block mx-auto p-5" data-provide="recaptcha" data-callback="EnableReport">
				</div>

				<input type="hidden" name="subject" value="Bug report">
				
				<div class="text-center">
					<button id="btn-report" class="btn btn-primary" type="submit" disabled>Report a bug</button>
				</div>
			  </form>

			</div>
			
			<!-- Contact address -->
			<?php get_template_part('inc/contact_address'); ?>
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
	
	<?php
		$section = null;
		if (isset($_GET['section'])) {
			$section = $_GET['section'];
		}
		
		$is_discount = ($section == "section-discount");
		$is_report = ($section == "section-report");
		$is_license = ($section == "section-license");
		$is_contact = ($section == "section-contact") || (!$is_discount && !$is_report && !$is_license);
		
		echo '<script type="text/javascript">';
		
		if ($is_contact) {
			echo '$("#section-license").hide();';
			echo '$("#section-report").hide();';
			echo '$("#section-discount").hide();';
		}
		
		if ($is_license) {
			echo '$("#section-contact").hide();';
			echo '$("#section-report").hide();';
			echo '$("#section-discount").hide();';
		}
		
		if ($is_discount) {
			echo '$("#section-contact").hide();';
			echo '$("#section-license").hide();';
			echo '$("#section-report").hide();';
		}
		
		if ($is_report) {
			echo '$("#section-contact").hide();';
			echo '$("#section-license").hide();';
			echo '$("#section-discount").hide();';
		}
		
		echo '</script>';
	?>

  </body>
</html>
