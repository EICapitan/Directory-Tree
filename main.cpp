// by ElCapitan; AT PROJECT Limited
// ver. atdt-1.2.4
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
  bool size;
  bool access;

  std::vector<std::vector<string>> context;

  Path(string path, bool argv[]) {
    this->path = path;
    this->all = argv[0];
    this->noColor = argv[1];
    this->size = argv[2];
    this->access = argv[3];
    auto const pos = path.find_last_of('/');
    this->name = path.substr(pos + 1);

    getContext(path);
  }

  void displayPath()
  {
    string const B_COLOR = "\u001b[34;1m";
    string const G_COLOR = "\u001b[32;1m";
    string const R_COLOR = "\u001b[31;1m";
    string const DEF_COLOR = "\u001b[39;0m";

    if (this->context.size() == 0) {
      return;
    }

    string *TZ = new string;

    if (this->name.size() < 7) {
      *TZ = this->name + "/\t\t\t\t";
    }
    else if (this->name.size() < 14)
    {
      *TZ = this->name + "/\t\t\t";
    }
    else
    {
      *TZ = this->truncate(this->name, 20) + "/\t\t";
    }

    string optional;
    if (this->size) {
      cout << "TOTAL SIZE: " << this->fullSize << endl;
      optional += "\t   SIZE\t";
    }

    if (this->access) {
      optional += "\tPUSER\tGROUP\tOTHER";
    }

    cout << *TZ << "TYPE\t" << optional << "\n  │\n";

    delete(TZ);

    for (auto it = this->context.begin(); it != this->context.end(); ++it)
    {
      std::vector<string> item = *it;
      char *mode = (char*)malloc(sizeof(char) * 9 + 1);
      const fs::path path(item[0]);
      string *ON_COLOR = new string;
      string *ADD = new string;
      int ad = 0;
      string *T = new string;
      string pa = item[0];
      struct stat st;
      if (stat(pa.c_str(), &st) == 0) {
        mode_t perm = st.st_mode;
        mode[0] = (perm & S_IRUSR) ? 'r' : '-';
        mode[1] = (perm & S_IWUSR) ? 'w' : '-';
        mode[2] = (perm & S_IXUSR) ? 'x' : '-';
        mode[3] = (perm & S_IRGRP) ? 'r' : '-';
        mode[4] = (perm & S_IWGRP) ? 'w' : '-';
        mode[5] = (perm & S_IXGRP) ? 'x' : '-';
        mode[6] = (perm & S_IROTH) ? 'r' : '-';
        mode[7] = (perm & S_IWOTH) ? 'w' : '-';
        mode[8] = (perm & S_IXOTH) ? 'x' : '-';
      }

      if (item[1] == "SYMLINK") {
        *ON_COLOR = R_COLOR;
      }
      else if (st.st_mode & S_IFDIR)
      {
        *ON_COLOR = B_COLOR;
      }
      else if ((st.st_mode & S_IEXEC) != 0)
      {
        *ON_COLOR = G_COLOR;
        *ADD = "*";
        ad = 1;
      }

      if (item[0].size() + ad < 4) {
        *T = "\t\t\t\t";
      }
      else if (item[0].size() + ad < 12)
      {
        *T = "\t\t\t";
      }
      else if (item[0].size() + ad < 20)
      {
        *T = "\t\t";
      }
      else
      {
        item[0] = this->truncate(item[0], 20);
        *T = "\t";
      }

      string additional;

      if (this->size) {
        additional += "\t" + item[2];
      }

      if (this->access && item[1] != "SYMLINK") {
        additional = additional + "\t " + mode[0] + mode[1] + mode[2] + "\t " + mode[3] + mode[4] + mode[5] + "\t " + mode[6] + mode[7] + mode[8] ;
      }

      if (this->noColor)
      {
        if (it == --this->context.end())
        {
          cout << "  └ " << item[0] << *ADD << *T << item[1] << "\t";
          cout << additional << endl;
          continue;
        }
        cout << "  ├ " << item[0] << *ADD << *T << item[1] << "\t";
        cout << additional << endl;
        continue;
      }

      if (it == --this->context.end())
      {
        cout << "  └ " << *ON_COLOR << item[0] << DEF_COLOR << *ADD << *T << item[1] << "\t";
        cout << additional << endl;
        continue;
      }
      cout << "  ├ " << *ON_COLOR << item[0] << DEF_COLOR << *ADD << *T << item[1] << "\t";
      cout << additional << endl;

      delete(mode);
      delete(ON_COLOR);
      delete(T);
      delete(ADD);
    }
  }

  private:
  string fullSize;

  string truncate(std::string str, size_t width, bool show_ellipsis=true)
  {
    if (str.length() > width) {
      if (show_ellipsis) {
          return str.substr(0, width) + "...";
      }
      else
      {
        return str.substr(0, width);
      }
    }
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

  string convertSize(size_t size, bool t = true) {
    static const char *SIZES[] = { " B", "KB", "MB", "GB" };
    int div = 0;
    size_t rem = 0;

    while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
        rem = (size % 1024);
        div++;
        size /= 1024;
    }

    double size_d = (float)size + (float)rem / 1024.0;
    string result = removeZeros(std::to_string(roundOff(size_d)));
    if (t) {
      result += "\t";
    } else {
      result += " ";
    }
    result += SIZES[div];
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
    size_t fsize = 0;
    for (const auto & entry : fs::directory_iterator(path))
    {
      string pathPart = entry.path();

      RemoveWord(this->path + "/", pathPart);

      if (not this->all) {
        if (pathPart[0] == '.') {
          continue;
        }
      }

      if (entry.is_symlink()) {
        std::vector<string> ctx = {pathPart + "@", "SYMLINK", "\t -"};
        this->context.push_back(ctx);
        continue;
      }

      if (entry.is_directory()) {
        std::vector<string> ctx = {pathPart + "/", "DIR", "\t -"};
        this->context.push_back(ctx);
        continue;
      }
      fsize += entry.file_size();
      string size = convertSize(entry.file_size());
      std::vector<string> ctx = {pathPart, "FILE", size};
      this->context.push_back(ctx);
    }
    this->fullSize = convertSize(fsize, false);
    alphaSort(this->context);
  }
};

