#include <fstream>
#include <iosfwd>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Components/Objects/Dialogue.h"
#include "Components/Renderers/SpriteRenderer.h"
#include "Components/Renderers/TextRenderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/RandomGenerator.h"
#include "Core/Utility/Vector2.h"
#include "Data/CustomerTrait.h"
#include "Data/DealData.h"
#include "Data/FontStyle.h"
#include "Data/Role.h"
#include "Managers/DialogueManager.h"
#include "nlohmann/json.hpp"


DialogueManager* DialogueManager::instance = nullptr;

//_______________
// Constructors

DialogueManager::DialogueManager(Game& game) : game(game) {
	if (instance != nullptr) {
		delete this;
		return;
	}
	instance = this;

	dialogue_box = game.getObject("dialogue_box");
	dialogue_box_size = dialogue_box->getComponent<SpriteRenderer>()->getSize();
	dialogue_offset = Vector2(40.f, -dialogue_box_size.y);
	convertJsonToMaps();
}

DialogueManager::~DialogueManager() {
}


//__________
// Getters

DialogueManager& DialogueManager::getInstance() {
	return *instance;
}


//___________________
// Public functions

void DialogueManager::generateNext() {
	if (dialogue_queue.empty()) return;
	std::pair<Role, std::string> pair = dialogue_queue.front();
	createDialogueObject(pair.first, pair.second);
	dialogue_queue.pop();
}

void DialogueManager::generateDialogue(Role role, std::string prompt) {
	std::string dialogue = getRandomDialogue(role, prompt);
	if (dialogue.empty()) return;
	if (!dialogue_renderers.empty() && dialogue_renderers.back()->isTyping()) {
		dialogue_queue.push(std::make_pair(role, dialogue));
	}  else createDialogueObject(role, dialogue);
}


void DialogueManager::generateDialogue(
	Role role, std::string prompt, std::string replace
) {
	std::string dialogue = getRandomDialogue(role, prompt, replace);
	if (dialogue.empty()) return;
	if (!dialogue_renderers.empty() && dialogue_renderers.back()->isTyping()) {
		dialogue_queue.push(std::make_pair(role, dialogue));
	} else createDialogueObject(role, dialogue);
}

void DialogueManager::createDialogueObject(Role role, std::string dialogue) {
	Object* dialogue_object = new Object(game, "dialogue", dialogue_box);
	bool is_merchant = role == Role::Merchant;
	
	FontStyle style = is_merchant ? FontStyle::IMFellDWPica : FontStyle::LibreBaskerville;
	std::string speaker = is_merchant ? "[You]: " : "[Client]: ";
	TextRenderer* text_renderer = new TextRenderer(game, *dialogue_object, style, speaker);
	
	text_renderer->setSize(is_merchant ? 24u : 20u);
	text_renderer->setMaxWidth(dialogue_max_width);
	text_renderer->typeText(speaker + dialogue);

	dialogue_renderers.push_back(text_renderer);
	dialogue_object->setAnchor(Vector2::scale(0.f));
	dialogue_object->setLocalPosition(dialogue_offset);
	new Dialogue(game, *dialogue_object, *text_renderer);
	updateDialogueList();
}

void DialogueManager::clearDialogue() {
	for (TextRenderer* renderer : dialogue_renderers) {
		game.deleteObject(&renderer->getObject());
	}
	dialogue_renderers.clear();
}


//____________________
// Private functions

void DialogueManager::updateDialogueList() {
	if (dialogue_renderers.size() > max_dialogue_lines) {
		TextRenderer* dialogue_renderer = dialogue_renderers.front();
		game.deleteObject(&dialogue_renderer->getObject());
		dialogue_renderers.erase(dialogue_renderers.begin());
	}

	float height_offset = dialogue_spacing - dialogue_box_size.y;
	for (TextRenderer* renderer : dialogue_renderers) {
		Object& object = renderer->getObject();
		float offset = object.getLocalPosition().x;
		object.setLocalPosition(Vector2(offset, height_offset));
		height_offset += renderer->getSize().y + dialogue_spacing;
	}
}

void DialogueManager::convertJsonToMaps() {
	std::ifstream merchant_stream(merchant_dialogue_path);
	nlohmann::json merchant_data = nlohmann::json::parse(merchant_stream);

	for (auto it = merchant_data.begin(); it != merchant_data.end(); ++it) {
		const std::string& prompt = it.key();
		merchant_lines[prompt] = it.value();
	}

	std::ifstream customer_stream(customer_dialogue_path);
	nlohmann::json customer_data = nlohmann::json::parse(customer_stream);

	for (auto it = customer_data.begin(); it != customer_data.end(); ++it) {
		const std::string& prompt = it.key();
		std::map<CustomerTrait, std::vector<std::string>> trait_dialogue;

		for (const auto& key_trait_pair : key_to_trait_map) {
			const auto& trait_key = key_trait_pair.first;
			const auto& trait_value = key_trait_pair.second;

			auto trait_it = it.value().find(trait_key);
			if (trait_it != it.value().end()) {
				trait_dialogue[trait_value] = *trait_it;
			} else {
				trait_dialogue[trait_value] = it.value()["default"];
			}
		}

		customer_lines[prompt] = trait_dialogue;
	}
}

std::string DialogueManager::getRandomDialogue(Role role, std::string prompt) {
	std::vector<std::string> lines;

	switch (role) {
		case Role::Merchant: {
			lines = merchant_lines.at(prompt);
			break;
		}
		case Role::Customer: {
			CustomerTrait trait = game.getDealData() != nullptr ? game.getDealData()->customer_trait : CustomerTrait::Haggler;
			lines = customer_lines.at(prompt).find(trait)->second;
			break;
		}
	}

	if (lines.empty()) return std::string();
	int random_index = utils::Random::randomIndex(lines.size());
	return lines.at(random_index);
}

std::string DialogueManager::getRandomDialogue(
	Role role, std::string prompt, std::string replace
) {
	std::string dialogue = getRandomDialogue(role, prompt);
	size_t position = dialogue.find(insert_block);
	if (position != std::string::npos) {
		dialogue.replace(position, insert_block.length(), replace);
	}
	return dialogue;
}
