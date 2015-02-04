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

	virtual int NextInt(void);

	virtual float NextFloat(float base, float limit);

	virtual float NextFloat(void);

	virtual double NextDouble(double base, double limit);

	virtual double NextDouble(void);

	virtual char NextChar(char base, char limit);

	virtual char NextChar(void);

	virtual bool NextBool(void);

	virtual long NextLong(long base, long limit);

	virtual long NextLong(void);

	virtual short NextShort(short base, short limit);

	virtual short NextShort(void);

	virtual int GetSeed(void) const;

private:

protected:

	std::mt19937 m_generator;

	int m_seed;

};

#endif // RANDOM_H
