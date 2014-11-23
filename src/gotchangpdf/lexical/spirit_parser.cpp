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
		using namespace std;
		using namespace exceptions;

		namespace qi = boost::spirit::qi;
		namespace lex = boost::spirit::lex;

		SpiritParser::SpiritParser(files::File * file, CharacterSource & stream)
			: lexical::Stream(stream), _file(file) {}

		SpiritParser::SpiritParser(const gotchangpdf::lexical::Parser &other)
			: lexical::Stream(other) { _file = other.file(); }

		SmartPtr<Object> SpiritParser::readObject()
		{
			SpiritLexer lexer;
			SpiritGrammar grammar(lexer);
			ast::IndirectObject obj;

			//auto off = tellg();
			//auto pos = off.seekpos();

			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, _file->GetFilename());
			pos_iterator_type input_end_pos;

			try
			{
				auto result = lex::tokenize_and_parse(input_begin_pos, input_end_pos, lexer, grammar, obj);
				if (result) {
					return nullptr;
				}
				else {
					//auto off2 = tellg();
					//auto pos2 = off2.seekpos();
					cout << "Parsing failed" << endl;
					const auto& pos = input_begin_pos.get_position();
					cout <<
						"Error at file " << pos.file << " line " << pos.line << " column " << pos.column << endl <<
						"'" << input_begin_pos.get_currentline() << "'" << endl <<
						setw(pos.column + 8) << " ^- here" << endl;

					return nullptr;
				}
			}
			catch (const qi::expectation_failure<lexer_type::iterator_type>& exception) {
				cout << "Parsing failed" << endl;

				//auto off2 = tellg();
				//auto pos2 = off2.seekpos();
				auto pos_begin = (*exception.first).value().begin();
				const auto& pos = pos_begin.get_position();
				cout <<
					"Error at file " << pos.file << " line " << pos.line << " column " << pos.column << " Expecting " << exception.what_ << endl <<
					"'" << pos_begin.get_currentline() << "'" << endl <<
					setw(pos.column + 8) << " ^- here" << endl;

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
