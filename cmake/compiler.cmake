add_library(all_warnings INTERFACE)
target_compile_options(
  all_warnings
  INTERFACE $<$<AND:$<BUILD_INTERFACE:1>,$<CXX_COMPILER_ID:MSVC>>:/W4>
            $<$<AND:$<BUILD_INTERFACE:1>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-Wall
            -Wextra -Wpedantic>)

add_library(warnings_are_errors INTERFACE)
target_compile_options(
  warnings_are_errors
  INTERFACE
  INTERFACE
    $<$<AND:$<BUILD_INTERFACE:1>,$<CXX_COMPILER_ID:MSVC>>:/WX>
    $<$<AND:$<BUILD_INTERFACE:1>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-Werror>)
