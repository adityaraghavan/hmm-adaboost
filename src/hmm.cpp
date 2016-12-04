#include "hmm.h"
#include <iostream>
#include <iterator>
#include <math.h>
#include <float.h>

Hmm::Hmm()
{

}

Hmm::Hmm(int n_states, int m_obsv_seq)
{
	this->setNumStates(n_states);

	this->setNumObsvSeq(m_obsv_seq);
}

Hmm::~Hmm()
{
}

void Hmm::setNumStates(int n_states)
{
	if (n_states > 0)
	{
		this->num_states = n_states;
	}
	else
	{
		this->num_states = 0;
	}
}

int Hmm::getNumStates()
{
	return this->num_states;
}

void Hmm::setNumObsvSeq(int n_obsv_seq)
{
	if (n_obsv_seq > 0)
	{
		this->num_obsv_seq = n_obsv_seq;
	}
	else
	{
		this->num_obsv_seq = 0;
	}
}

int Hmm::getNumObsvSeq()
{
	return this->num_obsv_seq;
}

void Hmm::setInitDist(std::vector<double> pi)
{
	this->init_dist = pi;
}

void Hmm::setStateTrans(std::vector<std::vector<double>> A)
{
	this->state_trasition = A;
}

void Hmm::setObsvProbab(std::vector<std::vector<double>> B)
{
	this->obsv_probab = B;
}

void Hmm::printStateTransition()
{
	std::cout << this->state_trasition;
}

void Hmm::printObsvProbab()
{
	std::cout << this->obsv_probab;
}

void Hmm::printInitDist()
{
	std::cout << this->init_dist;
}

void Hmm::printAlpha()
{
	std::cout << this->alpha;
}

void Hmm::printBeta()
{
	std::cout << this->beta;
}

void Hmm::printGamma()
{
	std::cout << this->gamma;
}

template<typename T>
std::ostream & operator<<(std::ostream & out, const std::vector<T>& vec)
{
	out << "Vector: \n";
	if (!vec.empty()) {
		out << '[';
		std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(out, ", "));
		out << "\b\b]";
	}
	out << "\n";
	return out;
}

template<typename T>
std::ostream & operator<<(std::ostream & out, const std::vector<std::vector<T>>& vec)
{
	out << "Vector: \n";
	for (int i = 0; i < vec.size(); i++)
	{
		if (!vec[i].empty()) {
			out << '[';
			std::copy(vec[i].begin(), vec[i].end(), std::ostream_iterator<T>(out, ", "));
			out << "\b\b]";
		}
		out << "\n";
	}
	return out;
}

void Hmm::ForwardAlgorithm(const std::vector<int>& obsv)
{
	//initialize scale
	this->scale.clear();
	this->scale.resize(obsv.size(), 0);
	
	//initialize alpha
	this->alpha.clear();
	this->alpha.resize(obsv.size(), std::vector<double>(this->num_states, 0));

	// Compute alpha(0)
	for (int i = 0; i < this->num_states; i++)
	{
		this->alpha[0][i] = this->init_dist.at(i) * this->obsv_probab[i][obsv[0]];
		this->scale[0] = this->scale[0] + this->alpha[0][i];
	}
	
	//scale alpha(0)(i)
	this->scale[0] = 1 / this->scale[0];
	if (_isnan(this->scale[0]) || isinf(this->scale[0]))
	{
		this->scale[0] = 0;
	}
	
	for (int i = 0; i < this->num_states; i++)
	{	
		this->alpha[0][i] = this->scale[0] * this->alpha[0][i];
	}

	// Calculate alpha(t)
	for (int t = 1; t < obsv.size(); t++)
	{
		for (int i = 0; i < this->num_states; i++)
		{
			//this->alpha[t][i] = 0;

			for (int j = 0; j < this->num_states; j++)
			{
				this->alpha[t][i] = this->alpha[t][i] + (this->alpha[t - 1][j] * this->state_trasition[j][i]);
			}

			this->alpha[t][i] = this->alpha[t][i] * this->obsv_probab[i][obsv[t]];
			this->scale[t] = this->scale[t] + this->alpha[t][i];
		}		

		this->scale[t] = 1 / this->scale[t];
		if (_isnan(this->scale[t]) || isinf(this->scale[t]))
		{
			this->scale[t] = 0;
		}
		for (int i = 0; i < this->num_states; i++)
		{
			this->alpha[t][i] = this->scale[t] * this->alpha[t][i];
		}

	}
}

