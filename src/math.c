int gcd(int a, int b)
{
	if (a < b)
	{
		int temp = a;
		a = b;
		b = temp;
	}

	// int quotient = a / b;
	int remainder = a % b;

	if (remainder == 0)
	{
		return b;
	}
	else
	{
		return gcd(b, remainder);
	}
}
