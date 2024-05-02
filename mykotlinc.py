from os import path, makedirs
from subprocess import run, PIPE
from sys import exit
from shutil import rmtree
from hashlib import md5
import shelve
import argparse

# The principle of this program is that when a Kotlin code file is executed, the Kotlin code will replace the contents of Helloworld.kt and run.
# When the Kotlin code file is modified, the code in Helloworld.kt is updated and rerun; otherwise, it is not updated.

# A minimal gradle project directory structure.
"""
|+-- src/
    |    |+-- main/
    |    |    |+-- kotlin/
    |    |    |    |-- Helloworld.kt
    |    |    |+-- resources/
    |-- build.gradle
    |-- gradlew
    |-- gradlew.bat
    |-- run.bat
    |-- settings.gradle
"""
#
# CurrentPath=path.dirname(path.abspath(__file__))
CurrentPath = path.dirname(__file__)
Content = {}
Content_templat_path_dict: dict[str, str] = {}
template_files_name = {
    "Helloworld.kt",
    "build.gradle",
    "gradlew",
    "gradlew.bat",
    "run.bat",
    "settings.gradle",
    "gradle.properties",
}
for i in template_files_name:
    template_path = path.join(CurrentPath, "template", i)
    with open(
        template_path,
        "r",
        encoding="UTF-8",
        errors="ignore",
    ) as f:
        Content[i] = f.read()
        Content_templat_path_dict[i] = template_path


