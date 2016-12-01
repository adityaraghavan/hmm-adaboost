#pragma once
#include <vector>
#include <string>

class ObservationSequences
{

public:
	std::string malwareFamilyName;
	~ObservationSequences();
	ObservationSequences(std::string);
	void getFileList();
	void getFileStream();

	int noOfFiles;
	int noOfTrainingFiles;
	int noOfScoringFiles;

private:
	std::vector<std::vector<int>> obsSequenceList;
	std::vector<std::string> distinctOpCodesList;
	
	std::vector<std::string> fileNameList;
};