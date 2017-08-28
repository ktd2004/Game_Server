/* Copyright 2013 by Lee yong jun
 * All rights reserved
 *
 * Distribute freely, except: don't remove my name from the source or
 * documentation (don't take credit for my work), mark your changes (don't
 * get me blamed for your possible bugs), don't alter or remove this
 * notice.  May be sold if buildable source is provided to buyer.  No
 * warrantee of any kind, express or implied, is included with this
 * software; use at your own risk, responsibility for damages (if any) to
 * anyone resulting from the use of this software rests entirely with the
 * user.
 *
 * Send bug reports, bug fixes, enhancements, requests, flames, etc., and
 * I'll try to keep a version up to date.  I can be reached as follows:
 * Lee yong jun          iamhere01@naver.com
 */
#include "StdAfx.h"

#define _CRT_RAND_S
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "Random.h"

unsigned int RandomGen::m_iSeed = 0;
RandomGen::RandomGen()
{
	m_iSeed = (unsigned int)time(NULL);
}

RandomGen::~RandomGen()
{
}


unsigned int RandomGen::GetValue()
{
	if ( !m_iSeed )
	{ rand_s(&m_iSeed);	}
	return m_iSeed;
}

RandomGen * RandomGen::instance()
{
	static RandomGen inst;
	return &inst;
}

//	LCG

LCG::LCG()
	: m_iSeed((unsigned int)time(NULL))
{ rand(); }

//	MersenneTwister

MersenneTwister::MersenneTwister()
{
	_init((unsigned int)time(NULL));
}

unsigned int MersenneTwister::rand()	// [0,0xFFFFFFFF]
{
	static unsigned int mag01[2] = { 0, MATRIX_A };
	unsigned int y;
	if (mti >= N) {
		int kk;
		for (kk=0;kk<N-M;kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (;kk<N-1;kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
		mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];
		mti = 0;
	}
	y = mt[mti++];
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680;
	y ^= (y << 15) & 0xefc60000;
	y ^= (y >> 18);
	return y;
}

void MersenneTwister::_init(unsigned int seed)
{
	mt[0] = seed & 0xffffffff;
	for (mti=1; mti < N; ++mti) {
		mt[mti] = (1812433253 * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
		mt[mti] &= 0xffffffff;
	}
}

void MersenneTwister::_init_by_array(unsigned int* key, int keylen)
{
	int i, j, k;
	_init(19650218);
	i=1; j=0;
	k = (N>keylen ? N : keylen);
	for (; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525)) + key[j] + j;
		mt[i] &= 0xffffffff;
		i++; j++;
		if (i>=N) { mt[0] = mt[N-1]; i = 1; }
		if (j>=keylen) j=0;
	}
	for (k=N-1; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941)) - i;
		mt[i] &= 0xffffffff;
		i++;
		if (i>=N) { mt[0] = mt[N-1]; i = 1; }
	}
	mt[0] = 0x80000000;
}


#if defined(C_PLUS_PLUS_11)

std::mt19937 Random::m_Gen;

Random::Random()
	: m_Dist(nullptr)
{
	m_Interval.push_back( 1.0f );
}

Random::~Random()
{
}

void Random::Reset()
{
	m_Px.clear();
}

bool Random::Px( double dPx )
{
	m_Interval.push_back((double)m_Interval.size()+1);
	m_Px.push_back( dPx );	// RandomBox 에서 구현할때 0.01 을 곱해서 주기 때문에 여기서는 나눴다.
	return true;
}

void Random::Update()
{
	if ( m_Px.size() > 0 )
	{
		m_Dist = new DistType( m_Interval.begin(), m_Interval.end(), m_Px.begin() );
	}
}

int Random::Rand()
{
	if ( !m_Dist )
	{
		Update();
	}
	
	if ( m_Dist )
	{
		DistType& r = *m_Dist;
		return static_cast<unsigned>(r(m_Gen)) - 1;
	}

	return -1;
}

double Random::Test( size_t N, std::vector<double>* realprob )
{
	printf( "Ideal case... \nP(x) = { ");
	for ( size_t i = 0; i < m_Px.size(); i++ )
	{
		printf( "%f ", m_Px[i] );
	}
	printf( " }\n" );

	std::vector<int> n;
	for( size_t i = 0; i < m_Px.size(); i++)
	{
		n.push_back(0);
	}

	for ( size_t i = 0; i < N; ++i )
	{
		int32 idx = Rand();
		if ( idx < 0 ) continue;
		++n[idx];
	}

	printf( "Real case.. \nF(x) = { " );
	for ( size_t j = 0; j < n.size(); j++ )
	{
		double v = (double)n[j]/N * 100.0f;
		printf( "%lf ", v );
		if ( realprob ) realprob->push_back( v );
	}

	printf( " }\n" );

	return 0;
}

#else
Random::Random() : m_bUpdated(false)
{
	// seed 값이 없을 경우에 srand_s를 한번만 호출하게 되어있다.
	rando();
}

Random::~Random()
{

}

void Random::Reset()
{
	m_Px.clear();
	m_Fx.clear();
	m_bUpdated = false;
}

bool Random::Px( double dPx )
{
	size_t i = m_Px.size();
	if ( dPx > 1.0f )
	{ return false;	}

	if ( i != 0 &&  m_Fx[i-1] >= 1.f )
	{ return false;	}

	if ( i != 0 && (m_Fx[i-1]+dPx) > 1.0f )
	{
		m_Px.push_back( 1.0f - m_Fx[i-1] );
		m_Fx.push_back( 1.0f );
		return false;
	}

	m_Px.push_back( dPx );
	if ( i == 0 )
	{ m_Fx.push_back( dPx ); }
	else
	{ m_Fx.push_back( m_Fx[i-1]+dPx ); }
	return true;
}

void Random::Update()
{
	if ( m_Px.size() == 0 )
	{ return; }

	size_t i = m_Px.size();
	if ( m_Fx[i-1] != 1.0f )
	{
		m_Fx.push_back( 1.0f );
		m_Px.push_back( 1.0f - m_Fx[i-1] );
	}
	m_bUpdated = true;
}

int Random::Rand()
{
	if ( !m_bUpdated )
	{ Update();	}

	// [0, 1] 의random number
	double r = (double)(rand() % RAND_MAX) / RAND_MAX;
	//double r = (double)(m_R.rand(RAND_MAX)) / RAND_MAX;
	//  F(x-1) < r <= F(x) 이면x를반환
	size_t iSize = m_Fx.size();
	for ( size_t i = 0; i < iSize; ++i )
	{
		if ( r <= m_Fx[i] )
			return (int)i;
	}

	// 코드가여기까지올리가없지만만약오면에러값반환
	return -1;
}

double Random::Test( size_t N, std::vector<double>* realprob )
{
	Update();
	printf( "Ideal case... \nP(x) = { ");
	for ( size_t i = 0; i < m_Px.size(); i++ )
	{
		printf( "%f ", m_Px[i] );
	}
	printf( " }\n" );

	std::vector<int> n;
	for( size_t i = 0; i < m_Fx.size(); i++)
	{
		n.push_back(0);
	}

	for ( size_t i = 0; i < N; ++i )
	{
		int32 idx = Rand();
		if ( idx < 0 ) continue;
		++n[idx];
	}

	printf( "Real case.. \nF(x) = { " );
	for ( size_t j = 0; j < m_Fx.size(); j++ )
	{
		double v = (double)n[j]/N;
		printf( "%lf ", v );
		if ( realprob ) realprob->push_back( v );
	}

	printf( " }\n" );

	return 0;
}

#endif