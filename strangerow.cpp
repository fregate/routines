
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

int main()
{
	std::cout << "strange row" << std::endl;


	std::uniform_int_distribution<> udstr(0, 3);
	std::mt19937_64 rng;
	rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());

	int iterations(0);
	long double res(0);

	long double min(0);
	long double max(0);

	char operation;
	while (iterations++ < 1000)
	{
		switch (udstr(rng))
		{
		case 0: // plus
			res += iterations;
			operation = '+';
			break;
		case 1: // minus
			res -= iterations;
			operation = '-';
			break;
		case 2: // multiplication
			res *= iterations;
			operation = '*';
			break;
		case 3: // divide
		default:
			res /= iterations;
			operation = '/';
			break;
		}

		max = std::max(res, max);
		min = std::min(res, min);

		std::cout << "[" << iterations << "] " << operation << " " << res << std::endl;
	}

	std::cout << "min: " << min << std::endl << "max: " << max << std::endl;

	return 0;
}
