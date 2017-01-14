#ifndef _LIBRARY_INFO_H
#define _LIBRARY_INFO_H

namespace gotchangpdf
{
	class LibraryInfo
	{
	public:
		static int MajorVersion() noexcept;
		static int MinorVersion() noexcept;
		static int PatchVersion() noexcept;
		static const char * Author() noexcept;

	private:
		LibraryInfo() = default;
	};
}

#endif /* _LIBRARY_INFO_H */
