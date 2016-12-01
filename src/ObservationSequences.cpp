#include "ObservationSequences.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "boost\filesystem.hpp"


using namespace std;
namespace bst = boost::filesystem;

ObservationSequences::~ObservationSequences() 
{
}

ObservationSequences::ObservationSequences(std::string folderName) 
{

	noOfFiles = 0;
	noOfTrainingFiles = 0;
	noOfScoringFiles = 0;
	this->malwareFamilyName = folderName;
}

void ObservationSequences::getFileList() {

	string folderName = "D:/Aditya/CS_266/Project/Dataset/" + this->malwareFamilyName + "/";
	cout << folderName << endl;
	bst::path p(folderName);
	for (auto i = bst::directory_iterator(p); i != bst::directory_iterator(); i++)
	{
		if (!bst::is_directory(i->path()))
		{	
			string fullFileName = folderName + i->path().filename().string();
			this->fileNameList.push_back(fullFileName);
		}
	}
}

void ObservationSequences::getFileStream() 
{

	this->noOfFiles = this->fileNameList.size();
	for (int index = 0; index < this->noOfFiles; index++) 
	{
		vector<int> tempFileStream;
		vector<string> opCodeStream;

		string tempFileName = this->fileNameList.at(index);
		ifstream tempReadFile;
		tempReadFile.open(tempFileName);

		string line;
		while (getline(tempReadFile, line))
		{
			int opCodeIndex = find(this->distinctOpCodesList.begin(), this->distinctOpCodesList.end(), line) - this->distinctOpCodesList.begin();
			int endIndex = this->distinctOpCodesList.size();

			if (opCodeIndex != endIndex)
			{
				tempFileStream.push_back(opCodeIndex);
			}
			else
			{
				this->distinctOpCodesList.push_back(line);
				int newOpCodeIndex = this->distinctOpCodesList.size()-1;

				tempFileStream.push_back(newOpCodeIndex);
			}
			opCodeStream.push_back(line);
		}
		this->obsSequenceList.push_back(tempFileStream);		
	}
	cout << this->distinctOpCodesList.size();
}