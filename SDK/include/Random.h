#pragma once

#include <DataTypes.h>

class RandomGen
{
	static unsigned int m_iSeed;
	RandomGen();
	~RandomGen();

public:
	static RandomGen*   instance();
	static unsigned int GetValue();
};

#define rando()	RandomGen::instance()->GetValue()

/**
LCG (Linear congruential generator)

link: http://en.wikipedia.org/wiki/Linear_congruential_generator

Source
	idx		m				a					c					output bits of seed in rand() / Random(L)
Numerical Recipes
	1		2^32			1664525				1013904223	
Borland C/C++
	2		2^32			22695477			1					bits 30..16 in rand(), 30..0 in lrand()
glibc (used by GCC)[4]
	3		2^32			1103515245			12345				bits 30..0
ANSI C: Watcom, Digital Mars, CodeWarrior, IBM VisualAge C/C++
	4		2^32			1103515245			12345				bits 30..16
Borland Delphi, Virtual Pascal
	5		2^32			134775813			1					bits 63..32 of (seed * L)
Microsoft Visual/Quick C/C++
	6		2^32			214013				2531011				bits 30..16
Apple CarbonLib
	7		2^31-1			16807				0					see MINSTD
MMIX by Donald Knuth
	8		2^64			6364136223846793005	1442695040888963407	
VAX's MTH$RANDOM,[5] old versions of glibc
	9		2^32			69069				1	
Random class in Java API
	10		2^48			25214903917			11					bits 47...16
LC53[6] in Forth (programming language)
	11		2^32-5			2^32-333333333		0	
*/
class LCG
{
private:
	unsigned int m_iSeed;
	enum {
		a = 1103515245,
		c = 12345,
		m = 0x7FFFFFFF,
	};
public:
	LCG();
	LCG(unsigned int _seed)
		: m_iSeed(_seed)
	{ rand(); }
	inline unsigned int randmax()
	{ return m; }
	inline void srand(unsigned int _seed)
	{ m_iSeed = _seed; }
	inline unsigned int rawrand()
	{ return m_iSeed = a * m_iSeed + c; }
	inline unsigned int rand()	// [0,m]
	{ return rawrand() & m; }
	inline unsigned int rand(unsigned int min, unsigned int max)	// [min,max]
	{ return (unsigned int)((unsigned __int64)rand() * (max - min) / m + min); }
	inline unsigned int rand(unsigned int max)	// [0,max]
	{ return rand(0, max); }
	inline double rand1()	// [0,1]
	{ return (double)rand() / (double)(m-1); }
	inline double rand2()	// [0,1)
	{ return (double)rand() / (double)m; }
	inline double rand3()	// (0,1)
	{ return ((double)rand() + 0.5) / (double)m; }
	inline double randf()	// rand1()
	{ return rand1(); }
	inline double randf53()
	{ return ((rand()>>5) * 67108864.0 + (rand()>>6)) * (1.0 / 9007199254740992.0); }
};

/*
Mersenne Twister

link: http://en.wikipedia.org/wiki/Mersenne_twister
*/
class MersenneTwister
{
	enum {
		N = 624,
		M = 397,
		MATRIX_A = 0x9908b0df,
		UPPER_MASK = 0x80000000,
		LOWER_MASK = 0x7fffffff,
	};
public:
	MersenneTwister();
	MersenneTwister(unsigned int seed)
	{ _init(seed); }
	MersenneTwister(unsigned int* key, int keylen)
	{ _init_by_array(key, keylen); }
	inline void srand(unsigned int seed)
	{ return _init(seed); }
	unsigned int rand();	// [0,0xFFFFFFFF]
	inline unsigned int rand(unsigned int min, unsigned int max)	// [min,max]
	{ return (unsigned int)((unsigned __int64)rand() * (max - min) / (0xFFFFFFFF) + min); }
	inline unsigned int rand(unsigned int max)	// [0,max]
	{ return rand(0, max); }
	inline double rand1()	// [0,1]
	{ return (double)rand() * (1.0/(double)0xFFFFFFFF); }
	inline double rand2()	// [0,1)
	{ return (double)rand() * (1.0/(double)100000000); }
	inline double rand3()	// (0,1)
	{ return ((double)rand() + 0.5) * (1.0/(double)100000000); }
	inline double randf()	// rand1()
	{ return rand1(); }
	inline double randf53()
	{ return ((rand()>>5) * 67108864.0 + (rand()>>6)) * (1.0 / 9007199254740992.0); }
private:
	unsigned int mt[N];
	int mti;
	void _init(unsigned int seed);
	void _init_by_array(unsigned int* key, int keylen);
};

#define C_PLUS_PLUS_11

#if defined(C_PLUS_PLUS_11)

#include <random>

class Random
{
	typedef std::piecewise_constant_distribution<>	DistType;
	std::vector<double >	m_Interval;			// È®·ü±¸°£
	std::vector<double >	m_Px;				// È®·ü
	DistType *				m_Dist;
	static std::mt19937		m_Gen;				// seed as wanted

public:
	Random();
	virtual ~Random();
	void	Reset();
	void	Update();
	bool	Px(double dPx);
	int		Rand();
	double	Test(size_t N, std::vector<double>* realprob = nullptr);
};

#else

class Random
{
	std::vector<double >  m_Px;      // È®·ü
	std::vector<double >  m_Fx;      // ´©ÀûÈ®·ü
	bool                  m_bUpdated;// °»½Å‰ç´ÂÁö ?
	LCG                   m_R;
public:
	Random();
	virtual ~Random();

	void	Reset();
	bool	Px(double dPx);
	void	Update();
	int		Rand();
	double	Test(size_t N, std::vector<double>* realprob = nullptr);
};

#endif