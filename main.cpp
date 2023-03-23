// by ElCapitan; AT PROJECT Limited
// ver. atdt-1.1.1
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

  std::vector<std::vector<string>> context;

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

    string TZ;

    if (this->name.size() < 7) {
      TZ = this->name + "/\t\t\t\t";
    }
    else if (this->name.size() < 14)
    {
      TZ = this->name + "/\t\t\t";
    }
    else 
    {
      TZ = this->truncate(this->name, 20) + "/\t\t";
    }

    cout << TZ << "TYPE\t\t   SIZE\n  │\n";

    for (auto it = this->context.begin(); it != this->context.end(); ++it) 
    {
      std::vector<string> item = *it;
      const fs::path path(item[0]);
      string ON_COLOR;
      string ADD;
      string T;
      string pa = item[0];
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

      if (item[0].size() < 11) 
      {
        T = "\t\t\t";
      } 
      else if (item[0].size() < 20) 
      {
        T = "\t\t";
      }
      else
      {
        item[0] = this->truncate(item[0], 20);
        T = "\t";
      }


      if (not this->all) {
        string word = item[0];
        if (word[0] == '.') {
          continue;
        }
      }

      if (this->noColor) 
      {
        if (it == --this->context.end())
        {
          cout << "  └ " << item[0] << "/" << ADD << T << item[1] << "\t\t"; 
          cout << item[2] << endl;
          continue;
        }
        cout << "  ├ " << item[0] << "/" << ADD << T << item[1] << "\t\t"; 
        cout << item[2] << endl;
        continue;
      }

      if (it == --this->context.end())
      {
        cout << "  └ " << ON_COLOR << item[0] << DEF_COLOR << "/" << ADD << T << item[1] << "\t\t"; 
        cout << item[2] << endl;
        continue;
      }
      cout << "  ├ " << ON_COLOR << item[0] << DEF_COLOR << "/"  << ADD << T << item[1] << "\t\t";
      cout << item[2] << endl;
    }
  }

  private:
  string truncate(std::string str, size_t width, bool show_ellipsis=true)
  {
    if (str.length() > width)
        if (show_ellipsis)
            return str.substr(0, width) + "...";
        else
            return str.substr(0, width);
    return str;
  }

  string removeZeros(string s) {
    for (int i = s.size() - 1; i >= 1; i--) {
      if (s.at(i) == '0') {
        s.pop_back();
      } else if (s.at(i) == '.') {
          s.pop_back();
          break;
      } else {
          break; 
      }
    }
    return s;
  }

  double roundOff(double n) {
    double d = n * 100.0;
    int i = d + 0.5;
    d = (float)i / 100.0;
    return d;
  }

  string convertSize(size_t size) {              
    static const char *SIZES[] = { " B", "KB", "MB", "GB" };
    int div = 0;
    size_t rem = 0;

    while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
        rem = (size % 1024);
        div++;
        size /= 1024;
    }

    double size_d = (float)size + (float)rem / 1024.0;
    string result = removeZeros(std::to_string(roundOff(size_d))) + "\t" + SIZES[div];
    return result;
  }

  int file_size(const char *path) {
    struct stat results;

    if (stat(path, &results) == 0) {
        return results.st_size;
    } else {
        return -1;
    }
  }

  static bool mycomp(std::vector<string> a, std::vector<string> b){
	  return a[0]<b[0];
  }

  static void alphaSort(std::vector<std::vector<string>> &a){
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
        std::vector<string> ctx = {pathPart, "DIR", "\t -"};
        this->context.push_back(ctx);
        continue;
      }
      string size = convertSize(entry.file_size());
      std::vector<string> ctx = {pathPart, "FILE", size};
      this->context.push_back(ctx);
    }
    alphaSort(this->context);
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
  const double x = 0.33, y = 42.3748;
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
