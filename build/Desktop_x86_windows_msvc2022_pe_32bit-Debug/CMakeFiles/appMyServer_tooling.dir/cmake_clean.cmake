file(REMOVE_RECURSE
  "MyServer/Main.qml"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/appMyServer_tooling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
