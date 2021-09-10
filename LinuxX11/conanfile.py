from conans import ConanFile
from conans.client.build.cmake import CMake

class GLES_Examples(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake", "virtualenv"
    name = "GLES_Examples"
    version = "0.1"
    exports_sources = ["CMakeLists.txt", "Common/*", "Chapter_2/*"]

    def requirements(self):
        self.requires("mesa/21.2.1")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()