from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.files import get, copy
import os


class VanillaPDFConan(ConanFile):
    name = "vanillapdf"
    description = "Cross-platform toolkit for creating and modifying PDF documents"
    license = "Apache-2.0"
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://vanillapdf.com"
    topics = ("pdf", "document", "toolkit")

    package_type = "library"
    settings = "os", "compiler", "build_type", "arch"

    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
    }

    def export_sources(self):
        # For local builds from repo checkout
        src = os.path.join(self.recipe_folder, "..")
        for pattern in ["CMakeLists.txt", "LICENSE.txt", "NOTICE.md"]:
            copy(self, pattern, src=src, dst=self.export_sources_folder)
        for dir in ["cmake", "include", "src"]:
            copy(self, f"{dir}/*", src=src, dst=self.export_sources_folder)

    def source(self):
        # For CCI builds - download if source not already exported
        if not os.path.exists(os.path.join(self.source_folder, "CMakeLists.txt")):
            get(self, **self.conan_data["sources"][self.version], strip_root=True)

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def requirements(self):
        self.requires("spdlog/[>=1.14]")
        self.requires("nlohmann_json/[>=3.11]")
        self.requires("openssl/[>=3.0 <4]")
        self.requires("zlib/[>=1.2]")
        self.requires("libjpeg-turbo/[>=2.0]")
        self.requires("openjpeg/[>=2.5]")

    def layout(self):
        cmake_layout(self, src_folder=".")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["VANILLAPDF_INTERNAL_VCPKG"] = False
        tc.variables["VANILLAPDF_EXTERNAL_OPENSSL"] = True
        tc.variables["VANILLAPDF_EXTERNAL_JPEG"] = True
        tc.variables["VANILLAPDF_EXTERNAL_OPENJPEG"] = True
        tc.variables["VANILLAPDF_EXTERNAL_ZLIB"] = True
        tc.variables["VANILLAPDF_EXTERNAL_SPDLOG"] = True
        tc.variables["VANILLAPDF_EXTERNAL_NLOHMANN_JSON"] = True
        tc.variables["VANILLAPDF_ENABLE_TESTS"] = False
        tc.variables["VANILLAPDF_ENABLE_BENCHMARK"] = False
        tc.variables["VANILLAPDF_ENABLE_PACKAGING"] = False
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE.txt", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["vanillapdf"]
        self.cpp_info.set_property("cmake_file_name", "vanillapdf")
        self.cpp_info.set_property("cmake_target_name", "vanillapdf::vanillapdf")
        self.cpp_info.requires = [
            "openssl::openssl",
            "zlib::zlib",
            "libjpeg-turbo::libjpeg-turbo",
            "openjpeg::openjpeg",
            "spdlog::spdlog",
            "nlohmann_json::nlohmann_json",
        ]
