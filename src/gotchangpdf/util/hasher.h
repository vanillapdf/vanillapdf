#ifndef _HASHER_H
#define _HASHER_H

namespace gotchangpdf
{
	template <typename T>
	struct Hasher
	{
		unsigned long operator()(const T& t) const
		{
			return 0;
		}

		bool operator()(const T& first, const T& second) const
		{
			return true;
		}
	};
}

#endif /* _BIND_H */