void Hmm::BackwardAlgorithm(const std::vector<int>& obsv)
{
	//resize beta
	this->beta.clear();
	this->beta.resize(obsv.size(), std::vector<double> (this->num_states, 0));

	// Initialize and scale beta(T-1)
	for (int i = 0; i < this->num_states; i++)
	{
		this->beta[obsv.size() - 1][i] = this->scale[obsv.size() - 1];
	}

	// Calculate beta values
	for (int t = obsv.size() - 2; t >= 0; t--)
	{
		for (int i = 0; i < this->num_states; i++)
		{
			for (int j = 0; j < this->num_states; j++)
			{
				this->beta[t][i] = this->beta[t][i] + (this->state_trasition[i][j] * this->obsv_probab[j][obsv[t + 1]] * this->beta[t + 1][j]);
			}

			this->beta[t][i] = this->scale[t] * this->beta[t][i];
		}
	}
}

void Hmm::CalculateGammas(const std::vector<int>& obsv)
{
	this->gamma.clear();
	this->digamma.clear();
	this->gamma.resize(obsv.size(), std::vector<double>(this->num_states, 0));
	this->digamma.resize(obsv.size(), std::vector<std::vector<double>>(this->num_states, std::vector<double>(this->num_states, 0)));
	double denom = 0;

	for (int t = 0; t < obsv.size()-1; t++)
	{
		denom = 0;

		for (int i = 0; i < this->num_states; i++)
		{
			for (int j = 0; j < this->num_states; j++)
			{
				denom = denom + this->alpha[t][i] * this->state_trasition[i][j] * this->obsv_probab[j][obsv[t+1]] * this->beta[t+1][j];
			}
		}
		if (denom == 0) {
			std::cout << denom << std::endl;
		}
		for (int i = 0; i < this->num_states; i++)
		{
			this->gamma[t][i] = 0;
			for (int j = 0; j < this->num_states; j++)
			{
				this->digamma[t][i][j] = (this->alpha[t][i] * this->state_trasition[i][j] * this->obsv_probab[j][obsv[t + 1]] * this->beta[t + 1][j]);
				this->digamma[t][i][j] = this->digamma[t][i][j] / denom;
				this->gamma[t][i] = this->gamma[t][i] + this->digamma[t][i][j];
			}
		}
	}

	//special case for gamma(T-1)(i)
	denom = 0;
	for (int i = 0; i < this->num_states; i++)
	{
		denom = denom + this->alpha[obsv.size() - 1][i];
	}
	for (int i = 0; i < this->num_states; i++)
	{
		this->gamma[obsv.size() - 1][i] = this->alpha[obsv.size() - 1][i] / denom;
	}
}

void Hmm::ScoringForwardAlgorithm(const std::vector<int>& obsv)
{
	//initialize alpha
	this->alpha.clear();
	this->alpha.resize(obsv.size(), std::vector<double>(this->num_states, 0));

	// Compute alpha(0)
	for (int i = 0; i < this->num_states; i++)
	{
		this->alpha[0][i] = this->init_dist.at(i) * this->obsv_probab[i][obsv[0]];
	}

	// Calculate alpha(t)
	for (int t = 1; t < obsv.size(); t++)
	{
		for (int i = 0; i < this->num_states; i++)
		{
			//this->alpha[t][i] = 0;

			for (int j = 0; j < this->num_states; j++)
			{
				this->alpha[t][i] = this->alpha[t][i] + (this->alpha[t - 1][j] * this->state_trasition[j][i]);
			}

			this->alpha[t][i] = this->alpha[t][i] * this->obsv_probab[i][obsv[t]];
		}
	}
}

void Hmm::Restimate(const std::vector<int>& obsv)
{
	double numer, denom;
	// restimate initial distribution
	for (int i = 0; i < this->num_states; i++)
	{
		this->init_dist[i] = this->gamma[0][i];
	}

	//re-estimate state transition probability
	for (int i = 0; i < this->num_states; i++)
	{
		for (int j = 0; j < this->num_states; j++)
		{
			numer = 0;
			denom = 0;

			for (int t = 0; t < obsv.size() - 1; t++)
			{
				numer = numer + this->digamma[t][i][j];
				denom = denom + this->gamma[t][i];
			}
			this->state_trasition[i][j] = numer / denom;
		}
	}

	//re-estimate observation probability
	for (int i = 0; i < this->num_states; i++)
	{
		for (int j = 0; j < this->num_obsv_seq; j++)
		{
			numer = 0.0;
			denom = 0.0;

			for (int t = 0; t < obsv.size(); t++)
			{
				if (obsv[t] == j)
				{
					numer = numer + this->gamma[t][i];
				}
				denom = denom + this->gamma[t][i];
			}

			this->obsv_probab[i][j] = numer / denom;
		}
	}
}

double Hmm::Score(const std::vector<int>& test)
{
	this->ForwardAlgorithm(test);
	
	double score = 0;

	for (int i = 0; i < this->num_states; i++)
	{
		score = score + this->alpha[test.size() - 1][i];
	}

	return score;
}
