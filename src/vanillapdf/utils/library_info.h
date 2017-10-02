#ifndef _LIBRARY_INFO_H
#define _LIBRARY_INFO_H

namespace vanillapdf {

class LibraryInfo {
public:
	static int MajorVersion() noexcept;
	static int MinorVersion() noexcept;
	static int PatchVersion() noexcept;
	static int BuildVersion() noexcept;
	static const char * Author() noexcept;

	static int BuildDay() noexcept;
	static int BuildMonth() noexcept;
	static int BuildYear() noexcept;

private:
	LibraryInfo() = default;
};

} // vanillapdf

#endif /* _LIBRARY_INFO_H */
