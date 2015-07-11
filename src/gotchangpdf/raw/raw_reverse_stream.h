#ifndef _RAW_REVERSE_STREAM_H
#define _RAW_REVERSE_STREAM_H

#include "raw_base_stream.h"

namespace gotchangpdf
{
	namespace raw
	{

#ifdef _MSC_VER
		/* http://connect.microsoft.com/VisualStudio/feedback/details/733720/ */

		/*
		The story here is somewhat complicated. VC has an obscure compiler option,
		/vd2 (documented at http://msdn.microsoft.com/en-us/library/7sf3txa8.aspx),
		that fixes an obscure bug involving virtual base classes. By default,
		VC does something that's slightly nonconformant to the C++ Standard.
		/vd2 changes VC's behavior to be conformant, but this inherently affects class layout.
		(This layout difference is why the default hasn't been changed to be conformant -
		that would break users attempting to mix code compiled with different major versions of VC.
		Our C++ Standard Library implementation forbids such mixing,
		but the compiler itself is somewhat more permissive.)
		So if users want /vd2, they have to compile everything that way.

		The twist is that the layout bug (which /vd2 fixes) affects iostreams,
		which uses virtual base classes, and our iostreams implementation has a
		separately compiled component (in msvcp100.dll/libcpmt.lib/etc.).
		When MS builds the STL's DLL/LIB, they're compiled the default way,
		without /vd2. As a result, people using /vd2 couldn't use iostreams,
		or they'd get bizarre crashes. Yuck.

		So, we added the do-nothing virtual functions _Add_vtordisp1() and _Add_vtordisp2().
		Their presence makes VC perform layout completely conformantly,
		regardless of /vd2 being used or not, and therefore makes iostreams usable both ways.

		_Add_vtordisp1() and _Add_vtordisp2() trigger warning C4250,
		talking about dominance. This warning is actually completely useless -
		it's saying that the compiler will do exactly what the Standard requires it to do.
		Therefore, we suppress it in the STL's headers (which must be /W4 /analyze clean).
		If you're deriving from fstream, you'll need to suppress this warning in your own code.
		*/
#pragma warning (push)
#pragma warning (disable : 4250)
#endif

		class ReverseStream : public raw::BaseStream::CharacterSource, public raw::BaseStream
		{
		public:
			friend class ReverseBuf;

		public:
			explicit ReverseStream(CharacterSource & stream);
			virtual ~ReverseStream();
			
			virtual void read(BufferPtr& result, types::uinteger len) override;
			virtual BufferPtr read(types::uinteger len) override;
			virtual char get_hex() override;
			virtual BufferPtr readline(void) override;

		private:
			class ReverseBuf : public raw::BaseStream::CharacterSourceBuffer
			{
			public:
				explicit ReverseBuf(CharacterSource & s);
				~ReverseBuf();
				//int sync();

				virtual pos_type seekoff(off_type,
					ios_base::seekdir,
					ios_base::openmode = ios_base::in | ios_base::out) override;

				virtual pos_type seekpos(pos_type,
					ios_base::openmode = ios_base::in | ios_base::out) override;

				/*
				virtual int_type underflow() override;
				virtual int_type uflow() override;
				virtual int_type pbackfail(int_type ch) override;
				virtual std::streamsize showmanyc() override;
				*/
			private:
				Buffer _buffer;
				//CharacterSource & _source;
				//std::streampos _offset;
			};
		};
	}

#ifdef _MSC_VER
#pragma warning (pop)
#endif
}

#endif /* _RAW_REVERSE_STREAM_H */
