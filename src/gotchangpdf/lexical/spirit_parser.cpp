#include "spirit_parser.h"

#include "exception.h"
#include "file.h"

#include "stream_object.h"
#include "boolean.h"
#include "null_object.h"
#include "Function.h"
#include "indirect_object.h"
#include "dictionary_object.h"
#include "array_object.h"
#include "integer_object.h"
#include "stream_object.h"
#include "real_object.h"
#include "string_object.h"
#include "indirect_object_reference.h"

#include "spirit_lexer.h"
#include "spirit_grammar.h"

#include <iomanip>

namespace gotchangpdf
{
	namespace lexical
	{
		using namespace lexical;
		using namespace exceptions;

		namespace qi = boost::spirit::qi;

		SpiritParser::SpiritParser(files::File * file, CharacterSource & stream)
			: lexical::Stream(stream), _file(file) {}

		SpiritParser::SpiritParser(const gotchangpdf::lexical::Parser &other)
			: lexical::Stream(other) { _file = other.file(); }

		SmartPtr<Object> SpiritParser::readObject()
		{
			lexical::SpiritLexer lexer;
			lexical::SpiritGrammar grammar(lexer);
			ast::IndirectObject obj;

			std::noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			//lexical::base_iterator_type input_begin_base(*this);
			//lexical::base_iterator_type input_end_base;

			//lexical::pos_iterator_type input_begin_pos(input_begin_base, input_end_base, "aa");
			//lexical::pos_iterator_type input_end_pos;

			//


			std::string str("1 0 obj\n"
				"[/YES 2]\n"
				"endobj");

			std::stringstream ss(str);

			std::noskipws(ss);

			lexical::base_iterator_type str_beg(ss);
			lexical::base_iterator_type str_end;

			lexical::pos_iterator_type str_beg_fwd(str_beg, str_end, "aa");
			lexical::pos_iterator_type str_end_fwd;

			//

			try
			{
				//auto result = boost::spirit::lex::tokenize_and_phrase_parse(input_begin_pos, input_end_pos, lexer, grammar, qi::in_state("WS")[lexer.self], obj);
				auto result = boost::spirit::lex::tokenize_and_parse(str_beg_fwd, str_end_fwd, lexer, grammar, obj);
				if (result) {
					return nullptr;
				}
				else {

					std::cout << "Parsing failed" << std::endl;
					const auto& pos = str_beg_fwd.get_position();
					std::cout <<
						"Error at file " << pos.file << " line " << pos.line << " column " << pos.column << std::endl <<
						"'" << str_beg_fwd.get_currentline() << "'" << std::endl <<
						std::setw(pos.column) << " ^- here" << std::endl;

					return nullptr;
				}
			}
			catch (const qi::expectation_failure<lexer_type::iterator_type>& exception) {
				std::cout << "Parsing failed" << std::endl;

				auto pos_begin = (*exception.first).value().begin();
				const auto& pos = pos_begin.get_position();
				std::cout <<
					"Error at file " << pos.file << " line " << pos.line << " column " << pos.column << " Expecting " << exception.what_ << std::endl <<
					"'" << pos_begin.get_currentline() << "'" << std::endl <<
					std::setw(pos.column) << " ^- here" << std::endl;

				return nullptr;
			}

			return SmartPtr<Object>(new IndirectObject(_file, 1, 2));
		}

		SmartPtr<Object> SpiritParser::peekObject()
		{
			/*
			auto position = tellg();
			auto obj = readObject();
			seekg(position);

			return obj;
			*/
			return SmartPtr<Object>(new IndirectObject(_file, 1, 2));
		}

		SmartPtr<gotchangpdf::Object> SpiritParser::readObjectWithType(gotchangpdf::Object::Type type)
		{
			/*
			auto obj = readObject();

			switch (type)
			{
			case Object::Type::Unknown:
				throw Exception("FIXME: Are your really trying to return unknown type??");
			case Object::Type::ArrayObject:
				return dynamic_wrapper_cast<MixedArrayObject>(obj);
			case Object::Type::Boolean:
				return dynamic_wrapper_cast<Boolean>(obj);
			case Object::Type::DictionaryObject:
				return dynamic_wrapper_cast<DictionaryObject>(obj);
			case Object::Type::Function:
				return dynamic_wrapper_cast<Function>(obj);
			case Object::Type::IntegerObject:
				return dynamic_wrapper_cast<IntegerObject>(obj);
			case Object::Type::NameObject:
				return dynamic_wrapper_cast<NameObject>(obj);
			case Object::Type::NullObject:
				return dynamic_wrapper_cast<NullObject>(obj);
			case Object::Type::RealObject:
				return dynamic_wrapper_cast<RealObject>(obj);
			case Object::Type::StreamObject:
				return dynamic_wrapper_cast<StreamObject>(obj);
			case Object::Type::HexadecimalString:
				return dynamic_wrapper_cast<HexadecimalString>(obj);
			case Object::Type::LiteralString:
				return dynamic_wrapper_cast<LiteralString>(obj);
			case Object::Type::IndirectObjectReference:
				return dynamic_wrapper_cast<IndirectObjectReference>(obj);
			case Object::Type::IndirectObject:
				return dynamic_wrapper_cast<IndirectObject>(obj);
			default:
				assert(false);
				throw Exception("FIXME: Unknown object type");
			}
			*/
			return SmartPtr<Object>(new IndirectObject(_file, 1, 2));
		}

		//void Stream::SetDeep( bool deep ) { _deep = deep; }

		//bool Stream::GetDeep( void ) const { return _deep; }
	}
}
