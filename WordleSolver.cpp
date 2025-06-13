#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
using namespace std;
class WordleSolver {

  private:
    vector<string> ignoreWords;
    vector<string> board;

  public: 
    void display(const vector<string> currentBoard) {
        cout << "[";
        for (size_t i = 0; i < currentBoard.size(); ++i) {
            cout << currentBoard[i];
            if (i < currentBoard.size() - 1) {
                cout << ", ";
            }
        }
    cout << "]";
    }

    void resetBoard() {
      board.clear();
      for(int i=0 ; i<5 ; ++i) {
        board.push_back(" _");
      }
    }

    void play() {
      resetBoard();
        initializeBoard();

        while (true) {
            std::vector<std::string> rows = letterWords();
            std::string ignore = getInput("Type in the letters to ignore (if any): ");
            std::istringstream iss(ignore);
            ignoreWords = std::vector<std::string>((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

            if (!ignoreWords.empty() && !ignoreWords[0].empty()) {
                rows = remover(rows, ignoreWords[0]);
            }

            std::vector<std::string> results = matcher(rows, board);
            results = mustHave(results);

            std::cout << "Here are the results of the matches:" << std::endl;
            if (results.empty()) {
                std::cout << "[]" << std::endl;
            } else {
                std::cout << "[";
                if (results.size() <= 10) {
                    for (size_t i = 0; i < results.size(); ++i) {
                        std::cout << results[i];
                        if (i < results.size() - 1) {
                            std::cout << ", ";
                        }
                    }
                } else {
                    for (size_t i = 0; i < 10; ++i) {
                        std::cout << results[i];
                        if (i < 9) {
                            std::cout << ", ";
                        }
                    }
                }
                std::cout << "]" << std::endl;
            }

            std::string nextMove = getInput("Do you want to continue (c), reset (r), or exit (e)? ");
            if (nextMove == "e") {
                break;
            } else if (nextMove == "r") {
                resetBoard();
                initializeBoard();
            } else if (nextMove == "c") {
                initializeBoard();
            }
        }
    }

    string getInput(string prompt) {
      cout << prompt << " ";
      string input;
      getline(cin,input);
      return input;
    }

    int userEntry(int location) {
      string initialState = board[location];
      board[location] = "=";
      display(board);
      string userInput = getInput("");

      if(userInput == " " || userInput.empty()) {
          board[location] = initialState;
          return location + 1;
      } else if (userInput == "0") {
          board[location] = initialState;
          return 5;
      } else if(userInput == "1") {
          board[location] = "_";
          return max(location - 1,0);
      } else {
          board[location] = userInput;
          return location + 1;
      }
    }

    void initializeBoard() {
      int i = 0;
      while(i < 5) {
        i = userEntry(i);
      }
      display(board);
      cout << endl;
    }

    vector<string> letterWords() {
      vector<string> rows;
      fstream file("Wordle_Solver/words.txt");
      if(file.is_open()) {
        string line;
        while (getline(file,line)) {
          rows.push_back(line);
        }
        file.close();
      } else {
        cerr << "Unable to open file";
      }
      return rows;
    }

    std::vector<std::string> matcher(std::vector<std::string> bigList, std::vector<std::string> target) {
        std::vector<std::string> results;

        for (const std::string word : bigList) {
            bool isMatch = true;
            for (int i = 0; i < 5; i++) {
                if (target[i] != "_" && target[i] != std::string(1, word[i])) {
                    isMatch = false;
                    break;
                }
            }
            if (isMatch) {
                results.push_back(word);
            }
        }

        return results;
    }

    std::vector<std::string> remover(const std::vector<std::string> bigList, const std::string ignore) {
        std::vector<std::string> newList;
        for (const auto word : bigList) {
            bool flag = true;
            for (char ch : ignore) {
                if (word.find(ch) != std::string::npos) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                newList.push_back(word);
            }
        }
        return newList;
    }  

    std::vector<std::string> mustHave(const std::vector<std::string> bigList) {
        std::string must = getInput("Type in the letters to include but not sure of location: ");
        std::vector<std::string> newList;
        for (const auto& word : bigList) {
            bool flag = true;
            for (char ch : must) {
                if (word.find(ch) == std::string::npos) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                newList.push_back(word);
            }
        }
        return newList;
    }

};

int main(){
      WordleSolver solver;
      solver.play();

      return 0;
    }
