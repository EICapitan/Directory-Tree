// by ElCapitan 
// atdt-032220231037
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <map>
#include <bits/stdc++.h>

namespace fs = std::filesystem;

using std::cin;
using std::cout;
using std::endl;
using std::string;

class Path 
{
  public:
  string name;
  string path;
  bool all;
  bool noColor;

  //std::vector<std::vector<string>> context;
  std::vector<string> context;

  Path(string path, bool argv[]) {
    this->path = path;
    this->all = argv[0];
    this->noColor = argv[1];
    auto const pos = path.find_last_of('/');
    this->name = path.substr(pos + 1);

    getContext(path);
  }

  void displayPath() 
  {
    string const B_COLOR = "\u001b[34;1m";
    string const G_COLOR = "\u001b[32;1m";
    string const DEF_COLOR = "\u001b[39;0m";

    if (this->context.size() == 0) {
      return;
    }

    cout << this->name << "/\n";

    for (auto it = this->context.begin(); it != this->context.end(); ++it) 
    {
      const fs::path path(*it);
      string ON_COLOR;
      string ADD;
      string pa = *it;
      struct stat st;
      stat(pa.c_str(), &st);

      if (fs::is_directory(path)) 
      {
        ON_COLOR = B_COLOR;
      }
      else if ((st.st_mode & S_IEXEC) != 0)
      {
        ON_COLOR = G_COLOR;
        ADD = "*";
      }
      

      if (not this->all) {
        string word = *it;
        if (word[0] == '.') {
          continue;
        }
      }

      if (this->noColor) 
      {
        if (it == --this->context.end())
        {
          cout << "  └ " << *it << endl;
          continue;
        }
        cout << "  ├ " << *it << endl;
        continue;
      }

      if (it == --this->context.end())
      {
        cout << "  └ " << ON_COLOR << *it << DEF_COLOR << ADD << endl;
        continue;
      }
      cout << "  ├ " << ON_COLOR << *it << DEF_COLOR << ADD << endl;
    }
  }

  private:
  static bool mycomp(string a, string b){
	  return a<b;
  }

  static void alphabaticallySort(std::vector<string> &a){
    int n=a.size();
    sort(a.begin(),a.end(),mycomp);
  }
  
  static void RemoveWord(string word, string &line) 
  {
    auto n = line.find(word);

    if (n != std::string::npos)
    {
      line.erase(n, word.length());
    }
  }

  void getContext(string path) 
  {
    for (const auto & entry : fs::directory_iterator(path)) 
    {
      string pathPart = entry.path();

      RemoveWord(this->path + "/", pathPart);

      if (entry.is_directory()) {
        //std::vector<string> ctx = { pathPart + "/", "dir" };
        this->context.push_back(pathPart + "/");
        continue;
      }
      //std::vector<string> ctx = {pathPart, "file"};
      this->context.push_back(pathPart);
    }
    alphabaticallySort(this->context);
  }
};

string getInput(string PS1) 
{
  string inp;
  cout << PS1;
  cin >> inp;
  return inp;
}

int main(int argc, char *argv[]) {
  bool arguments[] = { false, false };
  std::map<string, int> trace;
  trace["--all"] = 1;
  trace["-a"] = 1;
  trace["--no-color"] = 2;
  trace["-nc"] = 2;

  for (char **pargv = argv+1; *pargv != argv[argc]; ++pargv) {
    switch (trace[*pargv])
    {
    case 1:
      arguments[0] = true;
      break;

    case 2:
      arguments[1] = true;
      break;
    }
  }

  string currentDir = fs::current_path();
  Path current(currentDir, arguments);
  current.displayPath();
  return 0;
}
