#include "sometools.h"
// cl compile with /std:c++latest
#define TEMPLATE_FILE_NAME_VECTOR \
    std::vector<std::string> { "Helloworld.kt", "build.gradle", "gradlew", "gradlew.bat", "run.bat", "settings.gradle", "gradle.properties" }

bool mykotlinc(CmdOptions options)
{
    std::string currentDirectory = getCurrentDirectory();
    std::string templateDirectory = currentDirectory + "\\template";

    std::map<std::string, TemplateFile> template_files_dict = getAllTemplateFile(templateDirectory, TEMPLATE_FILE_NAME_VECTOR);

    std::string targetBuildDirectory = currentDirectory + "\\build" + "\\build";
    buildTargetDirectoryStructure(targetBuildDirectory, &template_files_dict);

    std::string codeFileContent = readFile(options.filePath);

    std::string codeFileHash = getFileHash(options.filePath);

    if (saveStringToFile(template_files_dict["Helloworld.kt"].tempBuildFilePath, codeFileContent))
    {
        template_files_dict["Helloworld.kt"].tempBuildFilePathHash = codeFileHash;
    }
    else
    {
        return false;
    }

    std::string commands = (strip(options.params).length() > 0) ? ("cd " + targetBuildDirectory + " && gradle run " + options.params) : ("cd " + targetBuildDirectory + " && gradle run");
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

int main(int argc, char *argv[])
{
    // CmdOptions options = { "D:\\KotlinCompiler\\mykotlinc\\test.kt", true, "-w --no-rebuild --configuration-cache --parallel --daemon --build-cache --max-workers 6" };
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
