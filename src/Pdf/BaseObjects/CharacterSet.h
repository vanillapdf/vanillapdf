#ifndef _CHARACTER_SET_H
#define _CHARACTER_SET_H

#include "Interface/ICharacterSet.h"

#include <vector>

namespace Pdf
{
	class CharacterSet : public ICharacterSet
	{
	public:
		CharacterSet();
		CharacterSet(const char *chars, int len);

		void CharacterSet::Insert(std::vector<Character>::const_iterator it, char* begin, char* end);
		void Insert(std::vector<Character>::const_iterator it, CharacterSet item);
		void PushBack(Character& ch);
		void PushBack(const Character& ch);
		void Reverse();
		long Size() const;
		const Character* Data() const;

		std::vector<Character>::const_iterator begin() const;
		std::vector<Character>::const_iterator end() const;

		bool operator==(const Character& other) const;
		bool operator==(const CharacterSet& other) const;
		bool operator<(const CharacterSet& other) const;
		Character operator[](unsigned int i) const;

		virtual Character At(long at) const override;

	private:
		std::vector<Character> _value;
	};
}

#endif /* _CHARACTER_SET_H */
