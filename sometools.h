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

// 解析命令行参数，返回CmdOptions对象
// mykotlinc -f main.kt -c -p "-include-runtime -d out.jar"
// mykotlinc -f <file_path> [-c] [-p <params>]
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
    // 检查文件路径是否存在
    if (!std::filesystem::exists(options.filePath))
    {
        std::cerr << "File not found: " << options.filePath << std::endl;
        exit(1);
    }

    return options;
}

// C++20 auto parameter 支持
void print(const auto &s)
{
    std::cout << s << std::endl;
}
// 读取一个文件的内容
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
// 获取一个文件的哈希值
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
// 获取当前目录，需要C++17支持，不能超过MAX_PATH长度
std::string getCurrentDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string filePath(buffer);
    std::size_t found = filePath.find_last_of("\\");
    std::string directory = filePath.substr(0, found);
    return directory;
}
// 获取所有模板文件，参数为模板文件目录路径，返回值为模板文件字典，key为文件名，value为模板文件对象
std::map<std::string, TemplateFile> getAllTemplateFile(const std::string &directoryPath, const std::vector<std::string> &templateFileNameVector)
{

    std::map<std::string, TemplateFile> template_files_dict;
    for (const auto &entry : std::filesystem::recursive_directory_iterator(directoryPath))
    {
        if (std::filesystem::is_regular_file(entry))
        {
            // 获取文件名
            std::string filename = entry.path().filename().string();
            // 判断文件名是否在模板文件列表中
            if (std::find(templateFileNameVector.begin(), templateFileNameVector.end(), filename) != templateFileNameVector.end())
            {
                // 读取文件内容
                std::ifstream file(entry.path());
                if (!file)
                {
                    std::cerr << "Failed to open file: " << entry.path() << std::endl;
                    continue;
                }
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                // 获取文件路径
                std::string path = entry.path().string();
                // 创建模板文件对象
                std::string tempBuildFilePath = "";
                std::string tempBuildFilePathHash = "";
                TemplateFile templateFile = {path, content, tempBuildFilePath, tempBuildFilePathHash};
                // 将模板文件对象添加到字典中
                template_files_dict[filename] = templateFile;
            }
        }
    }
    return template_files_dict;
}

//// 保存字符串到指定文件名，参数为文件名和字符串内容，如果文件存在则覆盖，如果文件不存在则创建，并自动创建文件的父级目录
bool saveStringToFile(const std::string &filename, const std::string &content)
{
    // 创建文件的父级目录
    std::filesystem::path filePath(filename);
    if (filePath.has_parent_path() && !std::filesystem::exists(filePath.parent_path()))
    {
        std::filesystem::create_directories(filePath.parent_path());
    }

    // 保存字符串到文件
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

// 构建目标目录结构
bool buildTargetDirectoryStructure(const std::string &targetBuildDirectory, std::map<std::string, TemplateFile> *template_files_dict)
{
    // 创建build目录
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
    // 创建src目录
    std::string srcDirectory = buildDirectory + "\\src";
    if (!std::filesystem::exists(srcDirectory))
    {
        std::filesystem::create_directory(srcDirectory);
    }
    // 创建main目录
    std::string mainDirectory = srcDirectory + "\\main";
    if (!std::filesystem::exists(mainDirectory))
    {
        std::filesystem::create_directory(mainDirectory);
    }
    // 创建kotlin目录
    std::string kotlinDirectory = mainDirectory + "\\kotlin";
    if (!std::filesystem::exists(kotlinDirectory))
    {
        std::filesystem::create_directory(kotlinDirectory);
    }
    // 创建resources目录
    std::string resourcesDirectory = mainDirectory + "\\resources";
    if (!std::filesystem::exists(resourcesDirectory))
    {
        std::filesystem::create_directory(resourcesDirectory);
    }
    // 保存模板文件到目标目录
    for (const auto &templateFile : *template_files_dict)
    {
        // 获取文件名
        std::string filename = templateFile.first;
        // 获取文件内容
        std::string content = templateFile.second.content;
        std::string tempBuildFilePath = "";

        // 模板文件路径
        // std::string path = templateFile.second.templateFilePath;
        // 保存文件到目标目录
        if (filename == "Helloworld.kt")
        {
            saveStringToFile(kotlinDirectory + "\\" + filename, content);
            tempBuildFilePath = kotlinDirectory + "\\" + filename;
        }
        else
        {
            saveStringToFile(buildDirectory + "\\" + filename, content);
            tempBuildFilePath = buildDirectory + "\\" + filename;
        }
        // 更新模板文件对象的tempBuildFilePath
        (*template_files_dict)[filename].tempBuildFilePath = tempBuildFilePath;
        // 计算文件哈希值
        std::string tempBuildFilePathHash = getFileHash(tempBuildFilePath);
        // 更新模板文件对象的tempBuildFilePathHash
        (*template_files_dict)[filename].tempBuildFilePathHash = tempBuildFilePathHash;
    }
    return true;
}

// 删除目录及其子目录和文件
bool removeDirectory(const std::string &directoryPath, std::map<std::string, TemplateFile> *template_files_dict)
{
    if (!std::filesystem::exists(directoryPath))
    {
        return false;
    }
    std::filesystem::remove_all(directoryPath);
    // 更新模板文件对象的tempBuildFilePath
    for (auto &templateFile : *template_files_dict)
    {
        templateFile.second.tempBuildFilePath = "";
        templateFile.second.tempBuildFilePathHash = "";
    }
    return true;
}

// 执行命令，返回命令执行结果
std::string executeCommand(const char *cmd)
{
    std::array<char, 128> buffer; // 定义了一个字符数组 buffer，用于存储从子进程读取的数据。
    std::string result;
    // 创建了一个管道。_popen 函数执行一个命令，并返回一个指向子进程的标准输出的 FILE 指针。
    // std::unique_ptr 确保 FILE 指针在离开作用域时被正确关闭。
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe)
    {
        // 抛出一个运行时错误。如果 _popen 函数失败，pipe 就会是一个空指针。
        throw std::runtime_error("popen() failed!");
    }
    // fgets 函数从 pipe 指向的 FILE 对象中读取最多 buffer.size() 个字符，并将它们存储到 buffer 中。
    // 如果读取成功，fgets 返回一个指向 buffer 的指针；如果到达文件末尾，它返回一个空指针。
    while (fgets(buffer.data(), (int)(buffer.size()), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}
// 去除字符串左侧的空白字符
std::string lstrip(const std::string &s)
{
    auto it = std::find_if(s.begin(), s.end(), [](char c)
                           { return !std::isspace(c); });
    return std::string(it, s.end());
}

// 去除字符串右侧的空白字符
std::string rstrip(const std::string &s)
{
    auto it = std::find_if(s.rbegin(), s.rend(), [](char c)
                           { return !std::isspace(c); });
    return std::string(s.begin(), it.base());
}

// 去除字符串两侧的空白字符
std::string strip(const std::string &s)
{
    return lstrip(rstrip(s));
}