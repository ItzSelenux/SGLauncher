#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

double evaluate(const char* expression)
{
	char* endptr;
	double result = strtod(expression, &endptr);
	if (errno == ERANGE) return NAN;
	while (*endptr != '\0')
	{
		while (isspace(*endptr)) endptr++;
		char op = *endptr++;
		
		switch (op)
		{
			case 'r': result = sqrt(result); break;
			case 'l': result = log(result); break;
			case 'S': result = asin(result); break;
			case 'C': result = acos(result); break;
			case 'T': result = atan(result); break;

			case 'c':
				if (*endptr == 'h') { endptr++; result = cosh(result); } 
				else result = cos(result);
				break;

			case 't':
				if (*endptr == 'h') { endptr++; result = tanh(result); } 
				else result = tan(result);
				break;

			case 's':
				if (*endptr == 'h') { endptr++; result = sinh(result); } 
				else result = sin(result);
				break;

			case '+': case '-': case '*': case '/': case '^': case '%': case 'b': case 'n':
				{
					double operand = strtod(endptr, &endptr);
					if (errno == ERANGE) return NAN;

					switch (op)
					{
						case '+': result += operand; break;
						case '-': result -= operand; break;
						case '*': result *= operand; break;
						case '/': 
							if (operand != 0) result /= operand; 
							else return NAN;
							break;
						case '^': result = pow(result, operand); break;
						case '%': result = operand / 100.0 * result; break;
						case 'b': 
							if (operand != 0) result = log(result) / log(operand); 
							else return NAN;
							break;
						case 'n': result = pow(result, 1.0 / operand); break;
						default: return NAN;
					}
				}
				break;

			default:
				return NAN;
		}
	}
	return result;
}