PDF Format Guide
================

In the first place, let's talk about the syntactic structure of the document.
If you're already familiar with the syntax, please skip to the `Document semantics`_.

File structure
--------------

The first part explains syntactic structure of the document:

- `Object types`_
- `Indirect objects`_
- `Filters`_
- `File header`_
- `File body`_
- `File cross-reference table`_
- `File trailer`_

Object types
^^^^^^^^^^^^

PDF file consists on sequence of object with a few basic types.
``BooleanObject``,
``IntegerObject``,
``RealObject``,
``StringObject``,
``NameObject``,
``ArrayObject``,
``DictionaryObject``,
``StreamObject`` and
``NullObject``.

.. code-block:: text

   true, false      % boolean types
   7                % integer type
   3.14159          % real type
   (Literal)        % literal string
   <00FF00FF>       % hexadecimal string
   /Name            % name object
   [0 1 2 3]        % homogenous array
   null             % null reference

   <<                       % Dictionary begin
       /Type /Dictionary    % Dictionary entry with key "/Type" and value "/Dictionary"
       /Value 10            % Dictionary entry with key "Value" and value "10"
   >>                       % Dictionary end

   <<                       % Stream header begin
       /Length 1337         % Stream byte stream length
   >>                       % Stream header end
   stream                   % Stream data begin
   %% byte stream data skipped for brevity %%
   endstream                % Stream data end

Indirect objects
^^^^^^^^^^^^^^^^

Any object can be labeled as indirect, meaning it is assigned an unique number within file.
In most places an indirect reference to object can be used instead of the object value itself.

.. code-block:: text

   1 0 obj
   (This is an indirect object)

   2 0 obj
   <<                       % Dictionary begin
       /Type (Dictionary)   % Sample dictionary entry
       /Value 1 0 R         % Dictionary entry with key "Value" and its value "(This is an indirect object)"
   >>                       % Dictionary end

This association has to be added into the cross-reference table, explained in the section `File cross-reference table`_.

Filters
^^^^^^^

PDF streams support compression, so that the file size can be optimized.
Multiple compression methods are supported, such as ``ASCII85DecodeFilter``, ``FlateDecodeFilter`` and others.

.. code-block:: text

   1 0 obj
   <<
       /Length 1337
       /Filter /FlateDecode
   >>
   stream
   %% byte stream data skipped for brevity %%
   endstream

In this sample was used ``FlateDecodeFilter``. The compressed byte stream size is exactly 1337 bytes long.

File header
^^^^^^^^^^^

Header basically contains only the version of the PDF specification this file conforms to.
Depending on the version, certain features are disabled, as they were not available in the earlier standards.
A very common feature nowdays is cross-reference section compression, which is available since PDF 1.4.

.. code-block:: text

   %PDF-1.4

File body
^^^^^^^^^

Body is composed from sequence of indirect objects defined in the `File cross-reference table`_.
For a complete sample view `Complete file example`_.

.. code-block:: text

   1 0 obj
   <</Pages 2 0 R /Type /Catalog>>
   endobj
   2 0 obj
   <</Count 0 /Type /Pages>>
   endobj
   3 0 obj
   <</CreationDate (D:20170831161944Z) /Producer (I am the producer)>>
   endobj

File cross-reference table
^^^^^^^^^^^^^^^^^^^^^^^^^^

Quote from `section 7.5.4 - Cross-Reference Table <_static/PDF32000_2008.pdf#G6.1839814>`_:

   The cross-reference table contains information that permits random access to indirect objects
   within the file so that the entire file need not be read to locate any particular object.

What this means, is that there are stored byte offsets for every indirect object.
When the file is read, the reader jumps to the `File trailer`_, which is at the very end of the file.
Finds an offset to this section, searches for a object by their unique identifier (object number).
From this point, the reader can just seek to the exact offset and read the object.

.. code-block:: text

   xref
   0 4
   0000000000 65535 f
   0000000009 00000 n
   0000000056 00000 n
   0000000097 00000 n

This sample defined a cross-reference section with 4 entries.
First entry is not used, denoted by the letter 'f' as "free".
Next entry is at the file offset 9. Letter 'n' means the entry is "in-use".

File trailer
^^^^^^^^^^^^

File trailer should be located at the very end of the file, thus can be found easily.
Trailer contains a byte offset to the files cross-reference table.
There are also a few metadata information about the file, for example whether the document is encrypted.

.. code-block:: text

   trailer
   <</Info 3 0 R /Root 1 0 R /Size 4>>
   startxref
   181
   %%EOF

This trailer states, that a cross-reference section can be found at offset 181.
Additionaly, the document root object is indirect with identifier 1 (object number).
Document metadata are stored in is indirect with identifier 3 (object number).
Both document root and metadata should be defined in the cross-reference table.

