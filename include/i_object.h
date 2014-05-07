#ifndef _I_OBJECT_H
#define _I_OBJECT_H

namespace gotchangpdf
{
	class IObject
	{
	public:
		enum class Type : unsigned char
		{
			Unknown = 0,
			ArrayObject,
			Boolean,
			DictionaryObject,
			Function,
			IntegerObject,
			NameObject,
			//NameTree,
			NullObject,
			//NumberTree,
			RealObject,
			//Rectangle,
			StreamObject,
			HexadecimalString,
			LiteralString,
			IndirectReference,
			IndirectObject
		};

		static const char* TypeName(Type type)
		{
			// TODO this should be a single line solution using preprocessor

			switch (type)
			{
			case Type::Unknown:
				return "Unknown";
			case Type::ArrayObject:
				return "ArrayObject";
			case Type::Boolean:
				return "Boolean";
			case Type::DictionaryObject:
				return "DictionaryObject";
			case Type::Function:
				return "Function";
			case Type::IntegerObject:
				return "IntegerObject";
			case Type::NameObject:
				return "NameObject";
			//case Type::NameTree:
			//	return "NameTree";
			case Type::NullObject:
				return "NullObject";
				//case Type::NumberTree:
				//	return "NumberTree";
			case Type::RealObject:
				return "RealObject";
				//case Type::Rectangle:
				//	return "Rectangle";
			case Type::StreamObject:
				return "StreamObject";
			case Type::HexadecimalString:
				return "HexadecimalString";
			case Type::LiteralString:
				return "LiteralString";
			case Type::IndirectReference:
				return "IndirectReference";
			case Type::IndirectObject:
				return "IndirectObject";
			default:
				return nullptr;
			}
		}

		virtual Type GetType(void) const = 0;
		virtual void Release() = 0;

	private:
		//IObject();
	};
}

#endif /* _I_OBJECT_H */
