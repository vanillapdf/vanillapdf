#include "file.h"
#include "xref.h"
#include "indirect_object.h"
#include "catalog.h"

#include "c_file.h"
#include "log.h"

using namespace gotchangpdf::files;

GOTCHANG_PDF_API FileHandle CALLING_CONVENTION File_Create(const char *filename)
{
	LOG_SCOPE(filename);

	try
	{
		auto file = new File(filename);
		return reinterpret_cast<FileHandle>(file);
	}
	catch (...)
	{
		// TODO log
		std::exception_ptr active_exception = std::current_exception();
		return nullptr;
	}
}

GOTCHANG_PDF_API void CALLING_CONVENTION File_Release(FileHandle handle)
{
	File* file = reinterpret_cast<File*>(handle);
	LOG_SCOPE(file->GetFilename());

	delete file;
}

GOTCHANG_PDF_API int CALLING_CONVENTION File_Initialize(FileHandle handle)
{
	File* file = reinterpret_cast<File*>(handle);
	LOG_SCOPE(file->GetFilename());

	// TODO
	file->Initialize();
	return 0;
}

GOTCHANG_PDF_API XrefHandle CALLING_CONVENTION File_Xref(FileHandle handle)
{
	File* file = reinterpret_cast<File*>(handle);
	LOG_SCOPE(file->GetFilename());

	auto table = file->GetXref();
	auto ptr = table.AddRefGet();

	return reinterpret_cast<XrefHandle>(ptr);
}

GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION File_GetIndirectObject(
	FileHandle handle, int objNumber, int genNumber)
{
	File* file = reinterpret_cast<File*>(handle);
	LOG_SCOPE(file->GetFilename());

	auto item = file->GetIndirectObject(objNumber, genNumber);
	auto ptr = item.AddRefGet();

	//boost::intrusive_ptr_add_ref(ptr);
	return reinterpret_cast<IndirectObjectHandle>(ptr);
}

GOTCHANG_PDF_API CatalogHandle CALLING_CONVENTION File_GetDocumentCatalog(
	FileHandle handle)
{
	File* file = reinterpret_cast<File*>(handle);
	LOG_SCOPE(file->GetFilename());

	auto item = file->GetDocumentCatalog();
	auto ptr = item.AddRefGet();

	//boost::intrusive_ptr_add_ref(ptr);
	return reinterpret_cast<CatalogHandle>(ptr);
}