# These files are used for creating NUGET bundles

# Step 1 - Extract docker binaries (for Linux currently)
./extract_packages.py 1.x.0 build archives
Expected result: Successfully extracted 2 packages

# Step 2 - Compile Windows binaries
# Step 3 - Restore NuGet packages
dotnet restore ../vanillapdf.csproj
# Step 4 - Build NuGet bundle
dotnet pack ../vanillapdf.csproj --no-build

