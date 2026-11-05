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
copyright = "2018-2026, Vanilla.PDF Labs s.r.o."
author = "Vanilla.PDF Labs s.r.o."
version = "{}.{}".format(_version_parts["MAJOR"], _version_parts["MINOR"])
release = _version

# -- General configuration -------------------------------------------------

extensions = [
    "breathe",
    "sphinx_copybutton",
    "sphinxext.opengraph",
    "sphinx_substitution_extensions",
]

# Make |release| resolvable inside literal blocks: code-block directives
# marked with :substitutions: and the :substitution-code: inline role
# (sphinx-substitution-extensions) read definitions from rst_prolog.
rst_prolog = """
.. |release| replace:: {}
""".format(_version)

breathe_projects = {
    "vanillapdf": "_build/doxygen/xml",
}
breathe_default_project = "vanillapdf"

# The "#include" line of every class links to Doxygen's header file page, which
# Breathe does not render. Consumers include c_vanillapdf_api.h anyway.
breathe_show_include = False

# Report every cross-reference that does not resolve. Read the Docs builds with
# fail_on_warning, so a renamed or removed API name fails the build instead of
# silently turning its links into plain text.
nitpicky = True

# Standard C integer types used by the public typedefs have no page to link to
nitpick_ignore = [
    ("cpp:identifier", name)
    for name in (
        "int8_t", "int16_t", "int32_t", "int64_t",
        "uint8_t", "uint16_t", "uint32_t", "uint64_t",
    )
]

# -- Options for HTML output -----------------------------------------------

html_theme = "sphinx_rtd_theme"

html_static_path = ["_static"]
html_logo = "_static/logo.png"
html_favicon = "_static/favicon.ico"

html_theme_options = {
    "logo_only": False,
    "navigation_depth": 3,
}

html_context = {
    "display_github": True,
    "github_user": "vanillapdf",
    "github_repo": "vanillapdf",
    "github_version": "main",
    "conf_py_path": "/doc/sphinx/",
}

# -- OpenGraph metadata (for link previews) --------------------------------

ogp_site_url = "https://vanillapdf.readthedocs.io/"
ogp_site_name = "Vanilla.PDF Documentation"
ogp_description_length = 200
