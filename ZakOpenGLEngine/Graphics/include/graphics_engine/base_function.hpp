#include <cmath>
#include <functional>

const std::function<double(double)> BASE_FUNCTION = [](double x)->double{
	double y = 0;
	double prev_y = 0;

	static const float ACCURACY = 1e-6;
	static const unsigned char MAX_ITERATIONS = 100;

	unsigned char iteration = 0;

	do
	{
		prev_y = y;
		y = -pow(sin(prev_y),2) + pow(cos(x),2) + x;
		if(fabs(y-prev_y) < ACCURACY)
			break;
	}
	while(iteration++ < MAX_ITERATIONS);

    return y;
};
