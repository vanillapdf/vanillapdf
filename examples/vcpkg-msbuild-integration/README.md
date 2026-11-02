# vcpkg MSBuild Integration

Consumes the Vanilla.PDF vcpkg port from a Visual Studio project, without CMake.

The project imports vcpkg's `vcpkg.props` and `vcpkg.targets`, the same files `vcpkg integrate install` adds to every project of a user. They add the include directory of the installed tree, link every library in its `lib` directory and copy the DLLs next to the executable. The project defines nothing itself, so the installed headers alone have to select `__declspec(dllimport)` for the shared library. Without it the exported constants such as `VANILLAPDF_ERROR_SUCCESS` fail to link with `LNK2019`.

The files are imported from the `external/vcpkg` submodule of this repository rather than through `vcpkg integrate install`, which would change the MSBuild setup of the whole user account.

## Running

From the repository root, install the port in classic mode with the dynamic `x64-windows` triplet, as the CI workflow does:

```powershell
.\external\vcpkg\vcpkg.exe install vanillapdf --head --overlay-ports=./ports --triplet=x64-windows
```

Then build and run the project from a Developer PowerShell:

```powershell
msbuild examples\vcpkg-msbuild-integration\vcpkg_msbuild_test.vcxproj /p:Configuration=Release /p:Platform=x64
.\examples\vcpkg-msbuild-integration\build\x64\Release\vcpkg_msbuild_test.exe
```

The test refuses to build against a static library, where it would link without `dllimport` and prove nothing.
