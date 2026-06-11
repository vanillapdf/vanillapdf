from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import check_min_cppstd
from conan.tools.files import get, copy
import os


class VanillaPDFConan(ConanFile):
    name = "vanillapdf"
    description = "Cross-platform toolkit for creating and modifying PDF documents"
    license = "Apache-2.0"
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://vanillapdf.com"
    documentation = "https://vanillapdf.readthedocs.io/"
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

    def validate(self):
        check_min_cppstd(self, 17)

    def requirements(self):
        self.requires("spdlog/[>=1.14]")
        self.requires("fmt/[>=10.0]")
        self.requires("openssl/[>=3.0 <4]")
        self.requires("zlib/[>=1.2]")
        self.requires("libjpeg-turbo/[>=2.0]")
        self.requires("openjpeg/[>=2.5]")

    def layout(self):
        cmake_layout(self, src_folder=".")

    def generate(self):
        tc = CMakeToolchain(self)
        # Use cache_variables so values are passed as -D flags on the cmake
        # command line, which is the recommended approach for build options.
        # This is critical for VANILLAPDF_INTERNAL_VCPKG which is checked
        # in vcpkg_init.cmake before project() loads the toolchain file.

        # Library type
        tc.cache_variables["BUILD_SHARED_LIBS"] = bool(self.options.shared)

        # MSVC CRT linkage — forward Conan's compiler.runtime so
        # vanillapdf_configure_msvc_runtime() picks /MT vs /MD correctly
        if self.settings.os == "Windows":
            tc.cache_variables["VANILLAPDF_USE_STATIC_CRT"] = self.settings.compiler.runtime == "static"

        # Dependency management
        tc.cache_variables["VANILLAPDF_INTERNAL_VCPKG"] = False
        tc.cache_variables["VANILLAPDF_EXTERNAL_OPENSSL"] = True
        tc.cache_variables["VANILLAPDF_EXTERNAL_JPEG"] = True
        tc.cache_variables["VANILLAPDF_EXTERNAL_OPENJPEG"] = True
        tc.cache_variables["VANILLAPDF_EXTERNAL_ZLIB"] = True
        tc.cache_variables["VANILLAPDF_EXTERNAL_SPDLOG"] = True
        tc.cache_variables["VANILLAPDF_EXTERNAL_FMT"] = True

        # Conan's CMakeDeps generates its own find_package config files
        tc.cache_variables["VANILLAPDF_SKIP_CMAKE_CONFIG_INSTALL"] = True

        # Disable developer-only features
        tc.cache_variables["VANILLAPDF_ENABLE_TESTS"] = False
        tc.cache_variables["VANILLAPDF_ENABLE_BENCHMARK"] = False
        tc.cache_variables["VANILLAPDF_ENABLE_PACKAGING"] = False

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
            "fmt::fmt",
        ]

        # Conan's CMakeDeps generates its own find_package config, so the
        # PUBLIC compile definition from CMakeLists.txt does not propagate
        # to consumers. Declare it here so VANILLAPDF_API resolves to
        # __declspec(dllimport) on Windows.
        if self.options.shared:
            self.cpp_info.defines = ["VANILLAPDF_CONFIGURATION_DLL"]
