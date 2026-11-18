Licensing
=========

Vanilla.PDF is licensed under the `Apache License 2.0
<https://www.apache.org/licenses/LICENSE-2.0>`_. The license permits use,
modification and distribution in open source and commercial software, without
royalties, and includes an express patent grant from every contributor.

The full text is in `LICENSE.txt
<https://github.com/vanillapdf/vanillapdf/blob/main/LICENSE.txt>`_ in the
repository root.

Redistribution
--------------

Section 4 of the license sets the conditions for distributing the library,
modified or not, in source or binary form. In particular:

- give recipients a copy of the license
- keep the copyright, patent, trademark and attribution notices of the source
- mark the files you modified
- include the attribution notices of the ``NOTICE`` file

The CMake install step places ``LICENSE.txt`` and ``NOTICE.md`` in
``share/vanillapdf``. Packagers can choose another directory with
``VANILLAPDF_INSTALL_LICENSEDIR``. The Debian and RPM packages ship both files
with the library in the runtime package ``libvanillapdf``.

Third-party libraries
---------------------

The library links the following open source libraries. Each is distributed
under its own license, which applies to binaries that include it. The
:doc:`building` guide describes how to turn the optional ones off.

.. list-table::
   :header-rows: 1
   :class: wrap
   :widths: 25 35 40

   * - Library
     - License
     - Linked
   * - `OpenSSL <https://www.openssl.org/>`_
     - Apache License 2.0
     - Unless ``VANILLAPDF_ENABLE_ENCRYPTION`` is off
   * - `libjpeg-turbo <https://libjpeg-turbo.org/>`_
     - IJG License, BSD 3-Clause, zlib License
     - Unless ``VANILLAPDF_ENABLE_JPEG`` is off
   * - `OpenJPEG <https://www.openjpeg.org/>`_
     - BSD 2-Clause
     - Unless ``VANILLAPDF_ENABLE_JPEG2000`` is off
   * - `zlib <https://zlib.net/>`_
     - zlib License
     - Unless ``VANILLAPDF_ENABLE_ZLIB`` is off
   * - `spdlog <https://github.com/gabime/spdlog>`_
     - MIT
     - Always
   * - `{fmt} <https://github.com/fmtlib/fmt>`_
     - MIT
     - Always
   * - `nlohmann-json <https://github.com/nlohmann/json>`_
     - MIT
     - Only with ``VANILLAPDF_ENABLE_LICENSING``, off by default
   * - `mimalloc <https://github.com/microsoft/mimalloc>`_
     - MIT
     - Only with ``VANILLAPDF_ENABLE_MIMALLOC``, off by default

vcpkg installs the license of each port it builds as
``share/<port>/copyright`` in the installed tree, which is where to collect
them when you ship the dependencies with your application.

Contributions
-------------

Contributions are accepted under the same Apache License 2.0. Every commit must
be signed off under the `Developer Certificate of Origin
<https://developercertificate.org/>`_; see `CONTRIBUTING.md
<https://github.com/vanillapdf/vanillapdf/blob/main/CONTRIBUTING.md>`_ for
details.
