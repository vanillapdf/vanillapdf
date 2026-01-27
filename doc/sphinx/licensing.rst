Licensing
=========

This section describes how is the product licensed as well as how to get the license.

License types
-------------

There are few basic types of product license, depending whether the product is being evaluated or running in product environment.
For more information about the licenses, please visit `Licenses <https://vanillapdf.com/page/licenses.html>`_.

Free license
^^^^^^^^^^^^

Free license allows a very limited functionality, such as extracting text and images or modifying document conents.
It is especially useful to verify, that a PDF file is supported by this toolkit.
Modifying documents with free license produces a watermark image on each page to distinguish documents produced with free license.

Temporary license
^^^^^^^^^^^^^^^^^

Temporary license is available and free of charge, to verify extended functionality offered by Vanilla.PDF.
This license is time limited and only works for 30 days.

Personal license
^^^^^^^^^^^^^^^^

Personal license unlocks all available features within the product.

   This license type is **exclusively** for internal business operations.
   This license type does **not allow** distribution of Derived Works to third parties, public facing web sites/applications, extranets, multi-site intranets or SaaS project usage scenarios.

Commercial license
^^^^^^^^^^^^^^^^^^

Commercial license unlocks all available features within the product.

   This license type **allows** distribution of Derived Works to third parties, public facing web sites/applications, extranets, multi-site intranets, app stores or SaaS project usage scenarios.

Features
--------

This is a list of most common features that are easily usable and documented within our examples.

Free features:

- Document merge and split (``merge.c``)
- PDF filter compression/decompression (``filter.c``)
- Extracting strings and images (``extract.c``)

Premium features:

- Document signing (``sign.c``)
- Decrypting documents (``decrypt.c``)

Using the license
-----------------

A license file is required to access library premium features.

Request
^^^^^^^

Requesting the license file is done via our `order form <https://vanillapdf.com/page/order.php>`_.
After processing your request, you'll receive an email with the invoice.
Upon completing the transaction, you'll receive another email, containing **your very own** license file.

.. note::

   Please, keep in mind, that every license file is issued to either a person or a company.
   It contains **confidential information** and should be kept **protected** against fraud.
   Whenever in doubt, that the license file is no longer secure, let us know, so we can issue a new one, without any harm.

Bundle
^^^^^^

To access premium features, library requires a license file, when the application is run.
We recommend to embed the license file into your application and protect it with encryption algorithm.

.. note::

   Code sample on how to protect the license file is available upon request.

Before the accessing premium features be sure to initialize the library with the license key.

For more details visit the ``LicenseInfo`` API.

Verify
^^^^^^

After the license file has been entered, the premium features should now be accessible.
To verify the license file is working properly, please use any of our :doc:`examples <examples>` for premium features.

License validity
----------------

License itself has unlimited validity for current version of the library.
Updating library to a newer version is restricted by the expiration field in the license file.

.. code-block:: text

   "expiration": "2022-10-01 18:50:36Z"

As long as the library build date is earlier then expiration date, the library accepts license file.
Determining the library build date can be done using file properties on windows.

.. image:: /_images/file_properties.png
   :alt: File properties of shared library file on windows OS

The file properties might be very slightly different to the exact values returned via ``LibraryInfo``.
