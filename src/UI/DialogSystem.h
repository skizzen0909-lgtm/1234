// src/UI/DialogSystem.h
#ifndef DIALOGSYSTEM_H
#define DIALOGSYSTEM_H
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
class Player;
class Entity;
struct DialogueOption {
    std::string text;
    std::string nextDialogueId;
    bool available = true;
    sf::Color textColor = sf::Color::White;
};
struct DialogueNode {
    std::string id;
    std::string text;
    std::string speakerName;
    std::string portraitPath; // Путь к портрету говорящего
    std::vector<DialogueOption> options;
    bool isEnding = false;
};
class DialogSystem {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::RectangleShape dialogBox;
    sf::Text dialogText;
    sf::Text speakerText;
    sf::Texture speakerPortrait;
    sf::Sprite portraitSprite;
    std::vector<DialogueNode> dialogTree;
    size_t currentNodeIndex = 0;
    bool active = false;
    Player* player = nullptr;
    Entity* npc = nullptr;
    void loadPortraitForNode();
    void updateOptionsDisplay();
    void confirmSelection();
    const DialogueNode* findDialogue(const std::string& id) const;
    const DialogueNode* getCurrentDialogue() const;
public:
    explicit DialogSystem(sf::RenderWindow& window, sf::Font& font);
    ~DialogSystem() = default;
    bool initialize();
    bool startDialogue(const std::string& dialogueId, Player* player, Entity* npc);
    void endDialogue();
    bool isInDialogue() const; // <-- Это правильный метод!
    bool isActive() const { return isInDialogue(); }
    void update(float deltaTime);
    void draw(sf::RenderTarget& target); // Исправлено: принимает sf::RenderTarget&, а не void
    void handleEvent(const sf::Event& event);
    // Запрет копирования
    DialogSystem(const DialogSystem&) = delete;
    DialogSystem& operator=(const DialogSystem&) = delete;
};
#endif // DIALOGSYSTEM_H