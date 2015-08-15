import qbs 1.0

Project {
    name: "dewalls"

    StaticLibrary {
        name: "dewalls"
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core"] }

        cpp.includePaths: ["src"]

        Export {
            Depends { name: "cpp" }
            cpp.includePaths: ["src"]
        }

        files: [
            "src/*.cpp",
            "src/*.h",
            "test/approx.h",
            "test/generaltests.cpp",
        ]
    }

    CppApplication {
        name: "dewalls-test"
        consoleApplication: true
        type: "application"

        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core"] }

        cpp.includePaths: ["src"]

        files: [
            "src/*.cpp",
            "src/*.h",
            "test/*.cpp",
            "test/*.h",
        ]
    }
}
