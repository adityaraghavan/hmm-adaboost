#pragma once
#include <vector>
#include <string>

class ObservationSequences
{

public:
	~ObservationSequences();
	ObservationSequences(std::string);

	int noOfFiles;
	std::string malwareFamilyName;
	std::string parentFolderPath;
	
	void getFileList();
	std::vector<std::string> fileNameList;

	void getFileStream();
	std::vector<std::string> distinctOpCodesList;

	void getTrainingData();
	std::vector<int> trainingData;

	void getScoringData();
	std::vector<std::vector<int>> malwareData;

	void addBenignData();
	void getBenignData(std::string);
	std::vector<std::vector<int>> benignData;
};