double evaluate(const char* expression)
{
	char* endptr;
	double result = strtod(expression, &endptr), operand;
	char op;

	while (*endptr != '\0')
	{
		while (isspace(*endptr)) endptr++;
		op = *endptr++;
		if (*endptr == '\0') break;
		operand = strtod(endptr, &endptr);

		switch (op)
		{
			case '+': result += operand; break;
			case '-': result -= operand; break;
			case '*': result *= operand; break;
			case '/': 
				if (operand != 0) 
					result /= operand; 
				else 
					return NAN;
				break;
			case '^': result = pow(result, operand); break;
			case '%': result = operand / 100.0 * result; break;
			case 'r': result = sqrt(result); break;
			case 'c': 
				if (*endptr == 'h')
				{
					endptr++;
					result = cosh(result);
				}
				else
				{
					result = cos(result);
				}
				break;
			case 't': 
				if (*endptr == 'h')
				{
					endptr++;
					result = tanh(result);
				}
				else
				{
					result = tan(result);
				}
				break;
			case 's': 
				if (*endptr == 'h')
				{
					endptr++;
					result = sinh(result);
				}
				else
				{
					result = sin(result);
				}
				break;
			case 'l': result = log(result); break;
			case 'b': 
				if (operand != 0) 
					result = log(result) / log(operand); 
				else 
					return NAN;
				break;
			case 'n': result = pow(result, 1.0 / operand); break;
			case 'S': result = asin(result); break;
			case 'C': result = acos(result); break;
			case 'T': result = atan(result); break;
			default: return NAN;
		}
	}
	return result;
}