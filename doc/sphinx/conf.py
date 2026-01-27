"""Sphinx configuration for Vanilla.PDF documentation."""

import subprocess
import os

# -- Run Doxygen to produce XML for Breathe --------------------------------

doc_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # doc/
subprocess.run(["doxygen", "Doxyfile-xml"], cwd=doc_dir, check=True)

# -- Project information ---------------------------------------------------

project = "Vanilla.PDF"
copyright = "2025, Vanilla.PDF contributors"
author = "Vanilla.PDF contributors"

# -- General configuration -------------------------------------------------

extensions = [
    "breathe",
]

breathe_projects = {
    "vanillapdf": "_build/doxygen/xml",
}
breathe_default_project = "vanillapdf"

# -- Options for HTML output -----------------------------------------------

html_theme = "sphinx_rtd_theme"

html_static_path = ["_static"]
