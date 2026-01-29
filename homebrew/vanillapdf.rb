class Vanillapdf < Formula
  desc "Cross-platform toolkit for creating and modifying PDF documents"
  homepage "https://vanillapdf.com"
  # Release URL and SHA256 are updated by the release workflow
  url "https://github.com/vanillapdf/vanillapdf/archive/refs/tags/v2.3.0.tar.gz"
  sha256 "0000000000000000000000000000000000000000000000000000000000000000"
  license "Apache-2.0"
  head "https://github.com/vanillapdf/vanillapdf.git", branch: "main"

  depends_on "cmake" => :build
  depends_on "jpeg-turbo"
  depends_on "nlohmann-json"
  depends_on "openjpeg"
  depends_on "openssl@3"
  depends_on "spdlog"

  uses_from_macos "zlib"

  def install
    args = %w[
      -DVANILLAPDF_INTERNAL_VCPKG=OFF
      -DVANILLAPDF_EXTERNAL_OPENSSL=ON
      -DVANILLAPDF_EXTERNAL_JPEG=ON
      -DVANILLAPDF_EXTERNAL_OPENJPEG=ON
      -DVANILLAPDF_EXTERNAL_ZLIB=ON
      -DVANILLAPDF_EXTERNAL_SPDLOG=ON
      -DVANILLAPDF_EXTERNAL_NLOHMANN_JSON=ON
    ]

    system "cmake", "-S", ".", "-B", "build", *args, *std_cmake_args
    system "cmake", "--build", "build"
    system "cmake", "--install", "build"
  end

  test do
    # Test library functionality by creating a PDF document
    (testpath/"test.c").write <<~C
      #include <vanillapdf/c_vanillapdf_api.h>
      #include <stdio.h>

      int main() {
        DocumentHandle *doc = NULL;
        error_type err = Document_Create("test.pdf", &doc);
        if (err != VANILLAPDF_ERROR_SUCCESS || doc == NULL) {
          printf("Failed to create document\\n");
          return 1;
        }
        Document_Release(doc);
        printf("PDF creation test passed\\n");
        return 0;
      }
    C

    system ENV.cc, "test.c", "-I#{include}", "-L#{lib}", "-lvanillapdf", "-o", "test"
    system "./test"
  end
end
