<!DOCTYPE html>
<html lang="en">
  <head>
    <!-- Meta tags -->
    {% include 'meta.html' %}

    <!-- Styles -->
    {% include 'styles.html' %}

    <!-- Favicons -->
    {% include 'favicons.html' %}

    <!-- Google Analytics -->
    {% include 'google_analytics.html' %}
  </head>

  <body>


    <!-- Navbar -->
    <nav class="navbar navbar-expand-lg navbar-dark" data-navbar="fixed">
      <div class="container">

        {% include 'navbar-left.html' %}

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
			
			  <object data="{{ assets_folder }}data/EULA.pdf" type="application/pdf" width="100%" height="100%">
			    <p><b>Example fallback content</b>: This browser does not support PDFs. Please download the PDF to view it: <a href="{{ assets_folder }}data/EULA.pdf">Download PDF</a>.</p>
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
			
			  <object data="{{ assets_folder }}data/terms_of_use.pdf" type="application/pdf" width="100%" height="100%">
			    <p><b>Example fallback content</b>: This browser does not support PDFs. Please download the PDF to view it: <a href="{{ assets_folder }}data/terms_of_use.pdf">Download PDF</a>.</p>
			  </object>
            </div>
          </div>
        </div>
      </section>

    </main><!-- /.main-content -->


	<!-- Footer -->
	<footer id="footer" class="footer py-7">
		{% include 'footer.html' %}
	</footer><!-- /.footer -->


    <!-- Scripts -->
    {% include 'scripts.html' %}

  </body>
</html>
