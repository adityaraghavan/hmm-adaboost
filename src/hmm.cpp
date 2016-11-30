#include "hmm.h"
#include <iostream>
#include <iterator>

hmm::hmm(int n_states, int m_obsv_seq)
{
	this->setNumStates(n_states);

	this->setNumObsvSeq(m_obsv_seq);
}

void hmm::setNumStates(int n_states)
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

int hmm::getNumStates()
{
	return this->num_states;
}

void hmm::setNumObsvSeq(int n_obsv_seq)
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

int hmm::getNumObsvSeq()
{
	return this->num_obsv_seq;
}

void hmm::printStateTransition()
{
	std::cout << this->state_trasition;
}

void hmm::printObsvProbab()
{
	std::cout << this->obsv_probab;
}

void hmm::printInitDist()
{
	std::cout << this->init_dist;
}

template<typename T>
std::ostream & operator<<(std::ostream & out, const std::vector<T>& vec)
{
	if (!vec.empty()) {
		out << '[';
		std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(out, ", "));
		out << "\b\b]";
	}
	return out;
}

template<typename T>
std::ostream & operator<<(std::ostream & out, const std::vector<std::vector<T>>& vec)
{
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

void hmm::ForwardAlgorithm(const std::vector<int>& obsv)
{
	std::vector<float> row;
	int temp;

	// Initialize alpha0
	for (int i = 0; i < this->num_states; i++)
	{
		temp = this->init_dist.at(i) * this->obsv_probab[i][obsv[0]];
		row.push_back(temp);
	}
	
	this->alpha.push_back(row);

	for (int t = 1; t < obsv.size(); t++)
	{
		row.clear();

		for (int i = 0; i < this->num_states; i++)
		{
			float sum = 0;

			for (int j = 0; j < this->num_states; j++)
			{
				sum = sum + alpha[t - 1][j] * this->state_trasition[j][i];
			}

			sum = sum * this->obsv_probab[i][obsv[t]];

			row.push_back(sum);
		}

		alpha.push_back(row);
	}
}

void hmm::BackwardAlgorithm(const std::vector<int>& obsv)
{
	std::vector<float> row;

	for (int i = 0; i < this->num_states; i++)
	{
		row.push_back(1);
	}

	this->beta.push_back(row);

	for (int t = obsv.size() - 2; t >= 0; t--)
	{
		row.clear();

		for (int i = 0; i < this->num_states; i++)
		{
			float sum = 0;

			for (int j = 0; j < this->num_states; j++)
			{
				sum = sum + (this->state_trasition[i][j] * this->obsv_probab[j][obsv[t + 1]] * this->beta[t + 1][j]);
			}

			row.push_back(sum);
		}

		this->beta.insert(this->beta.begin(), row);
	}
}
