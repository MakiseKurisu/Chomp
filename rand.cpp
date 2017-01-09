#include <random> 

std::random_device rd;
std::mt19937 gen(rd());

extern "C" int secure_rand(int min_rand, int max_rand)
{
	std::uniform_int_distribution<int> dist(min_rand, max_rand);
	return dist(gen);
}