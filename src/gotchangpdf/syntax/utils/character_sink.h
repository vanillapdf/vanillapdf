#ifndef _CHARACTER_SINK_H
#define _CHARACTER_SINK_H

#include <ostream>
#include <boost/iostreams/filtering_stream.hpp>

namespace gotchangpdf
{
	typedef std::ostream CharacterSink;
	typedef boost::iostreams::filtering_ostream CharacterFilteringSink;
}

#endif /* _CHARACTER_SINK_H */
