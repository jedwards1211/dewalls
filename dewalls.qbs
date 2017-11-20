import qbs 1.0

Project {
    name: "dewalls"

    DynamicLibrary {
        name: "dewalls"

        //For mac os x we need to build dylib instead of framework bundle. When running
        //macdepolyqt for release, with a framework, an extra "lib" is added to the
        //path which prevents macdeployqt from finding the correct library's location
        consoleApplication: true

        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["core"] }
        property string rpath: buildDirectory

        Export {
            Depends { name: "cpp" }
            cpp.includePaths: ["src"]
            cpp.rpaths: [product.rpath]
            cpp.cxxFlags: {
                if(qbs.toolchain.contains("gcc")) {
                    return ["-Wno-attributes"] //Ignore-around to a g++ bug, https://gcc.gnu.org/bugzilla/show_bug.cgi?id=43407
                }
                return []
            }
        }

        Group {
            fileTagsFilter: ["dynamiclibrary"]
            qbs.install: qbs.targetOS.contains("windows")
        }

        cpp.includePaths: ["src"]
        cpp.rpaths: [Qt.core.libPath]

        Properties {
            condition: qbs.targetOS.contains("windows")
            cpp.defines: ["DEWALLS_LIB"]
        }

        Properties {
            condition: qbs.targetOS.contains("osx")
            cpp.sonamePrefix: "@rpath"
            cpp.cxxFlags: {
                var flags = [
                            "-stdlib=libc++", //Needed for protoc
                            "-std=c++11", //For c++11 support
                            "-Werror", //Treat warnings as errors

                        ];

                if(qbs.buildVariant == "debug") {
                    flags.push("-fsanitize=address");
                    flags.push("-fno-omit-frame-pointer")
                }

                return flags;
            }

            cpp.driverFlags: {
                var flags = [];
                if(qbs.buildVariant == "debug") {
                    flags.push("-fsanitize=address")
                }
                return flags;
            }
        }

        Properties {
            condition: qbs.targetOS.contains("linux")
            cpp.cxxFlags: {
                var flags = ["-std=c++11", //For c++11 support
                             "-Werror"] //Treat warnings as error
                if(qbs.toolchain.contains("gcc")) {
                    flags.push("-Wno-attributes") //Ignore-around to a g++ bug, https://gcc.gnu.org/bugzilla/show_bug.cgi?id=43407
                }
                return flags
            }
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
        }

        Properties {
            condition: qbs.targetOS.contains("linux")
            cpp.cxxFlags: [
                "-std=c++11", //For c++11 support
                "-Werror" //Treat warnings as errors
            ]
        }

        files: [
            "test/*.cpp",
            "test/*.h",
            "test/dewalls-test.qrc",
        ]
    }
}
