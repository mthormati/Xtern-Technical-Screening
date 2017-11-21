#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <regex>
#include <stack>
using namespace std;

//A data structure that will be used to identify an href link
//and the type of that link (url, relative path, absolute path)
struct link {
  string href;
  char hrefType; // u = url, r = relative path, a = absolute path
};

//--------------------------------------------------------------------------------------------
//START CLASS

//This class will take care of calculating all of the browser contexts and printing them
class BrowserContext {
  vector<link> links; //A vector of all the href links
  stack<string> visited; 
  //A stack containing all of the links that have been visited.
  //A stack data structure was chosen because it makes the task of 
  //doing the BACK operation simpler by allowing the most recent path
  //to be popped, letting the program know which part of the current browser context to remove
  stack<string> backedLinks;
  //Whenever there is a BACK command, the current link is pushed into this stack. This stack will
  //be used to determine the links that need to be visited if a FORWARD command is called. The stack
  //will be cleared if there is any path given after a BACK command.

  void emptyBackedLinks();  
  //Empty the stack of backedLinks

  //Handle various path scenarios
  string handleURL(string path);
  string handleAbsolutePath(string path);
  string handleRelativePath(string path);
  void handleBack();
  void handleForward();
public:
  BrowserContext(vector<link> links);
  void printBrowserContext();
};

BrowserContext::BrowserContext(vector<link> links) {
  this->links = links;
}

void BrowserContext::emptyBackedLinks() {
  while (!backedLinks.empty()) {
    backedLinks.pop();
  }
}

string BrowserContext::handleURL(string path) {
  //Push the url to the stack and empty backedLinks
  visited.push(path); 
  emptyBackedLinks();
  return path;
}

string BrowserContext::handleAbsolutePath(string path) {
  string context;
  //if visited is not empty, make context the current url + path
  //else, make context the current path
  if (!visited.empty()) {
    context = visited.top();
    context = context.substr(0, context.find(".com") + 4) + path;
  } else {
    context = path;
  }
  visited.push(context);
  emptyBackedLinks(); 
  return context;
}

string BrowserContext::handleRelativePath(string path) {
  string context;
  //If visited is not empty, make context the current path + new path
  //else make context equal to the parameter path
  if (!visited.empty()) {
    context = visited.top() + "/" + path;
  } else {
    context = "/" + path;
  }
  visited.push(context);
  emptyBackedLinks();
  return context;
}

void BrowserContext::handleBack() {
  //If visited is not empty, pop the top of visited and push that value to backedLinks
  if (!visited.empty()) {
    string backedLink = visited.top();
    visited.pop();
    backedLinks.push(backedLink);
  }
}

void BrowserContext::handleForward() {
  //If backedLinks is not empty, pop the top of backedLinks and push to visited
  if (!backedLinks.empty()) {
    string forwardLink = backedLinks.top();
    backedLinks.pop();
    visited.push(forwardLink);
  }
}

void BrowserContext::printBrowserContext() {
  string context = "";
  //Loop through each link in the vector of links
  for (link path : links) {
    //Depending on the path, call a different handler
    if (path.hrefType == 'u') {
      context = handleURL(path.href);
    } else if (path.hrefType == 'a') {
      context = handleAbsolutePath(path.href);
    } else {
      if (path.href.compare("BACK") == 0) {
        handleBack();
      } else if (path.href.compare("FORWARD") == 0) { 
        handleForward();
      } else {
        context = handleRelativePath(path.href);
      } 
    }

    //If the visited stack is not empty, then print the top of the stack
    //else, print a new line
    if (!visited.empty()) {
      cout << visited.top() << endl;
    } else {
      cout << endl;
    }
  }
}


//--------------------------------------------------------------------------------------------
//START MAIN

//Used to determine if the given path is an absolute path
bool isAbsolutePath(string path) {
  regex reg("\\/[A-Za-z0-9]+"); //Regex to check if the path is a valid absolute path

  //If the path is not empty and the first character is /
  if (path.length() > 1 && path[0] == '/') {
    string subPath = "";  //Will be used to store everything between two consecutives / symbols
    int slashIndex = 0;

    //Find the first / after the first index of the path. It then creates a subPath consisting of the first /
    //and everything before the second /. Then compares the subpath with the regex defined above. Sets new path
    //equal to everything excluding the subPath just checked.
    while ((slashIndex = path.find_first_of("/", 1)) != -1) {
      subPath = path.substr(0, slashIndex);
      if (regex_match(subPath, reg)) {
        path = path.substr(slashIndex, path.length()); 
      } else {
        return false;
      }
    }

    //Does a final check on the remainder of the path to see if valid absolute path
    if (regex_match(path, reg)) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

//Given an href link, this function will determine if it is a url, relative path, or absolute path
//It also checks to make sure that the input is valid and will terminate the program if it is not
char determineHrefType(string href) {
  regex url("https:\\/\\/[A-Za-z0-9]+\\.com");  //Regex for matching url

  //If href matches regex url then return 'u'
  //else if the href is an absolute path then return 'a'
  //else add a / before the href and check if it is an absolute path
  if (regex_match(href, url)) {
    return 'u';
  } else if (isAbsolutePath(href)) {
    return 'a';
  } else {
    string absolute = "/" + href; 
    //If the new string is an absolute path then the original href was a relative path
    //else the href is not a valid input
    if (isAbsolutePath(absolute)) {
      return 'r';
    } else {
      cout << "Invalid input: " + href << endl;
      exit(0);
    }
  }
}

//Reads inputs from stdin and places each line as an element in the inputs param
void readInput(vector<link> &inputs) {
  string line = "";

  while (cin >> line) {
    link newLink;
    newLink.href = line;
    newLink.hrefType = determineHrefType(line);
    inputs.push_back(newLink);
  }
}

int main() {
  vector<link> inputs;  //Vector to store inputs
  readInput(inputs);   

  BrowserContext link(inputs);
  link.printBrowserContext();

  return 0;
}