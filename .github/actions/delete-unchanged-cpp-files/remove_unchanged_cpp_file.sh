#!/bin/bash

exclude_path=""
IFS=';' read -ra FILES <<< "$ALL_CHANGED_FILES"
for file in "${FILES[@]}"; do
  exclude_path="$exclude_path ! -path '$file'"
done
find_command="find $TARGET_PATH -type f \( -name "*.cpp" -o -name "*.cxx" -o -name "*.cc" \) $exclude_path -delete"
echo "$find_command"
eval "$find_command"
find "$TARGET_PATH" -type f \( -name "*.cpp" -o -name "*.cxx" -o -name "*.cc" \)