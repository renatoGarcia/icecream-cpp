from conan import ConanFile
from conans.model.version import Version


class CompressorRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"
    default_options = {"boost/*:header_only": True}

    def requirements(self):
        compiler = self.settings.compiler.value
        version = Version(self.settings.compiler.version.value)
        cppstd = int(self.settings.compiler.cppstd.value)

        self.requires("boost/1.86.0")
        self.requires("catch2/2.13.10")
        self.requires("fmt/11.0.2")

        if (
            (compiler == "gcc" and version > "6" and cppstd >= 14)
            or (compiler == "msvc" and cppstd >= 17)
            or (compiler not in ["gcc", "msvc"] and cppstd >= 14)
        ):
            self.requires("range-v3/0.12.0")