int main(int argc, char *argv[]) {
  string currentDir = fs::current_path();
  bool arguments[] = { false, false, false, false };
  std::map<string, int> trace;
  trace["--all"] = 1;
  trace["-a"] = 1;
  trace["--no-color"] = 2;
  trace["-n"] = 2;
  trace["--size"] = 3;
  trace["-s"] = 3;
  trace["--access"] = 4;
  trace["-A"] = 4;
  trace["--version"] = 5;

  for (char **pargv = argv+1; *pargv != argv[argc]; ++pargv) {
    switch (trace[*pargv])
    {
    case 1:
      arguments[0] = true;
      break;

    case 2:
      arguments[1] = true;
      break;

    case 3:
      arguments[2] = true;
      break;

    case 4:
      arguments[3] = true;
      break;

    case 5:
      cout << "DirectoryTree (ds) 1.2.4 [11.04.2023]\nCopyright (C) AT PROJECT Limited 2023\n" << endl;
      return 0;

    default:
      if (*pargv[0] != '-') {
        try {
          if (not *pargv[0] == '/') {
            currentDir += "/";
            currentDir += fs::path(*pargv);
          } else {
            currentDir = fs::path(*pargv);
          }

          if (currentDir.back() == '/' and currentDir.length() > 1) {
              currentDir.pop_back();
            }
        } catch (string err) {
          cout << "Folder can't be opened. Error: " << err << endl;
          return 0;
        }

        break;
      }

      string argm = *pargv;

      for (auto x: argm) {
        string arg (1, x);
        switch (trace["-" + arg])
        {
        case 1:
          arguments[0] = true;
          break;

        case 2:
          arguments[1] = true;
          break;

        case 3:
          arguments[2] = true;
          break;

        case 4:
          arguments[3] = true;
          break;
        }
      }
    }
  }
  
  Path current(currentDir, arguments);
  current.displayPath();
  return 0;
}
