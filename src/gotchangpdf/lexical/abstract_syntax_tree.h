#ifndef _ABSTRACT_SYNTAX_TREE_H
#define _ABSTRACT_SYNTAX_TREE_H

#include "object.h"
#include "token.h"
#include "object_reference_wrapper.h"

#include <vector>
#include <map>

namespace gotchangpdf
{
	namespace lexical
	{
		/*
		class ParseTree
		{
		public:
			virtual ~ParseTree() {};
		};

		class ObjectAST : public ParseTree
		{
		public:
			virtual ObjectReferenceWrapper<Object> Value() const = 0;
		};

		class NameObjectAST : public ObjectAST
		{
		public:
			virtual ObjectReferenceWrapper<Object> Value() const override;

		public:
			Token _value;
		};

		class IndirectObjectAST : public ObjectAST
		{
		public:
			virtual ObjectReferenceWrapper<Object> Value() const override;

		public:
			Token _obj, _gen;
			ObjectAST *child;
		};

		class IntegerObjectAST : public ObjectAST
		{
		public:
			virtual ObjectReferenceWrapper<Object> Value() const override;

		public:
			Token _value;
		};

		class DictionaryObjectAST : public ObjectAST
		{
		public:
			virtual ObjectReferenceWrapper<Object> Value() const override;

		public:
			std::map<std::string, ObjectAST*> _list;
		};

		class ArrayObjectAST : public ObjectAST
		{
		public:
			virtual ObjectReferenceWrapper<Object> Value() const override;

		public:
			std::vector<ObjectAST*> _list;
			bool _single_typed;
			Object::Type _single_type;
		};

		class SemanticAnalyzer
		{
		public:
			static void Analyze(ParseTree& tree);
		};
		*/
	}
}

#endif /* _ABSTRACT_SYNTAX_TREE_H */
