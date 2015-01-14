#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <chrono>

class Random
{
public:

	explicit Random(void);

	explicit Random(unsigned int seed);

	virtual ~Random(void);

	virtual int NextInt(int base, int limit);

	virtual float NextFloat(void);

	virtual double NextDouble(void);

	virtual char NextChar(void);

	virtual bool NextBool(void);

	virtual long NextLong(void);

	virtual short NextShort(void);

private:

protected:

	std::mt19937 m_generator;

};

#endif // RANDOM_H
