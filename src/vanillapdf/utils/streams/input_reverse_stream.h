#ifndef _INPUT_REVERSE_STREAM_H
#define _INPUT_REVERSE_STREAM_H

#include "utils/streams/input_stream_interface.h"

#include <mutex>

namespace vanillapdf {

#if defined(COMPILER_MICROSOFT_VISUAL_STUDIO)
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
#endif /* COMPILER_MICROSOFT_VISUAL_STUDIO */

class InputReverseStream : public IInputStream {
public:
	friend class ReverseBuf;

public:
	explicit InputReverseStream(IInputStreamPtr stream, types::stream_size size);

	virtual types::stream_size Read(char* result, types::stream_size len) override;
	virtual types::stream_size Read(Buffer& result, types::stream_size len) override;
	virtual BufferPtr Read(types::stream_size len) override;
	virtual BufferPtr Readline(void) override;
	virtual types::stream_size GetInputPosition() override;
	virtual void SetInputPosition(types::stream_size pos) override;
	virtual void SetInputPosition(types::stream_size pos, SeekDirection way) override;

	virtual void ExclusiveInputLock() override;
	virtual void ExclusiveInputUnlock() override;

	virtual bool Eof(void) const override;
	virtual bool Ignore(void) override;
	virtual int Get(void) override;
	virtual int Peek(void) override;

	virtual bool IsFail(void) const override;

	virtual operator bool(void) const override;

private:
	class ReverseBuf : public std::streambuf {
	public:
		explicit ReverseBuf(IInputStreamPtr stream, types::stream_size size);

		virtual pos_type seekoff(off_type,
			std::ios_base::seekdir,
			std::ios_base::openmode = std::ios_base::in | std::ios_base::out) override;

		virtual pos_type seekpos(pos_type,
			std::ios_base::openmode = std::ios_base::in | std::ios_base::out) override;

	public:
		int sync();
		virtual int_type underflow() override;
		virtual int_type uflow() override;
		virtual int_type pbackfail(int_type ch) override;
		virtual std::streamsize showmanyc() override;

	private:
		IInputStreamPtr m_stream;
		types::stream_offset _offset;
		types::stream_size _size;
		types::size_type _put_back;
		char *_base;

	private:
		Buffer _buffer;
	};

protected:
	std::unique_ptr<std::istream> m_stream;
	std::unique_ptr<ReverseBuf> m_buffer;
	std::unique_ptr<std::recursive_mutex> m_input_lock;
};

#if defined(COMPILER_MICROSOFT_VISUAL_STUDIO)
	#pragma warning (pop)
#endif /* COMPILER_MICROSOFT_VISUAL_STUDIO */

} // vanillapdf

#endif /* _INPUT_REVERSE_STREAM_H */