def get_file_hash(file_path: str) -> str:
    """
    Compute the MD5 hash of a file.

    This function reads a file in chunks of 4096 bytes and updates the MD5 hash
    for each chunk until the file ends. This method is efficient for large files
    as it doesn't need to load the entire file into memory.

    Args:
        file_path (str): The path to the file for which the MD5 hash is to be computed.

    Returns:
        str: The MD5 hash of the file as a hexadecimal string.

    Raises:
        FileNotFoundError: If `file_path` does not exist.
    """
    hasher = md5()
    with open(file_path, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hasher.update(chunk)
    return hasher.hexdigest()


def update_hash(
    filename_filepath_dict: dict[str, str],
    hash_file_path: str = path.join(CurrentPath, "temp", "hash.db"),
    update_file_name: str = "",
) -> None:
    """
    Update the hash values of files in a shelve database.

    This function opens a shelve database at `hash_file_path` and updates the hash values of files.
    If the `update_file_name` is provided, only the hash of that file is updated.
    Otherwise, the hash values of all files in `filename_filepath_dict` are updated.

    Args:
        filename_filepath_dict (dict[str, str]): A dictionary mapping file names to file paths.
        hash_file_path (str, optional): The path to the shelve database. Defaults to "temp/hash.db" in the current directory.
        update_file_name (str, optional): The name of the file to update. If empty, all files are updated. Defaults to "".

    Raises:
        FileNotFoundError: If the file specified by `update_file_name` does not exist in `filename_filepath_dict`.

    example:
        update_hash(
                    filename_filepath_dict=Content_templat_path_dict,
                    update_file_name="Helloworld.kt",
                )
    """
    if path.exists(path.dirname(hash_file_path)) is False:
        makedirs(path.dirname(hash_file_path), exist_ok=True)
    with shelve.open(hash_file_path) as s:
        if (s.__len__() < filename_filepath_dict.__len__()) and (
            not (update_file_name)
        ):
            s.update({k: get_file_hash(v) for k, v in filename_filepath_dict.items()})
        else:
            s[update_file_name] = get_file_hash(
                filename_filepath_dict[update_file_name]
            )


def template_file_is_modify(
    file_name: str,
    filename_filepath_dict: dict[str, str],
    hash_file_path: str = path.join(CurrentPath, "template", "hash.db"),
) -> bool:
    """
    Check if a template file has been modified.

    This function compares the current hash of the file with the stored hash in a shelve database.
    If the hashes do not match, the file has been modified.

    Args:
        file_name (str): The name of the file to check.
        filename_filepath_dict (dict[str, str]): A dictionary mapping file names to file paths.
        hash_file_path (str, optional): The path to the shelve database. Defaults to "template/hash.db" in the current directory.

    Returns:
        bool: True if the file has been modified, False otherwise.

    Raises:
        KeyError: If `file_name` does not exist in `filename_filepath_dict`.
    """
    with shelve.open(hash_file_path) as s:
        newhash = get_file_hash(filename_filepath_dict[file_name])
        oldhash = s.get(file_name)
        return newhash != oldhash


def make_most_simple_gradle_kotlin_project(current_path: str = CurrentPath):
    # read from template

    contentdict: dict[str, str] = Content

    try:
        path_hello_kotlin: str = path.join(current_path, "src", "main", "kotlin")
        path_hello_resources: str = path.join(current_path, "src", "main", "resources")
        makedirs(path_hello_kotlin, exist_ok=True)
        makedirs(path_hello_resources, exist_ok=True)
        path_Helloworld = path.join(path_hello_kotlin, "Helloworld.kt")
        # first write
        if not path.exists(path_Helloworld):
            with open(path_Helloworld, "w") as f:
                f.write(contentdict["Helloworld.kt"])
        else:
            # modify if update
            if template_file_is_modify(
                "Helloworld.kt", filename_filepath_dict=Content_templat_path_dict
            ):
                with open(path_Helloworld, "w") as ff:
                    ff.write(contentdict["Helloworld.kt"])
                print("updated Helloworld.kt")
                update_hash(
                    filename_filepath_dict=Content_templat_path_dict,
                    update_file_name="Helloworld.kt",
                )
        del contentdict["Helloworld.kt"]
        for k, v in contentdict.items():
            pathi = path.join(current_path, k)
            if not path.exists(pathi):
                with open(
                    pathi,
                    "w",
                    encoding="UTF-8",
                    errors="ignore",
                ) as f2:
                    f2.write(v)
            else:
                # modify if update
                if template_file_is_modify(
                    k, filename_filepath_dict=Content_templat_path_dict
                ):
                    with open(
                        pathi,
                        "w",
                        encoding="UTF-8",
                        errors="ignore",
                    ) as f3:
                        f3.write(v)
                    print(f"Updated {k}")
                    update_hash(
                        filename_filepath_dict=Content_templat_path_dict,
                        update_file_name=k,
                    )
    except Exception:
        print("Raise unknown error!")
        exit()


def mycompilter(
    file_path: str,
    project_path: str = CurrentPath,
    params: str = "",
    toclearbuild: bool = False,
):
    make_most_simple_gradle_kotlin_project(project_path)

    def read_from_code(path: str = file_path) -> str:
        with open(path, "r", encoding="UTF-8", errors="ingore") as f:
            return f.read()

    def change_code(write_code: str, original_code_path: str) -> bool:
        with open(original_code_path, "w+", encoding="UTF-8", errors="ignore") as f:
            if wirte_code == f.read():
                return False
            f.write(write_code)
            return True

    def delete_build(project_path: str = project_path):
        if project_path != CurrentPath:
            rmtree(project_path)
            print(f"Cleared build in {project_path} ")

    wirte_code = read_from_code(file_path)
    change_code(
        wirte_code,
        original_code_path=path.join(
            project_path, "src", "main", "kotlin", "Helloworld.kt"
        ),
    )

    command = (
        # f"echo on && cd {project_path} && gradle run -w --daemon --build-cache --max-workers 6 "
        f"cd {project_path} && gradle run {params}"
    )
    # print(command)
    # command = f"""echo '{command}' && {command}"""

    result = run(command, shell=True, stdout=PIPE, stderr=PIPE, text=True)

    print(result.stdout)

    if result.stderr and result.stderr.strip().__len__() > 0:
        print("Error information: \n")
        print(result.stderr)
    if toclearbuild:
        delete_build()

    print("Returncode: ", result.returncode)


def parse_arguments():
    parser = argparse.ArgumentParser(description="Compile Kotlin files.")
    parser.add_argument("file_path", type=str, help="Path to the Kotlin file.")
    parser.add_argument(
        "--clear", action="store_true", help="Clear build before compiling."
    )
    parser.add_argument(
        "--params",
        type=str,
        default="",
        help="Additional other gradle parameters for the compiler.",
    )
    return parser.parse_args()


if __name__ == "__main__":

    args = parse_arguments()

    if not path.exists(args.file_path):
        raise FileNotFoundError(f"File not found: {args.file_path}")

    mycompilter(
        file_path=args.file_path,
        project_path=path.join(CurrentPath, "build", "build"),
        params=args.params,
        toclearbuild=args.clear,
    )
