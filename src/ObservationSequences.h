#pragma once
#include <vector>
#include <string>

class ObservationSequences
{

public:
	std::string malwareFamilyName;
	std::string parentFolderPath;
	~ObservationSequences();
	ObservationSequences(std::string);
	
	void getFileList();
	void getFileStream();

	void getTrainingData();
	void getScoringData();
	void addBenignData();
	void getBenignData(std::string);

	int noOfFiles;
	int noOfTrainingFiles;
	int noOfScoringFiles;

private:
	std::vector<int> trainingData;
	std::vector<std::vector<int>> scoringData;
	std::vector<std::string> distinctOpCodesList;
	
	std::vector<std::string> fileNameList;
};