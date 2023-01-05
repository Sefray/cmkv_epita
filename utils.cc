#include "utils.hh"

#include <filesystem>
namespace fs = std::filesystem;

std::vector<std::string> get_file_list(int n)
{
  std::vector<std::string> file_list;
  for (const auto& entry : fs::directory_iterator("data/input"))
  {
    std::string filename = entry.path().filename();
    if (filename[1] == '0' + n)
      file_list.push_back(entry.path());
  }

  return file_list;
}


