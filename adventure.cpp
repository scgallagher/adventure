#include <iostream>
#include <ncurses.h>
#include <vector>
#include <map>

using namespace std;

class StoryNode {
  private:
    string action; // what the user selects to get to this node
    string prompt;
    vector<StoryNode*> choices;
  public:
    StoryNode(string action, string prompt) {
      this->action = action;
      this->prompt = prompt;
    }

    ~StoryNode() {}

    string getAction() {
      return this->action;
    }

    string getPrompt() {
      return this->prompt;
    }

    vector<StoryNode*> getChoices() {
      return this->choices;
    }

    void addChoice(StoryNode* choice) {
      this->choices.push_back(choice);
    }
};

StoryNode* createChoiceTree() {
  
  StoryNode* subNodeA1 = new StoryNode("Try the door", "You attempt to open the front door. It is locked.");
  StoryNode* subNodeA2 = new StoryNode("Peer in the window", "Some furniture, nothing special.");
  StoryNode* subNodeA3 = new StoryNode("Go around back", "You walk around to the back of the house. There is a door and it is slightly ajar.");

  StoryNode* nodeA = new StoryNode("Approach the house", "It's an old colonial farmhouse.");
  nodeA->addChoice(subNodeA1);
  nodeA->addChoice(subNodeA2);
  nodeA->addChoice(subNodeA3);

  StoryNode* subNodeB1 = new StoryNode("Get up", "Break time's over. You rise to your feet.");

  StoryNode* nodeB = new StoryNode("Sit down", "You sit down in the long grass. It's a nice place for a break.");
  nodeB->addChoice(subNodeB1);

  StoryNode* subNodeC1 = new StoryNode("Go home.", "You're not very adventurous, are you?");

  StoryNode* nodeC = new StoryNode("Turn and walk away", "You face a road that leads home.");
  nodeC->addChoice(subNodeC1);

  subNodeB1->addChoice(nodeA);
  subNodeB1->addChoice(nodeB);
  subNodeB1->addChoice(nodeC);

  StoryNode* root = new StoryNode("", "You find yourself in the middle of a field. The sky is dark, a storm approaches. You see a house in the distance.");
  root->addChoice(nodeA);
  root->addChoice(nodeB);
  root->addChoice(nodeC);

  return root;
}

int main(int argc, char **argv) {
  initscr();
  scrollok(stdscr, TRUE);
  keypad(stdscr, TRUE);

  int height, width;
  getmaxyx(stdscr, height, width);
  WINDOW* win = newwin(height, width, 0, 0);

  StoryNode* node = createChoiceTree();
  char usr_selection;
  do {
    addstr(node->getPrompt().c_str());
    addstr("\n");

    char option = 'a';
    map<char, StoryNode*> choiceMap;
    for (StoryNode* choice : node->getChoices()) {
      char opt_arr[] = {option, '\0'};
      addstr(opt_arr);
      addstr(") ");
      addstr(choice->getAction().c_str());
      addstr("\n");
      refresh();

      choiceMap[option++] = choice;
    }

    if (choiceMap.empty()) {
      addstr("\nThe End");
      getch();
      break;
    }

    addstr("> ");
    refresh();

    char usr_input[8];
    getstr(usr_input);

    if (usr_input[0] != '\0') {
      usr_selection = usr_input[0];
      node = choiceMap.find(usr_selection)->second;
    }

    addstr("\n");
    refresh();
  } while (usr_selection != 'q');

  endwin();
  return 0;
}