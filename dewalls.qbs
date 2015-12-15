import qbs 1.0

Project {
    name: "dewalls"

    DynamicLibrary {
        name: "dewalls"
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core"] }

        cpp.includePaths: ["src"]

        Export {
            Depends { name: "cpp" }
            cpp.includePaths: ["src"]
        }

        Group {
            fileTagsFilter: ["dynamiclibrary"]
            qbs.installDir: (qbs.targetOS.contains("darwin") ? product.name + ".framework/Versions/A" : "")
            qbs.install: true
        }

        Group {
            fileTagsFilter: ["bundle"]
            qbs.install: true
        }

        cpp.installNamePrefix: qbs.installRoot

        Properties {
            condition: qbs.targetOS.contains("windows")
            cpp.defines: ["DEWALLS_LIB"]
        }

        Properties {
            condition: qbs.targetOS.contains("osx")
            cpp.cxxFlags: [
                "-stdlib=libc++",
                "-std=c++11", //For c++11 support
                "-Werror" //Treat warnings as errors
            ]

            cpp.dynamicLibraries: [
                "c++"
            ]

            cpp.linkerFlags: [
                "-stdlib=libc++"
            ]
        }

        Properties {
            condition: qbs.targetOS.contains("linux")
            cpp.cxxFlags: [
                "-std=c++11", //For c++11 support
                "-Werror" //Treat warnings as errors
            ]
        }

        files: [
            "src/*.cpp",
            "src/*.h"
        ]
    }

    CppApplication {
        name: "dewalls-test"
        consoleApplication: true
        type: "application"

        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core"] }
        Depends { name: "dewalls" }

        cpp.includePaths: ["src"]

        Properties {
            condition: qbs.targetOS.contains("osx")
            cpp.cxxFlags: [
                "-stdlib=libc++",
                "-std=c++11", //For c++11 support
                "-Werror" //Treat warnings as errors
            ]

            cpp.dynamicLibraries: [
                "c++"
            ]

            cpp.linkerFlags: [
                "-stdlib=libc++"
            ]
        }

        Properties {
            condition: qbs.targetOS.contains("linux")
            cpp.cxxFlags: [
                "-std=c++11", //For c++11 support
                "-Werror" //Treat warnings as errors
            ]

            cpp.dynamicLibraries: [
                "c++"
            ]
        }


        files: [
            "test/*.cpp",
            "test/*.h",
            "test/dewalls-test.qrc",
        ]
    }
}
