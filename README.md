## Setup

- Ensure that you have a C++ compiler (we suggest [mingw](https://sourceforge.net/projects/mingw-w64/) for Windows).
- Setup your environment and ensure all C++ files are included in compilation.

  For example, if we use the [code runner](https://marketplace.visualstudio.com/items?itemName=formulahendry.code-runner) extension for VSCode, we would add this in `settings.json`:

``` 
  "code-runner.executorMap": {ulahendry.code-runner) extension for VSCode, we would add this in `settings.json`:
  
    "cpp": "cd $dir && g++ *.cpp -o $fileNameWithoutExt && $dir$fileNameWithoutExt",
  }
```

  If we were running C++ using VSCode directly, we would define the `tasks.json` file with corresponding `args` to include all C++ files:

  ```
  "tasks": [
    {
      "args": [
          "-fdiagnostics-color=always",
          "-g",
          "${workspaceFolder}/*.cpp",
          "-o",
          "${fileDirname}\\${fileBasenameNoExtension}"
        ]
    }
  ]
  ```

- `cd` to `main.cpp` under the `src` folder and compile the executable.
