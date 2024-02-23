#pragma once

#include <string>

struct CharacterData
{
	// Variables
	std::string name = "No name";
	std::string head_file_path = "";
	std::string torso_file_path = "";

	// Constructors
	CharacterData() {
	}
	CharacterData(
		std::string name,
		std::string head_file_path,
		std::string torso_file_path
	) {
		this->name = name;
		this->head_file_path = head_file_path;
		this->torso_file_path = torso_file_path;
	}
};