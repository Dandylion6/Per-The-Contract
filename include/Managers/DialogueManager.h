#pragma once

#include <cstdint>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "Components/Renderers/TextRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"
#include "Data/CustomerTrait.h"
#include "Data/Role.h"

class DialogueManager
{
public:
	// Constructors
	DialogueManager(Game& game);
	virtual ~DialogueManager();

	// Getters
	static DialogueManager& getInstance();

	// Functions
	void generateNext();
	void generateDialogue(Role role, std::string prompt);
	void generateDialogue(Role role, std::string prompt, std::string replace);
	void createDialogueObject(Role role, std::string dialogue);
	void clearDialogue();

private:
	// Constants
	const std::string merchant_dialogue_path = "assets/data/merchant_dialogue.json";
	const std::string customer_dialogue_path = "assets/data/customer_dialogue.json";
	const std::string insert_block = "[insert]";

	const uint8_t dialogue_spacing = 25u;
	const uint16_t dialogue_max_width = 550u;
	const uint8_t max_dialogue_lines = 4u;

	const std::map<std::string, CustomerTrait> key_to_trait_map{
		{ "firm", CustomerTrait::Firm },
		{ "haggler", CustomerTrait::Haggler }
	};

	// References
	Game& game;

	// Variables
	static DialogueManager* instance;
	Object* dialogue_box = nullptr;
	std::map<std::string, std::vector<std::string>> merchant_lines;
	std::map<std::string, std::map<CustomerTrait, std::vector<std::string>>> customer_lines;
	std::vector<TextRenderer*> dialogue_renderers;
	std::queue<std::pair<Role, std::string>> dialogue_queue;

	Vector2 dialogue_box_size;
	Vector2 dialogue_offset;

	// Functions
	void updateDialogueList();
	void convertJsonToMaps();
	std::string getRandomDialogue(Role role, std::string prompt);
	std::string getRandomDialogue(Role role, std::string prompt, std::string replace);
};