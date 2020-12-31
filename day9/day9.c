#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
	OUTSIDE,
	FIRST_TERM,
	SECOND_TERM,
};

static size_t declen(const char *txt)
{
	ssize_t delta = 0;
	size_t len, mul;
	const char *start;
	int state = OUTSIDE;
	const char *s;
	for (s = txt; *s; s++)
	{
		switch (state)
		{
		case OUTSIDE:
			if (*s == '(')
			{
				len = 0;
				start = s;
				state = FIRST_TERM;
			}
			break;

		case FIRST_TERM:
			if (*s != 'x')
			{
				len = len * 10 + *s - '0';
			}
			else
			{
				mul = 0;
				state = SECOND_TERM;
			}
			break;

		case SECOND_TERM:
			if (*s != ')')
			{
				mul = mul * 10 + *s - '0';
			}
			else
			{
				delta += mul * len;
				delta -= (len + s + 1 - start);
				s += len;
				state = OUTSIDE;
			}
			break;
		}
	}
	return s - txt + delta;
}

static size_t declenr(const char *txt, size_t end)
{
	ssize_t delta = 0;
	size_t len, mul;
	const char *start;
	int state = 0;
	const char *s = txt;
	for (size_t i = 0; *s && i < end; i++, s++)
	{
		switch(state)
		{
		case OUTSIDE:
			if (*s == '(')
			{
				len = 0;
				start = s;
				state = FIRST_TERM;
			}
			break;

		case FIRST_TERM:
			if (*s != 'x')
			{
				len = len * 10 + *s - '0';
			}
			else
			{
				mul = 0;
				state = SECOND_TERM;
			}
			break;

		case SECOND_TERM:
			if (*s != ')')
			{
				mul = mul * 10 + *s - '0';
			}
			else
			{
				delta += mul * declenr(s+1, len);
				delta -= (len + s+1 - start);
				s += len;
				i += len;
				state = OUTSIDE;
			}
			break;
		}
	}
	return s - txt + delta;
}

static size_t declen2(const char *txt)
{
	return declenr(txt, -1);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <input>\n", argv[0]);
		return -1;
	}

	FILE *input = fopen(argv[1], "rb");
	if (!input)
	{
		fprintf(stderr, "Cannot open %s for reading\n", argv[1]);
		return -1;
	}

	assert(declen("ADVENT") == 6);
	assert(declen("A(1x5)BC") == 7);
	assert(declen("(3x3)XYZ") == 9);
	assert(declen("A(2x2)BCD(2x2)EFG") == 11);
	assert(declen("(6x1)(1X3)A") == 6);
	assert(declen("X(8x2)(3x3)ABCY") == 18);

	assert(declen2("(3x3)XYZ") == 9);
	assert(declen2("X(8x2)(3x3)ABCY") == 20);
	assert(declen2("(27x12)(20x12)(13x14)(7x10)(1x12)A") == 241920);
	assert(declen2("(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN") == 445);

	char *line = NULL;
	size_t sline = 0;
	int status = getline(&line, &sline, input);
	fclose(input);
	if (status == -1)
	{
		fprintf(stderr, "Cannot load the data\n");
		return -1;
	}

	/* strip the \n */
	size_t len = strlen(line);
	if (line[len-1] == '\n')
	{
		line[len-1] = 0;
	}

	printf("Part1: %zu\n", declen(line));
	printf("Part2: %zu\n", declen2(line));
	free(line);

	return -1;
}
