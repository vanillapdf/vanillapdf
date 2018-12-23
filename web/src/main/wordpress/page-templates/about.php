<?php
/**
 * Template Name: About
 
 * @package WordPress
 * @subpackage Vanilla.PDF
 * @since Vanilla.PDF 1.0
 */
?>

<!DOCTYPE html>
<html lang="en">
  <head>
  <?php
		const PAGE_TITLE = 'Vanilla.PDF - About';
		const PAGE_DESCRIPTION = 'Brief bio about company history, project aim and future expansion.';

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
		    <img src="<?php echo get_assets_folder(); ?>/img/logo/vanilla-gold-large.png" alt="Vanilla.PDF gold large" width="450">
            <p class="lead-2 mt-6">Brief bio about company history, project aim and future expansion</p>
		  </div>
		</div>
      </div>
    </header><!-- /.header -->


    <!-- Main Content -->
    <main class="main-content">
	
	  <!--
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      | Our History
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      !-->
      <section id="section-company" class="section bg-vanilla-light">
      <div class="container">
        <div class="row">
          <div class="col-md-10 mx-auto">
		    <h2>Our History</h2>
			<p>This project started somewhere in the late 2013 as a hobby by our supreme leader <a href="mailto:jzikmund@vanillapdf.com">Juraj Zikmund</a>.</p>
			
			<blockquote class="blockquote lead-1 bg-white border p-5">
              <p>The reason I finally decided to create my own tool, was that all of the other PDF toolkits were either insufficient or just way too expensive. It was a clear sign there is a room for improvement.</p>
			  <p>It was a lot of work and sleepless nights in addition to a full-time job, so the progress took far more than it should. After a year or two I was able to create basic tool to fix broken files<br>- at that moment I've realized <strong>"Dude! This thing works!"</strong>.</p>
			  <p>I didn't want to keep a working tool just for myself, so I've started to very slowly shape it towards a public release. The hardest decision was how and when release the project. A lot of effort was put into guides, tutorial, API reference, website to deliver a high-quality product, so I hope it will serve a purpose.</p>
              <footer>Juraj Zikmund</footer>
            </blockquote>
			
            <p>Few years later in 2018 we founded Vanilla.PDF Labs with a mission to release, enhance and support this project.</p>
        </div>
      </div>
      </section>
	
	  <!--
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      | Our Company
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      !-->
      <section id="section-company" class="section">
      <div class="container">
        <div class="row gap-y">
          <div class="col-md-6">
		    <h2>Our company</h2>
			<p>The first tough choice was to pick short and accurate product name. After a months of struggling we come up with the <strong>Vanilla.PDF</strong> for two reasons.</p>
			<ul>
				<li>It is a flower, which defined our color scheme</li>
				<li>It has other meaning as blank or basic</li>
			</ul>
			<p>The <span class="text-quoted">blank</span> meaning fits quite well with our application, because it literally allows you to create blank PDF files - which other toolkits often doesn't.</p>
			<p>With the name in mind, it was the right time to focus on public profile, such as logos and website. Luckily, we have found a very creative graphic designer, who took a good care of the product appearance.</p>
			<p>If you'd like to know more about the company or simply anything regarding our portfolio, we would love to hear from you! Please visit section <a href="{{ pages_folder }}{{ contact_page }}?section=section-contact">Contact us</a>.</p>
          </div>
		  
		  <div class="col-md-5 ml-auto">
            <h2>Our Mission</h2>
            <p>We thrive on providing our customers the best possible solutions and services. It is no secret we are a small company, so we cannot compete with giants on the market. Instead, using our limited resources as efficient as possible is what we love.</p>
			
			<br>
			
			<p class="lead">The goal is set and we will carry on by:</p>

            <p>
              <i class="ti-check text-success mr-2"></i>
              <span> Creating new outstanding utilities</span>
            </p>
			
			<p>
              <i class="ti-check text-success mr-2"></i>
              <span> Deliver frequent and high-quality updates</span>
            </p>
          
            <p>
              <i class="ti-check text-success mr-2"></i>
              <span> Support our customers</span>
            </p>
          
            <p>
              <i class="ti-check text-success mr-2"></i>
              <span> Expand our product based on your feedback</span>
            </p>

			<p>
              <i class="ti-check text-success mr-2"></i>
              <span> Never gonna </span>
			  <span class="fw-500 text-success" data-typing="give you up, let you down, run around and desert you, make you cry, say goodbye, tell a lie and hurt you"></span>
            </p>
          </div>

        </div>
      </div>
      </section>
	  
	  <!--
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      | Numbers
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      !-->
	  
	  <?php
		$start = date_create('2013-12-01');
		$end = date_create();
		$diff = date_diff($start, $end);
	  ?>

      <section class="section bg-vanilla-light">
        <div class="container">
          <div class="row gap-y text-center">

            <div class="col-md-4">
              <p class="small text-uppercase ls-2">Sleepless nights</p>
              <p class="lead-8 lh-1 fw-700" data-provide="countup" data-from="0" data-to="<?= $diff->days ?>"></p>
            </div>

			<div class="col-md-4">
              <p class="small text-uppercase ls-2">Git commits</p>
              <p class="lead-8 lh-1 fw-700" data-provide="countup" data-from="0" data-to="1525"></p>
            </div>

            <div class="col-md-4">
              <p class="small text-uppercase ls-2">Lines of code</p>
              <p class="lead-8 lh-1 fw-700" data-provide="countup" data-from="0" data-to="33160"></p>
            </div>

          </div>
        </div>
      </section>

      <!--
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      | Our Location
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      !-->
      <section id="section-location" class="section">
        <div class="container">
		
		  <header class="section-header">
            <h2>Location</h2>
			<p class="lead">Our head office is located in the capital city of Slovakia, the heart of the central Europe</p>
          </header>

          <div class="row gap-y">
            <div class="col-md-5">
			  <div class="bg-vanilla-light h-full p-5">
			      <p class="lead">Vanilla.PDF Labs s.r.o.<br>Ul. 29. Augusta 2281/28<br>Bratislava, Slovakia</p>
			      
			      <div>
			      <span class="d-inline-block text-lighter" title="Email">Email:</span>
			      <span class="small-1"><a href="mailto:info@vanillapdf.com">info@vanillapdf.com</a></span>
			      </div>
			      
			      <div>
			      <span class="d-inline-block text-lighter" title="Phone">Phone:</span>
			      <span class="small-1">+421 903 245 072</span>
			      </div>
			  </div>
            </div>
			
			<div class="col-md-6 ml-auto">
              <div class="h-100" style="min-height: 300px" data-provide="map" data-lat="48.146" data-lng="17.122" data-marker-lat="48.146" data-marker-lng="17.122" data-info="&lt;strong&gt;Head office&lt;/strong&gt;&lt;br&gt;Ul. 29. Augusta 2281/28, Bratislava, Slovakia" data-style="light"></div>
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
