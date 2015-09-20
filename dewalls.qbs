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

        Properties {
            condition: qbs.targetOS.contains("osx") || qbs.targetOS.contains("linux")
            cpp.cxxFlags: [
                "-stdlib=libstdc++", //Needed for protoc
                "-std=c++11", //For c++11 support
                "-Werror" //Treat warnings as errors
            ]
        }

        Properties {
            condition: qbs.targetOS.contains("osx")

            cpp.dynamicLibraries: [
                "c++"
            ]
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

        Properties {
            condition: qbs.targetOS.contains("osx") || qbs.targetOS.contains("linux")
            cpp.cxxFlags: [
                "-stdlib=libstdc++", //Needed for protoc
                "-std=c++11", //For c++11 support
                "-Werror" //Treat warnings as errors
            ]
        }

        Properties {
            condition: qbs.targetOS.contains("osx")

            cpp.dynamicLibraries: [
                "c++"
            ]
        }

        files: [
            "src/*.cpp",
            "src/*.h",
            "test/*.cpp",
            "test/*.h",
        ]
    }
}
