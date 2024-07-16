double evaluate(char* expression) 
{
	gtk_widget_show(mathtext);
	char* endptr;
	double result = strtod(expression, &endptr);
	char op;
	double operand;

	while (*endptr != '\0') 
	{
		op = *endptr++;
		operand = strtod(endptr, &endptr);

		switch (op) 
		{
			case '+': result += operand; break;
			case '-': result -= operand; break;
			case '*': result *= operand; break;
			case '/': result /= operand; break;
			case '^': result = pow(result, operand); break;
			case '%': result = operand / 100.0 * result; break;
			case 'r': result = sqrt(result); break;
			case 'c': result = cos(result); break;
			case 't': result = tan(result); break;
			case 's': result = sin(result); break;
		}
	}
	return result;
}