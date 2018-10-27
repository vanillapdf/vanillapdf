<!DOCTYPE html>
<html lang="en">
  <head>
    <!-- Meta tags -->
    {% include 'meta.html' %}
	
	<?php
		// Extra styles
		wp_enqueue_style( 'main-page', get_template_directory_uri() . '/assets/css/main_page.css', 'template-page');
	?>

    <!-- Favicons -->
    {% include 'favicons.html' %}

    <!-- Google Analytics -->
    {% include 'google_analytics.html' %}
	
	<?php wp_head(); ?>
  </head>

  <body>


    <!-- Navbar -->
    <nav class="navbar navbar-expand-lg navbar-light" data-navbar="fixed">
      <div class="container">

        {% include 'navbar-left.html' %}

        <section class="navbar-mobile">
          <nav class="nav nav-navbar ml-auto">
            <a class="nav-link" href="#home">Home</a>
            <a class="nav-link" href="#section-features">Features</a>
			<a class="nav-link" href="#section-cta">Try it</a>
            <a class="nav-link" href="#section-pricing">Pricing</a>
            <a class="nav-link" href="#section-faq">FAQ</a>
			<a class="nav-link" href="#section-subscribe">Subscribe</a>
            <a class="nav-link" href="#footer">References</a>
          </nav>
        </section>

      </div>
    </nav><!-- /.navbar -->


    <!-- Header -->
    <header id="home" class="header text-white h-fullscreen" style="background-image: url({{ assets_folder }}img/bg/abstract.jpg)">
      <div class="overlay opacity-90 bg-vanilla"></div>

      <div class="container text-center">

        <div class="row h-100">
          <div class="col-lg-8 mx-auto align-self-center">

            <h1 class="display-1 my-6 color-vanilla-green"><strong>Vanilla.PDF</strong></h1>
            <p class="lead-4 color-vanilla-green"><strong>Vanilla.PDF</strong> is a cross-platform SDK for creating and modifying PDF documents</p>

            <hr class="w-80px">

            <p>
              <a class="btn btn-xl btn-round btn-primary w-250" href="#section-features">Features</a>
            </p>

          </div>
        </div>

      </div>
    </header><!-- /.header -->


    <!-- Main Content -->
    <main class="main-content">

      <!--
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      | Features
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      !-->
      <section id="section-features" class="section">
        <div class="container">
          <header class="section-header">
            <h2>Key features that make us unique</h2>
            <hr>
          </header>

		  <div class="row gap-y align-items-center">
            <div class="col-md-6 mr-auto">
              <h4>Quick and easy integration</h4>
              <p>Installation is very easy with pre-packaged binaries for multiple operating systems such as <strong>Windows</strong>, <strong>Linux</strong> and <strong>Mac</strong>. If your operating system is not supported, please contact us, so we may add its support in the future.</p>
			  <p>Written in native code with standardized interface, that can be used from almost any programming language!</p>
            </div>

		    <div class="col-md-5 text-center">
              <i class="fa fa-wrench fa-10x"></i>
            </div>
          </div>


		  <hr class="my-8">


          <div class="row gap-y align-items-center">
            <div class="col-md-6 ml-auto">
              <h4>Supreme control</h4>
              <p>Sometimes you simply cannot achieve a minor change, that would help your product appearence or usability. We grant our users <strong>maximum possible control</strong>, while their comfort is not hindered.</p>
			  <p>It can also serve as a <strong>post-processing</strong> step, if the output from a different tool is not sufficient.</p>
            </div>

            <div class="col-md-5 order-md-first text-center">
              <i class="fa fa-sliders fa-10x"></i>
            </div>
          </div>


          <hr class="my-8">


          <div class="row gap-y align-items-center">
            <div class="col-md-6 mr-auto">
			  <h4>Fast and subtle</h4>
			  <p>Postponing all reading and processing operation until the very last moment allows us to drastically improve <strong>performance</strong>. We have also developed a very sophisticated system for sharing internal objects, which saves a lot of <strong>unnecessary overhead</strong>.</p>
            </div>

            <div class="col-md-5 text-center">
              <i class="fa fa-tachometer fa-10x"></i>
            </div>
          </div>


          <hr class="my-8">


		  <div class="row gap-y align-items-center">
            <div class="col-md-6 ml-auto">
              <h4>Information, tutorials and manuals</h4>
              <p>Have you ever <span class="text-quoted">reinvented the wheel</span> and just later found out you could saved yourself a hassle, but the crucial piece of information was missing?</p>
			  <p>We try really hard to provide the best <strong>support</strong> for our customers. Starting with <strong>API reference</strong>, through step-by-step <strong>tutorial</strong> and last but not least the full source-code <strong>examples</strong> of read-world use.</p>
            </div>

            <div class="col-md-5 order-md-first text-center">
              <i class="fa fa-info fa-10x"></i>
            </div>
          </div>


        </div>
      </section>
	  
	  <!--
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      | CTA
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      !-->
      <section id="section-cta" class="section text-center bg-vanilla-light">
        <div class="container">
		
		  <header class="section-header">
            <h2>Try it Now!</h2>
            <hr>
			<p class="lead">Everything you might need in a few simple steps</p>
          </header>
		  
		  <div class="row gap-y">

            <div class="col-lg-3 d-flex">
              <a class="card card-body border text-center hover-shadow-6 text-default" href="{{ pages_folder }}{{ download_page }}">
                <p class="my-5"><i class="fa fa-download lead-8 text-lighter"></i></p>
                <h5>Download</h5>
                <p>Download the latest version</p>
              </a>
            </div>


            <div class="col-lg-3 d-flex">
              <a class="card card-body border text-center hover-shadow-6 text-default" href="{{ versions_folder }}{{ LATEST_VERSION }}/doc/index.html">
                <p class="my-5"><i class="fa fa-book lead-8 text-lighter"></i></p>
                <h5>Tutorial</h5>
                <p>Learn the basics</p>
              </a>
            </div>


            <div class="col-lg-3 d-flex">
              <a class="card card-body border text-center hover-shadow-6 text-default" href="{{ versions_folder }}{{ LATEST_VERSION }}/doc/hierarchy.html">
                <p class="my-5"><i class="fa fa-gear lead-8 text-lighter"></i></p>
                <h5>API reference</h5>
                <p>Explore possibilities</p>
              </a>
            </div>
			
			<div class="col-lg-3 d-flex">
              <a class="card card-body border text-center hover-shadow-6 text-default" href="{{ versions_folder }}{{ LATEST_VERSION }}/doc/page_examples.html">
                <p class="my-5"><i class="fa fa-flask lead-8 text-lighter"></i></p>
                <h5>Examples</h5>
                <p>Discover other concepts</p>
              </a>
            </div>

          </div>
		  
		  <hr class="w-10">
		  
		  <div class="row">
		  	<div class="mx-auto">
              <h5>Missing something?</h5>
			  <div class="pt-4">
			    <a class="btn btn-lg btn-round btn-secondary" href="{{ pages_folder }}{{ contact_page }}?section=section-contact">Contact us</a>
			  </div>
            </div>
		  </div>
        </div>
      </section>

      <!--
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      | Pricing
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      !-->
      <section id="section-pricing" class="section text-center">
        <div class="container">
		  <header class="section-header">
            <h2>Affordable Pricing</h2>
            <hr>
            <p class="lead">There's not any extra or hidden fee. Choose the license type that suits your needs!</p>
          </header>
		  
		  
			<div class="row">
			
			<div class="col-md-4 mx-auto">
              <div class="border rounded">
                <div class="text-center px-4 py-6">
                  <p class="mb-7"><i class="fa fa-search lead-8 text-dark"></i></p>
                  <h6 class="text-uppercase fw-500 ls-2 my-4">Evaluation</h6>
                  <p class="lead">Free license only for evaluation purposes</p>
                  <h2 class="lead-8 fw-200 mb-0">0{{ CURRENCY }}</h2>
				  <p class="small-2">Free</p>
				  
				  <button class="btn disabled">Limited support</button>
                </div>

                <div class="bg-vanilla-light px-4 py-6">
                  <h6 class="text-center mb-6">Evaluation Features</h6>
                  <ul class="list-unstyled mb-0">
					<li><i class="ti-check text-success mr-2"></i> Extract text and images</li>
                    <li><i class="ti-close text-danger mr-2"></i> Document signing</li>
					<li><i class="ti-close text-danger mr-2"></i> Document merge/split</li>
					<li><i class="ti-close text-danger mr-2"></i> Document encryption</li>
                    <li><i class="ti-close text-danger mr-2"></i> Royalty-free distribution</li>
                  </ul>
                </div>
				
				<div class="text-center px-4 py-6">
				  <a class="btn btn-outline-primary btn-round w-200" href="{{ pages_folder }}{{ download_page }}">Download</a>
				  <p>or <a href="{{ pages_folder }}{{ contact_page }}?section=section-license">Get a temporary license</a></p>
				</div>
              </div>
            </div>
			
			<div class="col-md-4 mx-auto">
              <div class="border rounded">
                <div class="text-center px-4 py-6">
                  <p class="mb-7"><i class="fa fa-user lead-8 text-dark"></i></p>
                  <h6 class="text-uppercase fw-500 ls-2 my-4">Personal</h6>
                  <p class="lead">Personal license for creating private tools</p>
                  <h2 id="personal-price" class="lead-8 fw-200 mb-0">{{ PERSONAL_LICENSE_WITH_SUPPORT_PRICE }}{{ CURRENCY }}</h2>
                  <p class="small-2">One-time fee</p>

                  <div class="btn-group dropdown">
                      <button id="personal-support" class="btn btn-secondary dropdown-toggle" data-toggle="dropdown">1 Year support</button>
                      <div class="dropdown-menu">
                        <a id="personal-one" class="btn dropdown-item">1 Year support</a>
                        <a id="personal-two" class="btn dropdown-item">2 Year support</a>
                        <a id="personal-three" class="btn dropdown-item">3 Year support</a>
                      </div>
                  </div>
                </div>

                <div class="bg-vanilla-light px-4 py-6">
                  <h6 class="text-center mb-6">Personal Features</h6>
                  <ul class="list-unstyled mb-0">
					<li><i class="ti-check text-success mr-2"></i> Extract text and images</li>
                    <li><i class="ti-check text-success mr-2"></i> Document signing</li>
					<li><i class="ti-check text-success mr-2"></i> Document merge/split</li>
					<li><i class="ti-check text-success mr-2"></i> Document encryption</li>
                    <li><i class="ti-close text-danger mr-2"></i> Royalty-free distribution</li>
                  </ul>
                </div>
				
				<div class="text-center px-4 py-6">
				  <a id="personal-purchase-link" class="btn btn-primary btn-round w-200" href="{{ pages_folder }}{{ order_page }}?product=personal-license&support=1">Purchase</a>
				  <p>or <a id="personal-support-link" href="{{ pages_folder }}{{ order_page }}?product=personal-support&support=1">Extend your support</a></p>
				</div>
              </div>
            </div>


			<div class="col-md-4 mx-auto">
			  <div class="border rounded">
				<div class="text-center px-4 py-6">
				  <p class="mb-7"><i class="fa fa-line-chart lead-8 text-dark"></i></p>
				  <h6 class="text-uppercase fw-500 ls-2 my-4">Commercial OEM</h6>
				  <p class="lead">Commercial license for creating public products</p>
				  <h2 id="commercial-price" class="lead-8 fw-200 mb-0">{{ COMMERCIAL_LICENSE_WITH_SUPPORT_PRICE }}{{ CURRENCY }}</h2>
				  <p class="small-2">One-time fee</p>
				  
					<div class="dropdown">
						<button id="commercial-support" type="button" class="btn btn-secondary dropdown-toggle" data-toggle="dropdown">1 Year support</button>
						<div class="dropdown-menu">
						  <a id="commercial-one" class="btn dropdown-item">1 Year support</a>
						  <a id="commercial-two" class="btn dropdown-item">2 Year support</a>
						  <a id="commercial-three" class="btn dropdown-item">3 Year support</a>
						</div>
					</div>
				</div>

				<div class="bg-vanilla-light px-4 py-6">
				  <h6 class="text-center mb-6">Commercial Features</h6>
				  <ul class="list-unstyled mb-0">
					<li><i class="ti-check text-success mr-2"></i> Extract text and images</li>
					<li><i class="ti-check text-success mr-2"></i> Document signing</li>
					<li><i class="ti-check text-success mr-2"></i> Document merge/split</li>
					<li><i class="ti-check text-success mr-2"></i> Document encryption</li>
					<li><i class="ti-check text-success mr-2"></i> Royalty-free distribution</li>
				  </ul>
				</div>
				
				<div class="text-center px-4 py-6">
				  <a id="commercial-purchase-link" class="btn btn-outline-primary btn-round w-200" href="{{ pages_folder }}{{ order_page }}?product=commercial-license&support=1">Purchase</a>
				  <p>or <a id="commercial-support-link" href="{{ pages_folder }}{{ order_page }}?product=commercial-support&support=1">Extend your support</a></p>
				</div>
			  </div>
			</div>
		  </div>

		  <hr class="w-10">

		  <div class="row">
		    <div class="mx-auto">
		  	  <h5>Too expensive?</h5>
		  	  <div class="py-4">
		  	    <a class="btn btn-lg btn-round btn-secondary" href="{{ pages_folder }}{{ contact_page }}?section=section-discount">Request a discount</a>
		  	  </div>
		    </div>
		  </div>

        </div>
      </section>

      <!--
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      | FAQ
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      !-->
      <section id="section-faq" class="section bg-vanilla-light">
        <div class="container">
          <header class="section-header">
            <h2>Frequently Asked Questions</h2>
            <hr>
            <p>Got a question? We've got answers. If you have some other questions, please contact us</p>
          </header>
		  
		  <div class="row gap-y">
		    <div class="col-md-6 col-xl-4">
              <h5>What is your very unique selling point?</h5>
              <p>In essence, <b>Vanilla.PDF</b> allows expert developers to fine-tune documents with maximum control. It can therefore serve as a post-processing step, if the output from a different tool is not sufficient.</p>
            </div>

		   <div class="col-md-6 col-xl-4">
              <h5>Can I try your product for free?</h5>
              <p>There is an evaluation option, where you can try the product with a very limited functionality. Simply download the latest version, compile any of the free demo projects and enjoy!</p>
            </div>

			<div class="col-md-6 col-xl-4">
              <h5>Can I get a discount?</h5>
              <p>There are various discounts available depending on your intentions, field of use, team size, start-ups and more. We love to support ambitious teams, so feel free to ask!</p>
            </div>

            <div class="col-md-6 col-xl-4">
              <h5>Can I continue to use product after the support period has expired?</h5>
              <p>Absolutely! After you have received your license key, you may use the product in the current version indefinitely. However, all updates including security updates and hotfixes are only available during supported time period.</p>
            </div>

            <div class="col-md-6 col-xl-4">
              <h5>What is the difference between personal and commercial license?</h5>
              <p>The only difference is that personal license is not allowed for distribution, public facing web sites/applications, extranets, multi-site intranets or SaaS project usage scenarios.</p>
            </div>
			
			<div class="col-md-6 col-xl-4">
              <h5>There is an error with the input document, what can I do?</h5>
              <p>This most often means, that the input document is malformed. In order to solve this problem, we might need to inspect the input document. Please contact us!</p>
            </div>
		  </div>

			<div class="row mt-8">
			  <div class="text-center mx-auto">
				<a class="btn btn-lg btn-round btn-secondary px-7" href="{{ pages_folder }}{{ faq_page }}">More answers</a>
			  </div>
			</div>

        </div>
      </section>


      <!--
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      | Subscribe
      |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
      !-->
      <section id="section-subscribe" class="section py-7">
        <div class="container">
          <div class="row gap-y align-items-center border bg-vanilla-light card-shadow">
            <div class="col-md-6">
              <p class="lead mb-0 text-center text-md-left">Subscribe to our newsletter and receive the latest news.</p>
            </div>

            <div class="col-md-4 ml-auto">
              <form id="subscribe-form" class="justify-content-center justify-content-lg-end" action="https://vanillapdf.us16.list-manage.com/subscribe/post?u=50f1056f5b063f4f6212a7789&amp;id=f584a2f38b" method="post" target="_blank">
                <div class="input-group">
                  <div class="input-group-prepend">
                    <span class="input-group-text"><i class="fa fa-envelope"></i></span>
                  </div>
				  
				  <!-- real people should not fill this in and expect good things - do not remove this or risk form bot signups -->
				  <input type="text" name="b_50f1056f5b063f4f6212a7789_f584a2f38b" tabindex="-1" value="" style="position: absolute; left: -5000px;" aria-hidden="true">
				  
				  <input id="mce-group[4397]-4397-0" name="group[4397][2]" type="hidden" checked> <!-- General -->
				  <input id="mce-group[4397]-4397-1" name="group[4397][1]" type="hidden" checked> <!-- New versions -->
				  
                  <input type="email" name="EMAIL" class="form-control" placeholder="Email Address">
                  <span class="input-group-append"></span>
                  <button class="btn btn-secondary">Subscribe</button>
                </div>
              </form>
            </div>
			
			<br>
			
			<p id="success-message" class="lead col-md-6 mx-auto text-center text-success" style="display: none"></p>
			<p id="error-message" class="lead col-md-6 mx-auto text-center text-danger" style="display: none"></p>
          </div>
        </div>
      </section>


    </main><!-- /.main-content -->


	<!-- Footer -->
	<footer id="footer" class="footer py-7">
		{% include 'footer.html' %}
	</footer><!-- /.footer -->


    <!-- Scroll top -->
    <button class="btn btn-circle btn-primary scroll-top"><i class="fa fa-angle-up"></i></button>


    <!-- Scripts -->
    {% include 'scripts.html' %}
	<script src="{{ assets_folder }}js/price_constants.js"></script>
	<script src="{{ assets_folder }}js/subscribe.js"></script>
	<script src="{{ assets_folder }}js/custom_navbar.js"></script>
	
	<script>
		{% include 'pricing.js' %}
	</script>

  </body>
</html>
