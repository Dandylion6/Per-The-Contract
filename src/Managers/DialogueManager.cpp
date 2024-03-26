#include <cstdlib>
#include <ctime>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

#include "Components/Renderers/TextRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Managers/DialogueManager.h"


//_______________
// Constructors

DialogueManager::DialogueManager(Game& game) : game(game) {
	std::srand(std::time(nullptr));
	dialogue_box = game.getObject("dialogue_box");
	convertJsonToMaps();
}

DialogueManager::~DialogueManager() {
}


//___________________
// Public functions

void DialogueManager::generateDialogue(Role role, std::string prompt) {
	std::string dialogue = getRandomDialogue(role, prompt);
	createDialogueObject(role, dialogue);
}

void DialogueManager::generateDialogue(Role role, std::string prompt, std::string replace) {
	std::string dialogue = getRandomDialogue(role, prompt, replace);
	createDialogueObject(role, dialogue);
}

void DialogueManager::createDialogueObject(Role role, std::string dialogue) {
	Object* dialogue_object = new Object(game, "dialogue", dialogue_box);
	TextRenderer* text_renderer = new TextRenderer(game, *dialogue_object, dialogue);
	text_renderer->setMaxWidth(100.f);

	Vector2 anchor = role == Role::Merchant ? Vector2(1.f, 1.f) : Vector2(0.f, 0.f);
	Vector2 offset = role == Role::Merchant ? 
		Vector2(dialogue_box_size.x - 20.f, 20.f - dialogue_box_size.y) :
		Vector2(20.f, 20.f - dialogue_box_size.y);

	dialogue_object->setAnchor(role == Role::Merchant ? Vector2(1.f, 1.f) : Vector2(0.f, 0.f));
	dialogue_object->setLocalPosition(offset);
}

void DialogueManager::clearDialogue() {
}


//____________________
// Private functions

void DialogueManager::convertJsonToMaps() {
	std::ifstream dialogue_stream(dialogue_map_path);
	json combined_data = json::parse(dialogue_stream);

	json merchant_data = combined_data["merchant"];
	for (auto it = merchant_data.begin(); it != merchant_data.end(); ++it) {
		const std::string& prompt = it.key();
		merchant_lines[prompt] = it.value();
	}

	json customer_data = combined_data["customer"];
	for (auto it = customer_data.begin(); it != customer_data.end(); ++it) {
		const std::string& prompt = it.key();
		customer_lines[prompt] = it.value();
	}
}

std::string DialogueManager::getRandomDialogue(Role role, std::string prompt) {
	std::vector<std::string> lines;

	switch (role) {
		case Role::Merchant: lines = merchant_lines[prompt]; break;
		case Role::Customer: lines = customer_lines[prompt]; break;
	}

	if (lines.empty()) return std::string();
	int random_index = std::rand() % lines.size();
	return lines.at(random_index);
}

std::string DialogueManager::getRandomDialogue(
	Role role, std::string prompt, std::string replace
) {
	std::string dialogue = getRandomDialogue(role, prompt);
	size_t pos = dialogue.find(insert_block);
	if (pos != std::string::npos) {
		dialogue.replace(pos, insert_block.length(), replace);
	}
	return dialogue;
}
