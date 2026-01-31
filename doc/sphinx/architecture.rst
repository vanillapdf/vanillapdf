Architecture
============

The library is written in standard C++17 and can be compiled using Visual
Studio 2022 (MSVC 17.x), Visual Studio 2026 (MSVC 18.x), GCC 8.1+, or
Clang 10+ (including AppleClang 15 on macOS). Official CMake presets cover
Windows, Linux, macOS, and Android targets.

Build is executed using cross-platform make tool CMake (https://cmake.org/).
CMake also integrates packaging system to provide installable packages for each platform.

Currently supported package formats:

- Debian ``.deb`` packages
- Homebrew packages
- NuGet packages

It provides only **ANSI C** API. The reason why I did not expose native C++ interface is rooted within the incompatibility of the C++ ABI between compilers.
Functions across the interface use standard C caller clean-up `cdecl <https://en.wikipedia.org/wiki/X86_calling_conventions#cdecl>`_ calling convention.

Error handling
--------------

Library uses C++ exceptions internally. Each interface function is wrapped inside try-catch block to prevent any exceptions to escape and potentially crash the application.

This is example, how interface functions usually look like:

.. code-block:: c

   error_type Buffer_SetData(BufferHandle* handle, string_type data, size_type size) {
   	Buffer* obj = reinterpret_cast<Buffer*>(handle);

   	if (obj == nullptr) {
   		return VANILLAPDF_ERROR_PARAMETER_VALUE;
   	}

   	if (data == nullptr) {
   		return VANILLAPDF_ERROR_PARAMETER_VALUE;
   	}

   	try
   	{
   		obj->assign(data, data + size);
   		return VANILLAPDF_ERROR_SUCCESS;
   	} catch (std::exception& e) {
   		// Store the error message
   		return VANILLAPDF_ERROR_GENERAL;
   	} catch (...) {
   		return VANILLAPDF_ERROR_GENERAL;
   	}
   }

.. note::

   The wrapping try-catch should have `negligible performance impact <https://en.wikipedia.org/wiki/Exception_handling#Exception_handling_implementation>`_ on most compilers.

Error codes and messages
^^^^^^^^^^^^^^^^^^^^^^^^

All exceptions thrown in this way are caught and their message is stored in a thread-local buffer.
This buffer is separate for each thread and has a pre-allocated size in case of memory shortage.

Following code snippet declares the structures that carries error information:

.. code-block:: c

   thread_local uint32_t m_error;
   thread_local size_type m_message_length;
   thread_local char m_message[constant::MAX_MESSAGE_SIZE];

Object ownership
----------------

All handles are basically opaque pointers to internal structures.
Library uses so-called intrusive pointer reference counting mechanism.
Usually, the structure and the reference counter are two separate objects.
In this case, the reference counter is embedded inside the structure body.

Intrusive vs Shared
^^^^^^^^^^^^^^^^^^^

Let's compare intrusive pointer with the traditional `C++ shared pointers <http://en.cppreference.com/w/cpp/memory/shared_ptr>`_.

Transferring object handle outside library bounds is more clear.

.. code-block:: c

   Buffer* buffer = new Buffer();
   *result = reinterpret_cast<BufferHandle*>(buffer);

   ...

   Buffer* buffer = reinterpret_cast<Buffer*>(handle);

Intrusive pointers can guarantee, that there are no multiple reference count objects.

Intrusive pointers should have a better performance (in some cases) comparing to traditional C++ shared pointers.
Main reason is that accessing the object required two pointer dereferences for shared pointer, while for intrusive only one.
The other reason is that whole object is allocated within a single allocation, while shared pointers are often not.

.. note::

   Shared pointer can be allocated using `make_shared <http://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared>`_.
   In addition, to ensure (not guarantee) that there is only a single reference counter object, the objects may be derived from `shared_from_this <http://en.cppreference.com/w/cpp/memory/enable_shared_from_this>`_.

File layer
----------

File layer allows access to file contents at the syntactic level.
It has some necessary semantic features that are required for parsing its syntax.

For example ``IndirectReferenceObjectHandle`` often has to be resolved to read an object.
The ``StreamObjectHandle`` has its ``Length`` often stored as an indirect object.
In order to validate this object, the ``Length`` has to be resolved to successfully parse an object.

IO Streams
^^^^^^^^^^

Library uses C++ io streams for reading source files and writing output files.
There are already interfaces, that represents these streams and will be used throughout the library interface.

- Source files with ``InputStreamHandle``
- Destination files with ``OutputStreamHandle``

.. note::

   These interfaces could be overriden in the future, so that user can provide custom implementation for reading source file.
   This is often helpful for interacting with other applications, that might need to share file access.

Tokenizer
^^^^^^^^^

Tokens are smallest syntactic elements and are separated by a whitespace or a delimiter.
Which characters are considered whitespace and which are considered delimiter is discussed in `section 7.2 - Lexical Conventions <_static/PDF32000_2008.pdf#G6.1638740>`_.

.. note::

   PDF supports comments, but they are currently ignored. They might be persisted in the future.

Tokenizer uses look-ahead to determine proper token type, since some of the tokens are ambiguous from the first character.
For example hexadecimal string is enclosed with angle brackets "<", ">" and the dictionary "<<", ">>".

Sample parsing loop for hexadecimal string:

.. code-block:: c

   int char = m_stream->Get();
   if (char == Delimiter::LESS_THAN_SIGN) {

   	int ahead = m_stream->Peek();
   	if (ahead == Delimiter::LESS_THAN_SIGN) {
   		return Token::Type::DICTIONARY_BEGIN;
   	}

   	for (;;) {
   		int hex_char = m_stream->Get();
   		if (hex_char == Delimiter::GREATER_THAN_SIGN) {
   			break;
   		}

   		if (IsNumeric(hex_char) || IsAlpha(hex_char)) {
   			continue;
   		}

   		// Found unknown character - terminate
   	}

   	return Token::Type::HEXADECIMAL_STRING;
   }

Parser
^^^^^^

Tokens are passed to the parser, who is responsible for constructing objects.
Parser uses look-ahead as well, since multiple tokens may form a single object.

.. image:: /_images/indirect_reference_parsing.png
   :alt: Diagram for parsing indirect object references

Function callbacks
------------------

Library provides multiple interfaces, that could be overriden by the calling application.

For instance, when signing a document, it is possible to use classic PKCS#12 (Personal Information Exchange described in `RFC 7292 <https://tools.ietf.org/html/rfc7292>`_).
Unfortunately, this would not work with smart cards, where the private key is not directly accessible.
User can override ``SigningKeyHandle`` and provide signing implementation outside library boundaries.

More extendable interfaces:

- Document signing with ``SigningKeyHandle``
- Document encryption with ``EncryptionKeyHandle``
- File writer callbacks with ``FileWriterObserverHandle``

Dependencies
------------

The library depends on the following libraries:

- `OpenSSL <https://www.openssl.org/>`_ -- encryption, decryption, and digital signatures
- `zlib <http://www.zlib.net/>`_ -- flate compression
- `libjpeg-turbo <https://libjpeg-turbo.org/>`_ -- JPEG image decoding
- `OpenJPEG <https://www.openjpeg.org/>`_ -- JPEG2000 image support
- `spdlog <https://github.com/gabime/spdlog>`_ -- logging
- `nlohmann-json <https://github.com/nlohmann/json>`_ -- configuration parsing

All dependencies are managed automatically via vcpkg. See :doc:`building` for
details on using system packages instead.
