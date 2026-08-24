#include "tools.h"

namespace {

struct ExtractOptions {
    std::string source_file;
};

error_type write_buffer_to_file(const std::string& output_filename, BufferHandle* data) {
    OutputStreamGuard output_stream;

    RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_CreateFromFile(output_filename.c_str(), output_stream.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_WriteBuffer(output_stream, data));
    RETURN_ERROR_IF_NOT_SUCCESS(OutputStream_Flush(output_stream));

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type process_stream(StreamObjectHandle* stream, biguint_type object_number, ushort_type generation_number) {
    DictionaryObjectGuard stream_dictionary;

    ObjectType type_object_type;
    ObjectType subtype_object_type;

    ObjectGuard type_object;
    ObjectGuard subtype_object;

    NameObjectGuard type_name;
    NameObjectGuard subtype_name;

    boolean_type contains_type = VANILLAPDF_RV_FALSE;
    boolean_type contains_subtype = VANILLAPDF_RV_FALSE;
    boolean_type contains_width = VANILLAPDF_RV_FALSE;
    boolean_type contains_height = VANILLAPDF_RV_FALSE;
    boolean_type contains_colorspace = VANILLAPDF_RV_FALSE;

    boolean_type is_type_xobject = VANILLAPDF_RV_FALSE;
    boolean_type is_subtype_image = VANILLAPDF_RV_FALSE;

    bool processed_with_params = false;

    RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetHeader(stream, stream_dictionary.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Contains(stream_dictionary, NameConstant_Type, &contains_type));
    RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Contains(stream_dictionary, NameConstant_Subtype, &contains_subtype));

    // Only image XObjects are extracted; anything else is skipped silently
    if (!contains_type || !contains_subtype) {
        return VANILLAPDF_TOOLS_ERROR_SUCCESS;
    }

    RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Find(stream_dictionary, NameConstant_Type, type_object.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Find(stream_dictionary, NameConstant_Subtype, subtype_object.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(Object_GetObjectType(type_object, &type_object_type));
    RETURN_ERROR_IF_NOT_SUCCESS(Object_GetObjectType(subtype_object, &subtype_object_type));

    if (type_object_type != ObjectType_Name || subtype_object_type != ObjectType_Name) {
        return VANILLAPDF_TOOLS_ERROR_SUCCESS;
    }

    RETURN_ERROR_IF_NOT_SUCCESS(NameObject_FromObject(type_object, type_name.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(NameObject_FromObject(subtype_object, subtype_name.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Equals(type_name, NameConstant_XObject, &is_type_xobject));
    RETURN_ERROR_IF_NOT_SUCCESS(NameObject_Equals(subtype_name, NameConstant_Image, &is_subtype_image));

    if (!is_type_xobject || !is_subtype_image) {
        return VANILLAPDF_TOOLS_ERROR_SUCCESS;
    }

    std::string output_filename = std::to_string(object_number) + "." + std::to_string(generation_number) + ".jpeg";

    RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Contains(stream_dictionary, NameConstant_Width, &contains_width));
    RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Contains(stream_dictionary, NameConstant_Height, &contains_height));
    RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Contains(stream_dictionary, NameConstant_ColorSpace, &contains_colorspace));

    if (contains_width && contains_height && contains_colorspace) {
        ObjectType width_object_type;
        ObjectType height_object_type;
        ObjectType colorspace_object_type;

        ObjectGuard width_object;
        ObjectGuard height_object;
        ObjectGuard colorspace_object;

        RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Find(stream_dictionary, NameConstant_Width, width_object.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Find(stream_dictionary, NameConstant_Height, height_object.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Find(stream_dictionary, NameConstant_ColorSpace, colorspace_object.out()));

        RETURN_ERROR_IF_NOT_SUCCESS(Object_GetObjectType(width_object, &width_object_type));
        RETURN_ERROR_IF_NOT_SUCCESS(Object_GetObjectType(height_object, &height_object_type));
        RETURN_ERROR_IF_NOT_SUCCESS(Object_GetObjectType(colorspace_object, &colorspace_object_type));

        if (width_object_type == ObjectType_Integer && height_object_type == ObjectType_Integer && colorspace_object_type == ObjectType_Name) {
            DCTDecodeFilterGuard encoding_filter;
            DictionaryObjectGuard encoding_dictionary;
            BufferGuard decoded_body;
            BufferGuard encoded_body;

            RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_Create(encoding_dictionary.out()));
            RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_InsertConst(encoding_dictionary, NameConstant_Width, width_object, VANILLAPDF_RV_FALSE));
            RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_InsertConst(encoding_dictionary, NameConstant_Height, height_object, VANILLAPDF_RV_FALSE));
            RETURN_ERROR_IF_NOT_SUCCESS(DictionaryObject_InsertConst(encoding_dictionary, NameConstant_ColorSpace, colorspace_object, VANILLAPDF_RV_FALSE));

            RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetBody(stream, decoded_body.out()));

            RETURN_ERROR_IF_NOT_SUCCESS(DCTDecodeFilter_Create(encoding_filter.out()));
            RETURN_ERROR_IF_NOT_SUCCESS(DCTDecodeFilter_EncodeParams(encoding_filter, decoded_body, encoding_dictionary, encoded_body.out()));

            RETURN_ERROR_IF_NOT_SUCCESS(write_buffer_to_file(output_filename, encoded_body));

            processed_with_params = true;
        }
    }

