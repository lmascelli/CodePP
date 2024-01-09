$project = Split-Path -Path (Get-Location) -Leaf
# this envoronment variable is used to locally install conan binaries if this
# package manager is used
$env:CONAN_HOME = "$((Get-Location).Path)/devel/conan"
$Script:Toolchain_file = "conan_toolchain.cmake"

# Check if a directory exists and create it if not. Then pop its location
function Script:check_n_pop_directory([String]$name) {
  New-Item -Type Directory -Path $name -ErrorAction Ignore
  Push-Location $name
}

function Script:ConanDependencies {
  # Check if a .conandependencies file already exists
  $Script:is_conan_depenendencies = Get-Item -Path "build/conan_toolchain.cmake" -ErrorAction Ignore

  # If it doesn't exists or is less recent than the conanfile.py file create or
  # update it and run conan install else just return
  if ($Script:is_conan_depenendencies) {
    $Script:conan_dependencies_time = (Get-Item -Path "build/conan_toolchain.cmake").LastWriteTime
    $Script:conanfile_time = (Get-Item -Path ".\conanfile.py").LastWriteTime
    Write-Output $Script:conan_dependencies_time
    Write-Output $Script:conanfile_time
    if ($Script:conanfile_time -lt $Script:conan_dependencies_time) {
      Write-Output "Conan dependencies up to date"
      return
    }
  }
  conan profile detect --force
  conan install . --output-folder="build/Debug" --build=missing -s compiler.cppstd=20 -s build_type=Debug
  conan install . --output-folder="build/Release" --build=missing -s compiler.cppstd=20 -s build_type=Release
}

function Script:Build([string[]] $arg_list) {
  # If there is a .conandependencies file check if conan install is needed
  $Script:is_conan_depenendencies = Get-Item -Path "build/conan_toolchain.cmake" -ErrorAction Ignore
  if ($Script:is_conan_depenendencies) {
    ConanDependencies
  }
  check_n_pop_directory("build/Debug")
  
  cmake -G"Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOCHAIN_FILE=$Script:Toolchain_file $arg_list ../..
  Copy-Item -Force compile_commands.json ../..
  cmake --build .
  Pop-Location
}

function Script:Release {
  check_n_pop_directory("build/Release")

  cmake -G"Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOCHAIN_FILE=$Script:Toolchain_file $arg_list ../..
  Copy-Item -Force compile_commands.json ../..
  cmake --build .
  if (Get-Item -Path "pycode/libPyCode.dll" -ErrorAction Ignore) {
    Move-Item -Path "pycode/libPyCode.dll" -Destination "../pycode.pyd" -Force
  }
  Pop-Location
}

function Script:Run {
  Push-Location build
  $command = './' + $project
  Invoke-Expression $command
  Pop-Location
}

switch ($args[0]) {
  "build" {
    Build $args[1..($args.Length)]
  }

  "release" {
    Release
  }

  "conan" {
    ConanDependencies
  }

  "run" {
    Build $args[1..($args.Length)]
    Run
  }

  "clean" {
    Remove-Item -Force -Recurse -Path "./build" -ErrorAction Ignore
  }

  default {
    Run
  }
}
