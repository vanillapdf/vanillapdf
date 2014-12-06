#include "spirit_parser.h"

#include "exception.h"
#include "file.h"
#include "indirect_object.h"

#include "spirit_lexer.h"
#include "spirit_grammar.h"

#include <iomanip>
//#include <boost/variant/get.hpp>

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

		SpiritParser::SpiritParser(const SpiritParser & other)
			: lexical::Stream(other) { _file = other.file(); }

		IndirectObjectPtr SpiritParser::readObject()
		{
			SpiritLexer lexer;
			SpiritGrammar grammar(lexer);
			IndirectObjectPtr obj = IndirectObject(_file);
			obj->SetOffset(tellg());

			// Don't skip whitespace explicitly
			noskipws(*this);

			// Direct cast to pos_iterator_type is not possible
			base_iterator_type input_begin_base(*this);
			base_iterator_type input_end_base;

			pos_iterator_type input_begin_pos(input_begin_base, input_end_base, _file->GetFilename());
			pos_iterator_type input_end_pos;

			try
			{
				auto result = lex::tokenize_and_parse(input_begin_pos, input_end_pos, lexer, grammar(_file), obj);
				if (result) {
					return obj;
				}
				else {
					auto offset = tellg();
					cout << "Parsing failed" << endl;
					const auto& pos = input_begin_pos.get_position();
					cout <<
						"Error at file " << pos.file << " offset " << offset << endl <<
						"'" << input_begin_pos.get_currentline() << "'" << endl <<
						setw(pos.column + 7) << " ^- here" << endl;

					throw exceptions::Exception("Parsing failed");
				}
			}
			catch (const qi::expectation_failure<lexer_type::iterator_type>& exception) {
				cout << "Parsing failed" << endl;

				auto offset = tellg();
				auto pos_begin = (*exception.first).value().begin();
				const auto& pos = pos_begin.get_position();
				cout <<
					"Error at file " << pos.file << " offset " << offset << " Expecting " << exception.what_ << endl <<
					"'" << pos_begin.get_currentline() << "'" << endl <<
					setw(pos.column + 7) << " ^- here" << endl;

				throw;
			}
		}

		IndirectObjectPtr SpiritParser::peekObject()
		{
			/*
			auto position = tellg();
			auto obj = readObject();
			seekg(position);

			return obj;
			*/
			return IndirectObjectPtr();
		}
		/*
		Deferred<gotchangpdf::Object> SpiritParser::readObjectWithType(gotchangpdf::Object::Type type)
		{

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

			return Deferred<Object>(new IndirectObject(_file));
		}
		*/

		//void Stream::SetDeep( bool deep ) { _deep = deep; }

		//bool Stream::GetDeep( void ) const { return _deep; }
	}
}
