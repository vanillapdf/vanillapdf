PDF format guide
================

This guide introduces the parts of the PDF file format that the library works
with. It starts with the syntax of the file and continues with the document
structure built on top of it. Readers already familiar with the syntax can
continue with `Document semantics`_.

File structure
--------------

The syntactic structure of a PDF file consists of:

- `Object types`_
- `Indirect objects`_
- `Filters`_
- `File header`_
- `File body`_
- `File cross-reference table`_
- `File trailer`_

Object types
^^^^^^^^^^^^

A PDF file is a sequence of objects of a few basic types:
:cpp:class:`BooleanObject <BooleanObjectHandle>`,
:cpp:class:`IntegerObject <IntegerObjectHandle>`,
:cpp:class:`RealObject <RealObjectHandle>`,
:cpp:class:`StringObject <StringObjectHandle>`,
:cpp:class:`NameObject <NameObjectHandle>`,
:cpp:class:`ArrayObject <ArrayObjectHandle>`,
:cpp:class:`DictionaryObject <DictionaryObjectHandle>`,
:cpp:class:`StreamObject <StreamObjectHandle>` and
:cpp:class:`NullObject <NullObjectHandle>`.

.. code-block:: text

   true, false      % boolean types
   7                % integer type
   3.14159          % real type
   (Literal)        % literal string
   <00FF00FF>       % hexadecimal string
   /Name            % name object
   [0 1 2 3]        % array
   null             % null object

   <<                       % Dictionary begin
       /Type /Dictionary    % Dictionary entry with key "/Type" and value "/Dictionary"
       /Value 10            % Dictionary entry with key "/Value" and value "10"
   >>                       % Dictionary end

   <<                       % Stream header begin
       /Length 1337         % Length of the stream data in bytes
   >>                       % Stream header end
   stream                   % Stream data begin
   %% byte stream data skipped for brevity %%
   endstream                % Stream data end

Indirect objects
^^^^^^^^^^^^^^^^

Any object can be labeled as indirect, which assigns it an object number that
is unique within the file. In most places an indirect reference can be used
instead of the object value itself.

.. code-block:: text

   1 0 obj
   (This is an indirect object)
   endobj

   2 0 obj
   <<                       % Dictionary begin
       /Type (Dictionary)   % Sample dictionary entry
       /Value 1 0 R         % Entry with key "/Value", referencing "(This is an indirect object)"
   >>                       % Dictionary end
   endobj

Each indirect object is listed in the cross-reference table, described in
`File cross-reference table`_.

Filters
^^^^^^^

Stream data can be encoded with filters, most often to compress it. The PDF
specification defines several filters, such as ``/FlateDecode`` and
``/ASCII85Decode``.

.. code-block:: text

   1 0 obj
   <<
       /Length 1337
       /Filter /FlateDecode
   >>
   stream
   %% byte stream data skipped for brevity %%
   endstream
   endobj

This sample uses the ``/FlateDecode`` filter. ``/Length`` is the size of the
encoded data, 1337 bytes.

File header
^^^^^^^^^^^

The header states the version of the PDF specification the file conforms to.
The version determines which features the file may use; for example,
cross-reference streams, which compress the cross-reference information, are
available since PDF 1.5.

.. code-block:: text

   %PDF-1.4

File body
^^^^^^^^^

The body is a sequence of indirect objects, each listed in the
`File cross-reference table`_. See `Complete file example`_ for a complete
file.

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

From `section 7.5.4, "Cross-Reference Table" <_static/PDF32000_2008.pdf#G6.1839814>`_:

   The cross-reference table contains information that permits random access to indirect objects
   within the file so that the entire file need not be read to locate any particular object.

The table stores the byte offset of every indirect object. A reader starts at
the `File trailer`_ at the end of the file, which holds the offset of the
cross-reference table. The table gives the offset of each object by its object
number, so the reader can seek directly to any object and parse it.

