/*

	Platformer Game - Made for the 3rd year of undergraduated project.
    Copyright (C) 2015  Corbat Lisa, Kieffer Joseph

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
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
