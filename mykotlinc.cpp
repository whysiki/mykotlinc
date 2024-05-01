#include "sometools.h" // 引入自定义的工具函数
// cl compile with /std:c++latest
#define TEMPLATE_FILE_NAME_VECTOR \
    std::vector<std::string> { "Helloworld.kt", "build.gradle", "gradlew", "gradlew.bat", "run.bat", "settings.gradle", "gradle.properties" }

bool mykotlinc(CmdOptions options)
{
    // 获取当前目录
    std::string currentDirectory = getCurrentDirectory();
    // print("获取当前目录: " + currentDirectory);
    // 获取模板文件
    std::string templateDirectory = currentDirectory + "\\template";
    // print("模板文件目录: " + templateDirectory);
    // 获取模板文件字典
    std::map<std::string, TemplateFile> template_files_dict = getAllTemplateFile(templateDirectory, TEMPLATE_FILE_NAME_VECTOR);
    // print("获取模板文件字典: " + template_files_dict.size());
    // 构建目标目录结构
    std::string targetBuildDirectory = currentDirectory + "\\build" + "\\build";
    buildTargetDirectoryStructure(targetBuildDirectory, &template_files_dict);
    // print("构建目标目录结构: " + targetBuildDirectory);
    // std::string codeFilePath = options.filePath;
    // bool toClear = options.clear;
    // std::string params = options.params;
    // 获取代码文件的内容
    std::string codeFileContent = readFile(options.filePath);
    // print("获取代码文件的内容: " + codeFileContent.length());
    // 获取代码文件的哈希值
    std::string codeFileHash = getFileHash(options.filePath);
    // print("获取代码文件的哈希值: " + codeFileHash);
    // 把代码文件内容写入到模板文件字典中的Helloworld.kt 的tempBuildFilePath 并更新哈希值tempBuildFilePathHash
    // print("模板文件文件目录：" + template_files_dict["Helloworld.kt"].tempBuildFilePath);
    if (saveStringToFile(template_files_dict["Helloworld.kt"].tempBuildFilePath, codeFileContent))
    {
        template_files_dict["Helloworld.kt"].tempBuildFilePathHash = codeFileHash;
        // print("保存代码文件内容到模板文件字典中的Helloworld.kt成功");
    }
    else
    {
        return false;
    }
    // 运行cmd命令
    // condition ? expression_if_true : expression_if_false
    std::string commands = (strip(options.params).length() > 0) ? ("cd " + targetBuildDirectory + " && gradle run " + options.params) : ("cd " + targetBuildDirectory + " && gradle run");
    // print("运行命令: " + commands);
    std::string result = executeCommand(commands.c_str());
    print(result);
    if (options.clear)
    {
        if (removeDirectory(targetBuildDirectory, &template_files_dict))
        {
            print("Cleared build in " + targetBuildDirectory);
        }
    }
    return true;
}

int main(int argc, char *argv[]) // int argc, char* argv[]
{
    // CmdOptions options = { "D:\\KotlinCompiler\\mykotlinc\\test.kt", true, "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" };
    //
    if (argc <= 1)
    {
        print("usage:\n mykotlinc [-h] [--clear] [--params PARAMS] file_path \n mykotlinc file_path [--clear] [--params PARAMS]");
        return 0;
    }
    CmdOptions options = parseCmdOptions(argc, argv);
    if (!mykotlinc(options))
    {
        std::cerr << "Failed to compile the Kotlin file." << std::endl;
        return 1;
    }
    return 0;
}
