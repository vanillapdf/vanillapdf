#ifndef _C_HANDLES_H
#define _C_HANDLES_H

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_handles.h
	* \brief This file contains all type forward
	* declarations returned by the library API.
	*
	* They are __intentionally hidden__, because they
	* have no meaning in terms of documentation.
	*/

	/** \cond FORWARD_DECLARATIONS */

	/**
	* \def DECLARE_OBJECT_HANDLE(x)
	* Declares opaque pointer to struct allocated by library.
	*/
	#define DECLARE_OBJECT_HANDLE(x) \
	typedef struct x##HandleTag x##Handle;

	#pragma region Syntax

	#pragma region Objects

	DECLARE_OBJECT_HANDLE(Object);
	DECLARE_OBJECT_HANDLE(ArrayObject);
	DECLARE_OBJECT_HANDLE(BooleanObject);
	DECLARE_OBJECT_HANDLE(NameObject);
	DECLARE_OBJECT_HANDLE(NullObject);
	DECLARE_OBJECT_HANDLE(RealObject);
	DECLARE_OBJECT_HANDLE(IntegerObject);
	DECLARE_OBJECT_HANDLE(StreamObject);
	DECLARE_OBJECT_HANDLE(StringObject);
	DECLARE_OBJECT_HANDLE(HexadecimalStringObject);
	DECLARE_OBJECT_HANDLE(LiteralStringObject);
	DECLARE_OBJECT_HANDLE(IndirectReferenceObject);
	DECLARE_OBJECT_HANDLE(DictionaryObject);
	DECLARE_OBJECT_HANDLE(DictionaryObjectIterator);

	// Attributes
	DECLARE_OBJECT_HANDLE(BaseObjectAttribute);
	DECLARE_OBJECT_HANDLE(ImageMetadataObjectAttribute);
	DECLARE_OBJECT_HANDLE(SerializationOverrideObjectAttribute);
	DECLARE_OBJECT_HANDLE(ObjectAttributeList);

	#pragma endregion

	#pragma region Files

	DECLARE_OBJECT_HANDLE(File);
	DECLARE_OBJECT_HANDLE(FileWriter);
	DECLARE_OBJECT_HANDLE(FileWriterObserver);

	DECLARE_OBJECT_HANDLE(Xref);
	DECLARE_OBJECT_HANDLE(XrefIterator);
	DECLARE_OBJECT_HANDLE(XrefEntry);
	DECLARE_OBJECT_HANDLE(XrefFreeEntry);
	DECLARE_OBJECT_HANDLE(XrefUsedEntry);
	DECLARE_OBJECT_HANDLE(XrefCompressedEntry);
	DECLARE_OBJECT_HANDLE(XrefChain);
	DECLARE_OBJECT_HANDLE(XrefChainIterator);

	DECLARE_OBJECT_HANDLE(FilterBase);
	DECLARE_OBJECT_HANDLE(FlateDecodeFilter);
	DECLARE_OBJECT_HANDLE(DCTDecodeFilter);
	DECLARE_OBJECT_HANDLE(ASCII85DecodeFilter);
	DECLARE_OBJECT_HANDLE(ASCIIHexDecodeFilter);
	DECLARE_OBJECT_HANDLE(LZWDecodeFilter);
	DECLARE_OBJECT_HANDLE(JPXDecodeFilter);

	#pragma endregion

	#pragma region Contents

	/* Content instructions */
	DECLARE_OBJECT_HANDLE(ContentObject);
	DECLARE_OBJECT_HANDLE(ContentOperation);
	DECLARE_OBJECT_HANDLE(ContentInstruction);
	DECLARE_OBJECT_HANDLE(ContentInstructionCollection);
	DECLARE_OBJECT_HANDLE(ContentInstructionCollectionIterator);

	/* Content operations */
	DECLARE_OBJECT_HANDLE(ContentOperationGeneric);
	DECLARE_OBJECT_HANDLE(ContentOperationTextFont);
	DECLARE_OBJECT_HANDLE(ContentOperationTextShow);
	DECLARE_OBJECT_HANDLE(ContentOperationTextShowArray);
	DECLARE_OBJECT_HANDLE(ContentOperationBeginText);
	DECLARE_OBJECT_HANDLE(ContentOperationEndText);

	/* Content operators */
	DECLARE_OBJECT_HANDLE(ContentOperator);
	DECLARE_OBJECT_HANDLE(ContentOperatorUnknown);

	/* Content objects */
	DECLARE_OBJECT_HANDLE(ContentObjectText);
	DECLARE_OBJECT_HANDLE(ContentObjectInlineImage);

	/* Content stream parser */
	DECLARE_OBJECT_HANDLE(ContentParser);

	#pragma endregion

	#pragma region Utils

	DECLARE_OBJECT_HANDLE(Buffer);
	DECLARE_OBJECT_HANDLE(BufferArray);
	DECLARE_OBJECT_HANDLE(IUnknown);

	DECLARE_OBJECT_HANDLE(SigningKey);
	DECLARE_OBJECT_HANDLE(EncryptionKey);
	DECLARE_OBJECT_HANDLE(PKCS12Key);

	DECLARE_OBJECT_HANDLE(InputStream);
	DECLARE_OBJECT_HANDLE(OutputStream);
	DECLARE_OBJECT_HANDLE(InputOutputStream);

	#pragma endregion

	#pragma endregion

	#pragma region Documents

	DECLARE_OBJECT_HANDLE(Document);

	DECLARE_OBJECT_HANDLE(Catalog);

	DECLARE_OBJECT_HANDLE(DocumentInfo);

	DECLARE_OBJECT_HANDLE(PageTree);
	DECLARE_OBJECT_HANDLE(PageObject);
	DECLARE_OBJECT_HANDLE(PageContents);

	DECLARE_OBJECT_HANDLE(DeveloperExtensions);
	DECLARE_OBJECT_HANDLE(DeveloperExtensionsIterator);
	DECLARE_OBJECT_HANDLE(DeveloperExtension);

	DECLARE_OBJECT_HANDLE(PageLabels);
	DECLARE_OBJECT_HANDLE(PageLabel);

	DECLARE_OBJECT_HANDLE(ViewerPreferences);
	DECLARE_OBJECT_HANDLE(PageRange);
	DECLARE_OBJECT_HANDLE(PageSubRange);

	DECLARE_OBJECT_HANDLE(OutlineBase);
	DECLARE_OBJECT_HANDLE(Outline);
	DECLARE_OBJECT_HANDLE(OutlineItem);
	DECLARE_OBJECT_HANDLE(OutlineItemColor);
	DECLARE_OBJECT_HANDLE(OutlineItemFlags);

	DECLARE_OBJECT_HANDLE(ResourceDictionary);

	DECLARE_OBJECT_HANDLE(NamedDestinations);
	DECLARE_OBJECT_HANDLE(Destination);

	DECLARE_OBJECT_HANDLE(PageAnnotations);
	DECLARE_OBJECT_HANDLE(Annotation);
	DECLARE_OBJECT_HANDLE(LinkAnnotation);

	DECLARE_OBJECT_HANDLE(InteractiveForm);

	DECLARE_OBJECT_HANDLE(DigitalSignature);
	DECLARE_OBJECT_HANDLE(ByteRangeCollection);
	DECLARE_OBJECT_HANDLE(ByteRange);

	DECLARE_OBJECT_HANDLE(Field);
	DECLARE_OBJECT_HANDLE(FieldCollection);
	DECLARE_OBJECT_HANDLE(ButtonField);
	DECLARE_OBJECT_HANDLE(TextField);
	DECLARE_OBJECT_HANDLE(ChoiceField);
	DECLARE_OBJECT_HANDLE(SignatureField);

	DECLARE_OBJECT_HANDLE(DocumentSigner);
	DECLARE_OBJECT_HANDLE(DocumentSignatureSettings);

	DECLARE_OBJECT_HANDLE(DocumentEncryptionSettings);

	/* Fonts */

	DECLARE_OBJECT_HANDLE(Font);
	DECLARE_OBJECT_HANDLE(Type0Font);
	DECLARE_OBJECT_HANDLE(FontMap);
	DECLARE_OBJECT_HANDLE(CharacterMap);
	DECLARE_OBJECT_HANDLE(EmbeddedCharacterMap);
	DECLARE_OBJECT_HANDLE(UnicodeCharacterMap);

	DECLARE_OBJECT_HANDLE(BaseFontRange);

	/* Common data structures */

	DECLARE_OBJECT_HANDLE(Date);
	DECLARE_OBJECT_HANDLE(Rectangle);

	#pragma endregion

	/** \endcond FORWARD_DECLARATIONS */

#ifdef __cplusplus
};
#endif

#endif /* _C_HANDLES_H */
