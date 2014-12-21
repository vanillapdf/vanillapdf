#ifndef _ITER_OFFSET_PARSER_H
#define _ITER_OFFSET_PARSER_H

#include "constants.h"

#include <boost/spirit/include/qi_parse.hpp>

///////////////////////////////////////////////////////////////////////////////
// definition the place holder 
namespace boost {
	namespace spirit {
		namespace repository {
			namespace qi
			{
				BOOST_SPIRIT_TERMINAL(iter_offset);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// implementation the enabler
namespace boost {
	namespace spirit
	{
		// We want custom_parser::iter_pos to be usable as a terminal only, 
		// and only for parser expressions (qi::domain).
		template <>
		struct use_terminal<qi::domain, repository::qi::tag::iter_offset>
			: mpl::true_
		{};
	}
}

///////////////////////////////////////////////////////////////////////////////
// implementation of the parser
namespace boost {
	namespace spirit {
		namespace repository {
			namespace qi
			{
				struct iter_offset_parser
					: boost::spirit::qi::primitive_parser<iter_offset_parser>
				{
					// Define the attribute type exposed by this parser component
					template <typename Context, typename Iterator>
					struct attribute
					{
						typedef gotchangpdf::types::stream_offset type;
					};

					// This function is called during the actual parsing process
					template <typename Iterator, typename Context
						, typename Skipper, typename Attribute>
						bool parse(Iterator& first, Iterator const& last
						, Context&, Skipper const& skipper, Attribute& attr) const
					{
							boost::spirit::qi::skip_over(first, last, skipper);

							const auto& pos = first.get_position();
							boost::spirit::traits::assign_to(pos.offset, attr);
							return true;
						}

					// This function is called during error handling to create
					// a human readable string for the error context.
					template <typename Context>
					boost::spirit::info what(Context&) const
					{
						return boost::spirit::info("iter_offset_parser");
					}
				};
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// instantiation of the parser
namespace boost {
	namespace spirit {
		namespace qi
		{
			// This is the factory function object invoked in order to create 
			// an instance of our iter_offset_parser.
			template <typename Modifiers>
			struct make_primitive<repository::qi::tag::iter_offset, Modifiers>
			{
				typedef repository::qi::iter_offset_parser result_type;

				result_type operator()(unused_type, unused_type) const
				{
					return result_type();
				}
			};
		}
	}
}

#endif /* _ITER_OFFSET_PARSER_H */
