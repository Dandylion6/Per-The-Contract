#pragma once

#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "Components/Renderers/TextRenderer.h"
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
	const Vector2 dialogue_box_size = Vector2(640.f, 316.f);
	const Vector2 merchant_offset = Vector2(
		dialogue_box_size.x - 20.f, 20.f - dialogue_box_size.y
	);
	const Vector2 customer_offset = Vector2(20.f, 20.f - dialogue_box_size.y);

	// References
	Game& game;

	// Variables
	Object* dialogue_box = nullptr;
	std::map<std::string, std::vector<std::string>> merchant_lines;
	std::map<std::string, std::vector<std::string>> customer_lines;
	std::vector<TextRenderer*> dialogue_renderers;

	// Functions
	void convertJsonToMaps();
	std::string getRandomDialogue(Role role, std::string prompt);
	std::string getRandomDialogue(Role role, std::string prompt, std::string replace);
};