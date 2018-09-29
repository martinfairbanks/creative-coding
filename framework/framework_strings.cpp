/* Strings */
void strconcat(char *str1, int strCountA, char *str2, int strCountB, char *dest)
{
	for (int i = 0; i < strCountA; i++)
	{
		*dest++ = *str1++;
	}

	for (int i = 0; i < strCountB; i++)
	{
		*dest++ = *str2++;
	}
	//*dest++ = '\n';
	*dest = 0;
}

int strlength(char *string)
{
	/*int i = 0;
	while (token[i] != '\0')
		i++;

	return i;*/

	int length = 0;
	while (*string++)
		length++;

	return length;

}

//strcmp returns 0 if equal
int sstrcmp(char *s1, char *s2)
{
	while (*s1 == *s2)
	{
		//if pointing to a zero value then we have compared the whole string
		if (!*s1)
			return 0;
		++s1;
		++s2;
	}

	/*if (*s1 < *s2)
	return -1;
	else
	return 1;

	same as below
	*/

	return ((*s1 < *s2) ? -1 : 1);
}

struct String
{
	char text[255];

	unsigned int length()
	{
		return strlength(text);
	}

	String operator+(String s)
	{
		String temp;
		strconcat(text, length(), s.text, s.length(), temp.text);
		return temp;
	}
	String operator=(char *s)
	{
		String temp;
		sprintf_s(temp.text, s);
		sprintf_s(text, s);
		return temp;
	}
	String operator+(char *s)
	{
		String temp;
		strconcat(text, length(), s, strlength(s), temp.text);
		return temp;
	}
	String operator+(int number)
	{
		String temp;
		String num{};
		_itoa(number, num.text, 10);
		strconcat(text, length(), num.text, num.length(), temp.text);
		return temp;
	}
};
