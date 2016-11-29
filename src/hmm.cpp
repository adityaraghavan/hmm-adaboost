#include "hmm.h"
#include <iostream>

hmm::hmm(int n_states, int m_obsv_seq)
{
	
	this->SetNumStates(n_states);

	this->SetNumObsvSeq(m_obsv_seq);

}

int hmm::GetNumStates()
{
	return this->num_states;
}

void hmm::SetNumObsvSeq(int n_obsv_seq)
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

int hmm::GetNumObsvSeq()
{
	return this->num_obsv_seq;
}

void hmm::PrintStateTransition()
{
	PrintTwoDimVector(this->state_trasition);
}

void hmm::PrintObsvProbab()
{
	PrintTwoDimVector(this->obsv_probab);
}

void hmm::PrintInitDist()
{
	PrintTwoDimVector(this->init_dist);
}

void hmm::PrintTwoDimVector(const std::vector<std::vector<float>>& vec)
{
	for (int i = 0; i < vec.size(); i++)
	{
		for (int j = 0; j < vec[i].size(); j++)
		{
			std::cout << vec[i][j] << " ";
		}

		std::cout << "\n";
	}
}

void hmm::SetNumStates(int n_states)
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
