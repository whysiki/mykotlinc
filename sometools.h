#pragma once
#include <cstdio>
#include <iostream>
#include <Windows.h>
#include <string>
#include <filesystem>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <array>
#include <boost/program_options.hpp>

typedef struct
{
    std::string templateFilePath;
    std::string content;
    std::string tempBuildFilePath;
    std::string tempBuildFilePathHash;
} TemplateFile;

typedef struct
{
    std::string filePath;
    bool clear;
    std::string params;
} CmdOptions;

CmdOptions parseCmdOptions(int argc, char *argv[]);
void print(const auto &s);
void create_directories_if_not_exists(const std::vector<std::string> &directories);
std::string readFile(const std::string &filename);
std::string getFileHash(const std::string &filename);
std::string getCurrentDirectory();
std::map<std::string, TemplateFile> getAllTemplateFile(const std::string &directoryPath, const std::vector<std::string> &templateFileNameVector);
bool saveStringToFile(const std::string &filename, const std::string &content);
bool buildTargetDirectoryStructure(const std::string &targetBuildDirectory, std::map<std::string, TemplateFile> *template_files_dict);
bool removeDirectory(const std::string &directoryPath, std::map<std::string, TemplateFile> *template_files_dict);
std::string executeCommand(const char *cmd);
std::string lstrip(const std::string &s);
std::string rstrip(const std::string &s);
std::string strip(const std::string &s);
//
//
//
//
//
//
//
CmdOptions parseCmdOptions(int argc, char *argv[])
{
    namespace po = boost::program_options;

    CmdOptions options;
    po::options_description desc("options");
    desc.add_options()("help,h", "show this help message and exit")("clear", po::bool_switch(&options.clear), "Clear build before compiling.")("params", po::value<std::string>(&options.params), "Additional other gradle parameters for the compiler.");

    po::options_description hidden("Hidden options");
    hidden.add_options()("file_path", po::value<std::string>(&options.filePath), "Path to the Kotlin file.");

    po::options_description all("All options");
    all.add(desc).add(hidden);

    po::positional_options_description p;
    p.add("file_path", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(all).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << "usage: mykotlinc.py [-h] [--clear] [--params PARAMS] file_path\n\nCompile Kotlin files.\n\npositional arguments:\n  file_path        Path to the Kotlin file.\n\n"
                  << desc << std::endl;
        exit(0);
    }

    if (options.filePath.empty())
    {
        std::cerr << "File path is required" << std::endl;
        exit(1);
    }
    if (!std::filesystem::exists(options.filePath))
    {
        std::cerr << "File not found: " << options.filePath << std::endl;
        exit(1);
    }

    return options;
}

// C++20 auto parameter
void print(const auto &s)
{
    std::cout << s << std::endl;
}
std::string readFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}
std::string getFileHash(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return std::to_string(std::hash<std::string>{}(content));
}
// C++17
std::string getCurrentDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string filePath(buffer);
    std::size_t found = filePath.find_last_of("\\");
    std::string directory = filePath.substr(0, found);
    return directory;
}
std::map<std::string, TemplateFile> getAllTemplateFile(const std::string &directoryPath, const std::vector<std::string> &templateFileNameVector)
{

    std::map<std::string, TemplateFile> template_files_dict;
    for (const auto &entry : std::filesystem::recursive_directory_iterator(directoryPath))
    {
        if (std::filesystem::is_regular_file(entry))
        {
            std::string filename = entry.path().filename().string();
            if (std::find(templateFileNameVector.begin(), templateFileNameVector.end(), filename) != templateFileNameVector.end())
            {
                std::ifstream file(entry.path());
                if (!file)
                {
                    std::cerr << "Failed to open file: " << entry.path() << std::endl;
                    continue;
                }
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                std::string path = entry.path().string();
                std::string tempBuildFilePath = "";
                std::string tempBuildFilePathHash = "";
                TemplateFile templateFile = {path, content, tempBuildFilePath, tempBuildFilePathHash};
                template_files_dict[filename] = templateFile;
            }
        }
    }
    return template_files_dict;
}

bool saveStringToFile(const std::string &filename, const std::string &content)
{
    // Create directories if not exists
    std::filesystem::path filePath(filename);
    if (filePath.has_parent_path() && !std::filesystem::exists(filePath.parent_path()))
    {
        std::filesystem::create_directories(filePath.parent_path());
    }

    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    file << content;
    file.close();
    return true;
}
void create_directories_if_not_exists(const std::vector<std::string> &directories)
{
    for (const auto &directory : directories)
    {
        if (!std::filesystem::exists(directory))
        {
            std::filesystem::create_directory(directory);
        }
    }
}

bool buildTargetDirectoryStructure(const std::string &targetBuildDirectory, std::map<std::string, TemplateFile> *template_files_dict)
{
    std::string buildDirectory = targetBuildDirectory;
    if (!std::filesystem::exists(buildDirectory))
    {
        std::filesystem::path buildDirectoryPath(buildDirectory);
        if (buildDirectoryPath.has_parent_path())
        {
            std::filesystem::create_directories(buildDirectoryPath);
        }
        else
        {
            std::filesystem::create_directory(buildDirectory);
        }
    }
    std::vector<std::string> directories = {
        buildDirectory + "\\src",
        buildDirectory + "\\src\\main",
        buildDirectory + "\\src\\main\\kotlin",
        buildDirectory + "\\src\\main\\resources"};

    std::string kotlinDirectory = buildDirectory + "\\src\\main\\kotlin";
    for (const auto &templateFile : *template_files_dict)
    {
        std::string filename = templateFile.first;
        std::string content = templateFile.second.content;
        std::string tempBuildFilePath = (filename == "Helloworld.kt") ? (kotlinDirectory + "\\" + filename) : (buildDirectory + "\\" + filename);
        std::string currentContent = std::filesystem::exists(tempBuildFilePath) ? readFile(tempBuildFilePath) : "";

        if (strip(currentContent) != strip(content))
        {
            saveStringToFile(tempBuildFilePath, content);
        }
        (*template_files_dict)[filename].tempBuildFilePath = tempBuildFilePath;
        std::string tempBuildFilePathHash = getFileHash(tempBuildFilePath);
        (*template_files_dict)[filename].tempBuildFilePathHash = tempBuildFilePathHash;
    }
    return true;
}

bool removeDirectory(const std::string &directoryPath, std::map<std::string, TemplateFile> *template_files_dict)
{
    if (!std::filesystem::exists(directoryPath))
    {
        return false;
    }
    std::filesystem::remove_all(directoryPath);
    for (auto &templateFile : *template_files_dict)
    {
        templateFile.second.tempBuildFilePath = "";
        templateFile.second.tempBuildFilePathHash = "";
    }
    return true;
}
std::string executeCommand(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), (int)(buffer.size()), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}
std::string lstrip(const std::string &s)
{
    auto it = std::find_if(s.begin(), s.end(), [](char c)
                           { return !std::isspace(c); });
    return std::string(it, s.end());
}
std::string rstrip(const std::string &s)
{
    auto it = std::find_if(s.rbegin(), s.rend(), [](char c)
                           { return !std::isspace(c); });
    return std::string(s.begin(), it.base());
}
std::string strip(const std::string &s)
{
    return lstrip(rstrip(s));
}