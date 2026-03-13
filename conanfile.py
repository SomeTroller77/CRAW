from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class CrawConan(ConanFile):
    name = "craw"
    version = "1.0"
    package_type = "library"

    license = "MIT"
    url = "https://github.com/SomeTroller77/CRAW"
    description = "C Reddit API wrapper"

    settings = "os", "compiler", "build_type", "arch"

    exports_sources = "CMakeLists.txt", "src/*", "include/*", "tests/*"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "CMakeToolchain", "CMakeDeps"

    requires = (
        "libcurl/8.18.0",
        "cjson/1.7.17"
    )

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["craw"]