#include "precompiled.h"
#include "header.h"

#include "raw_stream.h"
#include "exception.h"

#include <cassert>
#include <vector>
#include <regex>

namespace gotchangpdf
{
	namespace syntax
	{
		using namespace std;

		Stream& operator>> (Stream& s, Header& o)
		{
			auto data = s.readline();
			assert(data->size() > 0);

			string tmp = data->ToString();

			std::smatch sm;
			std::regex header_regex("%PDF-([0-9])\\.([0-9]).*");
			if (std::regex_match(tmp, sm, header_regex))
			{
				assert(sm.size() == 3);

				if (1 != stoi(sm[1]))
					throw Exception("Invalid PDF version " + stoi(sm[1]) + stoi(sm[2]));

				switch (stoi(sm[2]))
				{
				case 0:
					o._version = Version::PDF10;
					break;
				case 1:
					o._version = Version::PDF11;
					break;
				case 2:
					o._version = Version::PDF12;
					break;
				case 3:
					o._version = Version::PDF13;
					break;
				case 4:
					o._version = Version::PDF14;
					break;
				case 5:
					o._version = Version::PDF15;
					break;
				case 6:
					o._version = Version::PDF16;
					break;
				case 7:
					o._version = Version::PDF17;
					break;
				default:
					throw Exception("Invalid PDF version " + stoi(sm[1]) + stoi(sm[2]));
				}
			}
			else
			{
				throw Exception("Could not find a valid PDF header");
			}

			return s;
		}

		Version Header::GetVersion(void) const { return _version; }
	}
}