.. code-block:: text

   xref
   0 4
   0000000000 65535 f
   0000000009 00000 n
   0000000056 00000 n
   0000000097 00000 n

This sample defines a cross-reference section with 4 entries. The first entry
is marked ``f`` (free) and is not used by any object. The next entry, marked
``n`` (in use), places object 1 at byte offset 9.

File trailer
^^^^^^^^^^^^

The trailer is located at the end of the file, where a reader can find it
without parsing anything else. It contains the byte offset of the
cross-reference table and information about the file as a whole, for example
whether the document is encrypted.

.. code-block:: text

   trailer
   <</Info 3 0 R /Root 1 0 R /Size 4>>
   startxref
   181
   %%EOF

This trailer states that the cross-reference section is at offset 181. The
document catalog (``/Root``) is object 1 and the document information
dictionary (``/Info``) is object 3. Both are listed in the cross-reference
table.

For details see `section 7.5.5, "File Trailer" <_static/PDF32000_2008.pdf#G6.1639448>`_.

----

Document semantics
------------------

The document layer builds on the syntax and keeps the file semantically valid
when it is modified, so users do not need to know every detail of the file
format.

The root of the document hierarchy is the :cpp:class:`CatalogHandle`, which
holds the pages, the page layout, document actions and other document-wide
information. The pages are organized in the :cpp:class:`PageTreeHandle`, which
references each :cpp:class:`PageObjectHandle`. Page contents are described in
`Page contents`_.

Digital signatures can be created and verified:

- Sign documents using :cpp:func:`Document_Sign <DocumentHandle::Document_Sign>` with a :cpp:class:`PKCS12KeyHandle`
- Verify signatures using :cpp:func:`DigitalSignatureExtensions_Verify` with a :cpp:class:`TrustedCertificateStoreHandle`

The `sign.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/sign.cpp>`_
and `verify.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/verify.cpp>`_
sources show both in use. See :doc:`signature_verification` for a complete
guide to verification.

Common data structures
^^^^^^^^^^^^^^^^^^^^^^

Several general-purpose data structures are built from the basic object types,
such as dates (:cpp:class:`DateHandle`), rectangles and trees. Their interfaces
read and modify the underlying objects, so callers work with values rather
than syntax.

For example, a date can be set without knowing the PDF date format
(``D:YYYYMMDDHHmmSSOHH'mm``).

Trees are another example: PDF readers search large trees faster when they are
balanced, and the semantic layer takes care of keeping them so.

----

Page contents
-------------

Each :cpp:class:`PageObjectHandle` has a ``/Contents`` entry, a content stream
of operators that describe what is drawn on the page.

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

As in PostScript, every operator follows its operands:

.. code-block:: text

   operand operand operand operator

The number of operands depends on the operator.

For example, the following instruction selects the font named ``/TT0`` at a
size of 1:

.. code-block:: text

   /TT0 1 Tf

The following instruction shows a literal string at the current text position,
which is set by the preceding positioning and text-showing operators:

.. code-block:: text

   (This FAQ is prepared in support of) Tj

For the complete list of operators see `Table A.1, "PDF content stream operators" <_static/PDF32000_2008.pdf#G14.1029307>`_.

The content stream API is described in the :doc:`page contents API reference <api/contents>`.

----

Complete file example
---------------------

This is a minimal, structurally complete PDF file. Its page tree is empty, so
it has no pages, and it only carries document metadata.

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

The header declares that the file conforms to PDF 1.7. A reader then reads the
end of the file, where ``startxref`` gives the offset of the last
cross-reference section, here 181 bytes from the beginning of the file. The
file contains 3 indirect objects, numbered 1, 2 and 3.

.. note::

   The first entry in the cross-reference table is the head of the list of free
   entries. Objects can be freed and their numbers reused by incremental
   updates, which are beyond the scope of this guide.

The trailer references the root of the document structure, the catalog, which
is object 1. The catalog references the page tree, object 2, which is empty.
Object 3 is the document information dictionary, which records when the
document was created and the application that produced it.