    if (!processed_with_params) {
        BufferGuard encoded_body;

        RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_GetBodyRaw(stream, encoded_body.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(write_buffer_to_file(output_filename, encoded_body));
    }

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type process_object(ObjectHandle* obj, biguint_type object_number, ushort_type generation_number) {

    ObjectType type = ObjectType_Undefined;

    RETURN_ERROR_IF_NOT_SUCCESS(Object_GetObjectType(obj, &type));

    if (type == ObjectType_Stream) {
        StreamObjectGuard stream;

        RETURN_ERROR_IF_NOT_SUCCESS(StreamObject_FromObject(obj, stream.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(process_stream(stream, object_number, generation_number));
    }

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type process_xref(XrefHandle* xref) {
    XrefIteratorGuard xref_iterator;
    boolean_type valid = VANILLAPDF_RV_FALSE;

    RETURN_ERROR_IF_NOT_SUCCESS(Xref_GetIterator(xref, xref_iterator.out()));

    while (VANILLAPDF_ERROR_SUCCESS == XrefIterator_IsValid(xref_iterator, &valid)
        && VANILLAPDF_RV_TRUE == valid) {

        XrefEntryType type = XrefEntryType_Null;
        XrefEntryGuard entry;

        biguint_type object_number = 0;
        ushort_type generation_number = 0;

        RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_GetValue(xref_iterator, entry.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GetType(entry, &type));
        RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GetObjectNumber(entry, &object_number));
        RETURN_ERROR_IF_NOT_SUCCESS(XrefEntry_GetGenerationNumber(entry, &generation_number));

        if (type == XrefEntryType_Used) {
            XrefUsedEntryGuard used_entry;
            ObjectGuard obj;

            RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_FromEntry(entry, used_entry.out()));
            RETURN_ERROR_IF_NOT_SUCCESS(XrefUsedEntry_GetReference(used_entry, obj.out()));
            RETURN_ERROR_IF_NOT_SUCCESS(process_object(obj, object_number, generation_number));
        }

        if (type == XrefEntryType_Compressed) {
            XrefCompressedEntryGuard compressed_entry;
            ObjectGuard obj;

            RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_FromEntry(entry, compressed_entry.out()));
            RETURN_ERROR_IF_NOT_SUCCESS(XrefCompressedEntry_GetReference(compressed_entry, obj.out()));
            RETURN_ERROR_IF_NOT_SUCCESS(process_object(obj, object_number, generation_number));
        }

        RETURN_ERROR_IF_NOT_SUCCESS(XrefIterator_Next(xref_iterator));
    }

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type process_file(FileHandle* file) {
    XrefChainGuard chain;
    XrefChainIteratorGuard chain_iterator;
    boolean_type valid = VANILLAPDF_RV_FALSE;

    RETURN_ERROR_IF_NOT_SUCCESS(File_XrefChain(file, chain.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(XrefChain_GetIterator(chain, chain_iterator.out()));

    while (VANILLAPDF_ERROR_SUCCESS == XrefChainIterator_IsValid(chain_iterator, &valid)
        && VANILLAPDF_RV_TRUE == valid) {
        XrefGuard xref;

        RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_GetValue(chain_iterator, xref.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(process_xref(xref));
        RETURN_ERROR_IF_NOT_SUCCESS(XrefChainIterator_Next(chain_iterator));
    }

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

error_type process_page_contents(PageContentsHandle* page_contents, size_type page_number) {
    ContentInstructionCollectionGuard content_instruction_collection;

    size_type i = 0;
    size_type contents_size = 0;

    RETURN_ERROR_IF_NOT_SUCCESS(PageContents_GetInstructionCollection(page_contents, content_instruction_collection.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(ContentInstructionCollection_GetSize(content_instruction_collection, &contents_size));

    for (i = 0; i < contents_size; ++i) {
        ContentInstructionType instruction_type = ContentInstructionType_Undefined;
        ContentObjectType object_type = ContentObjectType_Undefined;
        ContentInstructionGuard content_instruction;
        ContentObjectGuard content_object;
        ContentObjectInlineImageGuard content_image;
        DictionaryObjectGuard content_image_dictionary;
        BufferGuard content_image_data;

        RETURN_ERROR_IF_NOT_SUCCESS(ContentInstructionCollection_At(content_instruction_collection, i, content_instruction.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(ContentInstruction_GetInstructionType(content_instruction, &instruction_type));

        if (instruction_type != ContentInstructionType_Object) {
            continue;
        }

        RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_FromInstruction(content_instruction, content_object.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(ContentObject_GetObjectType(content_object, &object_type));

        if (object_type != ContentObjectType_InlineImage) {
            continue;
        }

        RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectInlineImage_FromContentObject(content_object, content_image.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectInlineImage_GetDictionary(content_image, content_image_dictionary.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(ContentObjectInlineImage_GetData(content_image, content_image_data.out()));

        std::string output_filename = std::to_string(page_number) + "." + std::to_string(i);

        RETURN_ERROR_IF_NOT_SUCCESS(write_buffer_to_file(output_filename, content_image_data));
    }

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

int process_extract(const ExtractOptions& options) {
    size_type i = 0;
    size_type page_count = 0;

    FileGuard file;
    DocumentGuard document;
    CatalogGuard catalog;
    PageTreeGuard tree;

    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(options.source_file.c_str(), file.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));

    RETURN_ERROR_IF_NOT_SUCCESS(process_file(file));

    RETURN_ERROR_IF_NOT_SUCCESS(Document_OpenFile(file, document.out()));

    RETURN_ERROR_IF_NOT_SUCCESS(Document_GetCatalog(document, catalog.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(Catalog_GetPages(catalog, tree.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPageCount(tree, &page_count));

    for (i = 0; i < page_count; ++i) {
        PageContentsGuard page_contents;
        PageObjectGuard page_object;

        RETURN_ERROR_IF_NOT_SUCCESS(PageTree_GetPage(tree, i + 1, page_object.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(PageObject_GetContents(page_object, page_contents.out()));

        RETURN_ERROR_IF_NOT_SUCCESS(process_page_contents(page_contents, i + 1));
    }

    return VANILLAPDF_TOOLS_ERROR_SUCCESS;
}

} // namespace

void register_extract(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<ExtractOptions>();
    auto* command = app.add_subcommand("extract", "Extract images from a PDF document");

    command->add_option("-s,--source", options->source_file, "Source file")->required();

    command->callback([options, &exit_code]() {
        exit_code = process_extract(*options);
    });
}
