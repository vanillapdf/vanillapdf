#ifndef _OFFSET_ITERATOR_H
#define _OFFSET_ITERATOR_H

#include <boost/spirit/include/classic_position_iterator.hpp>

namespace gotchangpdf
{
	namespace lexical
	{
		template
		<
			typename ForwardIteratorT,
			typename PositionT
		>
		class offset_iterator : public boost::spirit::classic::position_iterator<ForwardIteratorT, PositionT, offset_iterator<ForwardIteratorT, PositionT>>
		{
		public:
			using base_t = boost::spirit::classic::position_iterator<ForwardIteratorT, PositionT, offset_iterator<ForwardIteratorT, PositionT>>;

		public:
			typedef typename base_t::value_type value_type;
			typedef PositionT position_t;

			offset_iterator() {}

			offset_iterator(
				const ForwardIteratorT& begin,
				const ForwardIteratorT& end) :
				base_t(begin, end),
				_startline(begin)
			{}

			template <typename FileNameT>
			offset_iterator(
				const ForwardIteratorT& begin,
				const ForwardIteratorT& end,
				FileNameT file) :
				base_t(begin, end, file),
				_startline(begin)
			{}

			template <typename FileNameT, typename LineT>
			offset_iterator(
				const ForwardIteratorT& begin,
				const ForwardIteratorT& end,
				FileNameT file, LineT line) :
				base_t(begin, end, file, line),
				_startline(begin)
			{}

			template <typename FileNameT, typename LineT, typename ColumnT>
			offset_iterator(
				const ForwardIteratorT& begin,
				const ForwardIteratorT& end,
				FileNameT file, LineT line, ColumnT column) :
				base_t(begin, end, file, line, column),
				_startline(begin)
			{}

			template <typename FileNameT, typename LineT, typename ColumnT, typename OffsetT>
			offset_iterator(
				const ForwardIteratorT& begin,
				const ForwardIteratorT& end,
				FileNameT file, LineT line, ColumnT column, OffsetT offset) :
				base_t(begin, end, PositionT(file, line, column, offset)),
				_startline(begin)
			{}

			offset_iterator(
				const ForwardIteratorT& begin,
				const ForwardIteratorT& end,
				const PositionT& pos) :
				base_t(begin, end, pos),
				_startline(begin)
			{}

			offset_iterator(const offset_iterator& iter)
				: base_t(iter), _startline(iter._startline) {}

			offset_iterator& operator=(const offset_iterator& iter)
			{
				base_t::operator=(iter);
				_startline = iter._startline;
				return *this;
			}

			ForwardIteratorT get_currentline_begin(void) const
			{
				return _startline;
			}

			ForwardIteratorT get_currentline_end(void) const
			{
				return get_endline();
			}

			std::basic_string<value_type> get_currentline(void) const
			{
				return std::basic_string<value_type>
					(get_currentline_begin(), get_currentline_end());
			}

		private:
			ForwardIteratorT _startline;

			friend class base_t;

			ForwardIteratorT get_endline() const
			{
				ForwardIteratorT endline = _startline;
				while (endline != this->_end && *endline != '\r' && *endline != '\n')
					++endline;

				return endline;
			}

			void newline(void) { _startline = this->base(); }
		};
	}
}

#endif /* _OFFSET_ITERATOR_H */