For more detailed information please visit `section 7.5.5 - File Trailer <_static/PDF32000_2008.pdf#G6.1639448>`_.

----

Document semantics
------------------

Document encapsulates syntactic operations and forms operations with semantic validity.
This abstraction level is provided so that users no longer need to understand all the caveats of the PDF file format.

Root of documents hierarchy is so called ``CatalogHandle``, which contains information about document pages, layout, actions and so on.
All document pages can be found within ``PageTreeHandle``, with references to their ``PageObjectHandle``.
Page contents are described in the `Page contents`_ section.

Digital signatures are fully supported:

- Sign documents using ``Document_Sign`` function with ``PKCS12KeyHandle``
- Verify signatures using ``DigitalSignatureExtensions_Verify`` with ``TrustedCertificateStoreHandle``

Complete examples are available at ``sign.cpp`` (signing) and ``verify.cpp`` (verification).
See also :doc:`signature_verification` for comprehensive verification guide.

Common data structures
^^^^^^^^^^^^^^^^^^^^^^

There are multiple general-purpose data structures that are built from the basic object types, such as ``DateHandle``, Rectangles or Trees.
Since they are composed from the basic object types, they provide interface to modify their back-end object in graceful manner.

For example when a date field is expected, the user does not need to know the syntax of date (D:YYYYMMDDHHmmSSOHH'mm).

Another example are trees, which should be balanced to help the PDF readers optimize searching within large amount of data.
This process is done by the semantic layer, who is responsible for making best efforts on such optimizations.

----

Page contents
-------------

Document ``PageObjectHandle`` contains entry called Contents, which contains serialized stream of PostScript instructions.

Sample page contents:

.. code-block:: text

   /P <</MCID 20 >>BDC
   BT
   /TT0 1 Tf
   0.0001 Tc
   -0.0009 Tw
   12 0 0 12 90 293.82 Tm
   (This FAQ is prepared in support of) Tj
   0.0003 Tc
   0.0007 Tw
   14.055 0 Td
   [( ISO 19005-1:2005, Docum)8(e)-1(nt m)8(a)-1(nagem)8(e)-1(nt \227 )] TJ
   0.0005 Tc
   -0.0005 Tw
   -14.055 -1.15 Td
   [(Electronic docum)8(e)-1(nt file for)-6(m)8(at for long-te)] TJ
   0.0004 Tc
   -0.001 Tw
   17.305 0 Td
   [(rm)8( preservation \227 Part 1: U)8(se of PDF 1.4 )] TJ
   0 Tw
   -17.305 -1.15 Td
   (\(PDF/A-1\) )Tj
   ET
   EMC

Essential clue is that all operators are preceeded by their parameters.
The generic formula for all postscript instructions is:

.. code-block:: text

   operand operand operand operator

The number of operands is always dependent on the operator.

For example, following code snippet sets current font to font associated with name /TT0 and it's size to 1:

.. code-block:: text

   /TT0 1 Tf

Following snippet prints a literal string at the current location on the page.
The current location depends on the previous move and print instructions.

.. code-block:: text

   (This FAQ is prepared in support of) Tj

For a complete list of postscript instructions visit `Table A.1 - PDF content stream operators <_static/PDF32000_2008.pdf#G14.1029307>`_.

For implementation reference visit the :doc:`Page Contents API reference <api/contents>`.

----

Complete file example
---------------------

This is a minimal valid PDF document, that can be opened by Adobe Acrobat Reader.
It has zero visible pages and it only contains document metadata.

.. code-block:: text

   %PDF-1.7
   1 0 obj
   <</Pages 2 0 R /Type /Catalog>>
   endobj
   2 0 obj
   <</Count 0 /Type /Pages>>
   endobj
   3 0 obj
   <</CreationDate (D:20170831161944Z) /Producer (I am the producer)>>
   endobj

   xref
   0 4
   0000000000 65535 f
   0000000009 00000 n
   0000000056 00000 n
   0000000097 00000 n
   trailer
   <</Info 3 0 R /Root 1 0 R /Size 4>>
   startxref
   181
   %%EOF

At the start, there is marker, that this file conforms to the latest PDF standard 1.7.
Next we seek to the end of the file, searching for offset to the last cross-reference section.
In this case, the cross-reference section is at offset 181 from the beginning of the file.
The file contains 3 indirect objects, with identifiers 1, 2, 3 (object numbers).

.. note::

   The first entry in the cross-reference table is a head of "free entries list".
   It is possible to free and reuse objects using incremental updates, but that is out of scope of this tutorial.

Trailer contains reference to the root of the document structure - Catalog, object number 1.
Catalog contains reference to page tree structure with object number 2.
Page tree is empty for this document - it does not contain any page.
The last object number 3. is just the document metadata, with the date it was created and name of the producer.

**That's it! We've covered the fundamentals of the PDF files.**
