#include "../header/Random.h"

/*explicit*/ Random::Random()
{
	m_generator.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

/*explicit*/ Random::Random(unsigned int seed)
{
	m_generator.seed(seed);
}

/*virtual*/ Random::~Random()
{

}

/*virtual*/ int Random::NextInt()
{
	return (int)(m_generator());
}

/*virtual*/ float Random::NextFloat()
{
	return (float)(m_generator());
}

/*virtual*/ double Random::NextDouble()
{
	return (double)(m_generator());
}

/*virtual*/ char Random::NextChar()
{
	return (char)(m_generator());
}

/*virtual*/ bool Random::NextBool()
{
	return (bool)(m_generator());
}

/*virtual*/ long Random::NextLong()
{
	return (long)(m_generator());
}

/*virtual*/ short Random::NextShort()
{
	return (short)(m_generator());
}
