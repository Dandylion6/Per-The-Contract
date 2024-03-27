#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

#include "Components/Renderers/TextRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Managers/DialogueManager.h"


//_______________
// Constructors

DialogueManager::DialogueManager(Game& game) : game(game) {
	dialogue_box_size = game.getEnvironmentFactory().getDialogueBoxSize();
	dialogue_box = game.getObject("dialogue_box");

	merchant_offset = Vector2(
		dialogue_box_size.x - 20.f, -dialogue_box_size.y
	);
	customer_offset = Vector2(20.f, -dialogue_box_size.y);

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
	text_renderer->setMaxWidth(dialogue_max_width);

	float height_offset = dialogue_spacing;
	for (TextRenderer* renderer : dialogue_renderers) {
		height_offset += renderer->getSize().y + dialogue_spacing;
	}

	float new_dialogue_height = text_renderer->getSize().y;
	if (height_offset + new_dialogue_height >= dialogue_box_size.y) {
		removeAllDialogue();
		height_offset = dialogue_spacing;
	}

	dialogue_renderers.push_back(text_renderer);
	Vector2 anchor = role == Role::Merchant ? Vector2(1.f, 0.f) : Vector2(0.f, 0.f);
	Vector2 offset = role == Role::Merchant ? merchant_offset : customer_offset;

	dialogue_object->setAnchor(anchor);
	dialogue_object->setLocalPosition(offset + Vector2(0.f, height_offset));
}

void DialogueManager::removeDialogue() {
	 TextRenderer* dialogue_renderer = dialogue_renderers.front();
	 game.deleteObject(&dialogue_renderer->getObject());
	 dialogue_renderers.erase(dialogue_renderers.begin());
}

void DialogueManager::removeAllDialogue() {
	for (TextRenderer* renderer : dialogue_renderers) {
		game.deleteObject(&renderer->getObject());
	}
	dialogue_renderers.clear();
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
	int random_index = utils::RandomGenerator::randomIndex(lines.size());
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
