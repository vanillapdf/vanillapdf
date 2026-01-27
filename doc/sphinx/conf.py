"""Sphinx configuration for Vanilla.PDF documentation."""

import subprocess
import os
import re

# -- Read version from cmake/version.cmake --------------------------------

repo_root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
version_cmake = os.path.join(repo_root, "cmake", "version.cmake")

_version_parts = {}
with open(version_cmake) as f:
    for match in re.finditer(
        r'set\(VANILLAPDF_VERSION_(MAJOR|MINOR|PATCH)\s+(\d+)\)', f.read()
    ):
        _version_parts[match.group(1)] = match.group(2)

_version = "{}.{}.{}".format(
    _version_parts["MAJOR"], _version_parts["MINOR"], _version_parts["PATCH"]
)

# -- Run Doxygen to produce XML for Breathe --------------------------------

doc_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # doc/

# Write a tiny override file so Doxygen picks up the version
_override = os.path.join(doc_dir, "sphinx", "_build", "doxygen-version-override.cfg")
os.makedirs(os.path.dirname(_override), exist_ok=True)
with open(_override, "w") as f:
    f.write("PROJECT_NUMBER = {}\n".format(_version))

subprocess.run(
    ["doxygen", "Doxyfile-xml", _override], cwd=doc_dir, check=True
)

# -- Project information ---------------------------------------------------

project = "Vanilla.PDF"
copyright = "2025, Vanilla.PDF contributors"
author = "Vanilla.PDF contributors"
version = "{}.{}".format(_version_parts["MAJOR"], _version_parts["MINOR"])
release = _version

# -- General configuration -------------------------------------------------

extensions = [
    "breathe",
    "sphinx_copybutton",
]

breathe_projects = {
    "vanillapdf": "_build/doxygen/xml",
}
breathe_default_project = "vanillapdf"

# -- Options for HTML output -----------------------------------------------

html_theme = "sphinx_rtd_theme"

html_static_path = ["_static"]
