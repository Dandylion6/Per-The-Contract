#pragma once

#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "Data/Role.h"

using json = nlohmann::json;

// Forward declerations
class Game;
class Object;

class DialogueManager
{
public:
	// Constructors
	DialogueManager(Game& game);
	virtual ~DialogueManager();

	// Functions
	void generateDialogue(Role role, std::string prompt);
	void generateDialogue(Role role, std::string prompt, std::string replace);
	void createDialogueObject(Role role, std::string dialogue);
	void clearDialogue();

private:
	// Constants
	const std::string dialogue_map_path = "assets/data/dialogue_map.json";
	const std::string insert_block = "[insert]";

	// References
	Game& game;

	// Variables
	Object* dialogue_box = nullptr;
	std::map<std::string, std::vector<std::string>> merchant_lines;
	std::map<std::string, std::vector<std::string>> customer_lines;

	// Functions
	void convertJsonToMaps();
	std::string getRandomDialogue(Role role, std::string prompt);
	std::string getRandomDialogue(Role role, std::string prompt, std::string replace);
};