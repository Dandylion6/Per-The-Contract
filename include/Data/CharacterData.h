#pragma once

#include <string>

enum Gender
{
	Male = 0,
	Female = 1
};

struct CharacterData
{
	// Variables
	std::string name = "No name";
	std::string head_file_path = "";
	std::string torso_file_path = "";
	Gender gender = Gender::Male;

	// Constructors
	CharacterData() {
	}
	CharacterData(
		std::string name,
		std::string head_file_path,
		std::string torso_file_path,
		int gender
	) {
		this->name = name;
		this->head_file_path = head_file_path;
		this->torso_file_path = torso_file_path;
		this->gender = static_cast<Gender>(gender);
	}
};